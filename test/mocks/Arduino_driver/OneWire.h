#pragma once
#include "CppUTestExt/MockSupport.h"

class OneWire
{
  public:
    OneWire(uint8_t pin) {
        mock().actualCall("OneWire->constructor(uint8_t)")
              .withUnsignedIntParameter("pin", pin);
    }
};