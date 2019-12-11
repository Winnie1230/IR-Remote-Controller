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
