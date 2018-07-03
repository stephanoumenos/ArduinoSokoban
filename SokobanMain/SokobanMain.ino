/* Projeto final EA076
    SOKOBAN
   Creditos: Hugo Loup Clement e Stefano De Checchi */


#include "sokoban.h"

#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <TimerOne.h>

#include <Wire.h>

#define DC_pin 5
#define CS_pin 4
#define RST_pin 3

#define X_pin A0
#define Y_pin A1
#define button_pin 2

Adafruit_PCD8544 lcd = Adafruit_PCD8544(DC_pin, CS_pin, RST_pin);

void
lcd_init()
{
  lcd.begin();
  lcd.setContrast(50);
  lcd.clearDisplay();
  lcd.display();
}

void
desenha_sprite(uint8_t x, uint8_t y, unsigned char* Sprite)
{
    const uint8_t res_x = 84;
    const uint8_t res_y = 48;
    for(int i = 0; i<6; ++i)
      for(int j = 0; j<6; ++j)
        if(Sprite[i] & ((0b100000 >> j) & 0b111111))
          lcd.drawPixel((y*6)+j, (x*6)+i, BLACK);
        else
          lcd.drawPixel((y*6)+j, (x*6)+i, 0);
    lcd.display();

}

volatile bool voltar_menu = false;
volatile bool botao_pressionado = false;
volatile bool limpar_memoria = false;
bool mover_baixo = false;
bool mover_cima = false;
bool mover_esquerda = false;
bool mover_direita = false;

volatile uint8_t mover_na_main = 0;

volatile uint8_t estou_movendo = false;
volatile bool movi_pelo_menos_uma_vez = false;

void
botao_apertado()
{
  static uint8_t i = 0;
  static uint16_t j = 0;
  // se o botao foi apertado...
  if(!digitalRead(button_pin))
    i++;
  else if(i){
    if(i>=50)
      limpar_memoria = true;
    else if(i>=20)
      voltar_menu = true;
    else
      botao_pressionado = true;
    i = 0;
  }
  if(estou_movendo){
    // nao faz nada
  } else if(mover_esquerda){
    if(j == 0){
      mover_na_main = 1;
      estou_movendo= true;
      movi_pelo_menos_uma_vez = true;
    }
  } else if(mover_direita){
    if(j == 0){
      mover_na_main = 2;
      estou_movendo = true;
      movi_pelo_menos_uma_vez = true;
    }
  } else if(mover_cima){

   if(j == 0){
      mover_na_main = 3;
      estou_movendo = true;
      movi_pelo_menos_uma_vez = true;
   }
  } else if(mover_baixo){

    if(j == 0){
      mover_na_main = 4;
      estou_movendo = true;
      movi_pelo_menos_uma_vez = true;
    }
  } else{
    j = 0;
    mover_na_main = 0;
  }
  j = (j+1)%2;
}


void
setup()
{
  Wire.setClock(10000);
  Serial.begin(9600);
  lcd_init();
  pinMode(button_pin, INPUT);
  Timer1.initialize(100000); // Tempo de espera = 100 ms
  Timer1.attachInterrupt(botao_apertado);
}


void
loop()
{
    /* Toda essa rotina esta relacionada a botoes serem apertados
      chamadas sao tratadas dentro do objeto Sokoban */
    static bool movi = false;
    static bool esperar = true;
    static Sokoban jogo = Sokoban();
    if(estou_movendo && mover_na_main){
      switch(mover_na_main){
        case 1: jogo.move_left(); break;
        case 2: jogo.move_right(); break;
        case 3: jogo.move_up(); break;
        case 4: jogo.move_down(); break;
      }
      mover_na_main = 0;
      estou_movendo = false;
    }
    if(botao_pressionado){
      botao_pressionado = false;
      jogo.button_pressed();
    } else if(voltar_menu){
      voltar_menu = false;
      jogo.ir_para_tela_inicial();
    } else if(limpar_memoria){
      limpar_memoria = false;
      jogo.limpar_memoria();
    }
  if(!movi){
    if(analogRead(Y_pin) > 768){
      mover_baixo = true;
      movi = true;
    } else if(analogRead(Y_pin) < 256){
      mover_cima = true;
      movi = true;
    } else if(analogRead(X_pin) < 256){
      mover_esquerda = true;
      movi = true;
    } else if(analogRead(X_pin) > 768){
      mover_direita = true;
      movi = true;
    }
  }
  if(movi && (500 < analogRead(Y_pin))  && (analogRead(Y_pin) < 550) && (500 < analogRead(X_pin)) && (analogRead(X_pin) < 550)){
    movi = false;
    if(!movi_pelo_menos_uma_vez){
      if(mover_baixo){
        jogo.move_down();
      } else if(mover_cima){
        jogo.move_up();
      } else if(mover_direita){
        jogo.move_right();
      } else if(mover_esquerda){
        jogo.move_left();
      }
    }
    movi_pelo_menos_uma_vez = false;
    mover_baixo = mover_cima = mover_esquerda = mover_direita = false;
  }
}
