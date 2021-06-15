#pragma once
#include <stdlib.h>
#include <stdbool.h>
#include "../entity/entity.h"

#define SPELL_COUNT 9
#define MAX_BUFFS 16 
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
  CasoDeCopiaStatus,
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
  third,
  flee
} Slot;

char *get_spell_name(Spell spell);
int do_dmg(Entity* target, int dmg);
bool is_harm_spell(Spell spell);

void estocada(Entity *caster, Entity *target);
void sangrado(Entity *target);
void corte_cruzado(Entity *caster, Entity *target);
void distraer(Entity *caster, Entity *target);

void curar(Entity *caster, Entity *target);
size_t destello_regenerador(Entity *caster, Entity *target);
void destello_regenerador_side_effect(Entity *caster, Entity *target, size_t amount);
void descarga_vital(Entity *caster, Entity *target);

void inyeccion_sql(Entity *caster, Entity *target);
void ataque_ddos(Entity *caster, Entity *target);
void fuerza_bruta(Entity *caster, Entity *target);
