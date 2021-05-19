#include <stdio.h>
#include <string.h>

#define DEBUG true

//get appeui and appkey from the thing network
#define DEVEUI "D896E0FF00000241"
#define APPEUI "70B3D57ED0041DA0"
#define APPKEY "850D282C603C163137BB71E9521D12D0"

int PWR_KEY = 9;
int RST_KEY = 6;
int LOW_PWR_KEY = 5;

bool ModuleState = false;

void setup()
{

    Serial1.begin(115200);
    SerialUSB.begin(115200);

    SerialUSB.println("Now turnning the AT module on.Please wait.");

    pin_init();
    SerialUSB.println("Init over");
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
