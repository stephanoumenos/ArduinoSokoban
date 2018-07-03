
#ifndef EEPROM_H
#define EEPROM_H

#include <Wire.h>
#include <Arduino.h>

unsigned char
ler_eprom(uint16_t endereco_leitura);

void
escrever_eprom(uint16_t endereco_escrita, unsigned char dado);


#endif
