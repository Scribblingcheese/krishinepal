#include <SoftwareSerial.h>
#include <Wire.h>
#include <ArduinoJson.h>
#include "DHT.h"
// RE and DE Pins set the RS485 module
// to Receiver or Transmitter mode
#define RE 8
#define DE 7
#define PHPIN A0
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
// Modbus RTU requests for reading NPK values
const byte nitro[] = {0x01,0x03, 0x00, 0x1e, 0x00, 0x01, 0xe4, 0x0c};
const byte phos[] = {0x01,0x03, 0x00, 0x1f, 0x00, 0x01, 0xb5, 0xcc};
const byte pota[] = {0x01,0x03, 0x00, 0x20, 0x00, 0x01, 0x85, 0xc0};
// A variable used to store NPK values
byte values[11];
SoftwareSerial mod(2, 3);
SoftwareSerial nodemcu(5,6);
void setup() {
pinMode(PHPIN , INPUT);
pinMode(DHTPIN, INPUT);
// Set the baud rate for the Serial port
Serial.begin(9600);
nodemcu.begin(9600);
mod.begin(9600);
dht.begin();
// Define pin modes for RE and DE
pinMode(RE, OUTPUT);
pinMode(DE, OUTPUT);
delay(500);
}
void loop() {
//StaticJsonBuffer<1000> jsonBuffer;
StaticJsonDocument<1000> doc;
JsonObject data = doc.to<JsonObject>();


// Read values
byte val1,val2,val3;
val1 = nitrogen();
delay(500);
val2 = phosphorous();
delay(500);
val3 = potassium();
delay(500);
float value = analogRead(A0); 
float v = (float) value / 1024;
v = v * 5;
v = v + 3.5;
float h = dht.readHumidity();
float t = dht.readTemperature();
data["PH"] = v;
data["temp"] = t;
data["h"] = h;
data["N"] = val1;
data["P"] = val2;
data["K"] = val3;
serializeJson(doc, nodemcu);
//data.printTo(nodemcu);
Serial.print("PH Value is:");
Serial.print(v);
Serial.println("");
Serial.print("Temperature:");
Serial.print(t);
Serial.println("");
Serial.print("Humidity:");
Serial.print(h);
Serial.println("");
Serial.println("");
Serial.println(" ");
// Print values to the serial monitor
Serial.print("Nitrogen: ");
Serial.print(val1);
Serial.println(" mg/kg");
Serial.print("Phosphorous: ");
Serial.print(val2);
Serial.println(" mg/kg");
Serial.print("Potassium: ");
Serial.print(val3);
Serial.println(" mg/kg");
delay(5000);
}
byte nitrogen(){
digitalWrite(DE,HIGH);
digitalWrite(RE,HIGH);
delay(100);
if(mod.write(nitro,sizeof(nitro))==8){
digitalWrite(DE,LOW);
digitalWrite(RE,LOW);
for(byte i=0;i<7;i++){
//Serial.print(mod.read(),HEX);
values[i] = mod.read();
//Serial.print(values[i],HEX);
}
Serial.println();
}
return values[4];
}
byte phosphorous(){
digitalWrite(DE,HIGH);
digitalWrite(RE,HIGH);
delay(100);
if(mod.write(phos,sizeof(phos))==8){
digitalWrite(DE,LOW);
digitalWrite(RE,LOW);
for(byte i=0;i<7;i++){
//Serial.print(mod.read(),HEX);
values[i] = mod.read();
//Serial.print(values[i],HEX);
}
Serial.println();
}
return values[4];
}
byte potassium(){
digitalWrite(DE,HIGH);
digitalWrite(RE,HIGH);
delay(100);
if(mod.write(pota,sizeof(pota))==8){
digitalWrite(DE,LOW);
digitalWrite(RE,LOW);
for(byte i=0;i<7;i++){
//Serial.print(mod.read(),HEX);
values[i] = mod.read();
//Serial.print(values[i],HEX);
}
//Serial.println();
}
return values[4];
}