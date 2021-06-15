#include "../common/player/player.h"
#include "../common/monster/monster.h"
#include <stdbool.h>

void test_player_spells(){

  Player *player = spawn_player(Hunter);
  Monster *monster = spawn_monster(Ruzalos);
  Player* players[1];
  players[0] = player;
  show_spells(player);
  Spell spell = get_spell_slot(player->spec, first);
  select_spell(player, spell);

  if (is_harm_spell(spell)){
    cast_spell(player->properties, monster->properties, spell);
  }
  show_status(player->properties, true);
  show_status(monster->properties, true);

  select_monster_spell(monster);
  cast_monster_spell(monster, players, 1, 4);

  show_status(player->properties, true);
  show_status(monster->properties, true);

  kill_player(player);
  kill_monster(monster);
}

int main(int argc, char **argv)
{
  test_player_spells();
}