/*


*/
/*
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
  LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
  WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

  Written by Jean Cuiller
*/
/* This code run on ESP8266 chip

*/
#define debugModeOn               // uncomment this define to debug the code
#define forceErase true                    // set to true in case of major issue with the eeprom tool
#define versionID "03"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <EEPROM.h>

#define defaultSerialSpeed 38400   // the default speed
/*

*/
#define updateClockCycle 600000        // time request using this cycle
#define updateClockLimit 1300000        // limit duration without receiving time information over that time bit error is raised 
#define updateSheetCycle 120000     // time request using this cycle /sec
#define updateDatabaseCycle 120000     // time request using this cycle /sec
#define updateRegisterCycle 120000    // millis sec
#define updateIndicatorCycle 100000    // millis sec
#define updateStatusCycle 5       // time request using this cycle / mn
#define updateStatusCycle 300000
#define updateIndicatorsCycle updateStatusCycle +5000
#define updateClockCycle 600000        // time request using this cycle
#define updateClockLimit 1300000        // limit duration without receiving time information over that time bit error is raised 
#define diagTimeCycle 120000
#define pendingTimeout 60000           // not wait for answer to a request longer that this timer
/* *** Define look for string environment

*/
#include <LookForString.h>        // used to look for string inside commands and parameters
#define parametersNumber 21       // must fit with the paramList
#define commandnumber 30      // must fit with the commandList
#include <configurationDefault.h>
/* *** commands list definition
    Define all the command's names
    Ended by "=" means this command will set the parameter of the seame name
*/
#define udpCommandnumber 8     // must fit with the commandList
String udpCommandList[udpCommandnumber] = {"timeResponse/", "updateRegistersRequest/", "indicatorsRequest/", "updateIndicatorRequest/", "writeEepromRequest/", "servicesInfo/", "updateStatus/", "\\End"};

#define timeResponse 0
#define updateRegistersRequest 1
#define indicatorsRequest 2
#define updateIndicatorRequest 3
#define writeEepromRequest 4
#define servicesInfo 5
#define updateStatus 6

/* *** commands list definition
    Define all the command's names
    Ended by "=" means this command will set the parameter of the seame name
*/
String commandList[commandnumber] = {"stAddr=", "SSpeed=", "SSID1=", "PSW1=", "SSID2=", "PSW2=",
                                     "confPin=", "inPin1=", "inPin2=", "inPin3=", "outPin1=", "outPin2=", "outPin3=", "outPin4=", "LEDPin=",
                                     "IP1=", "IP2=", "IP3=", "IP4=",
                                     "serverPort=", "listenPort=",
                                     "SSID=0", "SSID=1", "SSID=2",
                                     "RestartWifi", "ScanWifi",  "ShowWifi",
                                     "ShowEeprom", "EraseEeprom", "sendStatus"
                                    };
#define stAddrIdx 0
#define SSpeedIdx 1
#define SSID1Idx 2
#define PSW1Idx 3
#define SSID2Idx 4
#define PSW2Idx 5
#define confPinIdx 6
#define inPin1Idx 7
#define inPin2Idx 8
#define inPin3Idx 9
#define outPin1Idx 10
#define outPin2Idx 11
#define outPin3Idx 12
#define outPinI4dx 13
#define LEDPinIdx 14
#define IP1Idx 15
#define IP2Idx 16
#define IP3Idx 17
#define IP4Idx 18
#define serverPortIdx 19
#define listenPortIdx 20
#define SSID_0Idx 21
#define SSID_1Idx 22
#define SSID_2Idx 23
#define restartWifiIdx 24
#define scanWifiIdx 25
#define showWifiIdx 26
#define showEepromIdx 27
#define eraseEepromIdx 28
#define sendStatusIdx 29

#define registerUpdateSheetCycle 0
#define registerUpdateStatusCycle 1

#define registerSize 5
uint8_t Registers[registerSize] = {updateSheetCycle, updateStatusCycle, 0, 0, 0};
/* *** paramters list definition
    Define all the parameter's names
    Parameters that can be set to value must have a name that fit with the commandList (without "=")
*/

String paramList[parametersNumber] = {"stAddr", "SSpeed", "SSID1", "PSW1", "SSID2", "PSW2",
                                      "confPin", "inPin1", "inPin2", "inPin3", "outPin1", "outPin2", "outPin3", "outPin4", "LEDPin",
                                      "IP1", "IP2", "IP3", "IP4",
                                      "serverPort", "listenPort"
                                     };
/*
    define the exact number of characters that define the parameter's values
*/
unsigned int paramLength[parametersNumber] =  {4, 5, 50, 50, 50, 50,
                                               2, 2, 2, 2, 2, 2, 2, 2, 2,
                                               3, 3, 3, 3,
                                               4, 4
                                              };
