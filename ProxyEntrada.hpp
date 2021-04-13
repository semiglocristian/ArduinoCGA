#ifndef __PROXY_ENTRADA_H__
#define __PROXY_ENTRADA_H__

#include <Arduino.h>

class ProxyEntrada {
  private:
    const char * const
    _huellaDigital;
    const String _url;
    String _creaJson(int valor);
  public:
    ProxyEntrada(
      const char * huellaDigital,
      const String url);
    String set(int valor);
};
#endif
