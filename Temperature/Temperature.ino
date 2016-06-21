

/*
 *    TemperatureExample.ino - Temperature example using the Petduino library
 *    Copyright (c) 2015 Circuitbeard
 *
 *    Permission is hereby granted, free of charge, to any person
 *    obtaining a copy of this software and associated documentation
 *    files (the "Software"), to deal in the Software without
 *    restriction, including without limitation the rights to use,
 *    copy, modify, merge, publish, distribute, sublicense, and/or sell
 *    copies of the Software, and to permit persons to whom the
 *    Software is furnished to do so, subject to the following
 *    conditions:
 *
 *    This permission notice shall be included in all copies or
 *    substantial portions of the Software.
 *
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *    OTHER DEALINGS IN THE SOFTWARE.
 */

#include <LedControl.h>
#include <Petduino.h>
#include <RunningAverage.h>

#define NUMBER_COUNT 10
byte numbers[NUMBER_COUNT][8] = {
  {
    B00000000,
    B00000000,
    B11100000,
    B10100000,
    B10100000,
    B10100000,
    B11100000,
    B00000000,
  },
  {
    B00000000,
    B00000000,
    B00100000,
    B00100000,
    B00100000,
    B00100000,
    B00100000,
    B00000000,
  },
  {
    B00000000,
    B00000000,
    B11100000,
    B00100000,
    B11100000,
    B10000000,
    B11100000,
    B00000000,
  },
  {
    B00000000,
    B00000000,
    B11100000,
    B00100000,
    B11100000,
    B00100000,
    B11100000,
    B00000000,
  },
  {
    B00000000,
    B00000000,
    B10100000,
    B10100000,
    B11100000,
    B00100000,
    B00100000,
    B00000000,
  },
  {
    B00000000,
    B00000000,
    B11100000,
    B10000000,
    B11100000,
    B00100000,
    B11100000,
    B00000000,
  },
  {
    B00000000,
    B00000000,
    B11100000,
    B10000000,
    B11100000,
    B10100000,
    B11100000,
    B00000000,
  },
  {
    B00000000,
    B00000000,
    B11100000,
    B00100000,
    B00100000,
    B00100000,
    B00100000,
    B00000000,
  },
  {
    B00000000,
    B00000000,
    B11100000,
    B10100000,
    B11100000,
    B10100000,
    B11100000,
    B00000000,
  },
  {
    B00000000,
    B00000000,
    B11100000,
    B10100000,
    B11100000,
    B00100000,
    B11100000,
    B00000000,
  }
};

float temp;
float displayAverage;
unsigned int ones, tens;
int ledLight;

unsigned int currentInterval;
#define INTERVAL 10000
#define SLOW_INTERVAL 10000
#define FAST_INTERVAL 1000
// the interval in mS 
unsigned long previousMillis = 0;        // will store last time LED was updated

RunningAverage data = RunningAverage(10);
Petduino pet = Petduino();

void setup() {
  
  // initialize serial:
  Serial.begin(9600);
  
  currentInterval = INTERVAL;
  previousMillis = millis();
  
  // Setup Petduino
  pet.begin();
  ledLight = 8;

}

void loop() {

  // Update pet
  pet.update();

  // check to see if it's time to check the temperature; that is, if the
  // difference between the current time and last time you blinked
  // the LED is bigger than the interval at which you want to
  // blink the LED.
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= currentInterval) {
    Serial.println("Checking the temperature");
    // save the last time you blinked the LED 
    previousMillis = currentMillis;

    // Get the temperature
    temp = pet.getTemperature();
    data.addValue(temp);
    displayAverage = data.getAverage();
    displayAverage = (displayAverage * 9.0)/ 5.0 + 32.0;
  
    // Split the digits
    tens = displayAverage/10;
    ones = displayAverage-tens*10;
  
    // Generate & draw number graphic
    for(int b = 0; b < 8; b++){
      pet.drawRow(b, numbers[tens][b] | numbers[ones][b] >> 4);
    }
  }
  
  // Check button 1
  if(pet.isBtn1Pressed()) {
    ledLight -= 1;
    Serial.print("button 1 pressed index = ");
    Serial.println( ledLight );
    pet.setScreenBrightness(ledLight);
  }
  
  // Check button 2
  if(pet.isBtn2Pressed()) {
    ledLight += 1;
        Serial.print("button 2 pressed index = ");
    Serial.println( ledLight );
    pet.setScreenBrightness(ledLight);
    
  }
  
  // Wait for a second
  //delay(1000);

}
