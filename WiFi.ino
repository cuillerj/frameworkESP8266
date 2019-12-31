
void InitConfig()
{
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
  Udp.begin(udpListenPort);
  delay (5000);
  //  TraceToUdp("Ready! Use ", 0x02);
}

void ScanWifi() {
  Serial.println("scan start");

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0)
    Serial.println("no networks found");
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
      delay(10);
    }
  }
  Serial.println("");
}
void ConnectWifi(char *ssid, char *pass)
{
  retryWifi++;
  timeCheckWifi = millis();
  if (ssid == "default")
  {
    WiFi.begin();
  }
  else
  {
    WiFi.begin(ssid, pass);
  }
  if (digitalRead(configPin) == true)
  {
    Serial.println();
    Serial.print(app);
    Serial.println(versionID);
    Serial.print("\n Please wait... Connecting to ");
    Serial.print(ssid);
    Serial.print(":");
    Serial.println(retryWifi);    
  }
  uint8_t i = 0;
  while (WiFi.status() != WL_CONNECTED && i++ < 5)
  {
    delay(2000);
    if (i == 5) {
      if (digitalRead(configPin) == true) //  config mode
      {
        Serial.print("Could not connect to "); Serial.println(ssid);
      }
      delay(500);
      bitWrite(diagByte, bitDiagWifi, 1);       // position bit diag
      Udp.stop();

    }
    else
    {
      bitWrite(diagByte, bitDiagWifi, 0);       // position bit diag
      WiFi.mode(WIFI_STA);
      Udp.begin(udpListenPort);
      delay (5000);
      if (digitalRead(configPin) == true) //  run mode
      {
        WiFi.printDiag(Serial);
      }
    }
  }
}
void Restart() {
  Udp.stop();
  bitWrite(diagByte, bitDiagWifi, 0);       // position bit diagc              digitalWrite(connextionLED,false);
  WiFi.disconnect(true);
  delay(1000);
  restartCompleted = false;
  timeRestart = millis();
  retryWifi = 0x00;
  InitConfig();
}
