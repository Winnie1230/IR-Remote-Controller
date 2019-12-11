//EEPROM功能正常
//尚未加入wifi自動連線
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <PubSubClient.h>
#include <EEPROM.h>
#include <IRutils.h>
#include <IRsend.h>
#include "myeeprom.h"
#include "mywifi.h"

#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#define recvPin  2    // GPIO3 = URXD,//D4
#define sendPin  4    // D2


//------------------------------------------------------------------------------
// Tell IRremote which Arduino pin is connected to the IR Receiver (TSOP4838)
 ESP8266WebServer server(80);
 WiFiClient espClient;
 PubSubClient client(espClient);
  
 IRrecv irrecv(recvPin);
 IRsend irsend(sendPin);
 decode_results  results;//紅外線data
 int a; //for subscribe
 
//------setup()----------

void setup() {
  delay(1000);
  Serial.begin(115200);  
  WiFi.softAP(APSSID,APPSK); //設定apmode的密碼
  EEPROM.begin(4000);
  IPAddress myIP = WiFi.softAPIP();
  server.on("/wifi", homewifift);
  server.begin();
  irsend.begin();
  irrecv.enableIRIn();
  Serial.print( "\n\nReady!\n\n" );
  eepromload();
  numberread(0,currentPosition);
  wifiload();
}

//---------loop()------------
void loop() {
  WiFiClient Client;
  server.handleClient();
  client.loop();
  if (client.connect("ESP8266Client", mqttUser, mqttPassword ) && !a) {
    client.subscribe("qwe/#");
    a = 1;
  }

  if (irrecv.decode(&results)) {  // 接收到紅外碼
    dumpInfo(&results);           // Output the results
    // Output the results in RAW format
    dumpCode(&results);           // Output the results as source code
    Serial.println("");           // Blank line between entries
    irrecv.resume();              // Prepare for the next value
  }
  if (topicstate == 1) //APP開啟 Publish所有按鍵名稱
  {
    topicstate = 0;
    eepromload();
    publishButtonName();
  }
  else if (topicstate == 2) {  //APP學習按鍵
    topicstate = 0;
    eepromload();
    learnft();
    eeprompush();
  } 
  else if (topicstate == 3) { //發送紅外碼
    topicstate = 0;
    eepromload();
    emit();
  }
  else if (topicstate == 4) {  //傳送URL
    while (!Client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
    }
  delay(100);
    topicstate = 0;
    String postRequest = (String)("GET ") + url + " HTTP/1.1\r\n" + "Content-Type: text/html;charset=utf-8\r\n" + "Host: " + host + "\r\n" + "User-Agent: BuildFailureDetectorESP8266\r\n" + "Connection: Keep Alive\r\n\r\n";
    Serial.println(postRequest);
    Client.print(postRequest); // 發送HTTP請求
//    /** * 展示返回的所有信息 */
//    String line = Client.readStringUntil('\n');
//    while (line.length() != 0) {
//      Serial.println(line);
//      line = Client.readStringUntil('\n');
//    }
//    Serial.println(line);
    Client.stop();
    delay(1000);
  }

  else if (topicstate == 5) { //初始化
    topicstate = 0;
    reseteeprom();
    eepromload();
    numberread(0,currentPosition);
    Serial.println(currentPosition);
  }
}
//-------------loopend-------------
//--------Display IR code----------
void ircode (decode_results *results) {
  // Panasonic has an Address
  if (results->decode_type == PANASONIC) {
    Serial.print(results->address, HEX);
    Serial.print( ":" );
  }

  // Print Code
  serialPrintUint64(results->value, 16);
}

//--------Dump out the decode_results structure----------
void dumpInfo (decode_results *results) {
  // Check if the buffer overflowed
  if (results->overflow) {
    Serial.println( "IR code too long. Edit IRremoteInt.h and increase RAWBUF" );
    return;
  }
  Serial.print( "Code      : " );
  ircode(results);
  Serial.print( " (" );
  Serial.print(results->bits, DEC);
  Serial.println( " bits)" );
}
//---------Dump out the decode_results structure----------
void dumpCode (decode_results *results) {
  // Start declaration
  Serial.print( "unsigned int  " );          // variable type
  Serial.print( "rawData[" );                // array name
  Serial.print(results->rawlen - 1, DEC);  // array size
  Serial.print( "] = {" );                   // Start declaration

  // Dump data

  for (int i = 1;  i < results->rawlen;  i++) {
    Serial.print(results->rawbuf[i] * RAWTICK, DEC);
    if ( i < results->rawlen - 1 ) Serial.print(","); // ',' not needed on last one
    if (!(i & 1))  Serial.print(" ");
  }
  Serial.print( "};" );
  Serial.print( "  // " );
  Serial.print( " " );
  ircode(results);
  // Newline
  Serial.println("");
  // Now dump "known" codes
  if (results->decode_type != UNKNOWN) {
    // Some protocols have an address
    if (results->decode_type == PANASONIC) {
      Serial.print( "unsigned int  addr = 0x" );
      serialPrintUint64(results->address, 16);
      Serial.println(";");
    }
  }
}
//--------homewifi---------

