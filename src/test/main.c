#include "../common/player/player.h"

void test_player_spells(){

  Player *player = spawn_player(Hunter);
  Player *monster = spawn_player(Medic);

  show_spells(player);
  Spell spell = get_spell_slot(player->spec, first);
  select_spell(player, spell);

  if (is_harm_spell(spell)){
    cast_spell(player->properties, monster->properties, spell);
  }

  show_status(player->properties);
  show_status(monster->properties);

  show_spells(monster);
  Spell sec_spell = get_spell_slot(monster->spec, second);
  select_spell(monster, sec_spell);

  cast_spell(monster->properties, player->properties, sec_spell);

  show_status(player->properties);
  show_status(monster->properties);

  kill_player(player);
  kill_player(monster);
}

int main(int argc, char **argv)
{
  test_player_spells();
}