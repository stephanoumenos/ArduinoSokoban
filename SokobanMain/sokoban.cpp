

#include "sokoban.h"


Sokoban::Sokoban()
{
  /* Construtor. Seta estado inicial */
  for(int i = 0; i < TAM_Y; ++i)
    for(int j = 0; j < TAM_X; ++j)
      mapa[i][j] = VAZIO;
  Sokoban::load_saved();
  modo = MENU;
  Sokoban::ir_para_tela_inicial();
}

void
Sokoban::ir_para_tela_inicial()
{
  /* Muda variaveis e vai para a tela inicial */
  y_setinha = 0;
  modo = MENU;
  display_tela_inicial();
}

void
Sokoban::limpar_memoria()
{
  escrever_eprom(2046, 0);
  lcd.clearDisplay();
  lcd.setCursor(0,0);
  lcd.print("Memoria apagada, voltando ao menu...");
  lcd.display();
  delay(2500);
  modo = MENU;
  Sokoban::load_saved();
  mapas_disponiveis = 0;
  Sokoban::ir_para_tela_inicial();
}

void
Sokoban::display_tela_inicial()
{
  lcd.clearDisplay();
  lcd.setCursor(10, 0);
  lcd.print("Nivel 1");
  lcd.setCursor(10,12);
  lcd.print("Nivel 2");
  if(mapas_disponiveis < 1){
    lcd.setCursor(10,12);
    lcd.print("-------");
  }
  lcd.setCursor(10,24);
  lcd.print("Nivel 3");
  if(mapas_disponiveis < 2){
    lcd.setCursor(10,24);
    lcd.print("-------");
  }
  lcd.setCursor(10,36);
  lcd.print("Nivel 4");
  if(mapas_disponiveis < 3){
    lcd.setCursor(10,36);
    lcd.print("-------");
  }
  desenha_sprite(2*y_setinha, 0, setinha);
}

void
Sokoban::load_saved()
{
  /* Carrega progresso do jogador na memoria permanente */
    mapas_disponiveis = ler_eprom(2046);
}

void
Sokoban::carrega_mapa(uint8_t numero_mapa)
{
  /* Carrega mapa de indice dado por numero_mapa da EEPROM para a memoria */
   caixas_faltando = 0;
   for(uint16_t i = 0; i < TAM_Y; i++) {
    for(uint16_t j = 0; j < TAM_X; j++) {
      mapa[i][j] = (enum sprite) ler_eprom(numero_mapa*TAM_X*TAM_Y + i*TAM_X + j);
      if(mapa[i][j] == LUGAR_CAIXA)
        caixas_faltando++;
      else if(mapa[i][j] == JOGADOR){
          y_jogador = i;
          x_jogador = j;
      }
    }
  }
  Sokoban::imprime_mapa_completo();
  modo = JOGANDO;
}

void
Sokoban::imprime_matriz_serial()
{
  /* Imprime o mapa por Serial para vermos se está tudo OK (funcao de debug) */
  for(uint8_t i = 0; i < TAM_Y; i++) {
    for(uint8_t j = 0; j < TAM_X; j++) {
      Serial.print((int) mapa[i][j]);
      Serial.print(" ");
    }
    Serial.println(" ");
  }
}

unsigned char*
Sokoban::number_to_sprite(enum sprite num)
{
    /* Recebe um tipo de sprite e retorna seu vetor de sprite correspondente */
    switch(num){
        case VAZIO: return (unsigned char*) vazio;
        case JOGADOR: return (unsigned char*) jogador;
        case CAIXA: return (unsigned char*) sprite_caixa;
        case LUGAR_CAIXA: return (unsigned char*) sprite_lugar;
        case PAREDE: return (unsigned char*) parede;
        case LUGAR_CAIXA_COM_CAIXA: return (unsigned char*) sprite_lugar_caixa;
        case LUGAR_CAIXA_COM_JOGADOR: return (unsigned char*) sprite_lugar_jogador;
    }
}

void
Sokoban::imprime_mapa_completo()
{
  /* Imrime todo o mapa no LCD */
    for(uint8_t i = 0; i < TAM_Y; ++i)
        for(uint8_t j = 0; j < TAM_X; ++j)
            desenha_sprite(i, j, number_to_sprite(mapa[i][j]));

}

void
Sokoban::acabou_o_nivel()
{
  /* Rotina que é executada quando o jogador completa um nivel */
    lcd.clearDisplay();
    lcd.setCursor(0,0);
    lcd.print("Voce ganhou!");
    lcd.display();
    if(mapas_disponiveis <= 3 && mapa_selecionado == mapas_disponiveis)
        escrever_eprom(2046, ++mapas_disponiveis);
    delay(2500);
    mapa_selecionado++;
    if(mapa_selecionado <= 3)
        Sokoban::carrega_mapa(mapa_selecionado);
    else{
        lcd.clearDisplay();
        lcd.setCursor(0,0);
        lcd.print("Voce acabou o jogo!");
        lcd.print("Creditos: Hugo e Stefano");
        lcd.display();
        delay(5000);
        Sokoban::ir_para_tela_inicial();
    }
}

