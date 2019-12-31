void AffLed()
{
  LEDOn = false;
  digitalWrite(LEDPin, 0);
  if (restartCompleted == false)
  {
    //    digitalWrite(connectionLED, 0);
    //   connectionLEDOn = false;
    digitalWrite(LEDPin, 1);

    powerLEDOn = true;
    connectionLEDOn = false;
    return;
  }
  if ((diagByte && 0b00000011) == 0b00000000)
  {
    digitalWrite(LEDPin, 1);
    connectionLEDOn = true;
  }
  if ((diagByte && 0b00000011) == 0b00000011)
  {
    digitalWrite(LEDPin, 0);
    connectionLEDOn = false;
  }
  else {
    if (( diagByte && 0b00000011) == 0b00000001 || ( diagByte && 0b00000011) == 0b00000010)
    {
      connectionLEDOn = !connectionLEDOn;
      digitalWrite(LEDPin, connectionLEDOn);
    }
  }
  if ( diagByte == 0b00000000 && digitalRead(configPin) == false)
  {
    digitalWrite(LEDPin, 1);
    powerLEDOn = true;
  }
  else
  {
    powerLEDOn = !powerLEDOn;
    digitalWrite(LEDPin, 1);
  }
}
