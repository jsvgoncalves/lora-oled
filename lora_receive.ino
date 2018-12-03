/*
* Receiver.
*/
#include <config.h>
#include "WiFi.h"
#include <LoRa.h>
#include "SSD1306.h"

#define SCA 22
#define SCL 21

#define SCK     5    // GPIO5  -- SX1278's SCK
#define MISO    19   // GPIO19 -- SX1278's MISO
#define MOSI    27   // GPIO27 -- SX1278's MOSI
#define SS      18   // GPIO18 -- SX1278's CS
#define RST     12   // GPIO12 -- SX1278's RESET
#define DI0     26   // GPIO26 -- SX1278's IRQ(Interrupt Request)
#define BAND    868E6

SSD1306Wire display(0x3c, SCL, SCA);
String rssi = "RSSI --";
String packSize = "--";
String packet ;

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println();
  Serial.println("LoRa Sender Test");

  SPI.begin(SCK,MISO,MOSI,SS);
  LoRa.setPins(SS,RST,DI0);
  if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  LoRa.receive();

  // OLED init
  display.init();
  display.flipScreenVertically();

  Serial.println("Setup done");
}

void cbk(int packetSize) {
  packet ="";
  packSize = String(packetSize,DEC);
  for (int i = 0; i < packetSize; i++) { packet += (char) LoRa.read(); }
  rssi = "RSSI " + String(LoRa.packetRssi(), DEC) ;
}

void display_header() {
  display.clear();
  display.setFont(ArialMT_Plain_16);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString (64, 0, "Power Lab...");
}

void display_data() {
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString (0, 20, packet);
  display.drawString (0, 40, rssi);
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) { cbk(packetSize);  }
  display_header();
  display_data();
  display.display();
  delay(10);
}
