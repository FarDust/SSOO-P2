#include "../common/player/player.h"
#include "../common/monster/monster.h"
#include <stdbool.h>
#include <stdio.h>

void test_player_spells(){

  Player *player = spawn_player();
  set_player_class(player, Hacker);
  Monster *monster = spawn_monster(Ruzalos);
  Player* players[1];
  players[0] = player;
  player->properties->name = "Jose";
  player->properties->health = 8000;
  show_spells(player);
  char name[6];
  sprintf(name, "flavio");
  char* str1 = name;
  printf("%s\n",str1);
  Spell spell = get_spell_slot(player->spec, first);
  select_spell(player, spell);

  if (is_harm_spell(spell)){
    cast_spell(player->properties, monster->properties, spell);
  }
  show_status(player->properties, true);
  show_status(monster->properties, true);
  // printf("EspinaVenenosa-------------------------------------");
  // select_monster_spell(monster);
  // monster->current_spell=EspinaVenenosa;
  // char * msg = cast_monster_spell(monster, players, 1, 4);
  // printf(msg);
  // free(msg);
  // printf("EspinaVenenosa-------------------------------------");
  // monster->current_spell=Salto;
  // char * msg1 = cast_monster_spell(monster, players, 1, 4);
  // printf(msg1);
  // free(msg1);
  // printf("EspinaVenenosa-------------------------------------");
  // monster->current_spell=Ruzgar;
  // char * msg2 = cast_monster_spell(monster, players, 1, 4);
  // printf(msg2);
  // free(msg2);
  // printf("EspinaVenenosa-------------------------------------");
  monster->current_spell=CasoDeCopia;
  char * msg3 = cast_monster_spell(monster, players, 1, 4);
  printf(msg3);
  free(msg3);
  // printf("EspinaVenenosa-------------------------------------");
  // monster->current_spell=Reprobatron;
  // char * msg4 = cast_monster_spell(monster, players, 1, 4);
  // printf(msg4);
  // free(msg4);
  // printf("EspinaVenenosa-------------------------------------");
  // monster->current_spell=SudoRmRf;
  // char * msg5 = cast_monster_spell(monster, players, 1, 4);
  // printf(msg5);
  // free(msg5);
  // printf("EspinaVenenosa-------------------------------------");
  // monster->current_spell=Coletazo;
  // char * msg6 = cast_monster_spell(monster, players, 1, 4);
  // printf(msg6);
  // free(msg6);
  // show_status(player->properties, true);
  // show_status(monster->properties, true);
  // printf("EspinaVenenosa-------------------------------------");

  kill_player(player);
  kill_monster(monster);
}

int main(int argc, char **argv)
{
  test_player_spells();
}