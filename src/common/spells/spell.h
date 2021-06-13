#pragma once
#include <stdlib.h>
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
  CasoDeCopia,
  Sangrado,
  Taunted,
  TauntedBy,
  AttackBuff,
  BruteForceCharge,
  Desmoralized,
  Toxic,
  JumpBlocked,
} Buff;

typedef enum
{
  first,
  second,
  third
} Slot;

char *get_spell_name(Spell spell);

int do_dmg(Entity* target, int dmg);

void estocada(Entity *caster, Entity *target);
void corte_cruzado(Entity *caster, Entity *target);
void distraer(Entity *caster, Entity *target);

void curar(Entity *caster, Entity *target);
size_t destello_regenerador(Entity *caster, Entity *target);
void destello_regenerador_side_effect(Entity *caster, Entity *target, size_t amount);
void descarga_vital(Entity *caster, Entity *target);

void inyeccion_sql(Entity *caster, Entity *target);
void ataque_ddos(Entity *caster, Entity *target);
void fuerza_bruta(Entity *caster, Entity *target);
