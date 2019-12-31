int Serial_have_message() {
  if (Serial.available() )
  {
    if (digitalRead(configPin) == false) //  run mode
    {
      return 0;
    }
    else  //  configuration mode
    {
      Srequest = (Serial.readString());
      Serial.println(Srequest);
      commandReturn rc = SerialInput.GetCommand(Srequest); // look for commant inside the input
      int cmdIdx = rc.idxCommand;
      int cmdPos = rc.idxPos;
#if defined(debugModeOn)
      Serial.print("idxCommand:");
      Serial.println(cmdIdx);
#endif
      if (cmdIdx >= 0)
      {
        switch (cmdIdx) {
          case SSID1Idx:    // SSID1=
            {
#if defined(debugModeOn)
              Serial.println("previous SSID:");
              for (int i = 0; i < maxSSIDLength; i++)
              {
                Serial.print(ssid1[i], HEX);
                Serial.print("-");

                Serial.println("");
              }
#endif
              for (int i = 0; i < maxSSIDLength; i++)
              {
                ssid1[i] = 0x00;
              }
              UpdateEepromParameter(cmdIdx, Srequest, cmdPos);
              String id = Srequest.substring(cmdPos);
              id.toCharArray(ssid1, id.length() - 1);
#if defined(debugModeOn)
              Serial.println("new SSID:");
              for (int i = 0; i < maxSSIDLength; i++)
              {
                Serial.print(ssid1[i], HEX);
                Serial.print("-");
              }
              Serial.println("");
#endif
              break;
            }
          case PSW1Idx:    // PSW1=
            {
#if defined(debugModeOn)
              Serial.println("previous psw:");
              for (int i = 0; i < maxPSWLength; i++)
              {
                Serial.print(pass1[i], HEX);
                Serial.print("-");
              }
              Serial.println("");
#endif
              for (int i = 0; i < maxPSWLength; i++)
              {
                pass1[i] = 0x00;
              }
              UpdateEepromParameter(cmdIdx, Srequest, cmdPos);
              String id = Srequest.substring(cmdPos);
              id.toCharArray(pass1, id.length() - 1);
#if defined(debugModeOn)
              Serial.println("new psw:");
              for (int i = 0; i < maxPSWLength; i++)
              {
                Serial.print(pass1[i], HEX);
                Serial.print("-");
              }
              Serial.println("");
#endif
              break;
            }
          case SSID2Idx:    // SSID2=
            {
              {
#if defined(debugModeOn)
                Serial.println("previous SSID:");
                for (int i = 0; i < maxSSIDLength; i++)
                {
                  Serial.print(ssid2[i], HEX);
                  Serial.print("-");
                }
                Serial.println("");
#endif
                for (int i = 0; i < maxSSIDLength; i++)
                {
                  ssid2[i] = 0x00;
                }
                UpdateEepromParameter(cmdIdx, Srequest, cmdPos);
                String id = Srequest.substring(cmdPos);
                id.toCharArray(ssid2, id.length() - 1);
#if defined(debugModeOn)
                Serial.println("new SSID:");
                for (int i = 0; i < maxSSIDLength; i++)
                {
                  Serial.print(ssid2[i], HEX);
                  Serial.print("-");
                }
                Serial.println("");
#endif
                break;
              }
            }
          case PSW2Idx:     // PSW2=
            {
#if defined(debugModeOn)
              Serial.println("previous psw:");
              for (int i = 0; i < maxPSWLength; i++)
              {
                Serial.print(pass2[i], HEX);
                Serial.print("-");
              }
              Serial.println("");
#endif
              for (int i = 0; i < maxPSWLength; i++)
              {
                pass2[i] = 0x00;
              }
              UpdateEepromParameter(cmdIdx, Srequest, cmdPos);
              String id = Srequest.substring(cmdPos);
              id.toCharArray(pass2, id.length() - 1);
#if defined(debugModeOn)
              Serial.println("new psw:");
              for (int i = 0; i < maxPSWLength; i++)
              {
                Serial.print(pass2[i], HEX);
                Serial.print("-");
              }
              Serial.println("");
#endif
              break;
            }
          case showWifiIdx:     // ShowWifi
            {
              WiFi.printDiag(Serial);
              Serial.println(WiFi.localIP());
              Serial.print("ports:");
              Serial.print(udpPort[0]);
              Serial.print(" ");
              Serial.println(udpListenPort);
              break;
            }
          case restartWifiIdx:     // Restart
            {
              Serial.println("Restart Wifi");
              Udp.stop();
              bitWrite(diagByte, bitDiagWifi, 0);       // position bit diagc              digitalWrite(connextionLED,false);
              WiFi.disconnect(true);
              delay(1000);
              restartCompleted = false;
              timeRestart = millis();
              InitConfig();
              break;
            }
          case scanWifiIdx:     // ScanWifi
            {
              ScanWifi();
              break;
            }
          case SSID_0Idx:    // SSID=0
            {
              currentSSID = 0x00;
              break;
            }
          case SSID_1Idx:    // SSID=1
            {
              currentSSID = 0x01;
              break;
            }
          case SSID_2Idx:    // SSID=2
            {
              currentSSID = 0x02;
              break;
            }
          case showEepromIdx:    // ShowEeprom
            {
              ShowEeeprom();
              break;
            }
          case eraseEepromIdx:    // EraseEeprom
            {
              EraseEeprom();
              break;
            }
          case sendStatusIdx:    // sendStatus
            {
              SendStatus();
              break;
            }

          default:
            {
              UpdateEepromParameter(cmdIdx, Srequest, cmdPos);
              break;
            }
        }
      }
      else
      {
        Serial.println("Serial must be set NL & CR ");
        Serial.print("Supported commands with parameters lenght are: ");
        for (int i = 0; i < commandnumber; i++)
        {
          Serial.print(commandList[i]);

          if (i < parametersNumber)
          {
            Serial.print("(");
            Serial.print(paramLength[i]);
            Serial.print(")");
          }
          Serial.print(" ");
        }
        Serial.println("");
        delay(500);
      }
      return (0);
    }
  }
}
