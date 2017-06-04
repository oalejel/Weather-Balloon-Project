#include <SD.h>
#include <SPI.h>
#include <DHT.h>;

int millisecondsDelay = 100;
long int interval = 0;

//white sensor setup
#define DHTPIN 7     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino
int chk;
float hum;  //Stores humidity value
float temp; //Stores temperature value

void setup() {
  Serial.begin(9600);

  //white sensors
  dht.begin();

  //setup the SD card
  Serial.println("Initializing card");
  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return;
  }


  //make sure we identify when we start recording again
  File records = SD.open("data.csv", FILE_WRITE);
  Serial.println(records.isDirectory());


  records.write("\"Elapsed time (seconds)\",");
  records.write("\"Altitude (Meters)\",");
  records.write("\"Temperature (Celsius)\"\n");

  records.close();
}

void loop() {
  //white sensor data
  humidityPercent = dht.readHumidity();
  celciusTemp = dht.readTemperature();

  File records = SD.open("data.csv", FILE_WRITE);
  //get data
  int input = analogRead(A0);
  //print data for debugging in serial
  Serial.println(input);
  //transform integer to char array
  String inString = String(input);
  char buf[3];
  inString.toCharArray(buf, 4);

  char buf2[100];
  String(interval).toCharArray(buf2, 100);
  records.write(buf2);

  //add a comma and new line
  records.write(",");

  //write the char array to the file
  records.write(buf);

  //add a comma and new line
  records.write(",");
  records.write(buf);


  records.write("\n");
  //close file (you have to)
  records.close();

  delay(millisecondsDelay);
  interval++;
}


