void SetTime(char *input) {
#define dateShift 0
#define timeShift 9
  uint8_t jj = ((input[0 + dateShift] - 0x30) * 10 + (input[1 + dateShift] - 0x30));
  uint8_t mm = ((input[3 + dateShift] - 0x30) * 10 + (input[4 + dateShift] - 0x30));
  uint8_t aa = ((input[6 + dateShift] - 0x30) * 10 + (input[7 + dateShift] - 0x30));

  uint8_t hh = ((input[0 + timeShift] - 0x30) * 10 + (input[1 + timeShift] - 0x30));
  uint8_t mn = ((input[3 + timeShift] - 0x30) * 10 + (input[4 + timeShift] - 0x30));
  uint8_t ss = ((input[6 + timeShift] - 0x30) * 10 + (input[7 + timeShift] - 0x30));

  setTime(hh, mn, ss, jj, mm, 2000 + aa);
#if defined(debugModeOn)
  AffTime();
#endif
  lastReceivedTime = millis();
  bitWrite(diagByte, diagTimeUpToDate, 0);       // position bit diag
  lastUpdateClock=millis();
}

void AffTime()
{
#if defined(debugModeOn)
  Serial.println();
  Serial.print(hour());
  Serial.print(":");
  Serial.print(minute());
  Serial.print(":");
  Serial.print(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print("/");
  Serial.print(month());
  Serial.print("/");
  Serial.print(year());
  Serial.print(" ");
  Serial.println(weekday());
#endif
}
