#include "SevSeg.h"
#include "BluetoothSerial.h"
#include <Tone32.h>
#include <LiquidCrystal.h>

#define BUZZER_PIN 12
#define BUZZER_CHANNEL 0

LiquidCrystal lcd(32, 33, 25, 26, 27, 14);
SevSeg Display;
BluetoothSerial SerialBT;

char receivedMsg;// received value will be stored as CHAR in this variable

const char timer = 't';
const char seconds = 's';
const char minutes = 'm';
const char hours = 'h';
const char quit = 'q';
char alarmChar = 'a';

//global for switching between alarms
bool alarmSetting = true;

void setup(){
  
  Serial.begin(9600);
  SerialBT.begin("ESP-32");
  lcd.begin(16, 2);

  
  lcd.print("time.out");
  lcd.setCursor(13,2);
  lcd.print("RF");
  miniMilk();
  setupDisplay();
  alarmSetup();
  delay(5000);
  lcd.clear();
  lcd.print("Enter (t)imer");
  lcd.setCursor(0,1);
  lcd.print("to begin");
  milk();
  
  SerialBT.println("Enter (t)imer to begin");

}



void loop(){ 


  //Serial.println("top of loop");
  receivedMsg =(char)SerialBT.read();
  
  
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  
  if (SerialBT.available()) {
      Serial.print(receivedMsg);
  }

  Serial.println(alarmSetting);
  if (SerialBT.available() && receivedMsg == alarmChar){
    
    SerialBT.println("Choose Alarm: Buzzer (1) Morning Flower(2)");
    lcd.clear();
    lcd.print("Buzzer(1)");
    lcd.setCursor(0,1);
    lcd.print("MorningFlower(2)");
    while(1){
      receivedMsg = (char)SerialBT.read();
      if(SerialBT.available() && receivedMsg == '1'){
        alarmSetting = true;
        break;
      }
      if(SerialBT.available() && receivedMsg == '2'){
        alarmSetting = false;
        break;
      }
      if(SerialBT.available() && receivedMsg == quit){
        break;
      }
    }
      SerialBT.println("Enter (t)imer to begin");
      lcd.clear();
      lcd.print("Enter (t)imer");
      lcd.setCursor(0,1);
      lcd.print("to begin");
      milk();
 
  }





  
  if (SerialBT.available() && receivedMsg == timer){
    SerialBT.println("Enter (s)ec (m)in or (h)our");
    lcd.clear();
    lcd.print("Enter (s)ec");
    lcd.setCursor(0,1);
    lcd.print("(m)in (h)our");


    
    while(1){
      
      receivedMsg =(char)SerialBT.read();
      
      if(SerialBT.available() && receivedMsg == seconds){
            SerialBT.println("15(1) 30(2) 45(3) 60(4)");
            lcd.clear();
            lcd.print("15(1)   30(2)");
            lcd.setCursor(0,1);
            lcd.print("45(3)   60(4)");

            while(1){
                receivedMsg =(char)SerialBT.read();
                if(SerialBT.available() && (receivedMsg == '1' || receivedMsg == '2' || receivedMsg == '3' || receivedMsg == '4')){
                    secs(receivedMsg);
                    lcd.clear();
                    lcd.setCursor(3,0);
                    lcd.print("Time's Up!");
                    if(!alarmSetting){

                        morningFlower();
                        
                    }
                    
                    if(alarmSetting){

                      alarm();
                      
                    }
                    break;
                }

                if(SerialBT.available() && receivedMsg == quit){
                    break;
                }
                
        
        }
        break;
      }

      if(SerialBT.available() && receivedMsg == minutes){

        SerialBT.println("3(1) 5(2) 10(3) 15(4) 30(5) 45(6)");
        lcd.clear();
        lcd.print("3(1) 5(2) 10(3)");
        lcd.setCursor(0,1);
        lcd.print("15(4)30(5)45(6)");
        
        while(1){
              receivedMsg =(char)SerialBT.read();
              if(SerialBT.available() && (receivedMsg == '1' || receivedMsg == '2' || receivedMsg == '3' || receivedMsg == '4' || receivedMsg == '5' || receivedMsg == '6')){
                  Serial.println(receivedMsg);
                  mins(receivedMsg);
                  lcd.clear();
                  lcd.setCursor(3,0);
                  lcd.print("Time's Up!");
                  if(!alarmSetting){
                    morningFlower();
                  }
                  if(alarmSetting){
                    alarm();
                  }
                  break;
            }
            if(SerialBT.available() && receivedMsg == quit){
                    break;
            }
            
        }
        break;
      }
      
      if(SerialBT.available() && receivedMsg == hours){
        SerialBT.println("1(1) 1.5(2) 2(3) 3(4) 6(5) 12(6)");
        lcd.clear();
        lcd.print("1(1) 1.5(2) 2(3)");
        lcd.setCursor(0,1);
        lcd.print("3(4) 6(5) 12(6)");

          while(1){
          receivedMsg =(char)SerialBT.read();
          if(SerialBT.available() && (receivedMsg == '1' || receivedMsg == '2' || receivedMsg == '3' || receivedMsg == '4' || receivedMsg == '5' || receivedMsg == '6')){

              hrs(receivedMsg);
              if(!alarmSetting){
                  morningFlower();
              }
              if(alarmSetting){
                  alarm();
              }
              break;
          }
          if(SerialBT.available() && receivedMsg == quit){
                    break;
                }
        }
        break;
      }


      if(SerialBT.available() && receivedMsg == quit){
        break;
      }

   delay(50);
  }
  //Serial.println("bottom of loop");
  
  delay(20);
      SerialBT.println("Enter (t)imer to begin");
      lcd.clear();
      lcd.print("Enter (t)imer");
      lcd.setCursor(0,1);
      lcd.print("to begin");  
      milk();
      //Display.refreshDisplay();
      Display.blank();

    }
}



