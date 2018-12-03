/*
* Sender.
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

unsigned int counter = 0;

SSD1306Wire display(0x3c, SCL, SCA);

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

  // OLED init
  display.init();
  display.flipScreenVertically();

  Serial.println("Setup done");
}

void display_header() {
  display.clear();
  display.setFont(ArialMT_Plain_16);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString (64, 0, "Power Lab...");
}

void display_packets(int n) {
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString (0, 20, "Packet # " + String(n));
}

void loop() {
  Serial.println("Sending packet");
  display_header();

  // send packet
  LoRa.beginPacket();
  LoRa.print("hello ");
  LoRa.print(counter);
  LoRa.endPacket();

  display_packets(counter++);
  display.display();
  // Wait 1 sec before sending again
  delay(1000);
}
