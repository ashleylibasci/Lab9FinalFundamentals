/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#line 1 "/Users/ashleylibasci/IoTFolderLabs/Lab9FinalFundamentals/src/Lab9FinalFundamentals.ino"
#include "env.h" //including everything

#include <blynk.h>
#include <Wire.h>
#include "SparkFun_VCNL4040_Arduino_Library.h"
#include "oled-wing-adafruit.h"
#include "Particle.h"

void setup();
void loop();
#line 9 "/Users/ashleylibasci/IoTFolderLabs/Lab9FinalFundamentals/src/Lab9FinalFundamentals.ino"
SYSTEM_THREAD(ENABLED); // enables system

VCNL4040 proximitySensor; // declares VCNL & Adafruit as things
OledWingAdafruit display;

#define greenLED D7 // gives names for everything
#define yellowLED D6
#define blueLED D5
#define TMP A4
#define pot A1
#define button D4

uint16_t value; // uint

unsigned int firstPotValue = 0;
unsigned int secondPotValue = 0;
unsigned int lowValue = 0;
unsigned int highValue = 0;

void setup()
{
  Serial.begin(9600); // starts up everything
  Wire.begin();
  Blynk.begin(BLYNK_AUTH_TOKEN);

  pinMode(greenLED, OUTPUT); // says whether things are inputs/outputs
  pinMode(yellowLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
  pinMode(TMP, INPUT);
  pinMode(pot, INPUT);
  pinMode(button, INPUT);

  display.setup(); // starts up display
  display.clearDisplay();
  display.display();

  proximitySensor.begin(); // starts prox sensor
  proximitySensor.powerOffProximity();
  proximitySensor.powerOnAmbient();

  if (proximitySensor.begin() == false) // confirms if prox sensor is connected
  {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println("check wiring");
    display.display();
    while (1)
      ;
  }
}

void loop()
{
  display.loop(); // starts display and blynk
  Blynk.run();

  bool buttonValue = digitalRead(button);

  unsigned int ambientValue = proximitySensor.getAmbient(); // always getting the ambient values

  uint64_t reading = analogRead(TMP);        // read TMP
  double voltage = (reading * 3.3) / 4095.0; // calculations for TMP
  double celsius = (voltage - 0.5) * 10;
  double farenheit = (celsius * 9 / 5) + 32;

  Blynk.virtualWrite(V0, ambientValue); // sends data to blynk
  Blynk.virtualWrite(V1, celsius);
  Blynk.virtualWrite(V2, farenheit);

  value = analogRead(pot); // printing out potentiometer values
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println(value);
  display.display();

  if (buttonValue == true)
  {
    if (firstPotValue == 0)
    {
      firstPotValue = analogRead(pot);
    }
    else if (secondPotValue == 0)
    {
      secondPotValue = analogRead(pot);
      if (firstPotValue < secondPotValue)
      {
        lowValue = firstPotValue;
        highValue = secondPotValue;
      }
      else
      {
        highValue = firstPotValue;
        lowValue = secondPotValue;
      }
      Serial.println("lowValue:");
      Serial.println(lowValue);
      Serial.println("highValue:");
      Serial.println(highValue);
    }

    if (ambientValue <= lowValue) // if light too dark go blue
    {
      digitalWrite(blueLED, HIGH);
      digitalWrite(yellowLED, LOW);
      digitalWrite(greenLED, LOW);
      Blynk.logEvent("tooDark");
    }
    else if (ambientValue >= highValue) // if light too bright go yellow
    {
      digitalWrite(yellowLED, HIGH);
      digitalWrite(blueLED, LOW);
      digitalWrite(greenLED, LOW);
      Blynk.logEvent("tooBright");
    }
    else // if light fine go green
    {
      digitalWrite(greenLED, HIGH);
      digitalWrite(blueLED, LOW);
      digitalWrite(yellowLED, LOW);
    }

    // if (display.pressedA())
    // {
    //   display.clearDisplay();
    //   display.setTextSize(1);
    //   display.setTextColor(WHITE);
    //   display.setCursor(0, 0);
    //   display.println(ambientValue);
    //   display.display();
    //   Serial.println(ambientValue);
    // }

    // if (display.pressedB())
    // {
    //   display.clearDisplay();
    //   display.setTextSize(1);
    //   display.setTextColor(WHITE);
    //   display.setCursor(0, 0);
    //   display.println(celsius);
    //   display.setCursor(0, 15);
    //   display.println(farenheit);
    //   display.display();
    //   Serial.println(farenheit);
    //   Serial.println(celsius);
    // }
  }
}
