

#include "eeprom.hpp"

enum sprite {JOGADOR = 0, VAZIO = 1, CAIXA = 2, PAREDE = 3, LUGAR_CAIXA = 4};

#define TAM_X 14
#define TAM_Y 8

/* Definicoes dos mapas que vao ser gravados na memoria permanente */

static const enum sprite mapa1[]{ 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
                                  3, 3, 3, 3, 1, 4, 1, 4, 1, 3, 3, 3, 3, 3,
                                  3, 3, 3, 3, 1, 2, 1, 2, 1, 3, 3, 3, 3, 3,
                                  3, 3, 3, 3, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3,
                                  3, 3, 3, 3, 1, 1, 0, 1, 1, 3, 3, 3, 3, 3,
                                  3, 3, 3, 3, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3,
                                  3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
                                  3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
                                };

static const enum sprite mapa2[]{ 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
                                  3, 3, 3, 1, 1, 1, 3, 3, 3, 1, 1, 1, 3, 3,
                                  3, 3, 3, 1, 1, 2, 3, 3, 3, 1, 4, 1, 3, 3,
                                  3, 3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3,
                                  3, 3, 3, 0, 1, 2, 3, 3, 3, 1, 4, 1, 1, 3,
                                  3, 3, 3, 1, 1, 1, 3, 3, 3, 1, 1, 3, 3, 3,
                                  3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
                                  3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
                                };


static const enum sprite mapa3[]{ 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
                                  3, 3, 3, 4, 1, 1, 1, 1, 1, 1, 2, 1, 3, 3,
                                  3, 3, 3, 4, 4, 1, 2, 1, 2, 1, 2, 1, 3, 3,
                                  3, 3, 3, 1, 3, 1, 1, 1, 1, 1, 1, 1, 3, 3,
                                  3, 3, 3, 1, 3, 1, 1, 0, 1, 1, 1, 1, 3, 3,
                                  3, 3, 3, 1, 4, 1, 1, 1, 1, 1, 2, 1, 3, 3,
                                  3, 3, 3, 1, 1, 1, 1, 1, 1, 1, 4, 1, 3, 3,
                                  3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
                                };

static const enum sprite mapa4[]{ 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
                                  3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
                                  3, 3, 3, 3, 3, 3, 3, 3, 3, 1, 1, 1, 3, 3,
                                  3, 3, 3, 1, 1, 1, 1, 1, 3, 1, 3, 1, 3, 3,
                                  3, 3, 3, 0, 1, 2, 1, 1, 1, 1, 1, 1, 3, 3,
                                  3, 3, 3, 1, 1, 2, 3, 1, 1, 4, 3, 4, 3, 3,
                                  3, 3, 3, 3, 1, 1, 3, 3, 3, 3, 3, 3, 3, 3,
                                  3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
                                };


void
setup()
{
  Wire.setClock(10000);
  Serial.begin(9600);
  escrever_eeprom(2046, 0);
  for(int i = 0; i < 4*TAM_X*TAM_Y; ++i){
    if(i/(TAM_X*TAM_Y) == 0)
      escrever_eeprom(i, (int) mapa1[i]);
    else if(i/(TAM_X*TAM_Y) == 1)
      escrever_eeprom(i, (int) mapa2[i%(TAM_X*TAM_Y)]);
    else if(i/(TAM_X*TAM_Y) == 2)
      escrever_eeprom(i, (int) mapa3[i%(TAM_X*TAM_Y)]);
    else if(i/(TAM_X*TAM_Y) == 3)
      escrever_eeprom(i, (int) mapa4[i%(TAM_X*TAM_Y)]);
  }
}

void
loop()
{
  /* Verifica se gravamos corretamente */
  delay(5000);
  for(int i=0; i< 4*TAM_X*TAM_Y; ++i){
        if(!(i%TAM_X))
      Serial.println("");
    Serial.print(ler_eeprom(i));
    Serial.print(" ");

  }
}
