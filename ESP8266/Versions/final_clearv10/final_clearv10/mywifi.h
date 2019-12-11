#ifndef _MY_WIFI
#define _MY_WIFI
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

//-------定義區--------
//#define encoderA A1
  #define APSSID "espl22"
  #define APPSK  "0123456789"
  #define WIFI_TIMEOUT 10 //10*0.5 = 5 second to connect to wifi 
  
//-----------全域變數們宣告-----------
// extern volatile double velocityA; //馬達速度  volatile用於中斷變數
  extern const char*  host; //需要訪問的域名 
  extern const char* mqttServer; //MQTTserver密碼
  extern const char* mqttUser;
  extern const char* mqttPassword;
  extern const int httpsPort; // 需要訪問的埠
  extern const int mqttPort ;
  extern String CO2state ; //二氧化碳感測器
  extern String pm25state ; //pm2.5狀態
  extern String pm10state ; //pm10狀態
  extern String hchostate ; //hcho狀態
  extern String tvocstate ; //TVOC狀態
  extern String humidstate ; //濕度狀態
  extern String temperstate ; //溫度狀態
  extern String Currentstate; //電流感測器
  
  extern String Tempstate;
  extern String homessid; //家用wifi名稱
  extern String homepassword; 
  extern int topic;
  extern  String state;
 
//-----------函數宣告---------------

bool WiFiCheck();
void MqttSetting();
 #endif
