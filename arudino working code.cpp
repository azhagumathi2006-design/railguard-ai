#include <SoftwareSerial.h>
#include <TinyGPS++.h>

#define TRIG 9
#define ECHO 10
#define TEMP A0

SoftwareSerial gsm(7, 8); // GSM RX, TX
SoftwareSerial gpsSerial(4, 5);
TinyGPSPlus gps;

long duration;
float distance;
int tempValue;

void setup()
{
    Serial.begin(9600);
    gsm.begin(9600);
    gpsSerial.begin(9600);

    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);

    Serial.println("Rail Track Monitoring System Started");
}

void loop()
{

    // ---- Ultrasonic Sensor ----
    digitalWrite(TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);

    duration = pulseIn(ECHO, HIGH);
    distance = duration * 0.034 / 2;

    // ---- Temperature Sensor ----
    tempValue = analogRead(TEMP);
    float temperature = tempValue * (5.0 / 1023.0) * 100;

    // ---- GPS Reading ----
    while (gpsSerial.available())
    {
        gps.encode(gpsSerial.read());
    }

    Serial.print("Distance: ");
    Serial.println(distance);

    Serial.print("Temperature: ");
    Serial.println(temperature);

    // ---- Fault Condition ----
    if (distance < 5 || temperature > 60)
    {
        sendSMS("ALERT! Track Fault Detected");
    }

    delay(5000);
}

// ---- GSM SMS Function ----
void sendSMS(String message)
{
    gsm.println("AT+CMGF=1");
    delay(1000);
    gsm.println("AT+CMGS=\"+91XXXXXXXXXX\"");
    delay(1000);
    gsm.println(message);
    delay(100);
    gsm.write(26);
    delay(3000);
}