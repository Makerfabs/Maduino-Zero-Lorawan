#include <stdio.h>
#include <string.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define DEBUG true

// get appeui and appkey from the thing network
#define DEVEUI "UES YOUR DEVEUI"
#define APPEUI "UES YOUR APPEUI"
#define APPKEY "UES YOUR APPKEY"

#define PWR_KEY 9
#define RST_KEY 6
#define LOW_PWR_KEY 5

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET -1    // Reset pin # (or -1 if sharing Arduino reset pin)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
int led_flag = 0;

void setup()
{
    Serial1.begin(9600);
    SerialUSB.begin(9600);
    Wire.begin();
    oled_init();

    SerialUSB.println("Now turnning the Lorawan on.Please wait.");

    pin_init();
    SerialUSB.println("Init over");

    sendData("AT+CDEVEUI=" + String(DEVEUI), 3000, DEBUG);
    sendData("AT+CAPPEUI=" + String(APPEUI), 3000, DEBUG);
    sendData("AT+CAPPKEY=" + String(APPKEY), 3000, DEBUG);
    sendData("AT+CJOINMODE=0", 3000, DEBUG);
    sendData("AT+CCLASS=0", 30000, DEBUG);
    sendData("AT+CJOIN=1,0,8,8", 30000, DEBUG);
}

long int runtime = 20000;
void loop()
{
    if ((millis() - runtime) > 10000)
    {

        String downlink = "";
        if (led_flag == 1)
            downlink = lorawan_tx_rx("F8", 20000);
        if (led_flag == 0)
            downlink = lorawan_tx_rx("F7", 20000);

        if (downlink.endsWith("F8"))
            led_flag = 1;
        if (downlink.endsWith("F7"))
            led_flag = 0;

        display.clearDisplay();
        display.setCursor(0, 4);
        display.println(downlink);
        if (led_flag == 1)
        {
            display.setCursor(0, 20);
            display.println("LED ON");
        }
        else
        {
            display.setCursor(0, 20);
            display.println("LED OFF");
        }

        display.display();

        runtime = millis();
    }
    while (Serial1.available() > 0)
    {
        SerialUSB.write(Serial1.read());
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

void oled_init()
{
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    { // Address 0x3C for 128x32
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ; // Don't proceed, loop forever
    }
    Serial.println("SSD1306 found");

    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1); // Draw 2X-scale text
    display.setCursor(0, 4);
    display.println("Makerfabs Lorawan");
    display.setCursor(0, 20);
    display.println("Dowlink demo");
    display.display();
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

String lorawan_tx_rx(String msg_hex, const int timeout)
{
    String response = "";
    String rec_head = "OK+RECV:";

    char msg[50] = "";
    sprintf(msg, "AT+DTRX=1,2,5,%s", msg_hex.c_str());

    SerialUSB.println(msg);
    Serial1.println(msg);

    long int time = millis();
    while ((time + timeout) > millis())
    {
        while (Serial1.available())
        {
            char c = Serial1.read();

            if (c == '\r')
                continue;
            else if (c == '\n')
            {
                SerialUSB.println(response);
                
                if (response.indexOf(rec_head) != -1)
                {
                    SerialUSB.println("-----------Get downlink msg-----------");

                    String result = response.substring(response.indexOf(rec_head) + rec_head.length());

                    SerialUSB.println(result);
                    SerialUSB.println("-----------Over-----------");
                    return result;
                }
                else if (response.indexOf("ERR+SENT") != -1)
                {
                    SerialUSB.println("-----------Get downlink msg-----------");

                    String result = "SEND FAILED";

                    SerialUSB.println(result);
                    SerialUSB.println("-----------Over-----------");
                    return result;
                }

                response = "";
            }
            else
                response += c;
        }
    }
    SerialUSB.println(response);

    return "";
}
