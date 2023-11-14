#include <ESP8266WiFi.h>

const char* ssid     = "dpu";
const char* password = "";

const char* uzanti = "/iletisim";

const char* host = "10.30.43.158"; //web adresi
const uint16_t port = 80;

void setup() {
  Serial.begin(9600);

  WiFi.mode(WIFI_STA); // nodeMCU platformunda çalışacak
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
//*********************************************************************
void loop() {
  Serial.print("connecting to ");
  Serial.print(host);
  Serial.print(':');
  Serial.println(port);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    delay(5000);
    return; // döngünün başına dön
  }

  //burayı sildik
  /*
  // This will send a string to the server
  Serial.println("sending data to server");
  if (client.connected()) {
    client.println("hello from ESP8266");
  }
  */

  Serial.println("Web sunucusuna baglandi.");
  Serial.println("**************************************************************");
  //----------------------ip adresi verilen web sunucusundan ana sayfayı isteme
  String http_request = "";
  http_request += "GET ";
  http_request += uzanti;
  http_request += " HTTP/1.1\r\n";
  http_request += "Host: ";
  http_request += host;
  http_request += "\r\n";
  http_request += "Connection: Closed\r\n";
  client.println(http_request);
  //------------------------isteme sonu-----------------------------

  //--------------5sn boyunca web sitesinden cevap gelmesini bekle----------
  // wait for data to be available
  unsigned long timeout = millis(); // önceki zaman
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !"); // zaman aşımı oldu
      client.stop();
      delay(60000);
      return;
    }
  }
  //------------------------------------------------------------------------

  //--------------------web sitesinden cevabi okuma-------------------------
  String satir;
  while (client.available()) {
    satir = client.readStringUntil('\r');
    Serial.print(satir);
  }

  // Close the connection
  Serial.println();
  Serial.println("closing connection");
  Serial.println("**************************************************************");
  client.stop();

  delay(3000); // execute once every 5 minutes, don't flood remote service
}
