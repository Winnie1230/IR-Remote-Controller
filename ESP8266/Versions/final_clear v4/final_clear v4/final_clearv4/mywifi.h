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
  
//-----------全域變數們宣告-----------
// extern volatile double velocityA; //馬達速度  volatile用於中斷變數
  extern const char*  host; //需要訪問的域名 
  extern const char* mqttServer; //MQTTserver密碼
  extern const char* mqttUser;
  extern const char* mqttPassword;
  extern const int httpsPort; // 需要訪問的埠
  extern const int mqttPort ;
  extern String CO2state ; //二氧化碳感測器
  extern String Currentstate; //電流感測器
  extern String Pmstate ;
  extern String Tempstate ;
  extern String url ;
  extern String homessid; //家用wifi名稱
  extern String homepassword; 
  extern int topic;
  extern int topicstate; 
  extern  String state;
 
//-----------函數宣告---------------
 #endif
