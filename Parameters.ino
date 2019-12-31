void InitLookForList()
{
  for (int i = 0; i < commandnumber ; i++)
  {
    PcommandList[i] = &commandList[i];
  }
  for (int i = 0; i < parametersNumber ; i++)
  {
    PparamList[i] = &paramList[i];
  }
    for (int i = 0; i < udpCommandnumber ; i++)
  {
    PudpCommandList[i] = &udpCommandList[i];
  }
  ParamNumber.InitCommandsList(PparamList, parametersNumber);
  SerialInput.InitCommandsList(PcommandList, commandnumber);
  UdpInput.InitCommandsList(PudpCommandList, udpCommandnumber);
}

void setInitialParam(int number)
{
  int idxArray = number ;
  //Serial.print(paramList[number]);
  Serial.print(" set parameter : ");
  Serial.print(number);
  Serial.print(" size: ");
  //  uint8_t nb = paramLength[number];
  Serial.print(paramLength[idxArray]);
  Serial.println();

  if (paramType[idxArray] == 0x00)
  {
    Serial.print("0x00 rc: ");
    Serial.println(storedParam.SetParameter(number, paramLength[idxArray], paramValue[idxArray]));
  }
  if (paramType[idxArray] == 0x01)
  {
    unsigned int value = ExtractNumericValue(idxArray) ;
    uint8_t byteToStore[1];
    byteToStore[0] = uint8_t(value);
    Serial.print("0x01 rc: ");
    Serial.println(storedParam.SetParameter(number, 1, byteToStore));
  }
  if (paramType[idxArray] == 0x02 )
  {
    unsigned int value = ExtractNumericValue(idxArray) ;
    uint8_t byteToStore[2];
    byteToStore[1] = uint8_t(value) ;
    byteToStore[0] = uint8_t(value / 256) ;
    Serial.print(" 0x02 rc: ");
    Serial.println(storedParam.SetParameter(number, 2, byteToStore));
  }
  if (paramType[idxArray] == 0x03 )
  {
    unsigned int value = ExtractNumericValue(idxArray) ;
    uint8_t byteToStore[3];
    byteToStore[2] = uint8_t(value) ;
    byteToStore[1] = uint8_t(value / 256) ;
    byteToStore[0] = uint8_t(value / (256 * 256)) ;
    Serial.print(" 0x03 rc: ");
    Serial.println(storedParam.SetParameter(number, 3, byteToStore));
  }
}

unsigned int  ExtractNumericValue(uint8_t idxArray)
{
  uint8_t  byteToStore[3];

  unsigned long val = 0;
  for (int i = 0; i < paramLength[idxArray]; i++)
  {
    if (paramType[idxArray] == 0x01 || paramType[idxArray] == 0x02 || paramType[idxArray] == 0x03)
    {
      unsigned int idx = uint8_t(paramValue[idxArray][i]) - 0x30;
      unsigned int degre = paramLength[idxArray] - i - 1;
      int pwr = 1;
      for (int j = i; j < paramLength[idxArray] - 1; j++)
      {
        pwr = pwr * 10;
      }
      val = val + idx * pwr;
    }
    if (paramType[idxArray] == 0x01)
    {
      byteToStore[0] = uint8_t(val ) ;

    }
    if (paramType[idxArray] == 0x02)
    {
      byteToStore[1] = uint8_t(val) ;
      byteToStore[0] = uint8_t(val / 256) ;
    }
    if (paramType[idxArray] == 0x03)
    {
      byteToStore[2] = uint8_t(val) ;
      byteToStore[1] = uint8_t(val / 256) ;
      byteToStore[0] = uint8_t(val / (256 * 256)) ;
    }
  }
  return (val);
}