void setupDisplay(){
  byte numDigits = 2;
  byte digitPins[] = {22, 23};
  byte segmentPins[] = {15, 2, 4, 5, 18, 19, 21, 0};
  bool resistorsOnSegments = true;
  bool updateWithDelaysIn = true;
  byte hardwareConfig = COMMON_ANODE;
  Display.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments);
  Display.setBrightness(100);
}

///////////////////////////////////////
///////////////seconds/////////////////
///////////////////////////////////////
void secs(char receivedMsg){
    int x;
    switch(receivedMsg){
        case '1': 
          x = 1;
          break;
        case '2': 
          x = 2;
          break;
        case '3':
          x = 3;
          break;
        case '4':
          x = 4;
          break;
    }

    
  
    unsigned int number = (15*x)+1;
    const unsigned long period = 1000; // in millisecond
    unsigned long startMillis;
    unsigned long currentMillis;

    SerialBT.print(number-1);
    SerialBT.println(" s timer");
    lcd.clear();
    lcd.setCursor(3,0);
    lcd.print(number-1);
    lcd.print(" s timer");
    
    Display.refreshDisplay();
    
      while(number != 0){
        currentMillis = millis();
      if (currentMillis - startMillis >= period)
      { 
        number--;
        startMillis = currentMillis;
      }
    
      if(number <= 0)
      { 
        Display.refreshDisplay();
        Display.blank();
        break;
      }

      Display.setNumber(number);
      Display.refreshDisplay();
    }
    
    Display.blank();

      
}

///////////////////////////////////////
///////////////minutes/////////////////
///////////////////////////////////////
void mins(char receivedMsg){
    lcd.clear();
  
    int x;
    switch(receivedMsg){
        case '1': 
          x = 3;
          break;
        case '2': 
          x = 5;
          break;
        case '3':
          x = 10;
          break;
        case '4':
          x = 15;
          break;
        case '5': 
          x = 30;
          break;
        case '6': 
          x = 45;
          break;  
        case '7':
          x = 60;
          break;
        case '8':
          x = 90;
          break;

        //cases 7 and 8 can be invoked through hours();
    }

    
    unsigned int number = (x+1);
    const unsigned long period = 200; // in millisecond
    unsigned long startMillis;
    unsigned long currentMillis;
    Display.refreshDisplay();

    SerialBT.print(number-1);
    SerialBT.println(" min timer");
    lcd.setCursor(3,0);
    lcd.print(number-1);
    lcd.print(" min timer");
    
      while(number != 0){
          currentMillis = millis();
          
          if (currentMillis - startMillis >= period){
              number--;
              startMillis = currentMillis;
          }
        
          if(number <= 0){
              Display.blank();
              break;
          }
        
          Display.setNumber(number);
          Display.refreshDisplay();
      }
    
    Display.blank();


  
}

///////////////////////////////////////
///////////////hours///////////////////
///////////////////////////////////////
void hrs(char receivedMsg){

    lcd.clear();
    int x;
    switch(receivedMsg){
        case '1': 
          mins('7'); //60 mins
          return;
        case '2': 
          mins('8'); //90 mins
          return;
        case '3':
          x = 2; //120 mins
          break;
        case '4':
          x = 3; //3 hrs
          break;
        case '5':  
          x = 6; //6 hrs
          break;
        case '6': 
          x = 12; //12 hrs
          break;
    }

 
    unsigned int number = x+1;
    const unsigned long period = 5000; // in millisecond
    unsigned long startMillis;
    unsigned long currentMillis;
    Display.refreshDisplay();

    SerialBT.print(number-1);
    SerialBT.println(" hr timer");
    lcd.setCursor(3,0);
    lcd.print(number-1);
    lcd.print(" hour timer");
    
      while(number != 0){
        currentMillis = millis();
      if (currentMillis - startMillis >= period)
      {
        number--;
        startMillis = currentMillis;
      }
    
      if(number <= 0)
      { 
        Display.blank();
        break;
      }
    
      Display.setNumber(number);
      Display.refreshDisplay();
    }
    
    Display.blank();
}

