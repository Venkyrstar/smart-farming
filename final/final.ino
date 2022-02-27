/*
 *  This sketch demonstrates how to set up a simple HTTP-like server.
 *  The server will set a GPIO pin depending on the request
 *    http://server_ip/gpio/0 will set the GPIO2 low,
 *    http://server_ip/gpio/1 will set the GPIO2 high
 *  server_ip is the IP address of the ESP8266 module, will be 
 *  printed to Serial when the module is connected.
 */

#include <ESP8266WiFi.h>
#include <UbidotsMicroESP8266.h>
#include<dht.h>
dht DHT;

#define TOKEN  "RRKLzPmCdH1GTn2IY7a8EWr2EjG70a" 
#define WIFISSID "vijay"
#define PASSWORD "12345678"
const char* ssid = "vijay";
const char* password = "12345678";
Ubidots client(TOKEN);
// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);


void setup() {
  Serial.begin(115200);
  delay(10);
  client.wifiConnection(WIFISSID, PASSWORD);

  // prepare GPIO2
  pinMode(12, OUTPUT);
  digitalWrite(12, 0);
  pinMode(14, OUTPUT);
  digitalWrite(14, 0);
  
  // Connect to WiFi network
 
  
 
  
 
  
  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
  
}

void loop() {
 
    float soilmoisture = analogRead(A0);
    delay(100); 
    
    int chk=DHT.read11(D1);
    float temp=DHT.temperature;
    float humi=DHT.humidity;
    delay(100);
    
    client.add("SoilMoisture", soilmoisture);
    client.add("Humidity", humi);
    client.add("Temperature", temp);
    
    
    client.sendAll(true);
   

  // Check if a client has connected
  
  WiFiClient client1 = server.available();
  if (!client1) {
    return;
  }
  
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client1.available()){
    delay(1);
  }
  
  // Read the first line of the request
  String req = client1.readStringUntil('\r');
  Serial.println(req);
  client1.flush();
  
  // Match the request
  int val;int val1;
  if (req.indexOf("/gpio12/0") != -1)
    val1 = 0;
  else if (req.indexOf("/gpio12/1") != -1)
    val1 = 1;
   digitalWrite(12, val1);
   

  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nGPIO is now ";
  s += (val)?"high":"low";
  s += "</html>\n";

  // Send the response to the client
  client1.print(s);
  delay(1);
  Serial.println("Client disonnected");
    
    if (req.indexOf("/gpio14/0") != -1)
    val = 0;
  else if (req.indexOf("/gpio14/1") != -1)
    val = 1;
  else {
    Serial.println("invalid request");
    client1.stop();
    return;
  }

  // Set GPIO2 according to the request
  
  digitalWrite(14, val);
  
  

  // Prepare the response
  String s1 = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nGPIO is now ";
  s1 += (val)?"high":"low";
  s1 += "</html>\n";

  // Send the response to the client
  client1.print(s1);
  delay(1);
  Serial.println("Client disonnected");

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
}

