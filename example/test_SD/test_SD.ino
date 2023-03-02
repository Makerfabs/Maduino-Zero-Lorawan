#include <SPI.h>
#include <SD.h>

#define SD_CS 4

File myFile;

void setup()
{

  SerialUSB.begin(9600);

  delay(5000);

  if (!SD.begin(SD_CS))
  {
    SerialUSB.println("initialization failed!");
    while (1)
      ;
  }

  if (SD.exists("test.txt"))
  {
    SerialUSB.println("test.txt exists.");
  }
  else
  {
    SerialUSB.println("test.txt doesn't exist.");
  }

  // open a new file and immediately close it:
  SerialUSB.println("Creating example.txt...");

  myFile = SD.open("test.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile)
  {
    SerialUSB.print("Writing to test.txt...");
    myFile.println("testing 1, 2, 3.");
    // close the file:
    myFile.close();
    SerialUSB.println("done.");
  }
  else
  {
    // if the file didn't open, print an error:
    SerialUSB.println("error opening test.txt");
  }

  // re-open the file for reading:
  myFile = SD.open("test.txt");
  if (myFile)
  {
    SerialUSB.println("test.txt:");

    // read from the file until there's nothing else in it:
    while (myFile.available())
    {
      SerialUSB.write(myFile.read());
    }
    // close the file:
    myFile.close();
  }
  else
  {
    // if the file didn't open, print an error:
    SerialUSB.println("error opening test.txt");
  }
}

void loop()
{
  SerialUSB.println("loop");
  delay(3000);
}
