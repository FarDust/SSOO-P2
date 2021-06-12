#pragma once
#include <stdlib.h>
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
} Spell;

typedef enum
{
  Sangrado,
  Taunted,
  AttackBuff,
  BruteForceCharge,
} Buff;

char *get_spell_name(Spell spell);