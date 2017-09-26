// Serial Bluetooth
#include <SoftwareSerial.h>
SoftwareSerial BT(10, 11); //Rx,Tx

// Sensor
#include "DHT.h"
#define DHTPIN 8
#define Vcc 12
int GND = 9;
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

int ldr;

//int tones[] = {261, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 493};
#define speakerPin 13

// Motor
#define pwma 255
#define ENA 3
#define IN1 2
#define IN2 4
#define IN3 5
#define IN4 7
#define ENB 6
#define pwmb 255

// value from BT (App)
int Rxapp = 0; // value start
int count=0; //count
void ReadSensor() {
  delay(2000);
  ldr = analogRead(A0);
  ldr = map(ldr,0,1023,0,900);
  
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    BT.println("Failed to read from DHT sensor!");
    return;
  }
  /*Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");*/
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" *C  ");
  Serial.print("Lux: ");
  Serial.print(ldr);
  

  /*BT.print("Humidity: ");
  BT.print(h);
  BT.print(" %\t");*/
  BT.print("Temp: ");
  BT.print(t);
  BT.print(" Celsius  ");
BT.print("Lux: ");
BT.println(ldr);

}
void forward() {
  digitalWrite(IN1, 1);
  digitalWrite(IN2, 0);
  analogWrite(ENA, pwma);
  digitalWrite(IN3, 1);
  digitalWrite(IN4, 0);
  analogWrite(ENB, pwmb);

}
void reward() {
  digitalWrite(IN1, 0);
  digitalWrite(IN2, 1);
  analogWrite(ENA, pwma);
  digitalWrite(IN3, 0);
  digitalWrite(IN4, 1);
  analogWrite(ENB, pwmb);
}
void left() {
  digitalWrite(IN1, 1);
  digitalWrite(IN2, 0);
  analogWrite(ENA, pwma);
  digitalWrite(IN3, 0);
  digitalWrite(IN4, 0);
  analogWrite(ENB, 0);
}
void right() {
  digitalWrite(IN1, 0);
  digitalWrite(IN2, 0);
  analogWrite(ENA, 0);
  digitalWrite(IN3, 1);
  digitalWrite(IN4, 0);
  analogWrite(ENB, pwmb);
}
void stopm() {
  digitalWrite(IN1, 0);
  digitalWrite(IN2, 0);
  analogWrite(ENA, 0);
  digitalWrite(IN3, 0);
  digitalWrite(IN4, 0);
  analogWrite(ENB, 0);
}

void control() {
  if (Rxapp == 0) {
    stopm();
  }
  else if (Rxapp == 1) {
    forward();
  }
  else if (Rxapp == 2) {
    reward();
  }
  else if (Rxapp == 3) {
    left();
  }
  else if (Rxapp == 4) {
    right();
  }
}

void Buzzer(){
  tone(speakerPin, 493);
   delay(1000); 
  noTone(speakerPin);
}
//
//void LdrA(){
//  analogWrite(A2,255);
//}
//
//void LdrB(){
//  digitalWrite(LEDB,1);
//}
//
//void LdrC(){
//  digitalWrite(LEDC,1);
//}

void setup() {
  // Arduino
  Serial.begin(9600);
  Serial.println("start");

  // Sensor Air
  dht.begin();
 
  
  // Serial Bluetooth
  BT.begin(9600);

  // Motor
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(Vcc, OUTPUT);
  pinMode(GND, OUTPUT);
  digitalWrite(Vcc, HIGH);
  digitalWrite(GND, LOW);
    pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  digitalWrite(A1, HIGH);
  digitalWrite(A2, LOW);
  digitalWrite(A3, LOW);
}

void loop() {
 

  if (BT.available()) {
    Rxapp = BT.read();
    //Serial.println(Rxapp);

  }  
  if (Rxapp == 5) {
    count++;
    Rxapp = 0;
    
  }
  //Serial.println(count);
  if(count%2==0){
  control();
  
  }
  if(count%2==1){
  ReadSensor();
  if(ldr<100){
    Buzzer();
  }
  }
  

}
