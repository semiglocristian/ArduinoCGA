#ifndef __PROXY_SALIDA_H__
#define __PROXY_SALIDA_H__
#include "ResInt.hpp"

#include <Arduino.h>

class ProxySalida {
  private:
    const char * const
    _huellaDigital;
    const String _url;
    int _leeValor(
      const String texto);
  public:
    ProxySalida(
      const char * huellaDigital,
      const String url);
    ResInt get();
};
#endif
