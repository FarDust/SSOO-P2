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
char* do_dmg(Entity* target, int dmg);
bool is_harm_spell(Spell spell);
char* damage(Entity *target, int amount);
char* heal(Entity *target, int amount);

char* estocada(Entity *caster, Entity *target);
void sangrado(Entity *target);
char* corte_cruzado(Entity *caster, Entity *target);
char* distraer(Entity *caster, Entity *target);

char* curar(Entity *caster, Entity *target);
size_t destello_regenerador(Entity *caster, Entity *target);
char* destello_regenerador_side_effect(Entity *caster, Entity *target, size_t amount);
char* descarga_vital(Entity *caster, Entity *target);

char* inyeccion_sql(Entity *caster, Entity *target);
char* ataque_ddos(Entity *caster, Entity *target);
char* fuerza_bruta(Entity *caster, Entity *target);
