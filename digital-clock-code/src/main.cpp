#include <Arduino.h>
#include <NeoPixelBus.h>
#include <MCP7940.h>

#define LED_BY_SEGMENT 3
#define SEGMENT 7
#define DIGIT 4

#define MINUTE_PIN 14
#define HOUR_PIN 15

#define SEVEN_SEGMENT 8

int one[] = {0, 2};
int two[] = {3, 5};
int three[] = {6, 8};
int four[] = {9, 11};
int five[] = {12, 14};
int six[] = {15, 17};
int seven[] = {18, 20};

int digit_zero[6] = {0, 1, 2, 4, 5, 6};
int digit_one[2] = {0, 4};
int digit_two[5] = {0, 1, 3, 5, 6};
int digit_three[5] = {0, 1, 3, 4, 5};
int digit_four[4] = {0, 2, 3, 4};
int digit_five[5] = {1, 2, 3, 4, 5};
int digit_six[6] = {1, 2, 3, 4, 5, 6};
int digit_seven[3] = {0, 1, 4};
int digit_eight[7] = {0, 1, 2, 3, 4, 5, 6};
int digit_nine[6] = {0, 1, 2, 3, 4, 5};

int hour = 0;
int minute = 0;

const uint16_t PixelCount = LED_BY_SEGMENT * SEGMENT * DIGIT; // this example assumes 4 pixels, making it smaller will cause a failure
const uint8_t PixelPin = 9;  // make sure to set this to the correct pin, ignored for Esp8266

int colorSaturation = 30;

MCP7940_Class MCP7940;

// three element pixels, in different order and speeds
NeoPixelBus<NeoGrbFeature, NeoWs2812xMethod> strip(PixelCount, PixelPin);

RgbColor red(colorSaturation, 0, 0);
RgbColor green(0, colorSaturation, 0);
RgbColor blue(0, 0, colorSaturation);
RgbColor white(colorSaturation);
RgbColor black(0);

HslColor hslRed(red);
HslColor hslGreen(green);
HslColor hslBlue(blue);
HslColor hslWhite(white);
HslColor hslBlack(black);

RgbColor randomColor();
int* getNumberFromInt(int);
int getSegment(int);
void setDigit(int *, int, int);

void resetDigit(int position) {
  for(int i = (LED_BY_SEGMENT * SEGMENT * (position - 1)); i < (LED_BY_SEGMENT * SEGMENT * (position)); i++){
    strip.SetPixelColor(i, black);
    strip.Show();
  }
  
}

void setup()
{

  pinMode(MINUTE_PIN, INPUT);
  pinMode(HOUR_PIN, INPUT);
 
  strip.Begin();
  strip.SetPixelColor(0, red);
  strip.Show();
  
  while (!MCP7940.begin()) {
    strip.SetPixelColor(0, white);
    strip.Show();
  }
  
  while (!MCP7940.deviceStatus()) {
    bool deviceStatus = MCP7940.deviceStart();
    if (!deviceStatus) {
     strip.SetPixelColor(0, blue);
     strip.Show();
    }
  }
  MCP7940.adjust();
}


void loop()
{
  DateTime now = MCP7940.now();

  if (now.minute() != minute) {
    unsigned first_minute = (now.minute() / 10U) % 10;
    unsigned second_minute = (now.minute() / 1U) % 10;
    setDigit(getNumberFromInt(first_minute), getSegment(first_minute), 3);
    setDigit(getNumberFromInt(second_minute), getSegment(second_minute), 4);
    minute = now.minute();
  }
  
  if (now.hour() != hour) {
    if (now.hour() > 22 || now.hour() < 7) {
      colorSaturation = 20;
    } else {
      colorSaturation = 50;
    }
    unsigned first_hour = (now.hour() / 10U) % 10;
    unsigned second_hour = (now.hour() / 1U) % 10;
    setDigit(getNumberFromInt(first_hour), getSegment(first_hour), 1);
    setDigit(getNumberFromInt(second_hour), getSegment(second_hour), 2);
    hour = now.hour();
  }



  if (digitalRead(MINUTE_PIN) == HIGH) {
    MCP7940.adjust(DateTime(now.year(), now.month(), now.day(), now.hour(), now.minute() + 1, now.second()));
    delay(200);
  }

  if (digitalRead(HOUR_PIN) == HIGH) {
    MCP7940.adjust(DateTime(now.year(), now.month(), now.day(), now.hour() + 1, now.minute(), now.second()));
    delay(400);
  }
}

void setDigit(int *digit, int segment, int position) {
  resetDigit(position);
  for(int i = 0; i < segment; i++) {
    strip.SetPixelColor((digit[i]*3)+(LED_BY_SEGMENT * SEGMENT * (position - 1)), randomColor());
    strip.SetPixelColor((digit[i]*3+1)+(LED_BY_SEGMENT * SEGMENT * (position - 1)), randomColor());
    strip.SetPixelColor((digit[i]*3+2)+(LED_BY_SEGMENT * SEGMENT * (position - 1)), randomColor());
  }
  strip.Show();
}

RgbColor randomColor() {
  return RgbColor (random(30, 150), random(30, 150), random(30, 150));
}

int* getNumberFromInt(int number) {
  if (number == 0) {
    return digit_zero;
  } else if (number == 1) {
    return digit_one;
  } else if (number == 2) {
    return digit_two;
  } else if (number == 3) {
    return digit_three;
  } else if (number == 4) {
    return digit_four;
  } else if (number == 5) {
    return digit_five;
  } else if (number == 6) {
    return digit_six;
  } else if (number == 7) {
    return digit_seven;
  } else if (number == 8) {
    return digit_eight;
  } else if (number == 9) {
    return digit_nine;
  }
}

int getSegment(int number) {
  if (number == 0) {
    return 6;
  } else if (number == 1) {
    return 2;
  } else if (number == 2) {
    return 5;
  } else if (number == 3) {
    return 5;
  } else if (number == 4) {
    return 4;
  } else if (number == 5) {
    return 5;
  } else if (number == 6) {
    return 6;
  } else if (number == 7) {
    return 3;
  } else if (number == 8) {
    return 7;
  } else if (number == 9) {
    return 6;
  }
}