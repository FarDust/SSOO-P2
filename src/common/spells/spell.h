#pragma once
#include <stdlib.h>
#include <stdbool.h>
#include "../entity/entity.h"

#define SPELL_COUNT 9
#define MAX_BUFFS 16 
#define MAX_SLOTS 3
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
const char* do_dmg(Entity* target, int dmg);
int apply_buffs(Entity *caster, int amount);
bool is_harm_spell(Spell spell);
const char* damage(Entity *target, int amount);
const char* heal(Entity *target, int amount);

const char* estocada(Entity *caster, Entity *target);
const char *sangrado(Entity *target);
const char* corte_cruzado(Entity *caster, Entity *target);
const char* distraer(Entity *caster, Entity *target);

const char* curar(Entity *caster, Entity *target);
size_t destello_regenerador(Entity *caster, Entity *target);
const char* destello_regenerador_side_effect(Entity *caster, Entity *target, size_t amount);
const char* descarga_vital(Entity *caster, Entity *target);

const char* inyeccion_sql(Entity *caster, Entity *target);
const char* ataque_ddos(Entity *caster, Entity *target);
const char* fuerza_bruta(Entity *caster, Entity *target);

void write_message(char* puntero, const char* arr);
