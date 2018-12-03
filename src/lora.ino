/*
 * Exploring the board.
 */
#include "WiFi.h"
#include "SSD1306.h"

#define SCA 22
#define SCL 21

// Set OLED pins
SSD1306Wire display(0x3c, SCL, SCA);

void setup() {
  Serial.begin(115200);
  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // OLED init
  display.init();
  display.flipScreenVertically();

  Serial.println("Setup done");
}

void display_header() {
  display.clear();
  display.setFont(ArialMT_Plain_16);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  //void drawString(int16_t x, int16_t y, String text);
  display.drawString (64, 0, "Power Lab");
}

void display_networks(int n) {
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString (0, 20, "Wi-Fi Networks: " + String(n));
}

void loop() {
  display_header();
  Serial.println("scan start");

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0) {
    Serial.println("no networks found");
  } else {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
      delay(10);
    }
  }

  display_networks(n);
  display.display();

  Serial.println("");
  // Wait a bit before scanning again
  delay(5000);
}
