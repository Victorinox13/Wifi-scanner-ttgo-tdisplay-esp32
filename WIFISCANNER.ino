#include <WiFi.h>      // Wi-Fi scanning library
#include <TFT_eSPI.h>  // TFT display library
#include <Wire.h>

TFT_eSPI tft = TFT_eSPI(); // Initialize TFT instance
#define BUTTON_PIN 0
int buttonPressed = 0;
int networksScanned = 0;



void setup() {
  // Initialize serial communication for debugging
  Serial.begin(115200);

  //button
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Initialize the TFT display
  tft.init();
  tft.setRotation(0); // Set display to landscape orientation
  tft.fillScreen(TFT_BLACK); // Clear the display
  tft.setTextColor(TFT_GREEN, TFT_BLACK); // Set text color to white on black
  tft.setTextSize(1);

  // wifi initialization
  WiFi.mode(WIFI_STA); // Set Wi-Fi to station mode
  WiFi.disconnect();   // Ensure no active connections
  Serial.println("Wi-Fi initialized. Starting scan...");

  // Display a welcome message animation
  tft.fillScreen(TFT_BLACK);
    tft.setCursor((tft.width() - tft.textWidth("WiFi scanner")) / 2, 110);
    tft.println("WiFI scanner");
    tft.setCursor((tft.width() - tft.textWidth("Victor Deleeck")) / 2, 120);
    tft.println("Victor Deleeck");

    tft.setCursor((tft.width() - tft.textWidth("Press left button")) / 2, 20);
    tft.println("Press left button");
    tft.setCursor((tft.width() - tft.textWidth("to start scanning.")) / 2, 30);
    tft.println("to start scanning.");

}

void loop() {


  if (buttonPressed == 0 || buttonPressed == 2){
    buttonPressed = 0;
    delay(100);
  }else{
   
   Serial.println("going scanning");
   networksScanned = WifiScan();
   
   Serial.println("scanned");
   
  
  }
  
 int buttonState = digitalRead(BUTTON_PIN);
  if (buttonState == LOW) {
    buttonPressed = buttonPressed + 1;
    Serial.println("button pressed");
    tft.setCursor(10, 220);
    tft.println("                ");
    delay(500);
    tft.fillScreen(TFT_BLACK);
    
  };

  displayScan(networksScanned);
}
void displayScan(int n) { 
  


   if (n == 0) {
    
  } else {
    // Display the ESSIDs of found networks
    for (int i = 0; i < n; ++i) {
      // Format and display each network's ESSID
      String ssid = WiFi.SSID(i);
      long int signalStrength = WiFi.RSSI(i);
      // Display on TFT screen
      tft.setCursor(5, 30 + (i * 10)); // Adjust vertical position for each ESSID
      tft.printf("%d: %s %ld", i + 1, ssid.c_str(), signalStrength);

    }
    
  }

}
int WifiScan(){
  tft.setCursor(10, 220);
  tft.println("Scanning...");
  int n = WiFi.scanNetworks();
  Serial.println("this is var N");
  Serial.println(n);
  return n;
};


