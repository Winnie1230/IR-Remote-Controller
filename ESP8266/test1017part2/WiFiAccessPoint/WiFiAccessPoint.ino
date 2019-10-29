#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#ifndef APSSID
#define APSSID "fuck"
#define APPSK  "fuck1234567" //wifi密碼要大於8位數
#endif

/* Set these to your desired credentials. */
const char *apssid = APSSID;
const char *appassword = APPSK;
String ssid="" ; // Enter your WiFi name
String password="" ; // Enter WiFi password
const char* mqttServer = "soldier.cloudmqtt.com";
const int mqttPort = 10155;
const char* mqttUser = "gqozliri";
const char* mqttPassword = "jDZQvmnvp-Bx";


void setup() {
  Serial.begin ( 115200 );
  Serial.println("");
  // 設置內網
  IPAddress softLocal(192,168,128,1);   // 1 設置內網WIFI IP地址
  IPAddress softGateway(192,168,128,1);
  IPAddress softSubnet(255,255,255,0);
  WiFi.softAPConfig(softLocal, softGateway, softSubnet);
  
  String apName = ("ESP8266_"+(String)ESP.getChipId());  // 2 設置WIFI名稱
  const char *softAPName = apName.c_str();
  
  WiFi.softAP(softAPName, "adminadmin");      // 3創建wifi  名稱 +密碼 adminadmin
  
  IPAddress myIP = WiFi.softAPIP();  // 4輸出創建的WIFI IP地址
  Serial.print("AP IP address: ");      
  Serial.println(myIP);
  
  Serial.print("softAPName: ");  // 5輸出WIFI 名稱
  Serial.println(apName);
}
String instring="";
char x;

void loop() {
    if(Serial.available()){  
        delay(100);       
        x=Serial.read();
        if(x!=' ' && x!='\n'){
          instring += x;
        }
        else if(x==' '){
          ssid=instring;
          instring="";
        }
        else if(x=='\n'){
          password=instring;
          instring="";
        }  
      if(WiFi.status() != WL_CONNECTED && password!="" && ssid!="") {     
        Serial.println(ssid);
        Serial.println(password);  
        WiFi.begin(ssid, password); 
        Serial.println("Connecting to WiFi..");
      }
    Serial.println("ok");
   }
   
  
}
