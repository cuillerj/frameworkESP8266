
void ShowEeeprom()
{
  for (int i = 0; i < parametersNumber; i++)
  {
    String  Srequest = paramList[i];
    Serial.print(Srequest);
    Serial.print(": ");
    commandReturn rc = ParamNumber.GetCommand(Srequest); // look for commant inside the input
    int cmdIdx = rc.idxCommand;
    if (cmdIdx != -1)
    {
      if (paramType[i] == 0x01 || paramType[i] == 0x02 || paramType[i] == 0x03 )
      {
        numericParameter numericRC = storedParam.GetNumericParameter(cmdIdx);
        Serial.println(numericRC.parameterNumericValue);
      }
      if (paramType[i] == 0x00 )
      {
        parameter RC = storedParam.GetParameter(cmdIdx);
        Serial.print(RC.parameterValue);
      }
    }
        Serial.println();
  }
}
void EraseEeprom()
{
  Serial.println("erase eeprom please wait");
  storedParam.Erase(0);
  Serial.println("need power off / on");
}

void UpdateEepromParameter(uint8_t cmdIdx, String Srequest, int cmdPos)
{
  String  Sparam = commandList[cmdIdx];               // get command string
  Sparam = Sparam.substring(0, sizeof(Sparam) - 1); // remove last char " = "
  String  Svalue = Srequest;                       // get command value
  Svalue = Svalue.substring(cmdPos);              //
  //  int valueLen = sizeof(Svalue);
  char Cvalue[maxParameterLen + 50];
  Svalue.toCharArray(Cvalue, sizeof(Svalue));

  commandReturn rc = ParamNumber.GetCommand(Sparam); // look for parameter inside the input
  int paramIdx = rc.idxCommand;
  // int paramPos = rc.idxPos;

  if (paramIdx != -1)
  {
    int idxArray = paramIdx ;
    Serial.print(" set parameter : ");
    Serial.print(idxArray);
    Serial.print(" size: ");
    //  uint8_t nb = paramLength[number];
    Serial.print(paramLength[idxArray]);
    Serial.println();

    if (paramType[idxArray] == 0x00)
    {
      byte Bvalue[maxParameterLen + 50];
      Svalue.getBytes(Bvalue, paramLength[idxArray]);
      Serial.print("0x00 rc: ");
      Serial.println(storedParam.SetParameter(idxArray, paramLength[idxArray], Bvalue));
    }

    if (paramType[idxArray] == 0x01)
    {
      int value = atoi(Cvalue) ;
      uint8_t byteToStore[1];
      byteToStore[0] = uint8_t(value);
      Serial.print("0x01 rc: ");
      Serial.println(storedParam.SetParameter(idxArray, 1, byteToStore));
    }
    if (paramType[idxArray] == 0x02 )
    {
      int value = atoi(Cvalue) ;
#if defined(debugModeOn)
      Serial.print("atoi: ");
      Serial.println(value);
#endif
      uint8_t byteToStore[2];
      byteToStore[1] = uint8_t(value) ;
      byteToStore[0] = uint8_t(value / 256) ;
      Serial.print(" 0x02 rc: ");
      Serial.println(storedParam.SetParameter(idxArray, 2, byteToStore));
    }
    if (paramType[idxArray] == 0x03 )
    {
      int value = atoi(Cvalue) ;
#if defined(debugModeOn)
      Serial.print("atoi: ");
      Serial.println(value);
#endif
      uint8_t byteToStore[3];
      byteToStore[2] = uint8_t(value) ;
      byteToStore[1] = uint8_t(value / 256) ;
      byteToStore[0] = uint8_t(value / (256 * 256)) ;
      Serial.print(" 0x03 rc: ");
      Serial.println(storedParam.SetParameter(idxArray, 3, byteToStore));
    }
  }
}
void LoadEerpromParamters()
{
  String  Srequest = "stAddr";
#if defined(debugModeOn)
  Serial.print(Srequest);
  Serial.print(": ");
#endif
  commandReturn rc = ParamNumber.GetCommand(Srequest); // look for commant inside the input
  int cmdIdx = rc.idxCommand;
  if (cmdIdx != -1)
  {
    numericParameter numericRC = storedParam.GetNumericParameter(cmdIdx);
#if defined(debugModeOn)
    Serial.println(numericRC.parameterNumericValue);
#endif
    stAddr = numericRC.parameterNumericValue;
  }

  Srequest = "SSpeed";
#if defined(debugModeOn)
  Serial.print(Srequest);
  Serial.print(": ");
#endif
  rc = ParamNumber.GetCommand(Srequest); // look for commant inside the input
  cmdIdx = rc.idxCommand;
  if (cmdIdx != -1)
  {
    numericParameter numericRC = storedParam.GetNumericParameter(cmdIdx);
#if defined(debugModeOn)
    Serial.println(numericRC.parameterNumericValue);
#endif
    SSpeed = numericRC.parameterNumericValue;
  }


  Srequest = "inPin1";
#if defined(debugModeOn)
  Serial.print(Srequest);
  Serial.print(": ");
#endif
  rc = ParamNumber.GetCommand(Srequest); // look for commant inside the input
  cmdIdx = rc.idxCommand;
  if (cmdIdx != -1)
  {
    numericParameter numericRC = storedParam.GetNumericParameter(cmdIdx);
#if defined(debugModeOn)
    Serial.println(numericRC.parameterNumericValue);
#endif
    inPin1 = numericRC.parameterNumericValue;
    pinMode(inPin1, INPUT);
  }
  Srequest = "inPin2";
#if defined(debugModeOn)
  Serial.print(Srequest);
  Serial.print(": ");
#endif
  rc = ParamNumber.GetCommand(Srequest); // look for commant inside the input
  cmdIdx = rc.idxCommand;
  if (cmdIdx != -1)
  {
    numericParameter numericRC = storedParam.GetNumericParameter(cmdIdx);
#if defined(debugModeOn)
    Serial.println(numericRC.parameterNumericValue);
#endif
    inPin2 = numericRC.parameterNumericValue;
    pinMode(inPin2, INPUT);
  }
  Srequest = "inPin3";
#if defined(debugModeOn)
  Serial.print(Srequest);
  Serial.print(": ");
#endif
  rc = ParamNumber.GetCommand(Srequest); // look for commant inside the input
  cmdIdx = rc.idxCommand;
  if (cmdIdx != -1)
  {
    numericParameter numericRC = storedParam.GetNumericParameter(cmdIdx);
#if defined(debugModeOn)
    Serial.println(numericRC.parameterNumericValue);
#endif
    inPin3 = numericRC.parameterNumericValue;
    pinMode(inPin3, INPUT_PULLUP);
  }

  Srequest = "LEDPin";
#if defined(debugModeOn)
  Serial.print(Srequest);
  Serial.print(": ");
#endif
  rc = ParamNumber.GetCommand(Srequest); // look for commant inside the input
  cmdIdx = rc.idxCommand;
  if (cmdIdx != -1)
  {
    numericParameter numericRC = storedParam.GetNumericParameter(cmdIdx);
#if defined(debugModeOn)
    Serial.println(numericRC.parameterNumericValue);
#endif
    LEDPin = numericRC.parameterNumericValue;
  }

  Srequest = "confPin";
#if defined(debugModeOn)
  Serial.print(Srequest);
  Serial.print(": ");
#endif
  rc = ParamNumber.GetCommand(Srequest); // look for commant inside the input
  cmdIdx = rc.idxCommand;
  if (cmdIdx != -1)
  {
    numericParameter numericRC = storedParam.GetNumericParameter(cmdIdx);
#if defined(debugModeOn)
    Serial.println(numericRC.parameterNumericValue);
#endif
    configPin = numericRC.parameterNumericValue;
  }

  Srequest = "outPin1";
#if defined(debugModeOn)
  Serial.print(Srequest);
  Serial.print(": ");
#endif
  rc = ParamNumber.GetCommand(Srequest); // look for commant inside the input
  cmdIdx = rc.idxCommand;
  if (cmdIdx != -1)
  {
    numericParameter numericRC = storedParam.GetNumericParameter(cmdIdx);
#if defined(debugModeOn)
    Serial.println(numericRC.parameterNumericValue);
#endif
    outPin1 = numericRC.parameterNumericValue;
    pinMode(outPin1, OUTPUT);
  }
  Srequest = "outPin2";
#if defined(debugModeOn)
  Serial.print(Srequest);
  Serial.print(": ");
#endif
  rc = ParamNumber.GetCommand(Srequest); // look for commant inside the input
  cmdIdx = rc.idxCommand;
  if (cmdIdx != -1)
  {
    numericParameter numericRC = storedParam.GetNumericParameter(cmdIdx);
#if defined(debugModeOn)
    Serial.println(numericRC.parameterNumericValue);
#endif
    outPin1 = numericRC.parameterNumericValue;
    pinMode(outPin2, OUTPUT);
  }
  Srequest = "outPin3";
#if defined(debugModeOn)
  Serial.print(Srequest);
  Serial.print(": ");
#endif
  rc = ParamNumber.GetCommand(Srequest); // look for commant inside the input
  cmdIdx = rc.idxCommand;
  if (cmdIdx != -1)
  {
    numericParameter numericRC = storedParam.GetNumericParameter(cmdIdx);
#if defined(debugModeOn)
    Serial.println(numericRC.parameterNumericValue);
#endif
    outPin3 = numericRC.parameterNumericValue;
    pinMode(outPin3, OUTPUT);
  }
  Srequest = "outPin4";
#if defined(debugModeOn)
  Serial.print(Srequest);
  Serial.print(": ");
#endif
  rc = ParamNumber.GetCommand(Srequest); // look for commant inside the input
  cmdIdx = rc.idxCommand;
  if (cmdIdx != -1)
  {
    numericParameter numericRC = storedParam.GetNumericParameter(cmdIdx);
#if defined(debugModeOn)
    Serial.println(numericRC.parameterNumericValue);
#endif
    outPin4 = numericRC.parameterNumericValue;
    pinMode(outPin4, OUTPUT);
  }


  Srequest = "SSID1";
#if defined(debugModeOn)
  Serial.print(Srequest);
  Serial.print(": ");
#endif
  rc = ParamNumber.GetCommand(Srequest); // look for commant inside the input
  cmdIdx = rc.idxCommand;
  if (cmdIdx != -1)
  {
    parameter RC = storedParam.GetParameter(cmdIdx);
#if defined(debugModeOn)
    Serial.println(RC.parameterValue);
#endif
    for (int i = 0; i < sizeof(ssid1); i++)
    {
      ssid1[i] = RC.parameterValue[i];
    }
  }

  Srequest = "PSW1";
#if defined(debugModeOn)
  Serial.print(Srequest);
  Serial.print(": ");
#endif
  rc = ParamNumber.GetCommand(Srequest); // look for commant inside the input
  cmdIdx = rc.idxCommand;
  if (cmdIdx != -1)
  {
    parameter RC = storedParam.GetParameter(cmdIdx);
#if defined(debugModeOn)
    Serial.println(RC.parameterValue);
#endif
    for (int i = 0; i < sizeof(pass1); i++)
    {
      pass1[i] = RC.parameterValue[i];
    }
  }

  Srequest = "SSID2";
#if defined(debugModeOn)
  Serial.print(Srequest);
  Serial.print(": ");
#endif
  rc = ParamNumber.GetCommand(Srequest); // look for commant inside the input
  cmdIdx = rc.idxCommand;
  if (cmdIdx != -1)
  {
    parameter RC = storedParam.GetParameter(cmdIdx);
#if defined(debugModeOn)
    Serial.println(RC.parameterValue);
#endif
    for (int i = 0; i < sizeof(ssid2); i++)
    {
      ssid2[i] = RC.parameterValue[i];
    }
  }

  Srequest = "PSW2";
#if defined(debugModeOn)
  Serial.print(Srequest);
  Serial.print(": ");
#endif
  rc = ParamNumber.GetCommand(Srequest); // look for commant inside the input
  cmdIdx = rc.idxCommand;
  if (cmdIdx != -1)
  {
    parameter RC = storedParam.GetParameter(cmdIdx);
#if defined(debugModeOn)
    Serial.println(RC.parameterValue);
#endif
    for (int i = 0; i < sizeof(pass2); i++)
    {
      pass2[i] = RC.parameterValue[i];
    }
  }

  Srequest = "serverPort";
#if defined(debugModeOn)
  Serial.print(Srequest);
  Serial.print(": ");
#endif
  rc = ParamNumber.GetCommand(Srequest); // look for commant inside the input
  cmdIdx = rc.idxCommand;
  if (cmdIdx != -1)
  {
    numericParameter numericRC = storedParam.GetNumericParameter(cmdIdx);
#if defined(debugModeOn)
    Serial.println(numericRC.parameterNumericValue);
#endif
    udpPort[0] = numericRC.parameterNumericValue;
  }


  Srequest = "listenPort";
#if defined(debugModeOn)
  Serial.print(Srequest);
  Serial.print(": ");
#endif
  rc = ParamNumber.GetCommand(Srequest); // look for commant inside the input
  cmdIdx = rc.idxCommand;
  if (cmdIdx != -1)
  {
    numericParameter numericRC = storedParam.GetNumericParameter(cmdIdx);
#if defined(debugModeOn)
    Serial.println(numericRC.parameterNumericValue);
#endif
    udpListenPort = numericRC.parameterNumericValue;
  }

  Srequest = "IP1";
#if defined(debugModeOn)
  Serial.print(Srequest);
  Serial.print(": ");
#endif
  rc = ParamNumber.GetCommand(Srequest); // look for commant inside the input
  cmdIdx = rc.idxCommand;
  if (cmdIdx != -1)
  {
    numericParameter numericRC = storedParam.GetNumericParameter(cmdIdx);
#if defined(debugModeOn)
    Serial.println(numericRC.parameterNumericValue);
#endif
    IP1 = numericRC.parameterNumericValue;
    serverIP[0][0] = IP1;
  }

  Srequest = "IP2";
#if defined(debugModeOn)
  Serial.print(Srequest);
  Serial.print(": ");
#endif
  rc = ParamNumber.GetCommand(Srequest); // look for commant inside the input
  cmdIdx = rc.idxCommand;
  if (cmdIdx != -1)
  {
    numericParameter numericRC = storedParam.GetNumericParameter(cmdIdx);
#if defined(debugModeOn)
    Serial.println(numericRC.parameterNumericValue);
#endif
    IP2 = numericRC.parameterNumericValue;
    serverIP[0][1] = IP2;
  }

  Srequest = "IP3";
#if defined(debugModeOn)
  Serial.print(Srequest);
  Serial.print(": ");
#endif
  rc = ParamNumber.GetCommand(Srequest); // look for commant inside the input
  cmdIdx = rc.idxCommand;
  if (cmdIdx != -1)
  {
    numericParameter numericRC = storedParam.GetNumericParameter(cmdIdx);
#if defined(debugModeOn)
    Serial.println(numericRC.parameterNumericValue);
#endif
    IP3 = numericRC.parameterNumericValue;
    serverIP[0][2] = IP3;
  }

  Srequest = "IP4";
#if defined(debugModeOn)
  Serial.print(Srequest);
  Serial.print(": ");
#endif
  rc = ParamNumber.GetCommand(Srequest); // look for commant inside the input
  cmdIdx = rc.idxCommand;
  if (cmdIdx != -1)
  {
    numericParameter numericRC = storedParam.GetNumericParameter(cmdIdx);
#if defined(debugModeOn)
    Serial.println(numericRC.parameterNumericValue);
#endif
    IP4 = numericRC.parameterNumericValue;
    serverIP[0][3] = IP4;
  }
}

void SaveRegister(int  regId, byte value) {
  byte values[1];
  values[0] = value;
  if (regId < registerSize && regId >= 0) {
    storedParam.SetParameter(parametersNumber + regId, 1, values);
  }
}
void SaveRegisters() {
  for (int i = 0; i < registerSize; i++) {
    SaveRegister(i, Registers[i]);
  }
}
