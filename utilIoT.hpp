#ifndef __UTIL_IOT_H__
#define __UTIL_IOT_H__
#include <Arduino.h>

#define CONT_TYPE "Content-Type"
#define APP_JSN "application/json"

void conectaWiFi(char *ssid,
                 char *pass);
void iniciaTimestamp();
void getTimestamp(char *ts);
String urlDeColeccion(
  char *proyecto,
  char *coleccion);
String urlDeDocumento(
  char *proyecto, char *coleccion,
  char *id);
#endif
