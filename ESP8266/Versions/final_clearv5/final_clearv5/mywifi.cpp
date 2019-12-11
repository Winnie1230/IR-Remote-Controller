#include "mywifi.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <PubSubClient.h>
#include <EEPROM.h>

//---------全域變數們的宣告方法----------
 // volatile double velocityA; //馬達速度
 int outputA; //輸出的pwm
 const char* host = "winnieliu.ddns.net"; //需要訪問的域名 
 const char* mqttServer = "tailor.cloudmqtt.com"; //MQTTserver密碼
 const char* mqttUser = "ulvjkbqk";
 const char* mqttPassword = "TeaSomjnH7rb";
 const int httpsPort = 11230; // 需要訪問的埠
 const int mqttPort = 16109;
 String CO2state = "on";
 String Currentstate = "off";
 String Pmstate = "off";
 String Tempstate = "on";
 String url = "/state?co2=" + CO2state + "&pm=" + Pmstate + "&current=" + Currentstate + "&temp=" + Tempstate;
 String homessid = "Eating12345"; //家用wifi名稱
 String homepassword = "hsieh861227";
 String state = "";
 int topic;
 int topicstate;

 
//---------函數放的地方-----------------
//----------WiFiCheck()----------
bool WiFiCheck(){
    int wifi_connect_time = 0;
  // attempt to connect to Wifi network:
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED){
    if(wifi_connect_time > WIFI_TIMEOUT){ //5s
      Serial.println();
      Serial.println("connecting time out. Please enter ssid and password.");  //請使用者輸入wifi資料
      break;
    }
    else{
      delay(500);
      Serial.print(".");
      wifi_connect_time ++;
    }
  }

  if(WiFi.status() == WL_CONNECTED){
    Serial.println();
    Serial.print("Connected, IP address: ");
    Serial.println(WiFi.localIP());
    return true;
  }
  else
    return false;
}
