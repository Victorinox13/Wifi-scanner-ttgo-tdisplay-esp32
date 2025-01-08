#include <WiFi.h>     
#include <TFT_eSPI.h>  
#include <Wire.h>

TFT_eSPI tft = TFT_eSPI(); 

int LeftbuttonPressed = 3;
int RightbuttonPressed = 3;
int networksScanned = 0;

struct NetworkType {
  String ssid;        // WiFi SSID
  long rssi;          // Signal strength
  String ipAddress;   // IP address (or placeholder)
  String allInfo;     // Combined info string
  String encryptionType;
};

NetworkType networks[15];

void setup() {
  Serial.begin(115200);
  //button
  pinMode(0, INPUT_PULLUP);
  pinMode(35, INPUT_PULLUP);
  //scherm initialisatie
  tft.init();
  tft.setRotation(0); 
  tft.fillScreen(TFT_BLACK); 
  tft.setTextColor(TFT_GREEN, TFT_BLACK); 
  tft.setTextSize(1);
  // wifi initialization
  WiFi.mode(WIFI_STA); 
  WiFi.disconnect();  
  Serial.println("Wi-Fi initialized. Starting scan...");

  WelcomeScreen();
}

void loop() {

 int RightbuttonState = digitalRead(35);
 int LeftbuttonState = digitalRead(0);

  if (LeftbuttonState == LOW) {
    CheckForLeftBut(LeftbuttonState);
    RightbuttonPressed =0;
    delay(500);
  }
  if (RightbuttonState == LOW) {
    Serial.println(networks[RightbuttonPressed].ssid);
    Serial.println(networks[RightbuttonPressed].encryptionType);
    RightbuttonPressed += 1;
    LeftbuttonPressed = 0;  
    tft.setCursor(10, 220);
    tft.println("                ");// om scanning... te verwijderen
    SetDisplayCursor(10 * RightbuttonPressed + 10);
    delay(500);
  }
  
         if (LeftbuttonPressed == 1){
   
           Serial.println("going scanning");
           networksScanned = WifiScan();
           Serial.println("scanned");
           displayScan(networksScanned);
          
           LeftbuttonPressed = 0;
  
         }
  
}

void CheckForLeftBut(int LeftbuttonState) {

    Serial.println("button pressed");
    tft.setCursor(10, 220);
    tft.println("                ");// om scanning... te verwijderen
    LeftbuttonPressed = LeftbuttonPressed + 1;
     if (LeftbuttonPressed == 4){
        //Linker knop voor de eerste keer ingedrukt.
      LeftbuttonPressed = 1;
      tft.fillScreen(TFT_BLACK);

    }else if (LeftbuttonPressed == 0 || LeftbuttonPressed == 2){
          LeftbuttonPressed = 0;   
    }
}

