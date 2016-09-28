/* 1 for enable. If enabled then start "serial monitor" or
 * else the code will be stuck at while loop in setup()
 */
#define DEBUG      1
/* Number of iterations performed to average out a value.
 * The average value is then sent to cloud. In this case it will
 * be sent to ThingSpeak.
 */
#define ITERATION  12 // Iterations of temperature sensor data
/* Enable ThingSpeak IoT platform */
#define THING       1

#include <Bridge.h>
#include <Console.h>

#if THING
#include "ThingSpeak.h"
#include "YunClient.h"
YunClient client;
/* Get the channel number and write API key from your ThingSpeak channel */
unsigned long myChannelNumber = <replace by channel number>;
const char * myWriteAPIKey = <replace by write API key>;
#endif

int sensorPin = 0;
int ledPin = 13;

void setup() {

  Bridge.begin();
  pinMode (ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
#if THING
  ThingSpeak.begin(client);
#endif
#if DEBUG
  while (!Console);
#endif
}

void loop()
{
    /* Indicate through LED that we have passed the setup() */
    digitalWrite(ledPin, HIGH);

    /* get the average temperature value */
    float sensorValue = getSensorValue_TMP36();
#if THING
    /* 1 indicates that its data stream 1 in the channel
	 * Each ThingSpeak channel supports 8 data streams.
	 */
    ThingSpeak.writeField(myChannelNumber, 1, sensorValue, myWriteAPIKey);
#endif
}

/* Sensor used is TMP36 from Analog Devices */
float getSensorValue_TMP36() {
  int reading;
  float voltage;
  float temperatureC;
  float temperatureF;
  float average_tempF = 0;
  int i;
    
  for (i = 0; i < ITERATION; i++) {
    // ADC calculations
    reading = analogRead(sensorPin);
    voltage = reading * 5.0;
    voltage /= 1024.0;     
    temperatureC = (voltage - 0.5) * 100 ; 
    temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;
    average_tempF += temperatureF;
#ifdef DEBUG
    Console.print(temperatureF);
    Console.print("\n");
#endif
    delay(5000); //wait 5 seconds between consecutive sensor read
  }
  average_tempF = average_tempF / ITERATION;
#ifdef DEBUG
  Console.print("Average \t");
  Console.print(average_tempF);
  Console.print("\n");
#endif
  return average_tempF;
}