/*
   define the type of parameters 0x00 means String, 0x01 means number to be stored with one byte (<256), 0x02 means means number to be stored with two bytes (< 65 535),  0x03 means means number to be stored with 3 bytes (< 16 777 216)
*/
uint8_t paramType[parametersNumber] =         {0x02, 0x03, 0x00, 0x00, 0x00, 0x00,
                                               0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
                                               0x02, 0x02, 0x02, 0x02,
                                               0x02, 0x02
                                              };
/*
   define the default paramter's values that will be used before configuration
   The number of caracters must fit with the parameter length
*/
//byte paramValue[parametersNumber][50] =       { "yourfirstssid", "yourfirstpsw", "yoursecondssid", "yoursecondpass", "1830", "1831", "8888", "192", "168", "001", "005", "03"};


String *PparamList[parametersNumber];  // pointera array (to each paramter)
String *PcommandList[commandnumber];   // pointera array (to each command)
String *PudpCommandList[udpCommandnumber];   // pointera array (to each command)

// *** define the look for string tools
LookForStr ParamNumber(PparamList, parametersNumber);  // define object that look for a string inside parameters list
LookForStr SerialInput(PcommandList, commandnumber);   // define object that look for a string inside commands list
LookForStr UdpInput(PudpCommandList, udpCommandnumber);   // define object that look for a string inside commands list
/*
 * *** *** define the Eeprom management tools
*/
#include <ManageParamEeprom.h>              // include the code 
#define ramOffset 0                         // define the eeprom starting position (default 0 - can be change for other software compatibilty reason)
char keyword[] = "AzErTy";                  // must contain 6 charasters  - this keyword is used to check eeprom compatibity and can be modified
ManageParamEeprom storedParam (parametersNumber + registerSize, ramOffset, keyword);
/*
   Date Time managment
   uncomment either TimeMode or RTCMode to use up to date and time
   uncomment RTCMode in case you a RTC DS1307 hardware is connected to the arduino
   uncomment TimeMode in case of no specific hardware clock
*/
#define TimeMode
//#define RTCMode

#ifdef RTCMode
#include <RTClib.h>
RTC_DS1307 RTC;
#define TimeOn
#endif

#ifdef TimeMode
#include <TimeLib.h>
#define TimeOn
#endif
/*

*/
int configPin = 4;  // set to config mode when set to 3.3v - running mode when set to ground
//int inPin = 12; // Switch of udp trace when grounded  On when set to 3.3v - no longer used
//int outPin = 5;
int inPin1;
int inPin2;
int inPin3;
int outPin1;
int outPin2;
int outPin3;
int outPin4;

boolean outPinValue = false;
#define outPinIndicator 8
int LEDPin = 13;
int stAddr = 0;
int SSpeed = defaultSerialSpeed;
//int serverPort = 1903;
byte serverIP[2][4] = {{0xc0, 0xa8, 0x01, 0x62}, {0x00, 0x00, 0x00, 0x00}}; // default servers ip
int udpListenPort = 8888;
uint8_t IP1 = 0x00;
uint8_t IP2 = 0x00;
uint8_t IP3 = 0x00;
uint8_t IP4 = 0x00;

#define serviceDispatcher 0
#define nbUdpPort 2
int udpPort[nbUdpPort] = {1903, 0}; // default servers ports
IPAddress remoteAddr ;
#define maxSSIDLength 50
#define maxPSWLength 50
#define maxSendDataLenght 250   // maximum lenght of data to be sent to server
char ssid1[maxSSIDLength] = "cuiller2";       // first SSID  will be used at first
char pass1[maxPSWLength] = "5E98D9BEC6";      // first pass
char ssid2[maxSSIDLength] = "cuiller";        // second SSID will be used in case on connection failure with the first ssid
char pass2[maxPSWLength] = "116A2E2CC25E3DDC3593E13D29";  // second pass
uint8_t retryWifi = 0x00;         //
boolean connectionLEDOn = false;
boolean powerLEDOn = false;
boolean debugMode = false;
boolean LEDOn = false;
boolean restartCompleted = false;
boolean connectedStatus = false;
String app = "framework_ESP8266";
uint8_t vers = 0x01;
uint8_t addrStation[4] = {0x00, 0x00, 0x04, 0x03}; // 2 1er octets reserves commence ensuite 1024 0x04 0x00
uint8_t typeStation[4] = {0x00, 0x00, 0x04, 0x01};
String Srequest = "                                                                                                                                                                                                                                                                                                                     ";
uint8_t addrStationLen = sizeof(addrStation);
uint8_t typeStationLen = sizeof(typeStation);

//String services[] = {"Action", "MajSql"};
#define maxUDPResp 1024           // maximum UDP response size
char packetBuffer[maxUDPResp]; //buffer to hold incoming packet,
byte bufUdp[255];
uint8_t currentSSID = 0x00;         // current used SSID
char stationId[5];
#define openByPush 1
#define openByWeb 2
uint8_t openBy = 0x00;

