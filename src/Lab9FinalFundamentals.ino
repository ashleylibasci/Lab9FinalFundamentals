#include "env.h"

#include <blynk.h>
#include <Wire.h>
#include "SparkFun_VCNL4040_Arduino_Library.h"
#include "oled-wing-adafruit.h"
#include "Particle.h"

SYSTEM_THREAD(ENABLED);

VCNL4040 proximitySensor;
OledWingAdafruit display;

#define greenLED D7
#define yellowLED D6
#define blueLED D5
#define TMP A4

long startingProxValue = 0;
long deltaNeeded = 0;
boolean nothingThere = false;

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

  if (display.pressedA())
  {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println(ambientValue);
    display.display();
  }

  if (display.pressedB())
  {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println(celsius);
    display.setCursor(0, 15);
    display.println(farenheit);
    display.display();
  }
}