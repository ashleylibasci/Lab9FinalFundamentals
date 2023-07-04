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
SYSTEM_THREAD(ENABLED); //enables system

VCNL4040 proximitySensor; //declares VCNL & Adafruit as things
OledWingAdafruit display;

#define greenLED D7 //gives names for everything
#define yellowLED D6
#define blueLED D5
#define TMP A4
#define pot A1
#define button D4

uint16_t value;

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  Blynk.begin(BLYNK_AUTH_TOKEN);

  pinMode(greenLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
  pinMode(TMP, INPUT);
  pinMode(pot, INPUT);
  pinMode(button, INPUT);

  display.setup();
  display.clearDisplay();
  display.display();

  proximitySensor.begin();
  proximitySensor.powerOffProximity();
  proximitySensor.powerOnAmbient();

  if (proximitySensor.begin() == false)
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
  display.loop();
  Blynk.run();

  unsigned int ambientValue = proximitySensor.getAmbient();

  uint64_t reading = analogRead(TMP);
  double voltage = (reading * 3.3) / 4095.0;
  double celsius = (voltage - 0.5) * 100;
  double farenheit = (celsius * 9 / 5) + 32;

  Blynk.virtualWrite(V0, ambientValue);
  Blynk.virtualWrite(V1, celsius);
  Blynk.virtualWrite(V2, farenheit);

  value = analogRead(pot);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println(value);
  display.display();

  if (ambientValue <= 250)
  {
    digitalWrite(blueLED, HIGH);
    digitalWrite(yellowLED, LOW);
    digitalWrite(greenLED, LOW);
    Blynk.logEvent("tooDark");
  }
  else if (ambientValue >= 9000)
  {
    digitalWrite(yellowLED, HIGH);
    digitalWrite(blueLED, LOW);
    digitalWrite(greenLED, LOW);
    Blynk.logEvent("tooBright");
  }
  else
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