void
Sokoban::button_pressed()
{
  /* Jogador apertou o botao, tratar dependendo se estamos jogando ou no menu...*/
  if(modo == MENU){
    mapa_selecionado = y_setinha;
    carrega_mapa(y_setinha);
  } else if(modo == JOGANDO){
    carrega_mapa(mapa_selecionado);
  }
}

void
Sokoban::move(uint8_t y_dest, uint8_t x_dest, uint8_t y_dest_next, uint8_t x_dest_next)
{
  /* Rotina generica para mover o jogador ate as coordenadas y_dest, x_dest
     as coordenadas do espaço seguinte também sao dadas para que possamos verificar
     se o movimeto é valido u ao (y_dest_next e x_dest_next) */
    if(y_dest == TAM_Y || x_dest == TAM_X){
        // Acabou o mapa
        return;
    } else if(mapa[y_dest][x_dest] == PAREDE){
        // Bateu na parede
        return;
    } else if(mapa[y_dest][x_dest] == CAIXA || mapa[y_dest][x_dest] == LUGAR_CAIXA_COM_CAIXA){
        // Tem uma caixa no caminho
        if(y_dest_next == TAM_Y || x_dest_next == TAM_X){
            // Acabou o mapa
            return;
        } else if(mapa[y_dest_next][x_dest_next] == PAREDE ||
                  mapa[y_dest_next][x_dest_next] == CAIXA ||
                  mapa[y_dest_next][x_dest_next] == LUGAR_CAIXA_COM_CAIXA){
            // Não é possível o movimento já que há obstáculos no caminho
            return;
        }
        // A partir desse ponto, o movimento é possível
        if(mapa[y_dest][x_dest] == LUGAR_CAIXA_COM_CAIXA){
            // Tirou a caixa do lugar!
            caixas_faltando++;
        }
        if(mapa[y_dest_next][x_dest_next] == VAZIO){
            mapa[y_dest_next][x_dest_next] = CAIXA;
            desenha_sprite(y_dest_next, x_dest_next, sprite_caixa);
        } else if(mapa[y_dest_next][x_dest_next] == LUGAR_CAIXA){
            mapa[y_dest_next][x_dest_next] = LUGAR_CAIXA_COM_CAIXA;
            caixas_faltando--;
            desenha_sprite(y_dest_next, x_dest_next, sprite_lugar_caixa);
        }
    }
    // Agora temos que mover o jogador
    if((mapa[y_dest][x_dest] != LUGAR_CAIXA) &&
        mapa[y_dest][x_dest] != LUGAR_CAIXA_COM_CAIXA){
        mapa[y_dest][x_dest] = JOGADOR;
        desenha_sprite(y_dest, x_dest, jogador);
    } else{
        mapa[y_dest][x_dest] = LUGAR_CAIXA_COM_JOGADOR;
        desenha_sprite(y_dest, x_dest, jogador);
    }
    if(mapa[y_jogador][x_jogador] == JOGADOR){
        mapa[y_jogador][x_jogador] = VAZIO;
        desenha_sprite(y_jogador, x_jogador, vazio);
    } else{
        mapa[y_jogador][x_jogador] = LUGAR_CAIXA;
        desenha_sprite(y_jogador, x_jogador, sprite_lugar);
    }
    x_jogador = x_dest;
    y_jogador = y_dest;
    if(caixas_faltando == 0)
        Sokoban::acabou_o_nivel();
}

void
Sokoban::move_down()
{
  /* Wrapper para mover o jogador pra baixo ou setinha do menu pra baixo */
  if(modo == JOGANDO)
    Sokoban::move(y_jogador+1, x_jogador, y_jogador+2, x_jogador);
  else if(y_setinha < 3 && y_setinha < mapas_disponiveis){
    y_setinha++;
    Sokoban::display_tela_inicial();
  }
}

void
Sokoban::move_up()
{
  /* Wrapper para mover o jogador pra cima ou setinha do menu pra cima*/
  if(modo == JOGANDO)
    Sokoban::move(y_jogador-1, x_jogador, y_jogador-2, x_jogador);
  else if(y_setinha > 0){
    y_setinha--;
    Sokoban::display_tela_inicial();
  }
}

void
Sokoban::move_left()
{
  /* Wrapper para mover o jogador pra esquerda */
  if(modo == JOGANDO)
    Sokoban::move(y_jogador, x_jogador-1, y_jogador, x_jogador-2);
}

void
Sokoban::move_right()
{
  /* Wrapper para mover o jogador pra direita */
  if(modo == JOGANDO)
    Sokoban::move(y_jogador, x_jogador+1, y_jogador, x_jogador+2);
}
