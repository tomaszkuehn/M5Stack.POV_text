// ----------------------------------------------------------------------------
// Copyright (C) 2020, Tomasz Kuehn
//
// These coded instructions, statements, and computer programs are free 
// for personal use.
//
// Maintainer: Tomasz Kuehn
//
// Description: Magic wand main code
// ----------------------------------------------------------------------------

// define must ahead #include <M5Stack.h>
#define M5STACK_MPU6886 
// #define M5STACK_MPU9250 
// #define M5STACK_MPU6050
// #define M5STACK_200Q

#include <M5Stack.h>
#include "Display_array.h"
#include "pixels.h"

float accX = 0.0F;
float accY = 0.0F;
float accZ = 0.0F;

float gyroX = 0.0F;
float gyroY = 0.0F;
float gyroZ = 0.0F;

float pitch = 0.0F;
float roll  = 0.0F;
float yaw   = 0.0F;

float temp = 0.0F;

FireNeopixels fnp;

enum DispState {
  Start,
  Disp,
  Done
};

DispState dispState;
int dispPos;
CRGB col;
int brightness = 10;
 


// the setup routine runs once when M5Stack starts up
void setup(){

  // Initialize the M5Stack object
  M5.begin();
  /*
    Power chip connected to gpio21, gpio22, I2C device
    Set battery charging voltage and current
    If used battery, please call this function in your project
  */
  M5.Power.begin();
    
  M5.IMU.Init();

  M5.Lcd.setBrightness(10);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(GREEN , BLACK);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(10, 20);
  M5.Lcd.printf("Ready");
  dispState = Done;
  dispPos = 0;
  col = CRGB(brightness, 0, brightness);
}

void test() {
  
      for(int i = 0; i < 3; i++) {
        
          fnp.leds[i] = col;
        
      }
     
      while(1) {
        M5.update();
        if (M5.BtnA.wasReleased()) {
          fnp.update();
        }
        if (M5.BtnB.wasReleased()) {
          fnp.off();
        }
        delay(10);
        M5.Lcd.setCursor(10, 40);
        //M5.Lcd.printf("%d ", i);
      }
      fnp.update();
        
        delay(1000);
        fnp.off();
        //fnp.update();
        delay(1000);
        fnp.on();
       
        M5.Lcd.printf("Done");
}

// the loop routine runs over and over again forever
void loop() {
  int i;

  //test();
  //while(1);
   
  if(dispState != Disp) {
    M5.update();
    if (M5.BtnA.wasReleased()) {
      brightness+=50;
      if(brightness > 250) {
        brightness = 50;
      }
      fnp.off();
      col = CRGB(brightness, 0, brightness);
      i = brightness/50;
      if(i > 0) {i--;}
      fnp.leds[i] = col;
      fnp.update();
      delay(1000);
    }
  //M5.IMU.getGyroData(&gyroX,&gyroY,&gyroZ);
  M5.IMU.getAccelData(&accX,&accY,&accZ);
  //M5.IMU.getAhrsData(&pitch,&roll,&yaw);
  //M5.IMU.getTempData(&temp);
  /*
  M5.Lcd.setCursor(0, 40);
  M5.Lcd.printf("%6.2f  %6.2f  %6.2f      ", gyroX, gyroY, gyroZ);
  //M5.Lcd.setCursor(220, 42);
  //M5.Lcd.print(" o/s");
  M5.Lcd.setCursor(0, 65);
  M5.Lcd.printf(" %5.2f   %5.2f   %5.2f   ", accX, accY, accZ);
  //M5.Lcd.setCursor(220, 87);
  //M5.Lcd.print(" G");
  M5.Lcd.setCursor(0, 110);
  M5.Lcd.printf(" %5.2f   %5.2f   %5.2f   ", pitch, roll, yaw);
  //M5.Lcd.setCursor(220, 132);
  //M5.Lcd.print(" degree");
  //M5.Lcd.setCursor(0, 155);
  //M5.Lcd.printf("Temperature : %.2f C", Atemp.get());
  */
  }
  
  if(dispState == Start && accZ < 0.75) {
    dispState = Disp;
    //M5.Lcd.fillRect(0, 0, 320, 240, TFT_BLACK);
  }
  if(dispState == Done && accZ > 0.85) {
    dispState = Start;
    fnp.leds[0] = CRGB(0, 1, 0);
    fnp.update();
  }

  if(dispState == Disp) {
    
    if(dispPos < 60) {
      for(i = 0; i < 5; i++) {
        if(myText[dispPos][i]) {
          fnp.leds[i] = col;
          fnp.leds[9-i] = col;
        }
        else
        {
          fnp.leds[i] = 0;
          fnp.leds[9-i] = 0;
        }
      }
      fnp.update();
      delayMicroseconds(2600);
      //M5.Lcd.setCursor(10, 220);
      //M5.Lcd.printf("%d ", dispPos);
    }
    if(dispPos >= 60){
      dispState = Done;
      dispPos = 0;
      fnp.off();
    }
    else
    {
      dispPos++;
    }
  }
}
