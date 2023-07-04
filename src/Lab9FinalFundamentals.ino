#include "env.h" //including everything

#include <blynk.h>
#include <Wire.h>
#include "SparkFun_VCNL4040_Arduino_Library.h"
#include "oled-wing-adafruit.h"
#include "Particle.h"

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

  unsigned int ambientValue = proximitySensor.getAmbient(); // always getting the ambient values

  uint64_t reading = analogRead(TMP);        // read TMP
  double voltage = (reading * 3.3) / 4095.0; // calculations for TMP
  double celsius = (voltage - 0.5) * 100;
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

  if (ambientValue <= 250) // if light too dark go blue
  {
    digitalWrite(blueLED, HIGH);
    digitalWrite(yellowLED, LOW);
    digitalWrite(greenLED, LOW);
    Blynk.logEvent("tooDark");
  }
  else if (ambientValue >= 9000) // if light too bright go yellow
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
