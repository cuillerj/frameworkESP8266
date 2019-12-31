
void SendStatus() {
  String req = "";
  req += stAddr;
  req += "/status";
  req += "/ind_id/";
  req += 1;
  req += "=";
  req += diagByte;
  req += "/ind_id/";
  req += 2;
  req += "=";
  req += versionID;
  req += "/ind_id/";
  req += 3;
  req += "=";
  req += (millis() / 60000);
  req += "\\End"; // required end off frame
  req += "\r\n";  // marqueur de fin indispensable
  req += 0x00;    // marqueur de fin indispensable

#if defined(debugModeOn)
  Serial.print("Status: ");
  Serial.println(req);
#endif
  PrepareSendToUdp(req, serviceDispatcher);
  // This will send the request to the server
  delay(10);
}
void SendIndicators() {
  String req = "";
  req += stAddr;
  req += "/indicators";
  req += "/ind_id/";
  req += 4;
  req += "=";
  req += digitalRead(inPin1);
  req += "/ind_id/";
  req += 5;
  req += "=";
  req += analogRead(A0);
  req += "/ind_id/";
  req += 6;
  req += "=";
  req += indicator6Value;
  req += "/ind_id/";
  req += 7;
  req += "=";
  req += random(-2000, 2000);
  req += "/ind_id/";
  req += outPinIndicator;
  req += "=";
  req += outPinValue;
  req += "\\End"; // required end off frame
  req += "\r\n";  //  required end off frame
  req += 0x00;    //  required end off frame

#if defined(debugModeOn)
  Serial.print("Indicators: ");
  Serial.println(req);
#endif
  PrepareSendToUdp(req, serviceDispatcher);
  // This will send the request to the server
}
void SendRegisters() {
  String req = "";
  req += stAddr;
  req += "/registers";
  req += "/reg_id/";
  req += registerUpdateSheetCycle;
  req += "=";
  req += Registers[registerUpdateSheetCycle];
  req += "/reg_id/";
  req += registerUpdateStatusCycle;
  req += "=";
  req += Registers[registerUpdateStatusCycle];
  req += "\\End"; // required end off frame
  req += "\r\n";  //  required end off frame
  req += 0x00;    //  required end off frame

#if defined(debugModeOn)
  Serial.print("Registers: ");
  Serial.println(req);
#endif
  PrepareSendToUdp(req, serviceDispatcher);
  // This will send the request to the server
}
void SendToGoogleSheet(int number, int values[]) {
#define maxValuesToSend 10  // 
  number = min(number, maxValuesToSend);
  String req = "";
  req += stAddr;
  req += "/insertDataInSheet";
  for (int i = 0; i < number; i++) {
    req += "/row_id/";
    req += i;
    req += "=";
    req += values[i];
  }
  req += "\\End"; // required end off frame
  req += "\r\n";  //  required end off frame
  req += 0x00;    //  required end off frame
#if defined(debugModeOn)
  Serial.print("sheet: ");
  Serial.println(req);
#endif
  PrepareSendToUdp(req, serviceDispatcher);
  // This will send the request to the server
}
void SendToDatabase(int number, int type, int values[]) {
#define maxValuesToSend 10  // 
  number = min(number, maxValuesToSend);
  String req = "";
  req += stAddr;
  req += "/insertDataInDatabase";
  for (int i = 0; i < number; i++) {
    req += "/value/";
    req += type;
    req += "=";
    req += values[i];
  }
  req += "\\End"; // required end off frame
  req += "\r\n";  //  required end off frame
  req += 0x00;    //  required end off frame
#if defined(debugModeOn)
  Serial.print("database: ");
  Serial.println(req);
#endif
  PrepareSendToUdp(req, serviceDispatcher);
  // This will send the request to the server

}
void SendTimeRequested() {
  String req = "";
  req += stAddr;
  req += "/timeUpdateRequest/";
  req += "\\End"; // required end off frame
  req += "\r\n";  //  required end off frame
  req += 0x00;    //  required end off frames
#if defined(debugModeOn)
  Serial.print("time request: ");
  Serial.println(req);
#endif
  PrepareSendToUdp(req, serviceDispatcher);
  // This will send the request to the server
}
void PrepareSendToUdp(String req, uint8_t serviceId) {
  byte dataBin[maxSendDataLenght + 1];
  int len = 0;
  for (int i = 0; i <= maxSendDataLenght; i++)
  {
    if (req[i] == 0x00)
    {
      len = i - 3;
      break;
    }
    dataBin[i] = req[i];

  }
  SendToUdp(dataBin, len, serviceId);
}
void SendToUdp(byte * msg2, int mlen, uint8_t serviceId) {
#if defined(debugModeOn)
  Serial.print("send UDP port:");
  Serial.print(udpPort[serviceId]);
  Serial.print(" len:");
  Serial.println(mlen);
#endif
  Udp.beginPacket(serverIP[serviceId], udpPort[serviceId]);
  Udp.write(msg2, mlen);
  Udp.endPacket();
  timeLastSentUdp = millis();
}
void SendToUdp( int mlen, int serviceId) {
  if ( millis() - timeLastSentUdp  > 200)
  {
    delay (200);
  }
  Udp.beginPacket(serverIP[serviceId], udpPort[serviceId]);
  Udp.write(dataBin, mlen);
  Udp.endPacket();
  timeLastSentUdp = millis();
}
