// Serial port transmission

#define PWR_KEY 9
#define RST_KEY 6
#define LOW_PWR_KEY 5

void setup()
{

    Serial1.begin(9600);
    SerialUSB.begin(9600);

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
    delay(5000);
}