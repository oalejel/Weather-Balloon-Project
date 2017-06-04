#include <SD.h>
#include <SPI.h>
//white sensor (temp and humidity) library
#include <DHT.h>;
//purple sensor (precise temperature, pressure, and altitude) library 
#include "Seeed_BME280.h"
#include <Wire.h>

//! tweak this in case you dont want too much data or a big file size
int millisecondsDelay = 1000;
long int interval = 0;

//white sensor variables
#define DHTPIN 7     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino
int chk;
float hum;  //Stores humidity value
float temp; //Stores temperature value

//purple sensor variables
BME280 bme280;

void setup() {
  Serial.begin(9600);

  //white sensor begin
  dht.begin();

  //purple sensor initialization
  if(!bme280.init()){
    Serial.println("Device error!");
  }

  //setup the SD card
  Serial.println("Initializing card");
  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return;
  }

  //make sure we identify when we start recording again
  File records = SD.open("data.csv", FILE_WRITE);
  Serial.println(records.isDirectory());

  //ensure that this matches the order of data logged
  records.write("\"Elapsed time (seconds)\",");
  records.write("\"Altitude (Meters)\",");
  records.write("\"Temperature (Celsius)\"\n");

  //must close file to save things written to it properly
  records.close();
}

void loop() {
  //white sensor data
  float whiteHumidityPercent = dht.readHumidity();
  //this temp variable is btter than that of the other sensor,
  //since the DHT sensor measures degrees C below 0
  float whiteCelciusTemp = dht.readTemperature();

  float purpleHumidityPercent = bme280.getHumidity();
  float purplePascalPressure = bme280.getPressure();
  float purpleCelsiusTemp = bme280.getTemperature();
  //altitude calculation dependent on pressure
  float purpleAltitudeMeters = bme280.calcAltitude(purplePascalPressure);

  //open the data file every time we loop
  File records = SD.open("data.csv", FILE_WRITE);

  
  //transform float -> string -> char array
  String temp = String(whiteCelciusTemp);
  char tempBuffer[7];
  temp.toCharArray(tempBuffer, 7);

  String humidity = String(purpleHumidityPercent);
  char humidityBuffer[7];
  humidity.toCharArray(humidityBuffer, 7);
  
  //add a comma and new line
  records.write(",");
  //write the char array to the file
  records.write(tempBuffer);
  //add a comma and new line
  records.write(",");


  //new line for a new loop. DO NOT MODIFY BELOW!!!
  records.write("\n");
  //close file (you have to)
  records.close();

  delay(millisecondsDelay);
  interval++;
}


