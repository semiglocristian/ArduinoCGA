#include "ProxyEntrada.hpp"
#include <WiFiClientSecureBearSSL.h>
#include <ESP8266HTTPClient.h>
#include "utilIoT.hpp"
#include <ArduinoJson.h>
#include <Arduino.h>

ProxyEntrada::ProxyEntrada(
  const char* huellaDigital,
  const String url):
  _huellaDigital(huellaDigital),
  _url(url)
{ }

String
ProxyEntrada::set(int valor) {
  String error;
  String json = _creaJson(valor);
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
    http.addHeader(
      CONT_TYPE, APP_JSN);
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
  return error;
}

String ProxyEntrada::_creaJson(
  int valor) {
  StaticJsonDocument<48> doc;
  doc["fields"]["valor"]
  ["integerValue"] = valor;
  String json;
  serializeJson(doc, json);
  return json;
}
