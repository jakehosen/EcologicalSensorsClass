/*
 * Project Argon_Tutorial
 * Description:
 * Author:
 * Date:
 */


#include "Air_Quality_Sensor.h"
#include "Adafruit_BME280.h"
#include "Adafruit_Sensor.h"
#include "SeeedOLED.h"
#include "JsonParserGeneratorRK.h"

#include "cayenne-particle/cayenne-particle.h"

#define CAYENNE_PRINT serial
#define DUST_SENSOR_PIN D4
#define SENSOR_READING_INTERVAL 900000
#define AQS_PIN A2

AirQualitySensor aqSensor(AQS_PIN);
Adafruit_BME280 bme;


unsigned long lastInterval;
unsigned long lowpulseoccupancy = 0;
unsigned long last_lpo = 0;
unsigned long duration;

float ratio = 0;
float concentration = 0;

const char * eventName =            "env-vals";    
const char * myWriteAPIKey = "ELEH3K8AQ2RNZ9AK";




float field1;  // initialized as 1.1 for demo only
float field2;  // initialized as 2.2 for demo only
float field3;
float field4;
float field5;
float field6;
float field7;
float field8;
char msg[256];       // Character array for the snprintf Publish Payload





// setup() runs once, when the device is first turned on.
void setup() {
   Serial.begin(9600);
   delay(5000);
   pinMode(BATT, INPUT);
   //Initialize OLED Screen
   Wire.begin();
   SeeedOled.init();

   SeeedOled.clearDisplay();
   SeeedOled.setNormalDisplay();
   SeeedOled.setPageMode();

   //OLED Startup Message
   SeeedOled.setTextXY(2, 0);
   SeeedOled.putString("Ready");
   SeeedOled.setTextXY(3, 0);
   SeeedOled.putString("to");
   SeeedOled.setTextXY(4, 0);
   SeeedOled.putString("Rock and Roll");

   pinMode(DUST_SENSOR_PIN, INPUT);
   lastInterval = millis();


   //checking to see if the air quality sensor is ready.
   if (aqSensor.init())
   {
      Serial.println("Air Quality Sensor ready.");
    }
   else
   {
      Serial.println("Air Quality Sensor ERROR!");
   }


   //checking to see if the BME sensor is ready.
   if (bme.begin())
   {
      Serial.println("BME280 Sensor ready.");
   }
   else
   {
      Serial.println("BME280 Sensor ERROR!");
   }



}






// loop() runs over and over again, as quickly as it can execute.
void loop() {
int temp, pressure, humidity;   
duration = pulseIn(DUST_SENSOR_PIN, LOW);
lowpulseoccupancy = lowpulseoccupancy + duration;
ratio = lowpulseoccupancy / (SENSOR_READING_INTERVAL * 10.0);
if ((millis() - lastInterval) > SENSOR_READING_INTERVAL)
{
   
   lastInterval = millis();

   float voltage = analogRead(BATT) * 0.0011224;
   //This is the air quality code
  
    temp = (float)bme.readTemperature();
    pressure = (float)(bme.readPressure() / 100.0F);
    humidity = (float)bme.readHumidity();
   //getBMEValues(temp, pressure, humidity);
   Serial.printlnf("Temp: %d", temp);
   Serial.printlnf("Pressure: %d", pressure);
   Serial.printlnf("Humidity: %d", humidity);
   updateDisplay(temp, humidity, pressure, ratio);
   //createEventPayload(temp, humidity, pressure, quality);
    field1 = temp;
    field2 = pressure;
    field3 = humidity;
    field4 = voltage;
    field5 = aqSensor.slope();
    field6 = aqSensor.getValue();
      if (lowpulseoccupancy > 0)
  {
    field7 = lowpulseoccupancy;
    field8 = ratio;
  }
    lowpulseoccupancy = 0;
    snprintf(msg, sizeof(msg),"{\"1\":\"%.2f\", \"2\":\"%.1f\", \"3\":\"%.2f\", \"4\":\"%.2f\", \"5\":\"%.3f\", \"6\":\"%.2f\", \"7\":\"%.1f\", \"8\":\"%.4f\", \"k\":\"%s\"}", field1, field2, field3, field4, field5, field6, field7, field8, myWriteAPIKey);
    Particle.publish(eventName, msg, PRIVATE, NO_ACK);

}

}







//function to run OLED display.
void updateDisplay(float temp, float humidity, float pressure, long ratio)
{
 SeeedOled.clearDisplay();

 SeeedOled.setTextXY(0, 3);
 SeeedOled.putNumber(ratio);

 SeeedOled.setTextXY(2, 0);
 SeeedOled.putString("Temp: ");
 SeeedOled.putNumber(temp);
 SeeedOled.putString("C");

 SeeedOled.setTextXY(3, 0);
 SeeedOled.putString("Humidity: ");
 SeeedOled.putNumber(humidity);
 SeeedOled.putString("%");

 SeeedOled.setTextXY(4, 0);
 SeeedOled.putString("Press: ");
 SeeedOled.putNumber(pressure);
 SeeedOled.putString(" hPa");

 if (concentration > 1)
 {
   SeeedOled.setTextXY(5, 0);
   SeeedOled.putString("Dust: ");
   SeeedOled.putNumber(concentration); // Cast our float to an int to make it more compact
   SeeedOled.putString(" pcs/L");
 }
}

