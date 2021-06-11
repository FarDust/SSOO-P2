#include <stdlib.h>
#include "../spells/spell.h"
#include "../entity/entity.h"

typedef enum
{
  Hunter,
  Medic,
  Hacker,
} PlayerClass;

typedef struct player
{
  size_t uuid;
  size_t health;
  int toxic; // entero que marca el numero de turnos que este jugador esta envenenado
  bool dermoralized;
  PlayerClass spec;
  Spell current_spell;
} Player;

void show_spells(Player *player);
void select_spell(Spell spell);
void cast_spell(Player *player, Entity * target);