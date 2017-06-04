#include <SD.h>
#include <SPI.h>
//white sensor (temp and humidity) library
#include <DHT.h>
//purple sensor (precise temperature, pressure, and altitude) library 
#include "Seeed_BME280.h"
#include <Wire.h>

 
 
//! tweak this in case you dont want too much data or a big file size
int millisecondsDelay = 3000;
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
  records.write("\"Temperature (celsius)\",");
  records.write("\"Humidity (%)\",");
  records.write("\"Pressure (pascals)\",");
  records.write("\"Altitude (meters)\"\n");

  //must close file to save things written to it properly
  records.close();
}

void loop() {
  //white sensor data
  float whiteHumidityPercent = dht.readHumidity();
  //this temp variable is btter than that of the other sensor,
  //since the DHT sensor measures degrees C below 0
  float whiteCelsiusTemp = dht.readTemperature();

  float purpleHumidityPercent = bme280.getHumidity();
  float purplePascalPressure = bme280.getPressure();
  float purpleCelsiusTemp = bme280.getTemperature();
  //altitude calculation dependent on pressure
  float purpleAltitudeMeters = bme280.calcAltitude(purplePascalPressure);

  //open the data file every time we loop
  File records = SD.open("data.csv", FILE_WRITE);

  //transform float -> string -> char array
  int s = millis() / 1000.0;
  String timeSeconds = String(s);
  char timeBuffer[100];
  timeSeconds.toCharArray(timeBuffer, 100);
  
  String temp = String(whiteCelsiusTemp);
  char tempBuffer[7];
  temp.toCharArray(tempBuffer, 7);

  String humidity = String(purpleHumidityPercent);
  char humidityBuffer[7];
  humidity.toCharArray(humidityBuffer, 7);

  String pressure = String(purplePascalPressure);
  char pressureBuffer[7];
  pressure.toCharArray(pressureBuffer, 7);

  String altitude = String(purpleAltitudeMeters);
  char altitudeBuffer[7];
  altitude.toCharArray(altitudeBuffer, 7);

  //debug: write to serial
  Serial.print("time: ");
  Serial.println(timeSeconds);
  Serial.print("temp: ");
  Serial.print(temp);
  Serial.println(" C");
  Serial.print("humidity: ");
  Serial.print(humidity);
  Serial.println("%");
  Serial.print("pressure: ");
  Serial.print(pressure);
  Serial.println(" Pa");
  Serial.print("altitude: ");
  Serial.print(altitude);
  Serial.println(" m");
   Serial.println(" \n\n");

  //for each data point in a row, write() the buffer, and have a comma in between
  records.write(timeBuffer);
  records.write(",");
  records.write(tempBuffer);
  records.write(",");
  records.write(humidityBuffer);
  records.write(",");
  records.write(pressureBuffer);
  records.write(",");
  records.write(altitudeBuffer);

  //new line for a new loop. DO NOT MODIFY BELOW!!!
  records.write("\n");
  //close file (you have to)
  records.close();

  delay(millisecondsDelay);
  interval++;
}


