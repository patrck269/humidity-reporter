#include <Arduino.h>
#include "WiFi.h"
#include "ArduinoHttpClient.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <WiFiUdp.h>
// put function declarations here:


// defines
#define DHTPIN 4
#define DHTTYPE DHT11
#define MonPin 0
#define uS_TO_S_FACTOR 1000000ULL // Conversion factor for micro seconds to seconds
#define TIME_TO_SLEEP 240 // Time ESP32 will go to sleep (in seconds)

// Unit Numbering
#define UNITNUM 1
#define LOCATION "MUDROOM"


//objects
DHT dht(DHTPIN, DHTTYPE);
WiFiUDP udp;
// var declarations
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";


// Define your static IP, gateway, and subnet
IPAddress local_IP(192, 168, 5, UNITNUM); // Choose an unused IP on your network
IPAddress gateway(192, 168, 1, 1);    // Your router's IP
IPAddress subnet(255, 255, 240, 0);  // Your network's subnet mask
IPAddress primaryDNS(192, 168, 1, 1);    // Target

// Commented out because fuck the ADC on the esp32
// const int battFull = 2600;  // voltage when battery is full
// const int battEmpty = 1600; // voltage when battery is empty

const char* udpAddress = "192.168.1.100"; // e.g., "192.168.1.100" or "255.255.255.255"
const int udpPort = 5052;
int VPercent = 0;
int AVal = 0;
int Fuck = 0;
WiFiClient wifi;
//HttpClient client = HttpClient(wifi, "192.168.1.179", 8080);
int status = WL_IDLE_STATUS;


void setup() {
  // WiFi.mode(WIFI_OFF);
  Serial.begin(9600);
  // delay(10000);
  Serial.println("Serial Started");
  WiFi.disconnect(true);
  pinMode(3, OUTPUT);
  // Not bothering to check battery status
  //AVal = analogRead(MonPin);
  //VPercent = (AVal - 1600) / 10 ;
  // btStop();
  dht.begin();
  // delay(5000); // wait for console opening
  // setCpuFrequencyMhz(80);
  //Serial.print("XTAL crystal frequency: ");
  //uint32_t x = getXtalFrequencyMhz();
  //uint32_t y = getCpuFrequencyMhz();
  //Serial.println(x);
  //Serial.print("Current frequency: ");
  //Serial.println(y);
  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);

  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS)){
     Serial.println("STA Failed to configure");
   }

  WiFi.disconnect(true);
  delay(1000);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(3, HIGH);
    delay(500);
    Serial.println("Connecting to WiFi..");
    digitalWrite(3, LOW);
    delay(500);
    Fuck += 1;
    if (Fuck > 10){
      Serial.println("I BRING DISHONORU UPON MY FAMIRY!!!");
      delay(500);
      ESP.restart(); // Program self-katanas if it doesnt connect in a reasonable time frame
    }
  }
  Serial.println("Connected, Bitch");
  Serial.println(WiFi.localIP());
  // Serial.print("Current battery percentage: ");
  // Not bothering to check battery status
  //Serial.print(VPercent);
  //Serial.println("%");
  //Serial.println(AVal);

  digitalWrite(3, HIGH);
  delay(4000); // Sensor wont work without delay

}

void loop() {

  
  float h = dht.readHumidity();
  float f = dht.readTemperature(true);
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(f);
  Serial.print(F("°F"));

  udp.beginPacket(udpAddress, udpPort);
  udp.printf("Humidity:%f",h);
  udp.printf(" Temperature:%f",f);
  udp.printf(" Location:%s",LOCATION);
  udp.printf("\n");
  // udp.printf("Hello from ESP32! Millis: %lu", millis());
  udp.endPacket();
  delay(1000);
  // put your main code here, to run repeatedly:
  // client.get("/"+String(h)+","+f);
  // int statusCode = client.responseStatusCode();
  // client.stop();
  //String response = client.responseBody();
  // Serial.print("Status code: ");
  // Serial.println(statusCode);
  //Serial.print("Response: ");
  //Serial.println(response);
  
  // Old method of delaying send
  // delay(30000);

  // Commenting out because it seems to have an issue with reconnecting to wifi in a timely manner
  digitalWrite(3, LOW);
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  WiFi.disconnect(true);
  delay(2000);
  esp_deep_sleep_start();
}

