#include <Adafruit_NeoPixel.h>
#include "ShieldPattern.h"

#define PIN_SMALL 4
#define PIN_LARGE 6
#define LEDS_SMALL 9
#define LEDS_LARGE 13
#define MIN_CHASE_SPEED 60
#define MAX_CHASE_SPEED 30
#define CHASE_SPEED_INTERVAL 2

ShieldPattern small_circle = ShieldPattern(LEDS_SMALL, PIN_SMALL, NEO_GRB + NEO_KHZ800, &SmallCircleComplete);
ShieldPattern large_circle = ShieldPattern(LEDS_LARGE, PIN_LARGE, NEO_GRB + NEO_KHZ800, &LargeCircleComplete);

bool speedChange = false;
bool accelerating_small = true;
bool accelerating_large = true;

String pattern = "rainbow";
int lastButtonVal = HIGH;
const int buttonPin = 3;
uint16_t cycles = 1;

void setup() {
  pinMode(buttonPin, INPUT);
  digitalWrite(buttonPin, HIGH);
  small_circle.begin();
  large_circle.begin();

  small_circle.RainbowCycle(1);
  large_circle.RainbowCycle(1);

  attachInterrupt(digitalPinToInterrupt(buttonPin), ButtonHandler, CHANGE);
//  small_circle.TheaterChase(small_circle.Color(255,255,0), small_circle.Color(0,0,50), 100);
//  large_circle.TheaterChase(large_circle.Color(255,255,0), large_circle.Color(0,0,50), 100);
}

void loop() {
  small_circle.Update();
  large_circle.Update();
}

void ButtonHandler() {
  if (digitalRead(buttonPin) == HIGH) {
//    Serial.begin(9600);
//    Serial.println();
//    Serial.println("Button Pressed");
//    Serial.println("Changed pattern from " + (String)small_circle.PatternIndex);
    small_circle.NextPattern();
    large_circle.NextPattern();
    delay(2000);
//    Serial.println("Changed pattern to " + (String)small_circle.PatternIndex + " which correlates to " + small_circle.GetPattern());
//    Serial.end();
  }
}

void SmallCircleComplete()
{
  if (small_circle.GetPattern() == "COLOR_WIPE") {
    small_circle.Reverse();
  }
  if (speedChange)
  {
    if (accelerating_small)
    {
      small_circle.Interval -= CHASE_SPEED_INTERVAL;
      if (small_circle.Interval < MAX_CHASE_SPEED)
      {
        accelerating_small = false;
        small_circle.Interval += CHASE_SPEED_INTERVAL;
      }
    }
    else
    {
      small_circle.Interval += CHASE_SPEED_INTERVAL;
      if (small_circle.Interval > MIN_CHASE_SPEED)
      {
        accelerating_small = true;
        small_circle.Interval-= CHASE_SPEED_INTERVAL;
      }
    }
  }
}

void LargeCircleComplete()
{
  if (large_circle.GetPattern() == "COLOR_WIPE") {
    large_circle.Reverse();
  }
  if (speedChange)
  {
    if (accelerating_large)
    {
      large_circle.Interval -= CHASE_SPEED_INTERVAL;
      if (large_circle.Interval < MAX_CHASE_SPEED)
      {
        accelerating_large = false;
        large_circle.Interval += CHASE_SPEED_INTERVAL;
      }
    }
    else
    {
      large_circle.Interval += CHASE_SPEED_INTERVAL;
      if (large_circle.Interval > MIN_CHASE_SPEED)
      {
        accelerating_large = true;
        large_circle.Interval -= CHASE_SPEED_INTERVAL;
      }
    }
  }
}
