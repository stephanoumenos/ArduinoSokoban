
#include "eeprom.h"
#include <inttypes.h>

unsigned char
ler_eprom(uint16_t endereco_leitura)
{
    unsigned char endereco_dispositivo = 0b1010000 | ((endereco_leitura >> 8) & 0b111);
    unsigned char dado = 82;
    Wire.beginTransmission(endereco_dispositivo);
    Wire.write(endereco_leitura & 0xFF); // LSB
    Wire.endTransmission();
    Wire.requestFrom(endereco_dispositivo,1);
    if (Wire.available())
      dado = Wire.read();
    delay(6);
    return dado;
}

void
escrever_eprom(uint16_t endereco_escrita, unsigned char dado)
{
    uint16_t i;
    unsigned char endereco_dispositivo = 0b1010000 | ((endereco_escrita >> 8) & 0b111);
    Wire.beginTransmission(endereco_dispositivo);
    Wire.write(endereco_escrita & 0xFF); // LSB
    Wire.write(dado);
    Wire.endTransmission();
    delay(6);
}