void homewifift() {
  if (server.hasArg("ssid")) {
    homessid = server.arg("ssid");
    Serial.println(homessid);
  }
  if (server.hasArg("password")) {
    homepassword = server.arg("password");
    Serial.println(homepassword);
  }
  WiFi.begin(homessid, homepassword); //esp8266連到家中wifi
  if(WiFiCheck()){
    server.send(200, "text/html", "<h1>You are connected</h1>");
    Serial.println("connected");    
    eepromload();//將WIFI存入eeprom
    numberwrite(10, homessid.length());  
    uint16_t passwordstart =  namewrite(12,homessid) +10;   
    namewrite(passwordstart,homepassword);
    eeprompush();    
    MqttSetting();
  }
  else{
    server.send(200, "text/html", "<h1>Disconnected</h1>");
  }
}

//---------callback-----------

void callback(char* topic, byte * payload, unsigned int length) {
  String callMsg = "";
  Serial.print("Message arrived [");
  Serial.print(topic);   // 打印主题信息
  Serial.print("] ");
  if ( strcmp(topic, "qwe/APP/start") == 0)
    topicstate = 1;
  else if (strcmp(topic, "qwe/APP/learn") == 0)
    topicstate = 2;
  else if (strcmp(topic, "qwe/APP/control") == 0)
    topicstate = 3;
  else if (strcmp(topic, "qwe/Web") == 0)
    topicstate = 4;
  else if (strcmp(topic, "qwe/Initial") == 0)
    topicstate = 5;
  for (int i = 0; i < length; i++) {
    callMsg += char(payload[i]);
  }
  state = callMsg;
  Serial.println(callMsg);
}

//--------publishButtonName-----------

void publishButtonName() {
  int findcount = 1;
  uint16_t Position = ircodestart;
  uint16_t IRpos;
  String getname;
  eepromload();
  while (1)
  { server.handleClient();
    client.loop();
    if (findcount == 1)
    { char namec[20];
      findcount = findname(Position, getname, IRpos);
      getname.toCharArray(namec, 20);
      client.publish("qwe/8266/EEPROM", namec);
      delay(100);
    }
    else if (findcount == 0)
      break;
  }
}

//------------learnft()----------------

void learnft(){
  int timer=0;
   while (!irrecv.decode(&results)) {
      yield();
      timer++;
      Serial.println("Learning‧‧‧");
      if (timer == 10) {     //超時
        timer = 0;
        Serial.println("Learning fail");
        return;
      }
      if (state != "")
      {
        currentPosition += namewrite(currentPosition, state); //currentPoisition等於存放名字後的位址
        state = "";
      }
      delay(1000);
    }
    numberwrite(currentPosition, results.rawlen - 1);
    currentPosition += 2;//存放陣列size
    for (int count = 1;  count < results.rawlen;  count++) {
      numberwrite(currentPosition, (results.rawbuf[count] * RAWTICK)) ;
      currentPosition += 2;
    }
    numberwrite(0,currentPosition);
    Serial.println("Learning success");
    delay(1000);
    irrecv.resume();
}

//----------emit--------------

void emit(){
   uint16_t Position2 = ircodestart;
    uint16_t IRposition;  //紅外碼位置
    uint16_t IRlength;
    bool findorNot;
    String Name;
   
    while (Name != state) {
      findorNot = findname(Position2, Name, IRposition);
      if (findorNot == 0 && Name != state) {
        Serial.println("send fail");
        return;
      }
    }
    numberread(IRposition, IRlength);
    IRlength += 1;
    IRposition += 2;
    uint16_t buttonraw[IRlength-1];
    for (uint16_t count = 1;  count < IRlength;  count++) {
      numberread(IRposition, buttonraw[count - 1]);
      IRposition += 2;
      if (count == IRlength - 1)
        Serial.println(IRposition);
    }
    irsend.sendRaw(buttonraw, sizeof(buttonraw) / sizeof(buttonraw[0]), 38);
    Serial.println("sending");
    state = "";
}
//-----------wifiload--------
void wifiload(){
  eepromload();
  uint16_t passwordstart =  nameread(10, homessid)+10;
  nameread(passwordstart, homepassword);
  Serial.println("loadhomessid:");
  Serial.println(homessid);
  Serial.println("loadhomepassword:");
  Serial.println(homepassword);
  WiFi.begin(homessid,homepassword); //ssid pass 預設從eeprom裡面讀出來的資料
  if(WiFiCheck()){  //判斷是否有wifi連線
    Serial.println("connected");
    MqttSetting();
  }
  else
    Serial.println("disconnected");
}

//---------MqttSetting--------

void MqttSetting(){
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback); //用於接收server的訂閱topic
  
  while (!client.connected()) {
    delay(500);
    Serial.println("connect to mqtt...");
    if (client.connect("ESP8266Client", mqttUser, mqttPassword)) {
      Serial.println("connected");
      
      //test code 記得刪掉
      client.publish("qwe/8266","connected");
      topic = 0;
    }
  }
}
