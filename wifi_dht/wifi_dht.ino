#include "pin.h"
#include "ESP8266WiFi.h"
#include "ESP8266WiFiMulti.h"

#include "ESP8266HTTPClient.h"
#include "DHT.h"
#include "WiFiClient.h"

ESP8266WiFiMulti WiFiMulti;
DHT dht(D5, DHT11);

char *SSID = "realme X3";
char *PASSWORD = "00001122";

void setup() {

  Serial.begin(115200);
  Serial.setDebugOutput(true);

  Serial.println();
  Serial.println();
  Serial.println();

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(SSID, PASSWORD);

  while (WiFiMulti.run() != WL_CONNECTED){
    Serial.println("wait..");
    delay(500);
  }
}

void loop() {
  //DHT setup
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int l = random(100) % 2;
  if(isnan(h) || isnan(t)){
    Serial.println("Unable to read value from DHT sensor!");
    return;
  }
  Serial.printf("humidity: %f, temp: %f\n", h, t);

  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {
    WiFiClient client;
    HTTPClient http;
    const char *host = "192.168.159.9";
    const int port = 80;
    if(!client.connect(host, port)){
      Serial.println("connect failed");
      Serial.println("wait 3 sec...");
      delay(3000);
      return;
    }
    Serial.print("[HTTP] begin...\n");
    String url ="/aiot/adddata.php?t="+String(t)+"&h="+String(h)+"&l="+String(l);
    client.print(String("GET ")+url+" HTTP/1.1\r\n"+
    "Host: "+host+"\r\n"+
    "Connection: close\r\n\r\n");

    Serial.println("connect closed");
    client.stop();
  }

  delay(1000);
}
