#include "MetalDetector.h"

MetalDetector::MetalDetector(){
  pinMode(pin_pulse, OUTPUT); 
  digitalWrite(pin_pulse, LOW);
  pinMode(pin_cap, INPUT);
  if(sound)pinMode(pin_tone, OUTPUT);
  if(sound)digitalWrite(pin_tone, LOW);
}

void MetalDetector::detectMetal(int *metal_state){
  int minval=1023;
  int maxval=0;
  
  //perform measurement
  long unsigned int sum=0;
  for (int imeas=0; imeas<nmeas+2; imeas++){
    //reset the capacitor
    pinMode(pin_cap,OUTPUT);
    digitalWrite(pin_cap,LOW);
    delayMicroseconds(20);
    pinMode(pin_cap,INPUT);
    //apply pulses
    for (int ipulse = 0; ipulse < npulse; ipulse++) {
      digitalWrite(pin_pulse,HIGH); //takes 3.5 microseconds
      delayMicroseconds(3);
      digitalWrite(pin_pulse,LOW);  //takes 3.5 microseconds
      delayMicroseconds(3);
    }
    //read the charge on the capacitor
    int val = analogRead(pin_cap); //takes 13x8=104 microseconds
    minval = min(val,minval);
    maxval = max(val,maxval);
    sum+=val;
 
    //determine if buzzer should be on or off
    long unsigned int timestamp=millis();
    byte alertState=0;
    //if timestamp do not pass the previous flash time up to 10 seconds
    if (timestamp<prev_flash+10){
      if (diff>0)alertState=1;
      if (diff<0)alertState=2;
    }
    if (timestamp>prev_flash+flash_period){
      if (diff>0)alertState=1;
      if (diff<0)alertState=2;
      prev_flash=timestamp;   
    }
    if (flash_period>1000)alertState=0;

    //switch the buzzers to this setting
    if (alertState==0){
      if(sound)digitalWrite(pin_tone, LOW);
    }
    if (alertState==1){
      *metal_state = 1;
      if(sound)digitalWrite(pin_tone,HIGH);
    }
    if (alertState==2){
      *metal_state = 2;
      if(sound)digitalWrite(pin_tone,HIGH);
    }
  }

  //subtract minimum and maximum value to remove outliers
  sum-=minval; sum-=maxval;
  
  //process
  if (cumulativeSum64==0) cumulativeSum64=sum<<8; //set cumulativeSum64 to expected value
  long int avgsum=(cumulativeSum64+128)>>8; 
  diff=sum-avgsum;
  if (abs(diff)<avgsum>>10){      //adjust for small changes
    cumulativeSum64=cumulativeSum64+sum-avgsum;
    skip=0;
  } else {
    skip++;
  }
  if (skip>64){     // break off in case of prolonged skipping
    cumulativeSum64=sum<<8;
    skip=0;
  }

  // one permille change = 2 ticks/s
  if (diff==0) flash_period=1000000;
  else flash_period=avgsum/(2*abs(diff));  //if difference gets larger it will beep more 
  

  if (debug){
    Serial.print(nmeas); 
    Serial.print(" ");
    Serial.print(minval); 
    Serial.print(" ");
    Serial.print(maxval); 
    Serial.print(" ");
    Serial.print(sum); 
    Serial.print(" ");
    Serial.print(avgsum); 
    Serial.print(" ");
    Serial.print(diff); 
    Serial.print(" ");
    Serial.print(flash_period); 
    Serial.println();
  }
}