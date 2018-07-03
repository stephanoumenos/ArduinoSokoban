
#ifndef EEPROM_H
#define EEPROM_H

#include <Wire.h>
#include <Arduino.h>

unsigned char
ler_eeprom(uint16_t endereco_leitura);

void
escrever_eeprom(uint16_t endereco_escrita, unsigned char dado);


#endif
