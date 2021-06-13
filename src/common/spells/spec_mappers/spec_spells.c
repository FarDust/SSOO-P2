#include <math.h>
#include "spec_spells.h"

Spell get_hunter_spell(Slot slot){
  Spell spells[] = {Estocada,
                    CorteCruzado,
                    Distraer};
 
  return spells[slot];
}

Spell get_medic_spell(Slot slot){
  Spell spells[] = {Curar,
                    DestelloRegenerador,
                    DescargaVital};
 
  return spells[slot];
}

Spell get_hacker_spell(Slot slot){
  Spell spells[] = {InyeccionSQL,
                    AtaqueDDOS,
                    FuerzaBruta};
 
  return spells[slot];
}