#include "myeeprom.h"
#include <EEPROM.h>
/* *****************************
  這幾個函數都要自己丟入要存入的位置
  所以通常執行完函數之後要記得更新記錄 記憶體位置的變數
  ****************************** 
 */
//---------全域變數們的宣告方法----------

 // volatile double velocityA; //馬達速度
  uint16_t currentPosition; //EEProm初始存放位置
  uint8_t eeprommatrix[4000];
  
//---------函數放的地方-----------------

  uint16_t namewrite(uint16_t pos , String Name) {      //把name寫入eeprom的function
      uint8_t Length = Name.length();
      eeprommatrix[pos] = Length;  //第一位記錄Name的長度
      pos++;
      for (uint8_t i = 0 ; i < Length ; i++) {
        eeprommatrix[pos] = Name[i];
        pos++;
      }
      return Length + 1;
  }

 uint16_t nameread(uint16_t pos, String& Return) {      //讀出eeprom中的name
    uint8_t Length =  eeprommatrix[pos]; //第一位記錄Name的長度
    char Name[20];
    for (int i = 0; i < 20 ; i++)  Name[i] = '\0';
    pos++;
    for (int i = 0; i < Length ; i++) {
      Name[i] =  eeprommatrix[pos];
      pos++;
    }
    Return = Name;
    return Length + 1;
  }
  
  void numberwrite(uint16_t eepromaddr, uint16_t number) {   //寫入2bytes大小的數字到eeprom
    eeprommatrix[eepromaddr] = number >> 8;
    eepromaddr++;
    eeprommatrix[eepromaddr] = number;
  }
  
  void numberread(uint16_t pos , uint16_t& num) {     //讀出bytes大小的數字出來
    num = eeprommatrix[pos] << 8;
    pos++;
    num += eeprommatrix[pos];
  }

  //------------eepromupdate--------------
void eepromload(){     //先全部讀出來
  for(int i=0 ; i<4000 ; i++) 
    eeprommatrix[i] = EEPROM.read(i);  
    //currentPosition = eeprommatrix[0]<<8+eeprommatrix[1];
}

void eeprompush(){    //更新後再全部寫入
  for(int i=0 ; i<4000 ; i++) 
    EEPROM.write(i,eeprommatrix[i]);  
    EEPROM.commit();
}

 bool findname(uint16_t& startpos , String& answer , uint16_t& ircodepos) {
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

void reseteeprom(){     //出廠設定使用
  for(int i=2 ; i<4000 ; i++){
    EEPROM.write(i,0);
  }
  EEPROM.write(0,0);//初始化currentposition
  EEPROM.write(1,ircodestart);
  EEPROM.commit();
  
}
