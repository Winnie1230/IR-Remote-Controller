//藉由uRL的方式將esp8266連到家用網路
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#ifndef APSSID
#define APSSID "esp8266123"
#define APPSK  "fuckyou1234567890"
#endif

/* Set these to your desired credentials. */
const char *ssid = APSSID; //esp8266 APmode密碼
const char *password = APPSK;
String homessid=""; //家用wifi名稱
String homepassword="";

ESP8266WebServer server(80);

void setup() {
  delay(1000);
  Serial.begin(115200);
  WiFi.softAP(ssid,password); //設定apmode的密碼

  IPAddress myIP = WiFi.softAPIP();
  server.on("/homewifi", homewifift);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}

//接收URL的家用wifi密碼
void homewifift() {
  server.send(200, "text/html", "<h1>You are connected</h1>");
  if(server.hasArg("ssid")){
    homessid = server.arg("ssid");
    Serial.println(homessid);
  }
  if(server.hasArg("password")){
    homepassword = server.arg("password");
    Serial.println(homepassword);
  } 
  WiFi.begin(homessid,homepassword); //esp8266連到家中wifi
}
