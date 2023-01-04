#include <FastLED.h>
#define DATA_PIN 2

const uint8_t matrixW = 32;
const uint8_t matrixH = 32;
const uint8_t NUM_LED = matrixW*matrixH;

const int LAYER_NUM = 8;
const int LIGHT_TRESH = 160; //0~255
const uint8_t LIGHT_THRESH = 160;

CRGB leds[NUM_LED];

uint16_t shadowScaleX = 18;//0.07;
uint16_t shadowScaleY = 12;//0.05;
uint8_t leaves[LAYER_NUM][matrixW][matrixH];
uint8_t result[matrixW][matrixH];
uint16_t shadowNoiseBase[LAYER_NUM];
uint16_t windBase[LAYER_NUM];
uint16_t windDirection[LAYER_NUM][2];
uint16_t trembling =0;
uint16_t tremblingSpeed =5;//0.02;
uint16_t windSpeed = 1;//0.005;
uint16_t windNoiseZ = 0.5;
uint16_t smallWind = 0;
uint16_t smallWindSpeed = 8;//0.03;
uint16_t smallwindRange = 0.001;
uint16_t smallWindNoiseZ = 0.3;

void setup() {
  // put your setup code here, to run once:
  delay(1000);
  initMotion();
  initLed();
}

void loop() {
  // put your main code here, to run repeatedly:
  calcMotion();
  FastLED.show();
}

void initMotion(){
  for(int l=0;l<LAYER_NUM;l++){
    shadowNoiseBase[l]=l*2550;//random(0,5);
    windBase[l]=random(0,50);//random(0,0.2);
    windDirection[l][0]=3;//random(-2,2);
    windDirection[l][1]=random(0,2);
  }
}

void initLed(){
  FastLED.addLeds<WS2811,DATA_PIN>(leds,NUM_LED);
}

void calcMotion(){
  uint16_t tempNoise = 0;
  uint16_t windRangeX=0, windRangeY=0;
  uint16_t finalNoiseX=0, finalNoiseY=0;
  for(uint16_t l=0;l<LAYER_NUM;l++){
    for(uint16_t x=0;x<matrixW;x++){
      for(uint16_t y=0;y<matrixH;y++){
        windRangeX=inoise8(windBase[l])*(windDirection[l][0]+(y*0.01));
        windRangeY=inoise8(windBase[l])*(windDirection[l][1]+(x*0.01));
        finalNoiseX=shadowNoiseBase[l]+x*shadowScaleX+windRangeX+inoise8(smallWind)*smallwindRange;
        finalNoiseY=shadowNoiseBase[l]+y*shadowScaleY+windRangeY+inoise8(smallWind)*smallwindRange;

        tempNoise = inoise8(finalNoiseX,
                          finalNoiseY,
                          (windRangeX+windRangeY)*0.3+inoise8(smallWind)*0.2);
        leaves[l][x][y] = tempNoise;

        if(leaves[l][x][y]>LIGHT_THRESH) leaves[l][x][y]=map(leaves[l][x][y],LIGHT_THRESH,LIGHT_THRESH+60,0,255);
        else leaves[l][x][y]=0;
      }
    }
    windBase[l]+=windSpeed;
  }
  trembling+=tremblingSpeed;
  smallWind+=smallWindSpeed;
}

void fillLed(){
  for(uint8_t x=0;x<matrixW;x++){
    for(uint8_t y=0;y<matrixH;y++){
      result[x][y] = 0;
      for(uint8_t l=0;l<LAYER_NUM;l++){
        result[x][y] += leaves[l][x][y];
        constrain(result[x][y],0,255);
      }
      leds[YX(x,y)] = CRGB(result[x][y]);
    }
  }
}

uint16_t YX(uint8_t x, uint8_t y){
  return x*matrixH+y;
}
