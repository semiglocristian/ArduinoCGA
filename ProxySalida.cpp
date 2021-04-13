#include "ProxySalida.hpp"
#include <WiFiClientSecureBearSSL.h>
#include <ESP8266HTTPClient.h>
#include "ResInt.hpp"
#include <ArduinoJson.h>
#include <Arduino.h>

ProxySalida::ProxySalida(
  const char *huellaDigital,
  const String url) :
  _huellaDigital(huellaDigital),
  _url(url) {}

ResInt ProxySalida::get() {
  String error;
  int valor = 0;

  std::unique_ptr <
  BearSSL::WiFiClientSecure
  > clie(
    new BearSSL::
    WiFiClientSecure);
  clie->setFingerprint(
    _huellaDigital);

  HTTPClient http;
  Serial.print("Conectando ");
  Serial.println(
    "al servidor...");
  if (http.begin(*clie, _url)) {
    Serial.println(
      "Inicia GET...");
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
          valor =
            _leeValor(texto);
          break;
        case
            HTTP_CODE_NOT_FOUND:
          error = texto;
      }
    } else {
      error =
        http.errorToString(cod);
    }
    http.end();
  } else {
    error = "La conexión falló";
  }
  Serial.printf("valor: %d\n",
                valor);
  return ResInt(valor, error);
}
int ProxySalida::_leeValor(
  const String texto) {
  StaticJsonDocument<384> doc;
  deserializeJson(doc, texto);
  return doc["fields"]["valor"]
         ["integerValue"]
         .as<int>();
}