void alarm(){
  int freq = 2000;
  int channel = 0;
  int resolution = 8;
  Display.blank();
  ledcSetup(channel, freq, resolution);
  ledcAttachPin(12, channel);
  for(int j = 0; j < 3; j++){
    for(int i = 0; i < 3; i++){
      ledcWrite(channel, 200);
      delay(50);
      ledcWrite(channel, 0);
      delay(50);
    }
  delay(750);
 }
}

void alarmSetup(){
  int freq = 2000;
  int channel = 0;
  int resolution = 8;
  ledcSetup(channel, freq, resolution);
  ledcAttachPin(12, channel);
}

//half an hour well spent
void morningFlower(){
  //450 = 130 bpm quarter note
  //225 = 130 bpm eight note
  tone(BUZZER_PIN, NOTE_G4, 450, BUZZER_CHANNEL);
  noTone(BUZZER_PIN, BUZZER_CHANNEL);
  tone(BUZZER_PIN, NOTE_E4, 225, BUZZER_CHANNEL);
  noTone(BUZZER_PIN, BUZZER_CHANNEL);
  tone(BUZZER_PIN, NOTE_C4, 450, BUZZER_CHANNEL);
  noTone(BUZZER_PIN, BUZZER_CHANNEL);
  tone(BUZZER_PIN, NOTE_A4, 450, BUZZER_CHANNEL);
  noTone(BUZZER_PIN, BUZZER_CHANNEL);
  tone(BUZZER_PIN, NOTE_G4, 900, BUZZER_CHANNEL);
  noTone(BUZZER_PIN, BUZZER_CHANNEL);
  //m2
  delay(450);
  tone(BUZZER_PIN, NOTE_E4, 450, BUZZER_CHANNEL);
  noTone(BUZZER_PIN, BUZZER_CHANNEL);
  tone(BUZZER_PIN, NOTE_G4, 225, BUZZER_CHANNEL);
  noTone(BUZZER_PIN, BUZZER_CHANNEL);
  //m3
  tone(BUZZER_PIN, NOTE_F4, 450, BUZZER_CHANNEL);
  noTone(BUZZER_PIN, BUZZER_CHANNEL);
  tone(BUZZER_PIN, NOTE_G4, 225, BUZZER_CHANNEL);
  noTone(BUZZER_PIN, BUZZER_CHANNEL);
  tone(BUZZER_PIN, NOTE_F4, 450, BUZZER_CHANNEL);
  noTone(BUZZER_PIN, BUZZER_CHANNEL);
  tone(BUZZER_PIN, NOTE_D4, 450, BUZZER_CHANNEL);
  noTone(BUZZER_PIN, BUZZER_CHANNEL);
  tone(BUZZER_PIN, NOTE_E4, 450, BUZZER_CHANNEL);
  noTone(BUZZER_PIN, BUZZER_CHANNEL);
  //m4
  tone(BUZZER_PIN, NOTE_E5, 225, BUZZER_CHANNEL);
  noTone(BUZZER_PIN, BUZZER_CHANNEL);
  tone(BUZZER_PIN, NOTE_C5, 225, BUZZER_CHANNEL);
  noTone(BUZZER_PIN, BUZZER_CHANNEL);
  tone(BUZZER_PIN, NOTE_G4, 225, BUZZER_CHANNEL);
  noTone(BUZZER_PIN, BUZZER_CHANNEL);
  tone(BUZZER_PIN, NOTE_C5, 225, BUZZER_CHANNEL);
  noTone(BUZZER_PIN, BUZZER_CHANNEL);
  tone(BUZZER_PIN, NOTE_G4, 225, BUZZER_CHANNEL);
  noTone(BUZZER_PIN, BUZZER_CHANNEL);
  tone(BUZZER_PIN, NOTE_E4, 450, BUZZER_CHANNEL);
  noTone(BUZZER_PIN, BUZZER_CHANNEL);
  //m5
  tone(BUZZER_PIN, NOTE_A4, 450, BUZZER_CHANNEL);
  noTone(BUZZER_PIN, BUZZER_CHANNEL);
  tone(BUZZER_PIN, NOTE_G4, 225, BUZZER_CHANNEL);
  noTone(BUZZER_PIN, BUZZER_CHANNEL);
  tone(BUZZER_PIN, NOTE_E4, 450, BUZZER_CHANNEL);
  noTone(BUZZER_PIN, BUZZER_CHANNEL);
  tone(BUZZER_PIN, NOTE_C5, 450, BUZZER_CHANNEL);
  noTone(BUZZER_PIN, BUZZER_CHANNEL);
  tone(BUZZER_PIN, NOTE_G4, 900, BUZZER_CHANNEL);
  noTone(BUZZER_PIN, BUZZER_CHANNEL);
  //m6
  delay(450);
  tone(BUZZER_PIN, NOTE_A4, 675, BUZZER_CHANNEL);
  noTone(BUZZER_PIN, BUZZER_CHANNEL);    
  //m7
  tone(BUZZER_PIN, NOTE_B4, 450, BUZZER_CHANNEL);
  noTone(BUZZER_PIN, BUZZER_CHANNEL);
  tone(BUZZER_PIN, NOTE_A4, 225, BUZZER_CHANNEL);
  noTone(BUZZER_PIN, BUZZER_CHANNEL);
  tone(BUZZER_PIN, NOTE_F4, 450, BUZZER_CHANNEL);
  noTone(BUZZER_PIN, BUZZER_CHANNEL);
  tone(BUZZER_PIN, NOTE_D4, 450, BUZZER_CHANNEL);
  noTone(BUZZER_PIN, BUZZER_CHANNEL);
  tone(BUZZER_PIN, NOTE_C4, 225, BUZZER_CHANNEL);
  noTone(BUZZER_PIN, BUZZER_CHANNEL);
  //m8
  tone(BUZZER_PIN, NOTE_G2, 56, BUZZER_CHANNEL);
  noTone(BUZZER_PIN, BUZZER_CHANNEL);
  tone(BUZZER_PIN, NOTE_C3, 56, BUZZER_CHANNEL);
  noTone(BUZZER_PIN, BUZZER_CHANNEL);
  tone(BUZZER_PIN, NOTE_E3, 56, BUZZER_CHANNEL);
  noTone(BUZZER_PIN, BUZZER_CHANNEL);
  tone(BUZZER_PIN, NOTE_G3, 56, BUZZER_CHANNEL);
  noTone(BUZZER_PIN, BUZZER_CHANNEL);
  tone(BUZZER_PIN, NOTE_C4, 112, BUZZER_CHANNEL);
  noTone(BUZZER_PIN, BUZZER_CHANNEL);   
  tone(BUZZER_PIN, NOTE_E4, 112, BUZZER_CHANNEL);
  noTone(BUZZER_PIN, BUZZER_CHANNEL);
  tone(BUZZER_PIN, NOTE_G4, 112, BUZZER_CHANNEL);
  noTone(BUZZER_PIN, BUZZER_CHANNEL);
  tone(BUZZER_PIN, NOTE_C5, 112, BUZZER_CHANNEL);
  noTone(BUZZER_PIN, BUZZER_CHANNEL);
  tone(BUZZER_PIN, NOTE_C4, 56, BUZZER_CHANNEL);
  noTone(BUZZER_PIN, BUZZER_CHANNEL);   
  tone(BUZZER_PIN, NOTE_E4, 56, BUZZER_CHANNEL);
  noTone(BUZZER_PIN, BUZZER_CHANNEL);
  tone(BUZZER_PIN, NOTE_G4, 56, BUZZER_CHANNEL);
  noTone(BUZZER_PIN, BUZZER_CHANNEL);
  tone(BUZZER_PIN, NOTE_C5, 450, BUZZER_CHANNEL);
  noTone(BUZZER_PIN, BUZZER_CHANNEL);
  //fermata :o
}

void milk(){

byte milk1[] = {
  B00000,
  B00011,
  B00100,
  B00111,
  B00100,
  B00111,
  B00111,
  B00111
};

byte milk2[] = {
  B00000,
  B11000,
  B00100,
  B11100,
  B00100,
  B11100,
  B11100,
  B11100
};

byte milk3[] = {
  B00111,
  B00111,
  B00111,
  B00111,
  B00111,
  B00111,
  B00011,
  B00000
};

byte milk4[] = {
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11000,
  B00000
};

    lcd.createChar(1, milk1);
    lcd.createChar(2, milk2);
    lcd.createChar(3, milk3);
    lcd.createChar(4, milk4);
    lcd.setCursor(14,0);
    lcd.write(byte(1));
    lcd.setCursor(15,0);
    lcd.write(byte(2));
    lcd.setCursor(14,1);
    lcd.write(byte(3));
    lcd.setCursor(15,1);
    lcd.write(byte(4));
  
}


void miniMilk(){

  byte milk0[] = {
  B01110,
  B10001,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B01110
};
  lcd.createChar(0, milk0);
  lcd.setCursor(15,1);
  lcd.write(byte(0));

}
