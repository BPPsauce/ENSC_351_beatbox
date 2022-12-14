#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include "util.h"
#include "hardwareUpdate.h"

/*hardware specs defines*/
#define DISPLAY_I2C_ADDR 0x70

#define I2C_LINUX_BUS1 "/dev/i2c-1"

/*row define*/
#define ROW0 0X00
#define ROW1 0X02
#define ROW2 0X04
#define ROW3 0X06
#define ROW4 0X08
#define ROW5 0X0A
#define ROW6 0X0C
#define ROW7 0X0E

static unsigned char rows[] = {ROW7, ROW6, ROW5, ROW4, ROW3, ROW2, ROW1, ROW0};
static pthread_t displayThreadID;
static int stopping = 0;

// << 4 to get the one on the right
// the digit[0] and digit[1] will select the correct pattern from the table
static unsigned char numberTable[10][8] = {{0x02, 0x05, 0x05, 0x05, 0x05, 0x05, 0x02, 0x00}, //0
                                    {0x02, 0x06, 0x02, 0x02, 0x02, 0x02, 0x07, 0x00}, //1
                                    {0x07, 0x01, 0x01, 0x07, 0x04, 0x04, 0x07, 0x00}, //2
                                    {0x07, 0x01, 0x01, 0x03, 0x01, 0x01, 0x07, 0x00}, //3 
                                    {0x05, 0x05, 0x05, 0x07, 0x01, 0x01, 0x01, 0x00}, //4 
                                    {0x07, 0x04, 0x04, 0x07, 0x01, 0x05, 0x07, 0x00}, //5 
                                    {0x07, 0x04, 0x04, 0x07, 0x05, 0x05, 0x07, 0x00}, //6 
                                    {0x07, 0x05, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00}, //7
                                    {0x07, 0x05, 0x05, 0x02, 0x05, 0x05, 0x07, 0x00}, //8
                                    {0x07, 0x05, 0x05, 0x07, 0x01, 0x05, 0x07, 0x00}}; //9

//init for the i2c bus
static int initI2cBus (char* bus, int address){
    int i2cFileDesc = open(bus, O_RDWR);
    int result = ioctl(i2cFileDesc, I2C_SLAVE, address);
    if (result < 0) {
    perror("I2C: Unable to set I2C device to slave address.");
    exit(1);
}
    return i2cFileDesc;
}

//taken from the tutorials
static void writeI2cReg(int i2cFileDesc, unsigned char regAddr,unsigned char value){
    unsigned char buff[2];
    buff[0] = regAddr;
    buff[1] = value;
    int res = write(i2cFileDesc, buff, 2);
    res++;
    // if (res != 2) {
    //     perror("I2C: Unable to write i2c register.");
    //     exit(1);
    // }
}

//reset the display to empty
static void resetDisplay(){
    int i2cFileDesc = initI2cBus(I2C_LINUX_BUS1, DISPLAY_I2C_ADDR);
    for (int i = 0; i < 8; i++){
        writeI2cReg(i2cFileDesc, rows[i], 0x00);
    }
}

/*pin config for the i2c pins*/
static void i2cpinsInit(){
    //init for P_18 and P_17
    runCommand("config-pin P9_18 i2c");
    runCommand("config-pin P9_17 i2c");
}

//init the i2c pins and turn on the display
static void displayInternalInit(){
    i2cpinsInit();
    int i2cFileDesc = initI2cBus(I2C_LINUX_BUS1, DISPLAY_I2C_ADDR);
    writeI2cReg(i2cFileDesc, 0x21, 0x00);
    writeI2cReg(i2cFileDesc, 0x81, 0x00);
    resetDisplay();
}

//split an interge to 2 digits
static int * integerSplit(int number){
    if (number > 99){
        number = 99;
    }
    int * digit = malloc(sizeof(int) * 2);

    for(int i = 0; i < 2; i++){
        digit[i] = number%10;
        number = number/10;
    }

    return digit;
}

static int * doubleSplit(double number){
    if (number > 9.9){
        number = 9.9;
    }
    int * digit = malloc(sizeof(int) * 2);

    int intPart = (int)number; //cast it int
    double temp = number - intPart;
    temp = 10 * temp;
    int decPart = (int)temp;

    digit[0] = decPart;
    digit[1] = intPart;

    return digit;
}

static void printInteger(int reading){
    int * digit = integerSplit(reading);

    int i2cFileDesc = initI2cBus(I2C_LINUX_BUS1, DISPLAY_I2C_ADDR);
    //empty value arrays, ready for OR and shift operation
    unsigned char value[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    unsigned char shifted[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    for (int i = 0; i < 8; i++){
        //the number on the left is 4 bits shifted to the left
        shifted[i] = numberTable[digit[1]][i] << 4;
        value[i] |= numberTable[digit[0]][i];
        //then OR together to get the final value
        value[i] |= shifted[i];
    }

    for (int i = 0; i < 8; i++){
        writeI2cReg(i2cFileDesc, rows[i], value[i]);
    }
    //free the array
    free(digit);
}

static void printDouble(double reading){
    int * digit = doubleSplit(reading);

    int i2cFileDesc = initI2cBus(I2C_LINUX_BUS1, DISPLAY_I2C_ADDR);
    //empty value arrays, ready for OR and shift operation
    unsigned char value[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08};
    unsigned char shifted[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    for (int i = 0; i < 8; i++){
        //the number on the left is 4 bits shifted to the left
        shifted[i] = numberTable[digit[1]][i] << 4;
        value[i] |= numberTable[digit[0]][i];
        //then OR together to get the final value
        value[i] |= shifted[i];
    }

    for (int i = 0; i < 8; i++){
        writeI2cReg(i2cFileDesc, rows[i], value[i]);
    }
    //free the array
    free(digit);
}

/*mode array | num array*/
static void printMode(int mode){
    int i2cFileDesc = initI2cBus(I2C_LINUX_BUS1, DISPLAY_I2C_ADDR);
    unsigned char letterM[] = {0x50, 0x70, 0x70, 0x50, 0x50, 0x50, 0x50, 0x00};
    unsigned char value[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    for (int i = 0; i < 8; i++){
        value[i] |= numberTable[mode][i];
        //then OR together to get the final value
        value[i] |= letterM[i];
    }

    for (int i = 0; i < 8; i++){
        writeI2cReg(i2cFileDesc, rows[i], value[i]);
    }

}

/*thread for the display*/
static void *UpdateDisplay(void *_)
{
    while (!stopping){
        //display init
        // displayInit();

        int direction = getJoystickDir();
            //center - 0
            //up     - 1
            //down   - 2
            //left   - 3
            //right  - 4
        switch (direction)
        {
        case 0: //center
            printMode(getMode());
            break;
        case 1: //up
            printInteger(getVolume());
            sleep_for_ms(500);
            break;
        case 2: //down 
            printInteger(getVolume());
            sleep_for_ms(500);
            break;
        case 3: //left
            printDouble(getBPM() / 60.0);
            sleep_for_ms(500);
            break;
        case 4: //right
            printDouble(getBPM() / 60.0);
            sleep_for_ms(500);
            break;
        default:
            printf("Joystick reading not correct!\n");
            break;

        }
        sleep_for_ms(100);
    }
    return NULL;
}

/*init thread*/
void displayWriter_init(void)
{
    displayInternalInit();
    pthread_create(&displayThreadID, NULL, &UpdateDisplay, NULL);
}

/*clean up thread*/
void displayWriter_cleanup(void)
{
    stopping = 1;
    pthread_join(displayThreadID, NULL);
    resetDisplay();
}
