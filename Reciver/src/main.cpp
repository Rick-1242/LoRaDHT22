/*********
  Modified from the examples of the Arduino LoRa library
  More resources: https://randomnerdtutorials.com
*********/
// c
#include <SPI.h>
#include <LoRa.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// define the pins used by the transceiver module
#define ss 5
#define rst 14
#define dio0 2
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup()
{
  // initialize Serial Monitor
  Serial.begin(9600);
  while (!Serial)
    ;
  Serial.println("LoRa Receiver");
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  { // Address 0x3D for 128x64
    Serial.println("SSD1306 allocation failed");
  }
  // display.display();
  // delay(2000);
  // display.clearDisplay();

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
  // try to parse packet
  String LoRaData;
  int packetSize = LoRa.parsePacket();
  if (packetSize)
  {
    // received a packet
    Serial.print("Received packet '");

    // read packet
    while (LoRa.available())
    {
      display.clearDisplay();
      LoRaData = LoRa.readString();
      Serial.print(LoRaData);
      display.setTextSize(1);
      display.setTextColor(BLACK, WHITE); // Draw 'inverse' text
      display.setCursor(0, 10);
      // Display text
      display.print(LoRaData);
      display.print("\nwith RSSI: ");
      display.print(LoRa.packetRssi());
      display.display();
    }
  }
}
