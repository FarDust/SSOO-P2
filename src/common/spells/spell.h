#include "../entity/entity.h"

typedef enum
{
  Estocada,
  CorteCruzado,
  Distraer,
  Curar,
  DestelloRegenerador,
  DescargaVital,
  InyeccionSQL,
  AtaqueDDOS,
  FuerzaBruta,
  Salto,
  EspinaVenenosa,
  CasoDeCopia,
  Reprobatron,
  SudoRmRf,
  Ruzgar,
  Coletazo
} Spell;

typedef enum
{
  Sangrado,
  Taunted,
  AttackBuff,
  BruteForceCharge,
  Desmoralized,
  Toxic,
  CanJump,
} Buff;

char *get_spell_name(Spell spell);
int do_dmg(Entity* target, int dmg);