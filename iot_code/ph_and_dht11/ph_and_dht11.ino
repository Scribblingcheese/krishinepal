#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include "DHT.h"

#define PHPIN A0
#define DHTPIN 4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

SoftwareSerial nodemcu(5,6);



void setup(){

pinMode(PHPIN , INPUT);
pinMode(DHTPIN, INPUT);

 Serial.begin(9600);
 nodemcu.begin(9600);
 dht.begin(9600);



 Serial.println("Program is started");


 Serial.println("Let's go");
}

void loop(){

  StaticJsonBuffer<1000> jsonBuffer;
JsonObject& data = jsonBuffer.createObject();
  
float value = analogRead(A0);  
float v = (float) value / 1024;
v = v * 5;
v = v + 3.5;

float h = dht.readHumidity();

float t = dht.readTemperature();

data["PH"] = v;
data["temp"] = t;
data["h"] = h;

data.printTo(nodemcu);



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

delay(2000);

}