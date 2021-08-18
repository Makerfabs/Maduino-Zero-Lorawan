#include <stdio.h>
#include <string.h>
#define DEBUG true

//include the SD library:
#include <SPI.h>
#include <SD.h>

//www.makerfabs.com
//Test SD card
//Send AT Command via serial tool
//version:v1.1
//author:Vincent
//date:2021/05/22


int PWR_KEY = 9;
int RST_KEY = 6;
int LOW_PWR_KEY = 5;

// change this to match your SD shield or module;
// Maduino zero sim808 GPS tracker: pin 4
const int Chip_Select_SD = 4;

File myFile;

void setup()
{

    SerialUSB.begin(115200);
    //while (!SerialUSB)
    {
        ; // wait for Arduino serial Monitor port to connect
    }
    Serial1.begin(115200);
    SerialUSB.println("Maduino Zero Lorawan Node Test Start!");
    SerialUSB.println("Wait restart Lorawan");

    pin_init();

    SerialUSB.println("Start TX");


    Serial.print("Initializing SD card...");

    if (!SD.begin(Chip_Select_SD)) {
        Serial.println("initialization failed!");
        while (1);
    }
    Serial.println("initialization done.");

    if (SD.exists("test.txt")) {
      Serial.println("test.txt exists.");
    } else {
      Serial.println("test.txt doesn't exist.");
    }
  
    // open a new file and immediately close it:
    Serial.println("Creating example.txt...");

    myFile = SD.open("test.txt", FILE_WRITE);

    // if the file opened okay, write to it:
    if (myFile) {
      Serial.print("Writing to test.txt...");
      myFile.println("testing 1, 2, 3.");
      // close the file:
      myFile.close();
      Serial.println("done.");
    } else {
      // if the file didn't open, print an error:
      Serial.println("error opening test.txt");
    }
  
    // re-open the file for reading:
    myFile = SD.open("test.txt");
    if (myFile) {
      Serial.println("test.txt:");
  
      // read from the file until there's nothing else in it:
      while (myFile.available()) {
        Serial.write(myFile.read());
      }
      // close the file:
      myFile.close();
    } else {
      // if the file didn't open, print an error:
      Serial.println("error opening test.txt");
    }

    //set DEVEUI
    sendData("AT", 3000, DEBUG);
    sendData("AT+CTX=433000000,0,22,5", 3000, DEBUG);
}

void loop()
{
    while (Serial1.available() > 0)
    {
        SerialUSB.write(Serial1.read());
        yield();
    }
    while (SerialUSB.available() > 0)
    {
        Serial1.write(SerialUSB.read());
        yield();
    }
}

String sendData(String command, const int timeout, boolean debug)
{
    String response = "";
    Serial1.println(command);
    long int time = millis();
    while ((time + timeout) > millis())
    {
        while (Serial1.available())
        {
            char c = Serial1.read();
            response += c;
        }
    }
    if (debug)
    {
        SerialUSB.print(response);
    }
    return response;
}
void pin_init()
{
    pinMode(PWR_KEY, OUTPUT);
    pinMode(RST_KEY, OUTPUT);
    pinMode(LOW_PWR_KEY, OUTPUT);

    digitalWrite(RST_KEY, LOW);
    digitalWrite(LOW_PWR_KEY, HIGH);
    digitalWrite(PWR_KEY, HIGH);

    digitalWrite(PWR_KEY, LOW);
    delay(3000);
    digitalWrite(PWR_KEY, HIGH);
    delay(10000);
}
