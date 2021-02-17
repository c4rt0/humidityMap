#include <WiFi.h> 
#include <Wire.h>
#include "DHT.h"

#define DHTTYPE1 DHT22
#define DHTTYPE2 DHT22

//DHT Sensor;
uint8_t DHTPin1 = 14; 
uint8_t DHTPin2 = 13; 
DHT dht1(DHTPin1, DHTTYPE1); 
DHT dht2(DHTPin2, DHTTYPE2); 

float Temperature1;
float Humidity1;
float Temperature2;
float Humidity2;

const char* ssid = "yourWiFiSSID"; 
const char* password = "yourPassword";

WiFiServer server(80);

String header;

// Set the static IP address to specified:
IPAddress local_IP(192, 168, 0, 135);

//Change below's address to your current router(DHCP) address
IPAddress gateway(192, 168, 0, 1);

IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8); 
IPAddress secondaryDNS(8, 8, 4, 4);

void setup() {
Serial.begin(115200);
pinMode(DHTPin1, INPUT);
pinMode(DHTPin2, INPUT);
dht1.begin();
dht2.begin();

// This part of code will try create static IP address
if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
  Serial.println("STA Failed to configure");
}

Serial.print("Connecting to Wifi Network");
Serial.println(ssid);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Successfully connected to WiFi.");
  Serial.println("IP address of ESP32 is : ");
  Serial.println(WiFi.localIP());
  server.begin();
  Serial.println("Server started");

}

void loop()
{
  Temperature1 = dht1.readTemperature(); 
  Humidity1 = dht1.readHumidity(); 
  Temperature2 = dht2.readTemperature(); 
  Humidity2 = dht2.readHumidity(); 
  WiFiClient client = server.available();
  
  if (client) 
    { 
      Serial.println("Web Client connected ");
      String request = client.readStringUntil('\r'); 
      client.println("HTTP/1.1 200 OK");
      client.println("Content-type:text/html");
      client.println("Connection: close");
      client.println();
      // Display html web page
      client.println("<!DOCTYPE html><html>");
      client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
      client.println("<link rel=\"icon\" href=\"data:,\">");

      // CSS to style the table
      client.println("<style>body { text-align: center; font-family: \"Arial\", Arial;}");
      client.println("table { border-collapse: collapse; width:40%; margin-left:auto; margin-right:auto;border-spacing: 2px;background-color: white;border: 4px solid green; }");
      client.println("th { padding: 2px; background-color: #008000; color: white; }");
      client.println("tr { border: 5px solid green; padding: 2px; }");
      client.println("tr:hover { background-color:yellow; }");
      client.println("td { border:4px; padding: 2px; }");
      client.println(".img-container { text-align: center; }");
      client.println(".sensor { color:black; font-weight: bold; padding: 1px; }");

      // Web Page Heading
      client.println("</style></head><body><h1>So that's going to be our new</h1>");
      client.println("<h2>Temperature and Humidity meter :)</h2>");
      client.println("<h2>For every room... </h2>");
      client.println("<h2>Quite soon :D </h2>");
      client.println("<table><tr><th>MEASUREMENT</th><th>VALUE</th></tr>");
      client.println("<tr><td>Temp sensor 1</td><td><span class=\"sensor\">");
      client.println(dht1.readTemperature());
      client.println(" *C</span></td></tr>"); 
      client.println("<tr><td>Temp sensor 2</td><td><span class=\"sensor\">");
      client.println(dht2.readTemperature());
      client.println(" *C</span></td></tr>"); 
//      client.println("<tr><td>Temp. Fahrenheit</td><td><span class=\"sensor\">");
//      client.println(1.8 * dht.readTemperature() + 32);
//      client.println(" *F</span></td></tr>"); 
      client.println("<tr><td>Humidity 1</td><td><span class=\"sensor\">");
      client.println(dht1.readHumidity());
      client.println(" %</span></td></tr>"); 
      client.println("<tr><td>Humidity 2</td><td><span class=\"sensor\">");
      client.println(dht2.readHumidity());
      client.println(" %</span></td></tr>"); 
      client.println("<div class=\"img-container\">");
      client.println("<table><tr><img src=\"https://github.com/c4rt0/humidityMap/blob/main/images/Smiley.png\" alt=\"What are you looking at? :D\" width=\"500\" height=\"500\"></tr></table><span class=\"sensor\">");     
      client.println("</div>");      
      client.println("</body></html>"); 
      client.stop();
      client.println();
      Serial.println("Client disconnected.");
      Serial.println("");
    }
}