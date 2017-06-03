#include <SD.h>
#include <SPI.h>

int millisecondsDelay = 100;
long int interval = 0;


void setup() {
  Serial.begin(9600);

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


