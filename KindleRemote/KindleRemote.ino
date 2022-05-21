//#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>


const char* STASSID = "";
const char* STAPSK = "";
const char* HOSTNAME = "esp";

ESP8266WebServer server(80);

void setup(void) {
  Serial.begin(115200);

  // Connect to WiFi network
  WiFi.mode(WIFI_STA);
  WiFi.begin(STASSID, STAPSK);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Connected to "+String(STASSID));
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  // Set up mDNS responder:
  // - first argument is the domain name, in this example
  //   the fully-qualified domain name is "esp8266.local"
  // - second argument is the IP address to advertise
  //   we send our IP address on the WiFi network
  if (!MDNS.begin(HOSTNAME)) {
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(500);
    }
  }
  Serial.print("mDNS responder started at http://"+String(HOSTNAME)+".local");

  
  server.onNotFound([](){
    server.send(404, "text/plain", "Link wurde nicht gefunden!");  
  });

  server.on("/", []() {
    server.send(200, "text/plain", "ESP-Startseite!");
  });

  server.on("/test", HTTP_POST, []() {
    Serial.println("Triggered printPOST!");
    String postBody = server.arg("plain");
    Serial.println(postBody);

    // Add a header to respond with a new location for the browser to go to the home page again
    server.sendHeader("Location","/"); 
    // Send it back to the browser with an HTTP status 303 (See Other) to redirect
    server.send(303);
  }); 

  server.begin();

  // Add service to MDNS-SD
  MDNS.addService("http", "tcp", 80);
}

void loop(void) {
  server.handleClient();  
  MDNS.update();
}
