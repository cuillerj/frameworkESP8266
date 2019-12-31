void StandardLoop()
{
  delay(50);
  if (millis() - timeRestart >= 10000 && restartCompleted == false)
  {
    restartCompleted = true;
    if (CheckCurrentIP())                    // check ip address
    {
      if (configPin == true)
      {
        Serial.print("Ready to use ! ");
        PrintUdpConfig();
      }
    }
    else
    {
      if (configPin == true)
      {
        Serial.print("Not ready to use ! ");
        PrintUdpConfig();
      }
    }
    Udp.begin(udpListenPort);
    delay (5000);
    SendStatus();
    if (bitRead(diagByte, bitDiagReboot))
    {
      bitWrite(diagByte, bitDiagReboot, 0);
    }
    if (configPin == true)
    {
      WiFi.printDiag(Serial);
    }
  }
  if ((bitRead(diagByte, bitDiagWifi) || bitRead(diagByte, bitDiagIP)) &&  millis()  > diagTime + diagTimeCycle &&  retryWifi <= 5)
  {
#if defined debugModeOn
    Serial.println("request default wifi connection");
#endif

    diagTime = millis();
    if (currentSSID == 0x00)
    {
      ConnectWifi("default", "default");
    }
    if (currentSSID == 0x01 )
    {
      ConnectWifi(ssid1, pass1);
    }
    if (currentSSID == 0x02 )
    {
      ConnectWifi(ssid2, pass2);
    }
    WiFi.mode(WIFI_STA);
  }

  if (millis() - timeSerial >= 20)
  {
    int lenInput = Serial_have_message();
    {
      if (lenInput != 0)
      {

      }
    }
    timeSerial = millis();
  }


  if (millis() - timeCheck >= 30000)
  {
    boolean wifiStatus = false;
    if (WiFi.status() == WL_CONNECTED)
    {
      wifiStatus = true;
      bitWrite(diagByte, bitDiagWifi, 0);
#if defined debugModeOn
      Serial.println("wifi connected ");
#endif
    }
    else {
      bitWrite(diagByte, bitDiagWifi, 1);
#if defined debugModeOn
      Serial.println("wifi not connected ");
#endif
    }
    connectedStatus = CheckCurrentIP() && wifiStatus;

    freeMemory = ESP.getFreeHeap();
    String  strS = "free memory:" + String(freeMemory);

    if (debugMode == true)
    {
      Serial.println(strS);
    }
    timeCheck = millis();
  }
  if (millis() - timeUdp >= 100)
  {
    InputUDP();
    timeUdp = millis();
  }
  // *** refresh LED
  if (millis() - timeAffLED > 1000 && restartCompleted == true)
  {
    AffLed();
    timeAffLED = millis();
  }
  if (millis() - timeUpdateStatus >= updateStatusCycle)
  {
    SendStatus();
    timeUpdateStatus = millis();
  }

  if (!connectedStatus && retryWifi > 5)
  {
    currentSSID = (currentSSID + 1) % 3;
    Restart();
    timeCheckWifi = millis();
  }
#if defined TimeOn
  if ( bitRead(diagByte, diagTimeUpToDate) && (millis() - pendingRequestClock > 30000))
  {
    SendTimeRequested();
    pendingRequestClock = millis();
  }
#endif
  if (!bitRead(diagByte, diagTimeUpToDate) && (millis() - lastUpdateClock > updateClockLimit) && (millis() - pendingRequestClock > 30000))
  {
    bitWrite(diagByte, diagTimeUpToDate, 1);
  }

}
