
#include "HeatMap.h"
#include "PicoEncoder.h"

HeatMap::HeatMap(Adafruit_ST7735* tft) {
    this->tft = tft; // Initialize tft
    rectWidth = tft->width() / 2.6; // Initialize rectWidth
    rectHeight = tft->height() / 2.6; // Initialize rectHeight

    colors[0] = ST77XX_GREEN;
    colors[1] = ST77XX_YELLOW;
    colors[2] = ST77XX_BLUE;  //red

    far = ST7735_GREEN;
    near = ST7735_BLUE;
    middle = 0x471A;

    current = 0;
    previous = 0;

    direction = 0;

    
    
}
      
      void HeatMap::init(){

      tft->setRotation(1);
      tft->fillScreen(ST77XX_BLACK);
        
      encoder.begin(encoder_pinA);
      encoder.setPhases(0x334B35);

      makeAllGreen();      

      }
      

     void HeatMap::  makeAllGreen(){

         tft->fillRect(3, 36, rectWidth, rectHeight, far);
         tft->fillRect(56, 36, rectWidth, rectHeight, far);
         tft->fillRect(109, 36, rectWidth, rectHeight, far);

      }
      
      
     void HeatMap::cableDetectionHeatmap() {

      delay(50);
      cableValue = analogRead(A2);
      Serial.println(cableValue);
      encoder.update();
      current = encoder.step; 

      if(current - previous>0){ // son konum eksi önceki konum pozitifse saga veya yukari ileriyor

        Serial.println("Right");
        direction = 1;
      }
      if (current - previous<0){

        Serial.println("Left");
        direction = -1;
      }

      if (current - previous == 0){

            //Serial.print("no direction");
            direction = 0;

          }
      
      // Determine the color based on the cable value
      uint16_t color;
    
      if (cableValue > 70) 
      {
          if (cableValue < 500) {
              if (direction == 1) {
                  tft->fillRect(3, 36, rectWidth, rectHeight, far);
                  tft->fillRect(56, 36, rectWidth, rectHeight, middle);
                  tft->fillRect(109, 36, rectWidth, rectHeight, near);
              } else if (direction == -1) {
                  tft->fillRect(3, 36, rectWidth, rectHeight, near);
                  tft->fillRect(56, 36, rectWidth, rectHeight, middle);
                  tft->fillRect(109, 36, rectWidth, rectHeight, far);
              }
          } else {
              if (direction == 1 || direction == -1) {
                  tft->fillRect(3, 36, rectWidth, rectHeight, near);
                  tft->fillRect(56, 36, rectWidth, rectHeight, near);
                  tft->fillRect(109, 36, rectWidth, rectHeight, near);
              }
          }
      } 
      else {
          tft->fillRect(3,36 ,rectWidth, rectHeight, far);
          tft->fillRect(56, 36, rectWidth, rectHeight, far);
          tft->fillRect(109, 36, rectWidth, rectHeight, far);
      }

      previous = current; 
      }

      


     void HeatMap::metalDetectionHeatmap(long int dif) {


long int diff=abs(dif);
          // Read strength value from sensor
          delay(20);

          //Serial.println(cableValue);
          
          //Serial.println(cableValue);

          encoder.update();
          current = encoder.step; 

          if(current - previous>0){ // son konum eksi önceki konum pozitifse saga ileriyor

            Serial.println("sağ");
            direction = 1;
            

          }
          if (current - previous<0){

            Serial.println("Sol");
            direction = -1;
          }

          if (current - previous == 0){

            //Serial.print("no direction");
            direction = 0;
     
          }
          
          // Determine the color based on the cable value
          uint16_t color;
          if (diff > 10) {
              if (diff < 20) {
              if (direction == 1) {
                  tft->fillRect(3, 36, rectWidth, rectHeight, far);
                  tft->fillRect(56, 36, rectWidth, rectHeight, middle);
                  tft->fillRect(109, 36, rectWidth, rectHeight, near);
              } else if (direction == -1) {
                  tft->fillRect(3, 36, rectWidth, rectHeight, near);
                  tft->fillRect(56, 36, rectWidth, rectHeight, middle);
                  tft->fillRect(109, 36, rectWidth, rectHeight, far);
              }
          } else {
              if (direction == 1 || direction == -1) {
                  tft->fillRect(3, 36, rectWidth, rectHeight, near);
                  tft->fillRect(56, 36, rectWidth, rectHeight, near);
                  tft->fillRect(109, 36, rectWidth, rectHeight, near);
              }
          }
      } else {
          tft->fillRect(3,36 ,rectWidth, rectHeight, far);
          tft->fillRect(56, 36, rectWidth, rectHeight, far);
          tft->fillRect(109, 36, rectWidth, rectHeight, far);
      }

      previous = current; 


          
      }