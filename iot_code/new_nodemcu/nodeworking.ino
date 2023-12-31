#include <GDBStub.h>

#include <SoftwareSerial.h>
#include <ArduinoJson.h>


#include <ESP8266WiFi.h>  
#include <FirebaseESP8266.h>

#define WIFI_SSID "#####"
#define WIFI_PASSWORD "@@@@"

// Insert Firebase project API Key

// Insert RTDB URLefine the RTDB URL */
#define HOST "krishi-6c36b-default-rtdb.firebaseio.com" 

#define AUTH "fMP8GeZHDsggl4CeN6ZHbhKcKeltRSImMQJmpWT5" // secret key


FirebaseData fbdo;

SoftwareSerial nodemcu(D6,D5);

StaticJsonBuffer<1000> jsonBuffer;
JsonObject& data = jsonBuffer.parseObject(nodemcu);


void setup(){
Serial.begin(9600);
nodemcu.begin(9600);

WiFi.begin(WIFI_SSID, WIFI_PASSWORD); 

  while (WiFi.status() != WL_CONNECTED){
    delay(1000);

    Serial.println("Connecteing ...");
  }

  Serial.println("Wifi is connected good to go") ;
  
  Firebase.begin(HOST , AUTH);

  Serial.println("Firebase connection established");

while (!Serial) continue;

}

void loop(){

  Serial.print("Entered loop");

StaticJsonBuffer<1000> jsonBuffer;
JsonObject& data = jsonBuffer.parseObject(nodemcu);

if (data == JsonObject::invalid()) {
    //Serial.println("Invalid Json Object");
    jsonBuffer.clear();
    return;
  }


  float ph = data["PH"];
  Serial.print("PH value is:");
  Serial.println(ph);


  float t = data["temp"];
  Serial.print("Temperature is:");
  Serial.println(t);

  
 
  float h = data["h"];
  Serial.print("Humidity iis:");
  Serial.println(h);

  Firebase.setFloat(fbdo,"data/temperature",t);
  Firebase.setFloat(fbdo ,"data/humidity",h);
  Firebase.setFloat(fbdo,"data/ph",ph);

  Serial.println("Data is written into firebase");

}