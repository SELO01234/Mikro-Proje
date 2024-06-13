#ifndef METALDETECTOR_H
#define METALDETECTOR_H

#include <Arduino.h>

class MetalDetector{
  private:
    const byte npulse = 3;
    const bool sound = true;
    const bool debug = true;

    const byte pin_pulse=A0;
    const byte pin_cap  =A1;
    const byte pin_tone =10;

    const int nmeas=256;  //measurements to take
    long int cumulativeSum64=0; //running sum of 64 sums 
    long int skip=0;   //number of skipped sums
    long int flash_period=0;//period (in ms) 
    long unsigned int prev_flash=0; //time stamp of previous flash
    
  public:
    long int diff=0;        //difference between sum and avgsum
    MetalDetector();
    void detectMetal(int *metal_state);
};

#endif