/* INCLUDES */
#include <LiquidCrystal.h>
#include <Servo.h>

#define LEDVERDEPE A0 //parque esquerdo
#define LEDVERMELHOPE A1 //parque esquerdo vermelho
#define LEDVERDEPD A2
#define LEDVERMELHOPD A3

//INICIALIZAÇÃO DE LCD
const int rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//INICIALIZAÇÃO DE VARIÁVEIS GLOBAIS
int irSensor2 = 2;
int irSensor1 = 3;
int irSensorPE = 5;
int irSensorPD = 6;
int pos = 0;
int flag1 = 0;
int flag2 = 0;
int flag3 = 0;
int flag4 = 0;
int s1;
int s2;
int s3;
int s4;
int slot = 2;

Servo servo;

//SETUP

void setup() {
  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("LUSO PARKING");
  servo.attach(4);
  servo.write(0);
  countNumeroSlot();
  pinMode(irSensor1,INPUT);
  pinMode(irSensor2,INPUT);
  pinMode(irSensorPE,INPUT);
  pinMode(irSensorPD,INPUT);
  pinMode(LEDVERDEPE,OUTPUT);
  pinMode(LEDVERDEPD,OUTPUT);
  pinMode(LEDVERMELHOPE,OUTPUT);
  pinMode(LEDVERMELHOPD,OUTPUT);
  delay(1000);
}

void abrirCancela(){
  while(pos < 80){ 
      pos += 2;
      servo.write(pos);
      delay(20);
  }
}

void fecharCancela(){
  while(pos > 0){
    pos -= 2;
    servo.write(pos);
    delay(20);
  }
}


void countNumeroSlot(){
  s3 = digitalRead(irSensorPE);
  s4 = digitalRead(irSensorPD);
  if(s3 == LOW){
    slot -= 1;
  }
  if(s4 == LOW){
    slot -= 1;
  }
}

void reset(){
  flag1 = 0;
  flag2 = 0;
  flag3 = 0;
  flag4 = 0;
}

void loop() {


  /* LEITURA DE SENSORES DE CANCELAS */
  s1 = digitalRead(irSensor1);

  s2 = digitalRead(irSensor2);

  if(slot > 0){

    if(s1 == LOW && pos == 0){
      flag1 = 1; 
      abrirCancela();
    }

    if(s2 == LOW && flag2 == 0){
      flag2 = 1;
    }

    if(s2 == HIGH && flag2 == 1 && flag1 ==1){
      fecharCancela();
      reset();
      slot -= 1;
    }
  }

  if(slot < 2){

    if(s2 == LOW && pos == 0){ 
      flag3 = 1;
      abrirCancela();
    } 

    if(s1 == LOW && flag4 == 0){ 
      flag4 = 1;
    }

    if(s1 == HIGH && flag3 == 1 && flag4 == 1){
      fecharCancela();
      reset();
      slot += 1;
    }
  }

  /* LEITURA SENSORES DE PARQUE DE ESTACIONAMENTO */

  
  s3 = digitalRead(irSensorPE);

  s4 = digitalRead(irSensorPD);

  /*CONTROLO DE LEDS*/ 
  if(s3 == LOW){
    digitalWrite(LEDVERMELHOPE,HIGH);
    digitalWrite(LEDVERDEPE,LOW);
  } else {
    digitalWrite(LEDVERDEPE,HIGH);
    digitalWrite(LEDVERMELHOPE,LOW);
  }

  if(s4 == LOW){
    digitalWrite(LEDVERMELHOPD,HIGH);
    digitalWrite(LEDVERDEPD,LOW);
  } else {
    digitalWrite(LEDVERDEPD,HIGH);
    digitalWrite(LEDVERMELHOPD,LOW);
  }

  /*CONTROLO DE INFORMAÇÃO NO LCD 16X02*/

  if(slot == 0){
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("LUSO PARKING");
    lcd.setCursor(5,1);
    lcd.print("OCUPADO");
    delay(100);
  } else {
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("LUSO PARKING");
    lcd.setCursor(2,1);
    lcd.print("PARQUE LIVRE");
    delay(100);
  }

}