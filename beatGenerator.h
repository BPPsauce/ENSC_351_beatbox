// this is the file that helps to generate the beat and tell the lower level thread what beat to play
#ifndef BEATGENERATOR_H
#define BEATGENERATOR_H

/*init function for the beat generator*/
void beatPlayerInit(void);

/*clean up*/
void beatPlayerStop(void);

/*this function is being use as button sound output*/
/*output tones according to the gpio buttons */
void makeSingleTone(int buttonInput);

#endif
