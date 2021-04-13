#ifndef __RES_INT_H__
#define __RES_INT_H__

#include <Arduino.h>

class ResInt {
  public:
    const int valor;
    const String error;
    ResInt(int _valor,
           String _error );
    ResInt(const ResInt& ref );
    const char* valida();
};
#endif
