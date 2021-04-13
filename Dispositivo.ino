#include "utilIoT.hpp"
#include "ResInt.hpp"
#include "ProxySalida.hpp"
#include "ProxyEntrada.hpp"
#include "ProxyHistorial.hpp"

#define SSID "Casa Stark"
#define PASS  "Winterfell3312"
#define HUELLA_DIGITAL "798bb177930734ec69314098399b857fc89321e8"
#define DISPOSITIVO_ID "iot2"
#define ID_PROYECTO "cgaiotx"

// Pin del botón flash.
const int BOTON_FLASH = 0;
// Pin de un LED.
const int LED_INTEGRADO_2 =  2;
const String URL_SALIDA(
  urlDeDocumento(
    ID_PROYECTO, "Salida",
    DISPOSITIVO_ID));
const String URL_ENTRADA(
  urlDeDocumento(
    ID_PROYECTO, "Entrada",
    DISPOSITIVO_ID));
const String URL_HISTORIAL(
  urlDeColeccion(
    ID_PROYECTO, "Historial"));

int entrada = 0;
ProxySalida proxySalida(
  HUELLA_DIGITAL, URL_SALIDA);
ProxyEntrada proxyEntrada(
  HUELLA_DIGITAL, URL_ENTRADA);
ProxyHistorial proxyHistorial(
  DISPOSITIVO_ID, HUELLA_DIGITAL,
  URL_HISTORIAL);

void muestraSalida(int valor) {
  digitalWrite(
    LED_INTEGRADO_2,
    valor > 0 ? LOW : HIGH);
}
int recuperaEntrada() {
  const int estadoDelBoton =
    digitalRead(BOTON_FLASH);
  return estadoDelBoton == HIGH ?
         10 : 2;
}
void
muestraError(String mensaje) {
  Serial.println(mensaje.c_str());
}

void setup() {
  Serial.begin(115200);
  /* Inicializa el pin para
     LED_INTEGRADO_2 como salida.
  */
  pinMode(
    LED_INTEGRADO_2, OUTPUT);
  /* Inicializa el pin para
     BOTÓN_FLASH como entrada. */
  pinMode(BOTON_FLASH, INPUT);
  conectaWiFi(SSID, PASS);
  muestraLaSalidaDelServidor();
  enviaLaEntrada(true);
}

void loop() {
  muestraLaSalidaDelServidor();
  enviaLaEntrada(false);
}

void
muestraLaSalidaDelServidor() {
  const ResInt res =
    proxySalida.get();
  if (res.error.length() > 0) {
    muestraError(res.error);
  } else {
    muestraSalida(res.valor);
  }
}

void
enviaLaEntrada(bool forzosa) {
  const int nuevaEntrada =
    recuperaEntrada();
  if (forzosa ||
      entrada != nuevaEntrada) {
    String error =
      proxyEntrada.
      set(nuevaEntrada);
    if (error.length() > 0) {
      muestraError(error);
      return;
    }
    error = proxyHistorial.
            add(nuevaEntrada);
    if (error.length() > 0) {
      muestraError(error);
      return;
    }
    entrada = nuevaEntrada;
  }
}
