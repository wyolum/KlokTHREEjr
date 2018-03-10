#include <Wire.h>

#define ENTER 19
#define INC 18
#define DECR 5
#define MODE 17
void setup() {
  
  Serial.begin(115200);
  delay(200);
  Serial.println("hey there");
  //while(1){
    Serial.println("hey there");
    delay(1000);
    //}
    Wire.begin(4, 5);

  // clear /EOSC bit
  // Sometimes necessary to ensure that the clock
  // keeps running on just battery power. Once set,
  // it shouldn't need to be reset but it's a good
  // idea to make sure.
  Wire.beginTransmission(0xD0); // address DS3231
  Wire.write(0x0E);             // select register
  Wire.write(0b00011100);       // write register bitmap, bit 7 is /EOSC
  Wire.endTransmission();
  
}


void loop() {
   // send request to receive data starting at register 0
  Wire.beginTransmission(0xD0); // 0x68 is DS3231 device address
  Wire.write((byte)0);          // start at register 0
  Wire.endTransmission();
  Wire.requestFrom(0xD0, 3); // request three bytes (seconds, minutes, hours)
 
  while(Wire.available())
  { 
    int seconds = Wire.read(); // get seconds
    int minutes = Wire.read(); // get minutes
    int hours = Wire.read();   // get hours
 
    seconds = (((seconds & 0b11110000)>>4)*10 + (seconds & 0b00001111)); 
    minutes = (((minutes & 0b11110000)>>4)*10 + (minutes & 0b00001111)); 
    hours = (((hours & 0b00100000)>>5)*20 + ((hours & 0b00010000)>>4)*10 + (hours & 0b00001111)); 
 
    Serial.print(hours);
    Serial.print(":");
    Serial.print(minutes);
    Serial.print(":");
  }
  delay(1000);
  if (Serial.available()){
    Serial.print("received: ");
    Serial.println(Serial.read());
  }

  if (digitalRead(ENTER))
    Serial.println("ENTER");
  if (digitalRead(INC))
    Serial.println("INC");
  if (digitalRead(DECR))
    Serial.println("DECR");
  if (digitalRead(MODE))
    Serial.println("MODE");
}
