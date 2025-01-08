#include <WiFi.h>     
#include <TFT_eSPI.h>  
#include <Wire.h>

TFT_eSPI tft = TFT_eSPI(); 

int RightbuttonPressed = 0;
int LeftbuttonPressed = 0;
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

  
  Serial.println("going scanning");
  networksScanned = WifiScan();
  Serial.println("scanned");
  displayScan(networksScanned);

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
    tft.setCursor(10, 220);
    tft.println("                ");
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



void loop() {

 int RightbuttonState = digitalRead(35);
 int LeftbuttonState = digitalRead(0);

  if (LeftbuttonState == LOW) {

    
    LeftbuttonPressed += 1;
    RightbuttonPressed = 0;  
    tft.setCursor(10, 220);
    tft.println("                ");// om scanning... te verwijderen
    SetDisplayCursor(10 * LeftbuttonPressed + 10);
    delay(500);

    
  }
  if (RightbuttonState == LOW) {

    tft.setCursor(2 , LeftbuttonPressed * 10 +10);
    tft.setTextColor(TFT_GREEN, TFT_BLACK); 
    tft.println(" ");
    tft.setTextColor(TFT_GREEN, TFT_BLACK);

    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 10);

    Serial.println(networks[LeftbuttonPressed -1].ssid);
    Serial.println(networks[LeftbuttonPressed -1].encryptionType);
    Serial.println(networks[LeftbuttonPressed -1].rssi);

    tft.println(networks[LeftbuttonPressed -1].ssid);
    tft.setCursor(10, 20);
    tft.println(networks[LeftbuttonPressed -1].encryptionType);
    tft.setCursor(10, 30);
    tft.println(networks[LeftbuttonPressed -1].rssi);

    int status = WL_IDLE_STATUS;
    if (networks[LeftbuttonPressed -1].encryptionType == "OPEN"){
       WiFi.begin(networks[LeftbuttonPressed -1].ssid);
       Serial.println("\nConnecting");
       tft.setCursor(10, 40);
       tft.println("Connecting");

       while(WiFi.status() != WL_CONNECTED){
          Serial.print(".");
          delay(100);
       }      
       
      Serial.print("You're connected to the network");
      IPAddress ip = WiFi.localIP();
      tft.setCursor(10, 50);
      tft.print("IP: ");
      tft.println(ip);

      IPAddress gateway = WiFi.gatewayIP();
      tft.setCursor(10, 60);
      tft.print("GW: ");
      tft.println(gateway);


    }
    else if (networks[LeftbuttonPressed -1].ssid == "---"){
       WiFi.begin("---", "---");
       Serial.println("\nConnecting");
       tft.setCursor(10, 40);
       tft.println("Connecting");

       while(WiFi.status() != WL_CONNECTED){
          Serial.print(".");
          delay(100);
       }
      
      Serial.print("You're connected to the network");
      IPAddress ip = WiFi.localIP();
      tft.setCursor(10, 50);
      tft.print("IP: ");
      tft.println(ip);

      IPAddress gateway = WiFi.gatewayIP();
      tft.setCursor(10, 60);
      tft.print("GW: ");
      tft.println(gateway);
      portScanner();
    } 
    else {
       tft.setCursor(10, 120);
       tft.println("No actions yet.");
    }


  
   
    
    LeftbuttonPressed =0;
    delay(500);
  }
  
}




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




























// Define IP range
const char* subnet = "192.168.1."; // Base subnet
int startIP = 200;                   // Start of IP range
int endIP = 255;                   // End of IP range


//const int ports[] = {21, 23, 80, 443, 445, 554, 1723, 3389}; // Telnet, HTTP, HTTPS
//const char* portNames[] = {"FTP", "Telnet", "HTTP", "HTTPS", "FTPS", "rtsp", "PPTP", "RDP"}; // Names for ports

const int ports[] = {23, 80, 443, 554};
const int numPorts = sizeof(ports) / sizeof(ports[0]);
const char* portNames[] = {"Telnet", "HTTP", "HTTPS", "rtsp"}; // Names for ports


String openIp[256];         // Store IPs with open ports
String openPorts[256];      // Store corresponding open ports for each IP
int openIpCount = 0;

void portScanner(){
  
  for (int i = startIP; i <= endIP; i++) {
    String targetIP = subnet + String(i);
    String portList = ""; // Collect open ports for this IP
    bool isOpen = false;  // Track if any port is open for this IP

    for (int j = 0; j < numPorts; j++) {
      int port = ports[j];
      if (checkPort(targetIP, port)) {
        Serial.printf("Open port found: %s:%d (%s)\n", targetIP.c_str(), port, portNames[j]);
        tft.setCursor(2,0);
        tft.print("+");
        if (!portList.isEmpty()) {
          portList += ","; // Add comma separator for multiple ports
        }
        portList += portNames[j];
        isOpen = true;
      }
    }

    // If any port is open, store the IP and its open ports
    if (isOpen && openIpCount < 256) {
      openIp[openIpCount] = targetIP;
      openPorts[openIpCount] = portList;
      openIpCount++;
    }
  }

  // Display results
  Serial.println("\nIPs with open ports:");
  for (int i = 0; i < openIpCount; i++) {

    tft.setTextSize(0.5);
    tft.setCursor(2, 60 + 10*i);
    Serial.printf("%s %s\n", openIp[i].c_str(), openPorts[i].c_str());
    tft.printf("%s  %s\n", (openIp[i].substring(10)).c_str(), openPorts[i].c_str());
  }



}
bool checkPort(const String& ip, int port) {
  WiFiClient client;
  client.setConnectionTimeout(100); // Set connection timeout to 100ms
  

  // Try to connect to the IP and port
  if (client.connect(ip.c_str(), port)) {
    Serial.println("");
    Serial.printf("%s:%d... ", ip.c_str(), port);
    Serial.println(" ***************************** Open");
    client.stop();
    return true; // Port is open
  } else {
    Serial.printf(".");
    return false; // Port is closed
  }
}

