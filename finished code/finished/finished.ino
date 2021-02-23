#include <SPI.h>
#include <RFID.h>
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

Servo myservo;
#define greenbtn 7
#define I2C_ADDR 0x27 // กำหนดตำแหน่ง Address ของ I2C
#define BACKLIGHT_PIN 3
#define SDA_DIO 10
#define RESET_DIO 9
RFID RFID(SDA_DIO, RESET_DIO);
LiquidCrystal_I2C lcd(I2C_ADDR,2,1,0,4,5,6,7);
int buzzer = 6;

String data[3][4] = {
  {"4BE59399A4","1610900266","1"},
  {"7BA1C79588","1610900498","1"},
  {"16D0B5F88B","1234567890","0"}
};
String input;

void setup() {
  Serial.begin(9600);
  myservo.attach(3);
  myservo.write(148);
  pinMode(greenbtn, INPUT);
  lcd.begin(16,2);
  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.home();
  lcd.setCursor(4,0);
  lcd.print("Welcome!");
  lcd.home();
  lcd.setCursor(2,1);
  lcd.print("Press a card");
  
  SPI.begin();
  RFID.init();

  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, HIGH);
  
}

void loop() {
    
    if(RFID.isCard()){
      if(RFID.readCardSerial()){
        String strID = "";
        for (byte i = 0; i < 5; i++) {
          strID +=
          String(RFID.serNum[i],HEX);
        }
      strID.toUpperCase();
      digitalWrite(buzzer, LOW);
      delay(200);
      digitalWrite(buzzer, HIGH);
      delay(500);
      for(int i=0;i<5;i++){
        if(strID.indexOf(data[i][0])>=0)
        { 
          if(data[i][2] == "1"){
            lcd.clear();
            lcd.home();
            lcd.setCursor(5,0);
            lcd.print("Please");
            lcd.setCursor(4,1);
            lcd.print("Waiting..");
            delay(1500);        
            lcd.clear();
            lcd.home();
            lcd.setCursor(3,0);
            lcd.print("Student ID");
            lcd.setCursor(3,1);
            lcd.print(data[i][1]);
            delay(2000);
            lcd.clear();
            lcd.home();
            lcd.setCursor(3,0);
            lcd.print("Green = OK");
            lcd.setCursor(2,1);
            lcd.print("Red = Cancel");
            while(true){ //Wait, press the green button. "btnState = HIGH :exit loop"
              if(digitalRead(greenbtn) == HIGH){
                break;
              }
            }
            myservo.write(148);
            delay(2000);
            myservo.write(90);
            delay(1500);
            myservo.write(0);
            delay(2000);
            myservo.write(148);
            delay(1000);
            lcd.clear();
            lcd.home();
            lcd.setCursor(3,1);
            lcd.print("Success ^U^");
            lcd.setCursor(5,0);
            lcd.print("ENJOY!");
            delay(2000);
            lcd.clear();
            lcd.home();
            lcd.setCursor(4,0);
            lcd.print("Welcome!");
            lcd.home();
            lcd.setCursor(2,1);
            lcd.print("Press a card");
            break;
          }else if(data[i][2] == "0"){
            lcd.clear();
            lcd.home();
            lcd.setCursor(5,0);
            lcd.print("Plese");
            lcd.setCursor(4,1);
            lcd.print("Waiting..");
            delay(1500);
            lcd.clear();
            lcd.home();
            lcd.setCursor(2,0);
            lcd.print("This card is");
            lcd.setCursor(2,1);
            lcd.print("Unauthorized!");
            delay(1500);
            digitalWrite(buzzer, LOW);
            delay(50);
            digitalWrite(buzzer, HIGH);
            delay(50);
            digitalWrite(buzzer, LOW);
            delay(50);
            digitalWrite(buzzer, HIGH);
            delay(1500);
            lcd.clear();
            lcd.home();
            lcd.setCursor(4,0);
            lcd.print("Welcome!");
            lcd.setCursor(2,1);
            lcd.print("Press a card");
            //return;
          }
        }
      }
    }
  }
}
