#include "player.h"
#include <stdio.h>

void show_spells(Player *player){

  switch (player->spec)
  {
  case Hunter:
    printf("0 - %s\n", get_spell_name(Estocada));
    printf("1 - %s\n", get_spell_name(CorteCruzado));
    printf("2 - %s\n", get_spell_name(Distraer));
    break;
  case Medic:
    printf("0 - %s\n", get_spell_name(Curar));
    printf("1 - %s\n", get_spell_name(DestelloRegenerador));
    printf("2 - %s\n", get_spell_name(DescargaVital));
    break;
  case Hacker:
    printf("0 - %s\n", get_spell_name(InyeccionSQL));
    printf("1 - %s\n", get_spell_name(AtaqueDDOS));
    printf("2 - %s\n", get_spell_name(FuerzaBruta));
    break;

  default:
    break;
  }

}