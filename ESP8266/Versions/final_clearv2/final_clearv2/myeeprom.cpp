#include "myeeprom.h"
#include <EEPROM.h>
/* *****************************
  這幾個函數都要自己丟入要存入的位置
  所以通常執行完函數之後要記得更新記錄 記憶體位置的變數
  ****************************** 
 */
//---------全域變數們的宣告方法----------
 // volatile double velocityA; //馬達速度
  int currentPosition = 2; //EEProm初始存放位置

//---------函數放的地方-----------------

  int namewrite(int eepromaddr , String Name) {      //把name寫入eeprom的function
    uint8_t Length = Name.length();
  
    EEPROM.write(eepromaddr, Length); //第一位記錄Name的長度
    eepromaddr++;
    for (uint8_t i = 0 ; i < Length ; i++) {
      EEPROM.write(eepromaddr, Name[i]); //第一位記錄Name的長度
      eepromaddr++;
    }
    EEPROM.commit();
    return Length + 1;
  }
  
  int nameread(int pos, String& Return) {      //讀出eeprom中的name
    uint8_t Length = EEPROM.read(pos); //第一位記錄Name的長度
    char Name[20];
    for (int i = 0; i < 20 ; i++)  Name[i] = '\0';
  
    //Serial.println(Length);
  
    pos++;
    for (int i = 0; i < Length ; i++) {
      Name[i] = EEPROM.read(pos);
      pos++;
    }
    Return = Name;
    return Length + 1;
  }
  
  void numberwrite(int eepromaddr, uint16_t number) {   //寫入2bytes大小的數字到eeprom
    EEPROM.write(eepromaddr, number >> 8);
    eepromaddr++;
    EEPROM.write(eepromaddr, number);
    EEPROM.commit();
  }
  
  void numberread(int pos , uint16_t& num) {     //讀出bytes大小的數字出來
  
    num = EEPROM.read(pos) << 8;
    pos++;
    num += EEPROM.read(pos);
  }

 bool findname(int& startpos , String& answer , int& ircodepos) {
  uint16_t temp = 0; //暫存變數
  if (startpos < currentPosition) { //如果還有資料繼續找

    startpos += nameread(startpos, answer);
    numberread(startpos, temp);
    ircodepos = startpos;

    startpos += ((temp * 2) + 2);

    //有資料可以回傳
  }
  if (startpos == currentPosition)return 0;
  else return 1; //沒有資料可以回傳
}
