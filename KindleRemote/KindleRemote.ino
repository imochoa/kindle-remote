/*
Good guide
https://tttapa.github.io/ESP8266/Chap10%20-%20Simple%20Web%20Server.html

Docs

Don't re-invent the wheel!
https://github.com/esp8266/Arduino/blob/da6ec83b5fdbd5b02f04cf143dcf8e158a8cfd36/libraries/ESP8266WebServer/src/Parsing-impl.h

https://github.com/esp8266/Arduino/tree/da6ec83b5fdbd5b02f04cf143dcf8e158a8cfd36/libraries/ESP8266WebServer


*/

// #include <Servo.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h> 
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>   // Include the WebServer library

// https://tttapa.github.io/ESP8266/Chap10%20-%20Simple%20Web%20Server.html
#ifndef STASSID
#define STASSID "YOUR_SSID"
#define STAPSK  "YOUR_PASS"
#define HOSTNAME "esp8266"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

// TCP server at port 80 will respond to HTTP requests
ESP8266WiFiMulti wifiMulti;     // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'

ESP8266WebServer server(80);    // Create a webserver object that listens for HTTP request on port 80

void handleRoot();              // function prototypes for HTTP handlers
void handleServo();              // function prototypes for HTTP handlers
void handleNotFound();

// Servo servo;

void setup(void) {

   
  Serial.begin(115200);

  //Servo on pin D4
  // servo.attach(D4);

  // add Wi-Fi networks you want to connect to
  wifiMulti.addAP(ssid, password);     
  // wifiMulti.addAP("ssid_from_AP_2", "your_password_for_AP_2");
  // wifiMulti.addAP("ssid_from_AP_3", "your_password_for_AP_3");
  
    Serial.println("Connecting ...");
  int i = 0;
  while (wifiMulti.run() != WL_CONNECTED) { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
    delay(250);
    Serial.print('.');
  }
  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());              // Tell us what network we're connected to
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());           // Send the IP address of the ESP8266 to the computer

  if (MDNS.begin("esp8266.local")) {              // Start the mDNS responder for esp8266.local
    Serial.println("mDNS responder started");
  } else {
    Serial.println("Error setting up MDNS responder!");
  }

  server.on("/", HTTP_GET, handleRoot);     // Call the 'handleRoot' function when a client requests URI "/"
  server.on("/servo", HTTP_POST, handleServo);  // Call the 'handleLED' function when a POST request is made to URI "/LED"

  server.begin();                           // Actually start the server
  Serial.println("HTTP server started");
}


// String GET_root(){

//   String s;

//   s.reserve(1024);               // prevent ram fragmentation
//   s = 
//   "HTTP/1.1 200 OK\r\n"
//   "Content-Type: text/html\r\n"
//   "\r\n"
//   "<!DOCTYPE HTML>\r\n"
//   "<html>Hello from ESP8266 at ";

//   IPAddress ip = WiFi.localIP();
//   String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
//   s += ipStr;

//   s += "</html>\r\n\r\n";

//   Serial.println("Sending 200");
//   return s;

// }

// String GET_404(){

//   String s;
//   s = "HTTP/1.1 404 Not Found\r\n\r\n";

//   Serial.println("Sending 404");
//   return s;

// }


// // FROM https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/server-examples.html?highlight=wificlient#put-it-together
// // prepare a web page to be send to a client (web browser)
// String prepareHtmlPage()
// {
//   String htmlPage;
//   htmlPage.reserve(1024);               // prevent ram fragmentation
//   htmlPage = F("HTTP/1.1 200 OK\r\n"
//                "Content-Type: text/html\r\n"
//                "Connection: close\r\n"  // the connection will be closed after completion of the response
//                "Refresh: 5\r\n"         // refresh the page automatically every 5 sec
//                "\r\n"
//                "<!DOCTYPE HTML>"
//                "<html>"
//                "Analog input:  ");
//   htmlPage += analogRead(A0);
//   htmlPage += F("</html>"
//                 "\r\n");
//   return htmlPage;
// }


