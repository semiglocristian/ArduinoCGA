#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <ArduinoJson.h>

#define SSID "Casa Stark"
#define PASS  "Winterfell3312"
#define HUELLA_DIGITAL "798bb177930734ec69314098399b857fc89321e8"
#define URL "https://firestore.googleapis.com/v1/projects/gilpgiotx/databases/(default)/documents/Entrada/iot2"

ESP8266WiFiMulti WiFiMulti;
bool enviado = false;

String creaJSON() {
  /* Este valor debe obtenerse con
     ArduinoJson Assistant
  */
  StaticJsonDocument<48> doc;
  doc["fields"]["valor"]
  ["integerValue"] = 8;
  String json;
  serializeJson(doc, json);
  return json;
}

void iniciaWiFi() {
  /* Espera a que el hardware para
     WiFi esté listo. */
  for (uint8_t t = 4; t > 0;
       t--) {
    Serial.printf(
      "ESPERANDO %d...\n", t);
    Serial.flush();
    delay(1000);
  }
  Serial.println(
    "Conectando WiFi...");
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(SSID, PASS);
}

void setup() {
  Serial.begin(115200);
  // Serial.setDebugOutput(true);

  Serial.println();
  Serial.println();
  Serial.println();
  iniciaWiFi();
}

void loop() {
  if ((WiFiMulti.run() ==
       WL_CONNECTED) &&
      !enviado) {
    Serial.println(
      "WiFi conectado.");

    std::unique_ptr <
    BearSSL::WiFiClientSecure
    > clie(
      new BearSSL::
      WiFiClientSecure);
    clie->setFingerprint(
      HUELLA_DIGITAL);

    String json = creaJSON();
    Serial.println(json);

    HTTPClient http;
    Serial.print("Conectando ");
    Serial.println(
      "al servidor...");
    if (http.begin(*clie, URL)) {
      http.addHeader(
        "Content-Type",
        "application/json");
      Serial.print(
        "Inicia PATCH...\n");
      int cod = http.PATCH(json);
      if (cod > 0) {
        String texto =
          http.getString();
        Serial.printf(
          "Código de PATCH: %d\n",
          cod);
        Serial.println(texto);
        switch (cod) {
          case HTTP_CODE_OK:
          case
              HTTP_CODE_MOVED_PERMANENTLY:
            enviado = true;
        }
      } else {
        Serial.printf(
          "PATCH falló: %s\n",
          http.errorToString(cod).
          c_str());
      }
      http.end();
    } else {
      Serial.println(
        "La conexión falló");
    }
  }
  Serial.println(
    "Esperando 10s...");
  delay(10000);
}
