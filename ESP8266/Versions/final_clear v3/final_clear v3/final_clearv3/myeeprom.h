#ifndef _MY_EEPROM
#define _MY_EEPROM
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

//-------定義腳位--------
//#define encoderA A1


//-----------全域變數們宣告-----------
// extern volatile double velocityA; //馬達速度  volatile用於中斷變數
  extern int currentPosition; //EEProm初始存放位置
  extern uint8_t eeprommatrix[]; //把eeprom全部讀出來
//-----------函數宣告---------------
// void picontrolA(double, double);
  int namewrite(int,String);
  int nameread(int, String&);  
  void numberwrite(int,uint16_t);
  void numberread(int,uint16_t&);
  bool findname(int&,String&,int&);
  void eepromload();
  void eeprompush();
  void reseteeprom();
 #endif
