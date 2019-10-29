//用URL將esp8266連至家用網路
//連至MQTTserver
//接收、推播MQTT的訊息

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <PubSubClient.h>

#ifndef APSSID
#define APSSID "HIHIesp8266"
#define APPSK  "qwer12345"
#endif

/* Set these to your desired credentials. */
const char *ssid = APSSID; //esp8266 APmode密碼
const char *password = APPSK;
const char* mqttServer = "soldier.cloudmqtt.com"; //MQTTserver密碼
const int mqttPort = 10155;
const char* mqttUser = "gqozliri";
const char* mqttPassword = "jDZQvmnvp-Bx";
String homessid=""; //家用wifi名稱
String homepassword="";

ESP8266WebServer server(80);
WiFiClient espClient;
PubSubClient client(espClient);

//--------------setup------------------
void setup() {
  delay(1000);
  Serial.begin(115200);
  WiFi.softAP(ssid, password); //設定apmode的密碼

  IPAddress myIP = WiFi.softAPIP();
  server.on("/homewifi", homewifift);
  server.begin();
  //client.publish("esp/test", "hello"); //Topic name
  
}
//----------------loop-------------------
int topic;
void loop() {
  server.handleClient();
  client.loop();
  if(client.connect("ESP8266Client", mqttUser, mqttPassword )&& !topic){
    client.subscribe("esp/test");
    topic=1;
  }
}

//homewifift()接收URL的家用wifi密碼
void homewifift() {
  server.send(200, "text/html", "<h1>You are connected</h1>");
  if(server.hasArg("ssid")){
    homessid = server.arg("ssid");
    Serial.println(homessid);
  }
  if(server.hasArg("password")){
    homepassword = server.arg("password");
    Serial.println(homepassword);
  } 
  WiFi.begin(homessid,homepassword); //esp8266連到家中wifi
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback); //用於接收server的訂閱topic
 // client.setCallback(callback);
  while (!client.connected()) {
    delay(500);
    Serial.println("connect to mqtt...");
    if (client.connect("ESP8266Client", mqttUser, mqttPassword )){
      Serial.println("connected");  
      topic=0;   
    }
  }
  //client.subscribe("esp/test");
}

//callback()用於接收server的訂閱topic
void callback(char* topic, byte* payload, unsigned int length) {
      String callMsg = "";
      Serial.print("Message arrived [");
      Serial.print(topic);   // 打印主题信息
      Serial.print("] ");
    
      for (int i = 0; i < length; i++) {
        callMsg += char(payload[i]);
      }      
      Serial.println(callMsg);
  }
