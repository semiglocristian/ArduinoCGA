#ifndef __PROXY_HISTORIAL_H__
#define __PROXY_HISTORIAL_H__

#include <Arduino.h>

class ProxyHistorial {
  private:
    const char * const
    _dispositivoId;
    const char * const
    _huellaDigital;
    const String _url;
    String _creaJson(int valor);
  public:
    ProxyHistorial(
      const char * dispositivoId,
      const char * huellaDigital,
      const String url);
    String add(int valor);
};
#endif
