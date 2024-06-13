#include <Adafruit_GFX.h>    
#include <Adafruit_ST7735.h> 
#include <Adafruit_ST7789.h> 
#include <SPI.h>
#include "MetalDetector.h"
#include "CableDetector.h"
#include "HeatMap.h"




#define TFT_CS        17
#define TFT_RST       -1 
#define TFT_DC        20

const uint8_t scl = 3;
const uint8_t sda = 4;






int previous = 0;
int current = 0;

int cable_center = 0;

bool rightTurn = false; // true ise left doner





int buttonNumber;
int pageNumber = 0; //0 home, 1 metal, 2 cable, 3 metalHeat, 4 cableHeat

class KeyPad {
  private:
    uint8_t scl_pin;
    uint8_t sda_pin;

    bool arr[8] = {0};
  public:
    KeyPad(uint8_t scl_pin,  uint8_t sda_pin) {
      this->scl_pin = scl_pin;
      this->sda_pin = sda_pin;
    }

    void init() {
      pinMode(scl_pin, OUTPUT);
      pinMode(sda_pin, INPUT);
    }

    void read() {
      for (uint8_t i = 0; i < 8; i++) {
        digitalWrite(scl_pin, HIGH);
        digitalWrite(scl_pin, LOW);

        arr[i] = !digitalRead(sda_pin);

        if (arr[i] == 1) {
          buttonNumber = i;
        }
      }

    }
};

KeyPad keypad(scl, sda);
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);





void drawtext(String text, int x, int y, uint16_t color) {
  tft.setCursor(x, y);
  tft.setTextColor(color);
  tft.setTextSize(1);
  tft.print(text);
}

MetalDetector MetalDetector;

class MetalPage {
  private:
    int metal_state = 0;
    int prev_state = 0;
  public:
    MetalPage() {}

    void init() {
      tft.setRotation(1);
      tft.fillScreen(ST77XX_BLACK);
    }

    void printMetal() {
      MetalDetector.detectMetal(&metal_state);
      delay(50);
      if (metal_state != prev_state) {
        
        tft.fillRect(0, 0, tft.width(), 10, ST77XX_BLACK);
        
        
        if (metal_state == 1) {
          drawtext("Ferrous Metal", 0, 0, ST77XX_GREEN);
        } else if (metal_state == 2) {
          drawtext("Non-Magnetic Metal", 0, 0, ST77XX_GREEN);
        }

        prev_state = metal_state;
      }
      
    }
};




CableDetector cableDetector;

class CablePage {
  private:
    int value = 0;
  public:
    CablePage() {}

    void init() {
      tft.setRotation(1);
      tft.fillScreen(ST77XX_BLACK);
    }

  void printCable() {
   cableDetector.detectCable();
   int value = cableDetector.getCableValue();
   
   Serial.println(value);
   delay(100);

  tft.fillRect(0, 0, tft.width(), 10, ST77XX_BLACK);

  if (value > 400) {
    drawtext("cable detected", 0, 0, ST77XX_RED);
    cableDetector.beep(1000, 50);  
  } else {
    drawtext("cable removed", 0, 0, ST77XX_RED);
  }
  
  }


};

CablePage CablePage;
MetalPage metalPage;


HeatMap heatmap( &tft); 

class MetalHeatMapPage{
  private:
    int metal_state = 0;
  public:
    MetalHeatMapPage() {}

    void updateLocalization(){
      MetalDetector.detectMetal(&metal_state);
      heatmap.metalDetectionHeatmap(MetalDetector.diff);
    }
};

class CableHeatMapPage{
  private:
  public:
    CableHeatMapPage() {}

    void updateLocalization(){
      heatmap.cableDetectionHeatmap();
    }
 
};

CableHeatMapPage cableHeatmapPage;
MetalHeatMapPage metalHeatMapPage;

class HomePage {
  private:
    int position;
    unsigned int positionColor[4] = {ST77XX_WHITE};
  public:
    HomePage() {
      position = 0;
    }

    void setPosition(int position) {
      this->position = position;
    }

    void init() {
      tft.setRotation(1);
      tft.fillScreen(ST77XX_BLACK);


      tft.fillRect(0, 0, tft.width(), 10, ST77XX_GREEN);
      drawtext("1- Metal Detection", 0, 0, ST77XX_BLACK);
      drawtext("2- Cable Detection", 0, 10, ST77XX_WHITE);
      drawtext("3- Metal Detection HeatMAP", 0, 20, ST77XX_WHITE);
      drawtext("4- Cable Detection HeatMAP", 0, 30, ST77XX_WHITE);
      drawtext("Use 1 to go back", 0, 40, ST77XX_BLUE);
      drawtext("Use 2 to go upwards", 0, 50, ST77XX_BLUE);
      drawtext("Use 6 to go downwards", 0, 60, ST77XX_BLUE);
      drawtext("Use 3 to enter", 0, 70, ST77XX_BLUE);
    


    }
    