// serial fields
#define sizeDataBin 255
byte dataBin[sizeDataBin];
#define maxUDPResp  100
char resp[maxUDPResp] = "";
int frameLen = 0;
int frameCount = 0;
uint8_t frameFlag = 0x00;
uint8_t trameNumber = 0x00;
int udpFlag = 0;
byte bufParam[5];
byte bufSerialOut[maxUDPResp + 10];
IPAddress currentIP;
IPAddress nullIP = {0, 0, 0, 0};
//
/*

*/
#define updateClockCycle 600000        // time request using this cycle
#define updateClockLimit 1300000        // limit duration without receiving time information over that time bit error is raised 
#define updateSheetCycle 120000     // time request using this cycle /sec
#define updateDatabaseCycle 120000     // time request using this cycle /sec
#define updateRegisterCycle 120000    // millis sec
#define updateIndicatorCycle 100000    // millis sec
#define updateStatusCycle 5       // time request using this cycle / mn
#define updateStatusCycle 300000
#define updateIndicatorsCycle updateStatusCycle +5000
#define updateClockCycle 600000        // time request using this cycle
#define updateClockLimit 1300000        // limit duration without receiving time information over that time bit error is raised 
#define diagTimeCycle 120000
#define pendingTimeout 60000           // not wait for answer to a request longer that this timer
/*

*/
unsigned long timeSerial = 0;
unsigned long timeCheck = 0;
unsigned long timeAffLED = 0;
unsigned long timeUdp = 0;
unsigned long timeLastSentUdp = 0;
unsigned long timeCheckWifi = 0;
unsigned long timeRestart = 0;
unsigned long lastReceivedTime = 0;
unsigned long timeUpdateStatus = 0;
unsigned long timeUpdateIndicators = 0;
unsigned long timeInsertGoogleSheet = -1000;
unsigned long timeSendRegister = -10000;
unsigned long timeInsertDatabase = -5000;
unsigned long lastUpdateClock = updateClockCycle + 5000;
unsigned long pendingRequestClock;
unsigned long diagTime = 0;

/*

*/


// internal data
#define bitDiagWifi 0
#define bitDiagIP 1
#define diagTimeUpToDate 2
#define bitDiagReboot 7
uint8_t  diagByte = 0b10000111; // (reboot,,,,,IP,Wifi)
int indicator6Value=99;

String Sresp;
String respTypeArray[] = {"resptime/"};
#define nbrespTypeArray 1


unsigned int freeMemory = 0;
int id;
WiFiUDP Udp;

void setup() {
  Serial.begin(defaultSerialSpeed);
  delay(1000);
#if defined(debugModeOn)
  Serial.println("start");
#endif
  Sresp.reserve(maxUDPResp);
  delay(1000);
  InitLookForList();
  uint8_t paramVersion = storedParam.GetVersion();
#if defined(debugModeOn)
  Serial.print("eeprom version: ");
  Serial.println(paramVersion, HEX);
#endif
  if (paramVersion == 0xff || forceErase == true )
  {
    Serial.println("init eeprom: ");
    storedParam.Init();
    for (int i = 0; i < parametersNumber ; i++)
    {
      setInitialParam(i);
    }
        currentSSID=0x01;
  }

  LoadEerpromParamters();

  if (defaultSerialSpeed != SSpeed)
  {
#if defined(debugModeOn)
    Serial.print("restart serial at: ");
    Serial.println(SSpeed);
#endif
    delay(200);
    Serial.end();
    delay(500);
    Serial.begin(SSpeed);
    delay(1000);
  }

  // udpPort[0] = serverPort; //
  timeRestart = millis();
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
  // TraceToUdp("Ready! Use ", 0x02);
  pinMode(LEDPin, OUTPUT);
  digitalWrite(LEDPin, 0);


 
  randomSeed(analogRead(7));
}

void loop() {
  StandardLoop();
  /*
     insert specific code below
  */
  if (millis() - timeSendRegister >= updateRegisterCycle)
  {
    SendRegisters();
    timeSendRegister = millis();
  }
  if (millis() - timeInsertGoogleSheet >= updateSheetCycle)
  {
#define nbValues 7
    int values[nbValues];
    values[0] = analogRead(0);
    for (int i = 1; i < nbValues - 1; i++)
    {
      values[i] = digitalRead(i);
    }
#if defined(debugOn)
    Serial.println(" sheet:");
#endif
    SendToGoogleSheet(nbValues, values);
    timeInsertGoogleSheet = millis();
  }
  if (millis() - timeInsertDatabase >= updateDatabaseCycle)
  {
#define nbValues 7

    int values[nbValues];
    int type = 0;
    values[0] = analogRead(0);
    for (int i = 1; i < nbValues - 1; i++)
    {
      values[i] = digitalRead(i);
    }
#if defined(debugOn)
    Serial.println(" sheet:");
#endif
    SendToDatabase(nbValues, type, values);
    timeInsertDatabase = millis();
  }

  if (millis() - timeUpdateIndicators >= updateIndicatorsCycle)
  {
    SendIndicators();
    timeUpdateIndicators = millis();
  }
  digitalWrite(outPin1, outPinValue);
}