// String read_request(WiFiClient& client){

//   String req="";
//   String line;
//     while (client.connected())
//     {
//       // read line by line what the client (web browser) is requesting
//       if (client.available())
//       {
//         line = client.readStringUntil('\r');
//         req+=line;
//         Serial.print(line);
//         // wait for end of client's request, that is marked with an empty line
//         if (line.length() == 1 && line[0] == '\n')
//         {
//           // client.println(prepareHtmlPage());
//           // Why is there a client.println????
//           break;
//         }
//       }
//     }
//   return req;
// }
  

void loop(void) {
    
    server.handleClient();                    // Listen for HTTP requests 

  // MDNS.update();

  // // Check if a client has connected
  // WiFiClient client = server.available();
  // if (!client) {
  //   return;
  // }
  // Serial.println("");
  // Serial.println("New client");

  // // Wait for data from client to become available
  // while (client.connected() && !client.available()) {
  //   delay(1);
  // }

  // // Read the first line of HTTP request
  // // String req = client.readStringUntil('\r');


  // String req = read_request(client);
  // Serial.println("NEW");
  // Serial.println(req);
  // Serial.println("DONE");
  // // String req_extra = client.getString();



  // // First line of HTTP request looks like "GET /path HTTP/1.1"
  // // Retrieve the "/path" part by finding the spaces
  // int addr_start = req.indexOf(' ');
  // int addr_end = req.indexOf(' ', addr_start + 1);
  // if (addr_start == -1 || addr_end == -1) {
  //   Serial.print("Invalid request: ");
  //   Serial.println(req);
  //   return;
  // }

  // // String req 
  // String html_method;
  // String path;
  // html_method = req.substring(0,addr_start);

  // path = req.substring(addr_start + 1, addr_end);
  // Serial.print("Request: ");
  // Serial.println(req);
  // client.flush();

  // String s;
  // if (html_method == "GET"){
  //   if (path == "/"){
  //     s = GET_root();
  //   }
  //   else {
  //     Serial.print("GET Endpoint did not exist: ");
  //     Serial.println(path);
  //     s = GET_404();
  //   }
  // } else if (html_method == "POST"){
  //   Serial.println("....called post!");
  //     Serial.println("PATH");
  //     Serial.println(path);
  //     Serial.println("REQ");
  //     Serial.println(req);
  // }


  // // if (path == "/") {
  // //   // IPAddress ip = WiFi.localIP();
  // //   // String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
  // //   // // s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>Hello from ESP8266 at ";
    
  // //   // s = 
  // //   // "HTTP/1.1 200 OK\r\n"
  // //   // "Content-Type: text/html\r\n"
  // //   // "\r\n"
  // //   // "<!DOCTYPE HTML>\r\n"
  // //   // "<html>Hello from ESP8266 at ";
    
  // //   // s += ipStr;

  // //   // s += "</html>\r\n\r\n";

  // //   s = GET_root();

  // //   Serial.println("Sending 200");
  // // } else {

  // //   // Endpoint triggered
  // //   // servo.write(180);
  // //   // delay(1000);
  // //   // servo.write(0);
    
  // //   s = "HTTP/1.1 404 Not Found\r\n\r\n";
  // //   Serial.println("Sending 404");
  // // }


  // client.print(s);

  // Serial.println("Done with client");
}


void handleRoot() {                         // When URI / is requested, send a web page with a button to toggle the LED
  server.send(200, "text/html", "<form action=\"/LED\" method=\"POST\"><input type=\"submit\" value=\"Toggle LED\"></form>");
}


void handleServo() {                          // If a POST request is made to URI /LED
  // digitalWrite(led,!digitalRead(led));      // Change the state of the LED
  server.sendHeader("Location","/");        // Add a header to respond with a new location for the browser to go to the home page again
  server.send(303);                         // Send it back to the browser with an HTTP status 303 (See Other) to redirect
}

void handleNotFound(){
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}
