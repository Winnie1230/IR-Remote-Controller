//加入button remove功能
//APP sensor尚未完成
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

const String esp8266id = "qwe";  //產品序號
int esp8266idLength = esp8266id.length();

//------------------------------------------
 ESP8266WebServer server(80);
 WiFiClient espClient;
 PubSubClient client(espClient);
 WiFiClient Client;
  
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
  server.handleClient();
  client.loop();
  if (client.connect("ESP8266Client", mqttUser, mqttPassword ) && !a) {
    char sub[50];
    String substr = esp8266id + "/#";
    substr.toCharArray(sub,50);
    client.subscribe(sub);
    a = 1;
  }

  if (irrecv.decode(&results)) {  // 接收到紅外碼
    dumpInfo(&results);           // Output the results
    // Output the results in RAW format
    dumpCode(&results);           // Output the results as source code
    Serial.println("");           // Blank line between entries
    irrecv.resume();              // Prepare for the next value
  }
}
//-------------loopend-------------
//---------callback-----------

void callback(char* topic, byte * payload, unsigned int length) {
  String callMsg = "";
  Serial.print("Message arrived [");
  Serial.print(topic);   // 打印主题信息
  Serial.print("] ");
   for (int i = 0; i < length; i++) {
    callMsg += char(payload[i]);
  }
  state = callMsg;
  Serial.println(callMsg);
  String temp = String(topic);
  int topiclength = temp.length();
  
  for(int i = 0 ; i<(topiclength-esp8266idLength) ; i++)  topic[i] = topic[i+esp8266idLength];  //先把序號處理掉
  for(int i = (topiclength-esp8266idLength) ; i<topiclength ; i++)  topic[i] = '\0';
  
  if ( strcmp(topic, "/APP/APPState")== 0){ //APP開啟 Publish所有按鍵名稱
    eepromload();
    publishButtonName();
  }
  else if (strcmp(topic, "/APP/Learn") == 0){   //APP學習按鍵
    eepromload();
    learnft();
    publishstr("/8266/Callback/LearnCallback","successful");
    Serial.print("currentPosition:");
    Serial.println(currentPosition);
    eeprompush();
  }
  else if (strcmp(topic,"/APP/Control") == 0){ //發射紅外碼
    eepromload();
    emit();   
  }
  else if (strcmp(topic, "/Web/SensorState")== 0){ //傳送sensor 開關狀態給server
    eepromload();
    String statestring;
    nameread2(2,statestring);
    CO2state = statestring[0]; //二氧化碳感測器
    pm25state = statestring[1]; //pm2.5狀態
    pm10state = statestring[2]; //pm10狀態
    hchostate = statestring[3]; //hcho狀態
    tvocstate = statestring[4]; //TVOC狀態
    humidstate = statestring[5]; //濕度狀態
    temperstate = statestring[6]; //溫度狀態
    Currentstate= statestring[7]; 
    String url = "/state?co2=" + CO2state + "&pm2.5=" + pm25state + "&pm10=" + pm10state + "&hcho=" + hchostate + "&tvoc=" + tvocstate + "&humid=" + humidstate + "&temp=" + temperstate + "&current=" + Currentstate ;
    sendurl(url);
    Serial.println(url);
  }
  else if (strcmp(topic, "/Web/Initial") == 0){  //出廠設定(初始化)
    reseteeprom();
    eepromload();
    numberread(0,currentPosition);
    sendurl("/initialcheck?result=1");
  }
   else if (strcmp(topic, "/Web/SensorChange") == 0){  //web變更sensor狀態
     eepromload();
     namewrite2(2,callMsg);
     eeprompush();
  }
  else if (strcmp(topic, "/APP/Remove") == 0){ //移除按鈕
     btnremove(state);
  }
}
//--------sendurl----------
void sendurl(String data){      //傳送URL 發送HTTP請求
  if(!Client.connect(host, httpsPort)) {
      Serial.println("connection Server failed");
      return;
    }
    delay(100);
    String postRequest = (String)("GET ") + data + " HTTP/1.1\r\n" + "Content-Type: text/html;charset=utf-8\r\n" + "Host: " + host + "\r\n" + "User-Agent: BuildFailureDetectorESP8266\r\n" + "Connection: Keep Alive\r\n\r\n";
    Client.print(postRequest); 
    Client.stop();
    delay(1000);
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

//--------publishButtonName-----------
void publishButtonName() {
  int findcount = 1;
  uint16_t Position = ircodestart;
  uint16_t IRpos;
  int none =1;
  String getname;
  String namestack;
  char namec[100];
  eepromload();
  while (1)
  { server.handleClient();
    client.loop();
    if (findcount == 1)
    { 
      findcount = findname(Position, getname, IRpos);
      namestack += getname;
      namestack += " ";
      none =0;
      delay(100);
    }

    else if (findcount == 0 && !none) {
      break;  //找完了
    }
    else if (none) {
      Serial.println("none");
      return; //沒有任何按鈕
    }    
  }
     namestack[namestack.length()-1] = '\0';
     publishstr("/8266/Callback/EEPROM", namestack);
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
        publishstr("/8266/Callback/LearnCallback","fail");
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
//-------publishstr()--------
void publishstr(String topic , String message){
  char topchar[50];
  char messagechar[100];
  String idtopic = esp8266id + topic ;
  idtopic.toCharArray(topchar,50);
  message.toCharArray(messagechar,100);
  client.publish(topchar,messagechar);
  
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
      publishstr("/8266","connected");
      topic = 0;
    }
  }
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
