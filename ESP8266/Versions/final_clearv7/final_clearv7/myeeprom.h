#ifndef _MY_EEPROM
#define _MY_EEPROM
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

//-------定義腳位--------
//#define encoderA A1
 #define ircodestart 60  //ircode 開始位置，若改變記憶體存放方式要修改

//-----------全域變數們宣告-----------
// extern volatile double velocityA; //馬達速度  volatile用於中斷變數
  extern uint16_t currentPosition; //EEProm初始存放位置
  extern uint8_t eeprommatrix[]; //把eeprom全部讀出來
//-----------函數宣告---------------
// void picontrolA(double, double);
  uint16_t namewrite(uint16_t,String);
  uint16_t nameread(uint16_t, String&);
  void namewrite2(uint16_t, String);
  void nameread2(uint16_t,String&); 
  void numberwrite(uint16_t,uint16_t);
  void numberread(uint16_t,uint16_t&);
  bool findname(uint16_t&,String&,uint16_t&);
  void eepromload();
  void eeprompush();
  void reseteeprom();
  void eepromerase();
 #endif
