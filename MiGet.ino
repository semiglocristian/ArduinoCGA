#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <ArduinoJson.h>

#define SSID "Casa Stark"
#define PASS  "Winterfell3312"
#define HUELLA_DIGITAL "798bb177930734ec69314098399b857fc89321e8"
#define URL "https://firestore.googleapis.com/v1/projects/gilpgiotx/databases/(default)/documents/Salida/iot2"

/** Controlador de WiFi. */
ESP8266WiFiMulti WiFiMulti;

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

void
muestraJSON(String texto) {
  /* Este valor debe obtenerse con
     ArduinoJson Assistant. */
  StaticJsonDocument <384> doc;
  /* A partir del texto devuelto
     por Internet, se obtiene el
     árbol JSON. */
  deserializeJson(doc, texto);
  const int valor =
    doc["fields"]["valor"]
    ["integerValue"].as<int>();
  Serial.printf(
    "valor: %d\n", valor);
}

void setup() {
  /* Inicializa el monitor serial
     y calibra su velocidad. */
  Serial.begin(115200);
  /* Monitoreo detallado en la
     salida para encontrar
     errores. */
  // Serial.setDebugOutput(true);

  /* Separa el reporte de esta
     ejecución. */
  Serial.println();
  Serial.println();
  Serial.println();
  iniciaWiFi();
}

void loop() {
  /* Verifica que el WiFi esté
     conectado. */
  if (WiFiMulti.run() ==
      WL_CONNECTED) {
    Serial.println(
      "WiFi conectado.");

    /* Crea un apuntador a un
       cliente de WiFi encriptado.
    */
    std::unique_ptr <
    BearSSL::WiFiClientSecure
    > clie(
      new BearSSL::
      WiFiClientSecure);
    // Asigna la huella digital.
    clie -> setFingerprint(
      HUELLA_DIGITAL);

    HTTPClient http;
    Serial.print("Conectando ");
    Serial.println(
      "al servidor...");
    /* Abre un conexión a la URL
       usando el cliente de WiFi
       encriptado. */
    if (http.begin(*clie, URL)) {
      Serial.println(
        "Inicia GET...");
      /* Hace get a la URL y
         obtiene un código que
         indica el resultado. */
      int cod = http.GET();
      if (cod > 0) {
        String texto =
          http.getString();
        Serial.printf(
          "Código de GET: %d\n",
          cod);
        Serial.println(texto);
        switch (cod) {
          case HTTP_CODE_OK:
          case
              HTTP_CODE_MOVED_PERMANENTLY:
            muestraJSON(texto);
            break;
          case
              HTTP_CODE_NOT_FOUND:
            Serial.println(
              "No encontrado");
        }
      } else {
        Serial.printf(
          "GET falló: %s\n",
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
    "Esperando 2s...");
  delay(2000);
}