  void traverseMenu(int menuInput) {

      if (menuInput == 2) {
        if (this->position > 0) {
          tft.fillScreen(ST77XX_BLACK);
          this->position = this->position - 10;
          for (int i = 0; i < 4; i++) {
              positionColor[i] = ST77XX_WHITE;
          }
          positionColor[this->position / 10] = ST77XX_BLACK;
          tft.fillRect(0, this->position, tft.width(), 10, ST77XX_GREEN);
          drawtext("1- Metal Detection", 0, 0, positionColor[0]);
          drawtext("2- Cable Detection", 0, 10, positionColor[1]);
          drawtext("3- Metal Detection HeatMAP", 0, 20, positionColor[2]);
          drawtext("4- Cable Detection HeatMAP", 0, 30, positionColor[3]);
          drawtext("Use 1 to go back", 0, 40, ST77XX_BLUE); //I don't know why blue gives red output and red gives blue output
          drawtext("Use 2 to go upwards", 0, 50, ST77XX_BLUE); //I don't know why blue gives red output and red gives blue output
          drawtext("Use 6 to go downwards", 0, 60, ST77XX_BLUE); //because of that I wrote blue but my main purpose is red
          drawtext("Use 3 to enter", 0, 70, ST77XX_BLUE);
        }
      } else if (menuInput == 6) {
        if(this->position < 30){
          tft.fillScreen(ST77XX_BLACK);
          this->position = this->position + 10;
          for (int i = 0; i < 4; i++) {
            positionColor[i] = ST77XX_WHITE;
          }
          positionColor[this->position / 10] = ST77XX_BLACK;
          tft.fillRect(0, this->position, tft.width(), 10, ST77XX_GREEN);
          drawtext("1- Metal Detection", 0, 0, positionColor[0]);
          drawtext("2- Cable Detection", 0, 10, positionColor[1]);
          drawtext("3- Metal Detection HeatMAP", 0, 20, positionColor[2]);
          drawtext("4- Cable Detection HeatMAP", 0, 30, positionColor[3]);
          drawtext("Use 1 to go back", 0, 40, ST77XX_BLUE); //I don't know why blue gives red output and red gives blue output
          drawtext("Use 2 to go upwards", 0, 50, ST77XX_BLUE); //I don't know why blue gives red output and red gives blue output
          drawtext("Use 6 to go downwards", 0, 60, ST77XX_BLUE); //because of that I wrote blue but my main purpose is red
          drawtext("Use 3 to enter", 0, 70, ST77XX_BLUE);
        }
      } 
      
      
      else if (menuInput == 3) {
        if (this->position == 0) {
          pageNumber = 1;
          metalPage.init();
        }
        if (this->position == 10) {
          pageNumber = 2;
          CablePage.init();
        }
        if (this->position == 20) {
          pageNumber = 3;
          heatmap.init();
          
        }
        if (this->position == 30) {
          pageNumber = 4;
          heatmap.init();
        }
      }
    }
};

HomePage home;




void setup() {
  cableDetector.init();

  Serial.begin(9600);
 
  keypad.init();
  tft.initR(INITR_GREENTAB);
 
  home.init();
 
}


void loop() {

  keypad.read();
  //Serial.println(buttonNumber);


  if (buttonNumber + 1 == 4) {
    
  }  

  if (pageNumber == 0) {
    home.traverseMenu(buttonNumber + 1);
   // delay(50);
  } 
  else if (pageNumber == 1) {
    metalPage.printMetal();
    delay(100);
    if (buttonNumber + 1 == 1) {
      pageNumber = 0;
      home.init();
      home.setPosition(0);
    }
  } 
  else if (pageNumber == 2) {
    delay(50);
    CablePage.printCable();
    
    if (buttonNumber + 1 == 1) {
      pageNumber = 0;
      home.init();
      home.setPosition(0);
    }
  }
  else if (pageNumber == 3) {
    metalHeatMapPage.updateLocalization();
   
    if (buttonNumber + 1 == 1) {
      pageNumber = 0;
      home.init();
      home.setPosition(0);
    }
  }
  else if (pageNumber == 4) {
    
    cableHeatmapPage.updateLocalization();
   
    if (buttonNumber + 1 == 1) {
      pageNumber = 0;
      home.init();
      home.setPosition(0);
    }
  }

buttonNumber = -1;
}