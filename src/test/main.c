#include "../common/player/player.h"
#include "../common/monster/monster.h"

void test_player_spells(){

  Player *player = spawn_player(Hunter);
  show_spells(player);
  kill_player(player);
}

int main(int argc, char **argv)
{
  test_player_spells();
}