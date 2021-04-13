#include "ResInt.hpp"
#include <Arduino.h>

ResInt::ResInt(int _valor,
               String _error ):
  valor(_valor),
  error(_error) {}

ResInt::ResInt(
  const ResInt& ref ):
  valor(ref.valor),
  error(ref.error) {}
