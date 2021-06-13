#include "../common/player/player.h"
#include "../common/monster/monster.h"

void test_player_spells(){
  Player *player = malloc(sizeof(Player));
  Entity *entity = malloc(sizeof(Entity));
  player->properties = entity;
  player->spec = Hunter;
  player->current_spell = 0;
  entity->health = 0;
  entity->uuid = 0;

  show_spells(player);

  free(player->properties);
  free(player);
}

int main(int argc, char **argv)
{
  test_player_spells();
}