void displayScan(int n) { 
  
  tft.setCursor(67,5);
  tft.println(n);

  if (n == 0) {
    
  } else {

    for (int i = 0; i < n; ++i) {

      String ssid = WiFi.SSID(i);
      
      long int signalStrength = WiFi.RSSI(i);

      networks[i].ssid = WiFi.SSID(i);
      networks[i].rssi = WiFi.RSSI(i);
      networks[i].ipAddress = WiFi.localIP().toString(); // Replace with actual IP logic if needed
      switch (WiFi.encryptionType(i))
            {
            case WIFI_AUTH_OPEN:
                networks[i].encryptionType = ("open");
                break;
            case WIFI_AUTH_WEP:
                networks[i].encryptionType = ("WEP");
                break;
            case WIFI_AUTH_WPA_PSK:
                networks[i].encryptionType = ("WPA");
                break;
            case WIFI_AUTH_WPA2_PSK:
                networks[i].encryptionType = ("WPA2");
                break;
            case WIFI_AUTH_WPA_WPA2_PSK:
                networks[i].encryptionType = ("WPA+WPA2");
                break;
            case WIFI_AUTH_WPA2_ENTERPRISE:
                networks[i].encryptionType = ("WPA2-EAP");
                break;
            case WIFI_AUTH_WPA3_PSK:
                networks[i].encryptionType = ("WPA3");
                break;
            case WIFI_AUTH_WPA2_WPA3_PSK:
                networks[i].encryptionType = ("WPA2+WPA3");
                break;
            case WIFI_AUTH_WAPI_PSK:
                networks[i].encryptionType = ("WAPI");
                break;
            default:
                networks[i].encryptionType = ("unknown");
            }


      tft.setCursor(10, 20 + (i * 10));
      
     

      tft.setCursor(7, 20 + (i * 10));
      
      if (networks[i].encryptionType == "OPEN"){tft.setTextColor(TFT_PINK, TFT_BLACK); tft.print("*"); tft.setTextColor(TFT_GREEN, TFT_BLACK);}
      if (networks[i].encryptionType == "WPA2"){tft.setTextColor(TFT_WHITE, TFT_BLACK); tft.print("*"); tft.setTextColor(TFT_GREEN, TFT_BLACK);}
      if (networks[i].encryptionType == "WPA3"){tft.setTextColor(TFT_GREEN, TFT_BLACK); tft.print("*"); tft.setTextColor(TFT_GREEN, TFT_BLACK);}



      tft.setCursor(15, 20 + (i * 10));
      
      String MaxstatsWifi = String(i + 1) + ": " + ssid + " " + String(signalStrength);
      String MedstatsWifi = String(i + 1) + ": " + ssid;
      String MinstatsWifi = ssid;

      networks[i].allInfo = MaxstatsWifi;

      if ((tft.textWidth(MinstatsWifi) +20) >= tft.width()){
        tft.setTextSize(0.5);
        tft.println(ssid);
        tft.setTextSize(1);
      }else if ((tft.textWidth(MedstatsWifi) + 20) >= tft.width()){
        tft.println(ssid);
      }else if ((tft.textWidth(MaxstatsWifi) + 20) >= tft.width()){
        tft.setTextColor(TFT_YELLOW, TFT_BLACK); 
        tft.printf("%d: ", i + 1);
        tft.setTextColor(TFT_GREEN, TFT_BLACK);
        tft.println(ssid); // Print part of the text normally
        
      }else{
        tft.setTextColor(TFT_YELLOW, TFT_BLACK); 
        tft.printf("%d: ", i + 1);
        tft.setTextColor(TFT_GREEN, TFT_BLACK);
        tft.printf("%s ", ssid.c_str()); // Print part of the text normally
        tft.setTextColor(TFT_RED, TFT_BLACK); 
        tft.println(signalStrength);
        tft.setTextColor(TFT_GREEN, TFT_BLACK);
      }

      

    }
    
  }

}
int WifiScan(){
  
  tft.setCursor(10, 220);
  tft.println("Scanning...");
  int n = WiFi.scanNetworks();
  Serial.println("this is var N");
  Serial.println(n);
   tft.fillScreen(TFT_BLACK);
  return n;
};



void WelcomeScreen(){ tft.fillScreen(TFT_BLACK);
 
    tft.setCursor((tft.width() - tft.textWidth("WiFi scanner")) / 2, 110);
    tft.println("WiFI scanner");
    tft.setCursor((tft.width() - tft.textWidth("Victor Deleeck")) / 2, 120);
    tft.println("Victor Deleeck");

    tft.setCursor((tft.width() - tft.textWidth("Press left button")) / 2, 20);
    tft.println("Press left button");
    tft.setCursor((tft.width() - tft.textWidth("to start scanning.")) / 2, 30);
    tft.println("to start scanning.");
};

void SetDisplayCursor(int y){
    tft.setCursor(2 ,y - 10);
    tft.setTextColor(TFT_GREEN, TFT_BLACK); 
    tft.println(" ");
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    tft.setCursor(2 ,y);
    tft.setTextColor(TFT_WHITE, TFT_BLACK); 
    tft.println("|");
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
}
