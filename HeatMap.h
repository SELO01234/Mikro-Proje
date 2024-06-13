#ifndef HEATMAP_H
#define HEATMAP_H

#include <Arduino.h>
#include <Adafruit_ST7735.h>
#include "PicoEncoder.h"

class HeatMap {
private:
    Adafruit_ST7735* tft; // Declare tft as a member variable

public:
    PicoEncoder encoder;
    int rows = 3;
    int cols = 3;
    float rectWidth;
    float rectHeight;

    int cableValue;

    const int encoder_pinA = 6;
    //const int encoder_pinB = 7;

    uint16_t colors[3];
    uint16_t far;
    uint16_t near;
    uint16_t middle;

    int current;
    int previous;

    int direction;

    HeatMap(Adafruit_ST7735* tft);

    void metalDetectionHeatmap(long int diff);
    void cableDetectionHeatmap();
    void makeAllGreen(); 
    void init();
 
};

#endif