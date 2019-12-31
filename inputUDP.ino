void InputUDP() {

  int packetSize = Udp.parsePacket();  // message from UDP
  remoteAddr = Udp.remoteIP();
  if (packetSize)
  {
    IPAddress remote = Udp.remoteIP();
#if defined(debugModeOn)
    Serial.print("UDPRSize ");
    Serial.print(packetSize);
    Serial.print(": ");
    for (int i = 0; i < 4; i++)
    {
      Serial.print(remote[i], DEC);
      if (i < 3)
      {
        Serial.print(".");
      }
    }
    Serial.print(",port ");
    Serial.print(Udp.remotePort());
    Serial.print(" ");
#endif
#if defined(debugModeOn)
    Serial.print("awnser1:");
    Serial.println(packetSize);
    delay(1000);
#endif
    Udp.read(packetBuffer, packetSize);
    commandReturn rc = UdpInput.GetCommand(packetBuffer); // look for commant inside the input
    int cmdIdx = rc.idxCommand;
    int cmdPos = rc.idxPos;
#if defined(debugModeOn)
    Serial.print("idxCommand:");
    Serial.println(cmdIdx);
#endif
    switch (cmdIdx) {
      case timeResponse: {
          char *ptr = NULL;
          ptr = &packetBuffer[cmdPos];
#if defined(debugModeOn)
          Serial.print("ptr:");
          Serial.print(ptr);
          Serial.print("-");
          Serial.println(*ptr);
          delay(1000);
#endif
          SetTime(ptr);
          break;
        }
      case updateRegistersRequest: {
#if defined(debugModeOn)
          Serial.print("updateRegisterRequest:");
          Serial.print(packetBuffer[cmdPos], HEX);
          Serial.print("-");
          Serial.print(cmdPos);
          Serial.print(" ");
#endif
          for (int i = 0; i < floor((packetSize - cmdPos) / 2) ; i++) {
#if defined(debugModeOn)
            Serial.print(packetBuffer[cmdPos + 2 * i], HEX);
            Serial.print("-");
            Serial.println(packetBuffer[cmdPos + 2 * i + 1], HEX);
#endif
            if (packetBuffer[cmdPos + 2 * i] >= 0 && packetBuffer[cmdPos + 2 * i] < registerSize) {
              Registers[packetBuffer[cmdPos + 2 * i]] = packetBuffer[cmdPos + 2 * i + 1];
            }

#if defined(debugModeOn)
            else {
              Serial.println("unknow register");
            }
#endif
          }
          break;
        }

      case updateIndicatorRequest: {
#if defined(debugModeOn)
          Serial.print("updateIndicatorRequest");
          Serial.print(packetBuffer[cmdPos], HEX);
          Serial.print("-");
          Serial.println(packetBuffer[cmdPos + 1], HEX);
#endif
          int equalSign = -1;
          String inId;
          for (int i = cmdPos; i < packetSize; i++) {
            if (packetBuffer[i] == 0x3d) { // look for =
              equalSign = i;
              break;
            }
            else {
              if (isDigit(packetBuffer[i])) {
                // convert the incoming byte to a char and add it to the string:
                inId += (char)packetBuffer[i];
              }
            }
          }
          String inValue;
          for (int i = equalSign; i < packetSize; i++) {
            if (isDigit(packetBuffer[i])) {
              // convert the incoming byte to a char and add it to the string:
              inValue += (char)packetBuffer[i];
            }
          }
          if (inId.toInt() == outPinIndicator) {
            outPinValue = inValue.toInt() & 0x01;
          }
          if (inId.toInt() == 6) {
            indicator6Value = inValue.toInt();
          }
          SendIndicators();
          timeUpdateIndicators = millis();
          break;
        }
      case writeEepromRequest: {
#if defined(debugModeOn)
          Serial.println("writeEepromRequest");
#endif
          SaveRegisters();
          break;
        }
      case servicesInfo: {
#if defined(debugModeOn)
          Serial.print("servicesInfo:");
          Serial.print(packetBuffer[cmdPos], HEX);
          int port = packetBuffer[cmdPos + 1] * 256 + packetBuffer[cmdPos + 2];
          Serial.print(" port:");
          Serial.println(port);
#endif
          if (packetBuffer[cmdPos] >= 0 && packetBuffer[cmdPos] < nbUdpPort) {
            for (int i = 0; i < 4; i++) {
              serverIP[packetBuffer[cmdPos]][i] = remoteAddr[i];  // update server address
            }
            udpPort[packetBuffer[cmdPos]] = packetBuffer[cmdPos + 1] * 256 + packetBuffer[cmdPos + 2];
          }
#if defined(debugModeOn)
          else {
            Serial.println(" unknow service:");
          }
#endif
          break;
        }
      case updateStatus: {
#if defined(debugModeOn)
          Serial.println("status request");
#endif
          SendStatus();
          timeUpdateStatus = millis();
          break;
        }

      default: {
#if defined(debugModeOn)
          Serial.println("unknown Udp command");
#endif
          break;
        }
    }
  }
}

boolean CheckCurrentIP()
{
  currentIP = WiFi.localIP();
  if (currentIP == nullIP)
  {
    if (debugMode == true)
    {
      Serial.println("IP address issue");
    }
    bitWrite(diagByte, bitDiagIP, 1);       // position bit diag
    return false;

  }
  else

  {
    bitWrite(diagByte, bitDiagIP, 0);       // position bit diag
    return true;
  }
}

void PrintUdpConfig() {
  for (int i = 0; i < (nbUdpPort); i++)
  {
    Serial.print(i);
    Serial.print(" Udp");
    Serial.print(" ");
    Serial.print(" port:");
    Serial.println(udpPort[i]);
  }
}
