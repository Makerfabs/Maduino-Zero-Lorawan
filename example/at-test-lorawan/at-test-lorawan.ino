#include <stdio.h>
#include <string.h>
#include "DHT.h"

#define DEBUG true

//get appeui and appkey from the thing network
#define DEVEUI "D896E0FF00000241"
#define APPEUI "70B3D57ED0041DA0"
#define APPKEY "DAC6118A3445572B02EFB73443AF2EB9"

#define DHTPIN 7      // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11 // DHT 11

DHT dht(DHTPIN, DHTTYPE);

int PWR_KEY = 9;
int RST_KEY = 6;
int LOW_PWR_KEY = 5;

bool ModuleState = false;
long int runtime = 20000;

void setup()
{

    Serial1.begin(9600);
    SerialUSB.begin(9600);

    dht.begin();

    SerialUSB.println("Now turnning the Lorawan on.Please wait.");

    pin_init();
    SerialUSB.println("Init over");

    //set DEVEUI
    //sendData("AT+CDEVEUI?", 3000, DEBUG);
    sendData("AT+CDEVEUI=" + String(DEVEUI), 3000, DEBUG);

    //set APPEUI and APPKEY
    sendData("AT+CAPPEUI=" + String(APPEUI), 3000, DEBUG);
    sendData("AT+CAPPKEY=" + String(APPKEY), 3000, DEBUG);

    //set join mod "OTAA"
    sendData("AT+CJOINMODE=0", 3000, DEBUG);

    //join lorawan
    sendData("AT+CCLASS=0", 30000, DEBUG);

    //join lorawan
    sendData("AT+CJOIN=1,0,10,1", 30000, DEBUG);

    //set one message to webgate
    //AT+DTRX=1,2,10,4D414B455246414253
    //sendData("AT+DTRX=1,2,5,FF 00 88", 3000, DEBUG);
}

void loop()
{
    if (runtime - millis() > 10000)
    {
        float h = dht.readHumidity();
        float t = dht.readTemperature();
        int hum = (int)(h);
        int temp = (int)(t * 10);
        char msg[30] = "";
        sprintf(msg, "AT+DTRX=1,2,5,%02x%04x", hum, temp);
        SerialUSB.println(msg);
        sendData((String)msg, 3000, DEBUG);

        runtime = millis();
    }
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
