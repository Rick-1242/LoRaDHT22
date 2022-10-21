/********
  Modified from the examples of the Arduino LoRa library
  More resources: https://randomnerdtutorials.com
*********/

#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include <DHT.h>

// define the pins used by the transceiver module
#define ss 5
#define rst 14
#define dio0 2
#define TEMPERATURE 15 // Temp and humidity sensor
#define DHTTYPE DHT22

DHT dht(TEMPERATURE, DHTTYPE);

int counter = 0;

void setup()
{
  // initialize Serial Monitor
  Serial.begin(9600);
  while (!Serial)
    ;
  Serial.println("LoRa Sender");

  dht.begin();

  // setup LoRa transceiver module
  LoRa.setPins(ss, rst, dio0);

  // replace the LoRa.begin(---E-) argument with your location's frequency
  // 433E6 for Asia
  // 866E6 for Europe
  // 915E6 for North America
  while (!LoRa.begin(866E6))
  {
    Serial.println(".");
    delay(500);
  }
  // Change sync word (0xF3) to match the receiver
  // The sync word assures you don't get LoRa messages from other LoRa transceivers
  // ranges from 0-0xFF
  LoRa.setSyncWord(0xF2);
  Serial.println("LoRa Initializing OK!");
}

void loop()
{
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t))
  {
    Serial.println("Failed to read from DHT sensor!");
  }
  float hic = dht.computeHeatIndex(t, h, false);
  Serial.print("Sending packet: ");
  Serial.println(counter);

  // Send LoRa packet to receiver
  LoRa.beginPacket();
  LoRa.print("\nTemperature: ");
  LoRa.print(t);
  LoRa.print("\nHumidity: ");
  LoRa.print(h);
  LoRa.print("\nPacket: ");
  LoRa.print(counter);
  LoRa.endPacket();

  counter++;

  delay(2000);
}