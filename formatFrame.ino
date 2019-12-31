void FormatFrame(boolean request, boolean toAck, uint8_t frameLen)
{
  /*
     format frame according to server protocol
  */
  trameNumber++;
  /*
  GatewayLink.PendingDataLenSerial = frameLen + 2;
  GatewayLink.frameSwitch = uint8_t(trameNumber % 256);; //
  GatewayLink.PendingReqSerial = PendingReqRefSerial;
  if (request) {
    bitWrite(GatewayLink.PendingReqSerial, requestFrameBit, 1);
  }
  else {
    bitWrite(GatewayLink.PendingReqSerial, requestFrameBit, 0);
  }
  if (toAck) {
    bitWrite(GatewayLink.PendingReqSerial, toAcknoledgeBit, 1);
  }
  else {
    bitWrite(GatewayLink.PendingReqSerial, toAcknoledgeBit, 0);
  }
  GatewayLink.PendingDataReqSerial[0] = unitGroup;
  GatewayLink.PendingDataReqSerial[1] = unitId;
  GatewayLink.PendingDataReqSerial[2] = 0x00;
  */
  /*
     add 0x00 and CRC8 at the end of the frame
  */
  /*
  GatewayLink.PendingDataReqSerial[frameLen] = 0x00;
  GatewayLink.PendingDataReqSerial[frameLen + 1] = CRC8(GatewayLink.PendingDataReqSerial , frameLen);
#if defined(debugConnection)
  Serial.print("send CRC:0x");
  Serial.println(CRC8(GatewayLink.PendingDataReqSerial , frameLen), HEX);
#endif
*/
}
