#include "DHT.h"
#define MASTER_EN 11                 // connected to RS485 Enable pin
#define DHT_PIN 10
#define DHTTYPE DHT11
#define FIRE_SENSOR_PIN 8
#define PULSE_SENSOR_PIN 6
DHT dht(DHT_PIN, DHTTYPE);
float lastSendMillis = 0;
float sendIntervalMillis = 3000;
bool onFire = false;
bool hasPulse = false;
int temperature = 0;
int humidity = 0;

String getBooleanString(bool value)
{
  if(value)
  {
    return "true";
  }
  return "false";
}

void setup() {
  pinMode(MASTER_EN , OUTPUT);        // Declare Enable pin as output
  Serial.begin(9600);                 // set serial communication baudrate 
  digitalWrite(MASTER_EN , LOW);      // Make Enable pin low, Receiving mode ON
  pinMode(FIRE_SENSOR_PIN,INPUT);
  pinMode(PULSE_SENSOR_PIN,INPUT);
  dht.begin();
}
 
void loop() {
    temperature = dht.readTemperature();
    humidity = dht.readHumidity();
    onFire = digitalRead(FIRE_SENSOR_PIN) == 0;
    hasPulse = digitalRead(PULSE_SENSOR_PIN) == 0;

    delay(5);
    sendInput();
}

void sendInput()
{
    if(lastSendMillis != 0 && lastSendMillis + sendIntervalMillis > millis())
    {
        return;
    }
    String tempratureString = "Temperature =" + String(temperature) + "\n";
    String humidityString = "Humidity =" + String(humidity)+ "\n";
    String hasPulseString ="HasPulse =" + getBooleanString(hasPulse)+ "\n";
    String onFireString = "OnFire =" + getBooleanString(onFire);
    String inputString = tempratureString + humidityString + hasPulseString +  onFireString;
    digitalWrite(MASTER_EN , HIGH);     // Make Enable pin high to send Data
    delay(5);                           // required minimum delay of 5ms
    Serial.print(inputString + "*\n");     // Send String serially, End String with *
    Serial.flush();                     // wait for transmission of data
    digitalWrite(MASTER_EN , LOW);      // Receiving mode ON
    lastSendMillis = millis();
}
