#ifndef SOKOBAN_H
#define SOKOBAN_H

#include "eeprom.h"

#include <inttypes.h>

#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

#define TAM_X 14
#define TAM_Y 8

/* Pega a variavel global do lcd da main */
extern Adafruit_PCD8544 lcd;
/* Pega funcao de desenhar sprite da main */
extern void desenha_sprite(uint8_t x, uint8_t y, unsigned char* Sprite);

/* Definicoes de Sprites */

static unsigned char jogador[] = {0b111111,
                                         0b101011,
                                         0b101111,
                                         0b101111,
                                         0b101011,
                                         0b111111};

static unsigned char sprite_caixa[] = {0b111111,
                                              0b110011,
                                              0b101101,
                                              0b101101,
                                              0b110011,
                                              0b111111};

static unsigned char sprite_lugar[] = {0b111111,
                                       0b100001,
                                       0b100001,
                                       0b100001,
                                       0b100001,
                                       0b111111};

 static unsigned char sprite_lugar_jogador[] = {0b111111,
                                                0b100001,
                                                0b101011,
                                                0b110011,
                                                0b101101,
                                                0b111111};

static unsigned char vazio[] = {0b000000,
                                       0b000000,
                                       0b000000,
                                       0b000000,
                                       0b000000,
                                       0b000000};

static unsigned char parede[] = {0b111111,
                                       0b111111,
                                       0b111111,
                                       0b111111,
                                       0b111111,
                                       0b111111};

static unsigned char sprite_lugar_caixa[] = {0b111111,
                                             0b101001,
                                             0b111111,
                                             0b101001,
                                             0b101001,
                                             0b111111};

static unsigned char setinha[] = {0b100000,
                                  0b111000,
                                  0b111100,
                                  0b111000,
                                  0b100000,
                                  0b000000};

enum sprite {JOGADOR = 0, VAZIO = 1, CAIXA = 2, PAREDE = 3, LUGAR_CAIXA = 4,
             LUGAR_CAIXA_COM_CAIXA = 5, LUGAR_CAIXA_COM_JOGADOR = 6};
enum modo_atual {JOGANDO, MENU};

class Sokoban{
public:

  Sokoban();
  void ir_para_tela_inicial();
  void imprime_mapa_completo();
  void imprime_matriz_serial();
  void move_down();
  void move_up();
  void move_left();
  void move_right();
  void button_pressed();
  void limpar_memoria();

private:
  enum modo_atual modo;
  uint8_t x_jogador;
  uint8_t y_jogador;
  uint8_t y_setinha;
  uint8_t mapas_disponiveis;
  uint8_t mapa_selecionado;
  uint8_t caixas_faltando;
  enum sprite mapa[TAM_Y][TAM_X];

  void display_tela_inicial();
  void load_saved();
  void acabou_o_nivel();
  void carrega_mapa(uint8_t numero_mapa);
  void move(uint8_t y_dest, uint8_t x_dest, uint8_t y_dest_next, uint8_t x_dest_next);
  unsigned char* number_to_sprite(enum sprite num);
};


#endif
