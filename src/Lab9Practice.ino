SYSTEM_MODE(MANUAL);
SYSTEM_THREAD(ENABLED);

uint16_t value;

#define pot A1
#define button D4

int firstPotValue = NULL;
int secondPotValue = NULL;
int lowValue = NULL;
int highValue = NULL;

void setup()
{
  pinMode(pot, INPUT);
  pinMode(button, INPUT);
}

void loop()
{
  bool buttonValue = digitalRead(button);

  if (buttonValue == true)
  {
    Serial.println("button pushed");

    if (firstPotValue == NULL)
    {
      firstPotValue = analogRead(pot);
    }
    else if (secondPotValue == NULL)
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
  }

  delay(1000);
}