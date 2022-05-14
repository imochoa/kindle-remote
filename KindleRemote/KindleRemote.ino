/*

*/

#include <Servo.h>

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>

#ifndef STASSID
#define STASSID "YOUR_SSID"
#define STAPSK  "YOUR_PASS"
#define HOSTNAME "esp8266"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

// TCP server at port 80 will respond to HTTP requests
WiFiServer server(80);

Servo servo;

void setup(void) {

   
  Serial.begin(115200);

  //Servo on pin D4
  servo.attach(D4);
  
  // Connect to WiFi network
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Set up mDNS responder:
  // - first argument is the domain name, in this example
  //   the fully-qualified domain name is "esp8266.local"
  // - second argument is the IP address to advertise
  //   we send our IP address on the WiFi network
  if (!MDNS.begin("esp8266")) {
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");

  // Start TCP (HTTP) server
  server.begin();
  Serial.println("TCP server started");

  // Add service to MDNS-SD
  MDNS.addService("http", "tcp", 80);
}


String GET_root(){

  String s;

  s.reserve(1024);               // prevent ram fragmentation
  s = 
  "HTTP/1.1 200 OK\r\n"
  "Content-Type: text/html\r\n"
  "\r\n"
  "<!DOCTYPE HTML>\r\n"
  "<html>Hello from ESP8266 at ";

  IPAddress ip = WiFi.localIP();
  String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
  s += ipStr;

  s += "</html>\r\n\r\n";

  Serial.println("Sending 200");
  return s;

}

String GET_404(){

  String s;
  s = "HTTP/1.1 404 Not Found\r\n\r\n";

  Serial.println("Sending 404");
  return s;

}


// FROM https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/server-examples.html?highlight=wificlient#put-it-together
// prepare a web page to be send to a client (web browser)
String prepareHtmlPage()
{
  String htmlPage;
  htmlPage.reserve(1024);               // prevent ram fragmentation
  htmlPage = F("HTTP/1.1 200 OK\r\n"
               "Content-Type: text/html\r\n"
               "Connection: close\r\n"  // the connection will be closed after completion of the response
               "Refresh: 5\r\n"         // refresh the page automatically every 5 sec
               "\r\n"
               "<!DOCTYPE HTML>"
               "<html>"
               "Analog input:  ");
  htmlPage += analogRead(A0);
  htmlPage += F("</html>"
                "\r\n");
  return htmlPage;
}


String read_request(WiFiClient& client){

  String req="";
  String line;
    while (client.connected())
    {
      // read line by line what the client (web browser) is requesting
      if (client.available())
      {
        line = client.readStringUntil('\r');
        req+=line;
        Serial.print(line);
        // wait for end of client's request, that is marked with an empty line
        if (line.length() == 1 && line[0] == '\n')
        {
          // client.println(prepareHtmlPage());
          // Why is there a client.println????
          break;
        }
      }
    }
  return req;
}
  

void loop(void) {

  MDNS.update();

  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  Serial.println("");
  Serial.println("New client");

  // Wait for data from client to become available
  while (client.connected() && !client.available()) {
    delay(1);
  }

  // Read the first line of HTTP request
  // String req = client.readStringUntil('\r');


  String req = read_request(client);
  Serial.println("NEW");
  Serial.println(req);
  Serial.println("DONE");
  // String req_extra = client.getString();



  // First line of HTTP request looks like "GET /path HTTP/1.1"
  // Retrieve the "/path" part by finding the spaces
  int addr_start = req.indexOf(' ');
  int addr_end = req.indexOf(' ', addr_start + 1);
  if (addr_start == -1 || addr_end == -1) {
    Serial.print("Invalid request: ");
    Serial.println(req);
    return;
  }

  // String req 
  String html_method;
  String path;
  html_method = req.substring(0,addr_start);

  path = req.substring(addr_start + 1, addr_end);
  Serial.print("Request: ");
  Serial.println(req);
  client.flush();

  String s;
  if (html_method == "GET"){
    if (path == "/"){
      s = GET_root();
    }
    else {
      Serial.print("GET Endpoint did not exist: ");
      Serial.println(path);
      s = GET_404();
    }
  } else if (html_method == "POST"){
    Serial.println("....called post!");
      Serial.println("PATH");
      Serial.println(path);
      Serial.println("REQ");
      Serial.println(req);
  }


  // if (path == "/") {
  //   // IPAddress ip = WiFi.localIP();
  //   // String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
  //   // // s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>Hello from ESP8266 at ";
    
  //   // s = 
  //   // "HTTP/1.1 200 OK\r\n"
  //   // "Content-Type: text/html\r\n"
  //   // "\r\n"
  //   // "<!DOCTYPE HTML>\r\n"
  //   // "<html>Hello from ESP8266 at ";
    
  //   // s += ipStr;

  //   // s += "</html>\r\n\r\n";

  //   s = GET_root();

  //   Serial.println("Sending 200");
  // } else {

  //   // Endpoint triggered
  //   // servo.write(180);
  //   // delay(1000);
  //   // servo.write(0);
    
  //   s = "HTTP/1.1 404 Not Found\r\n\r\n";
  //   Serial.println("Sending 404");
  // }


  client.print(s);

  Serial.println("Done with client");
}
