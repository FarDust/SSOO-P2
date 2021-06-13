#include "spell.h"
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

char * get_spell_name(Spell spell){
  char *SPELL_NAME[] = {
    "Estocada",
    "Corte Cruzado",
    "Distraer",
    "Curar",
    "Destello Regenerador",
    "Descarga Vital",
    "Inyeccion SQL",
    "Ataque DDOS",
    "Fuerza Bruta"
  };
  return SPELL_NAME[spell];
}

int apply_buffs(Entity *caster, int amount){
  int new_amount = amount;
  if (caster->buff[AttackBuff] > 0)
  {
    new_amount = 2 * new_amount;
    printf("Entity %ld - Damage buff x2", caster->buff[AttackBuff])
  }
  return new_amount;
}

void damage(Entity *target, int amount){
  target->health =- (size_t)fmin(target->health, amount);
  target->health = (size_t)fmax(fmin(target->health, target->max_health), 0);
}

void heal(Entity *target, int amount){
  target->health = (size_t)fmin(fmax(0, target->health + amount), target->max_health);
}

void estocada(Entity *caster, Entity *target){
  damage(target, apply_buffs(caster, 1000));
  target->buff[Sangrado] += 1;
  target->buff[Sangrado] = (unsigned int)fmin(target->buff[Sangrado], 3);
};

void corte_cruzado(Entity *caster, Entity *target){
  damage(target,  apply_buffs(caster, 3000));
}

void distraer(Entity *caster, Entity *target){
  /*set TauntedBy to uuid of caster and Taunted to true*/
  target->buff[TauntedBy] = caster->uuid;
  target->buff[Taunted] = true;
}

void curar(Entity *caster, Entity *target){
  size_t RECOVERY_AMOUNT = 2000;
  if (caster->buff[CasoDeCopia] == true)
  {
    heal(caster, RECOVERY_AMOUNT);
  }
  else
  {
    heal(target, RECOVERY_AMOUNT);
  }
}

size_t destello_regenerador(Entity *caster, Entity *target){
  time_t t;
  srand((unsigned) time(&t));

  size_t light_damage = 750 + rand() % (1250 + 1);
  damage(target,  apply_buffs(caster, light_damage));
  return light_damage;
}

void destello_regenerador_side_effect(Entity *caster, Entity *target, size_t amount){
  if (caster->buff[CasoDeCopia] == true)
  {
    heal(caster, amount);
  }
  else
  {
    heal(target, amount);
  }
}

void descarga_vital(Entity *caster, Entity *target){
  size_t damage_dealt = 2 * (caster->max_health - caster->health);
  damage(target,  apply_buffs(caster, damage_dealt));
}

void inyeccion_sql(Entity *caster, Entity *target){
  /* set turns of attack buff */
  Entity *real_target;
  if (caster->buff[CasoDeCopia] == true)
  {
    real_target = caster;
  }
  else
  {
    real_target = target;
  }
  real_target->buff[AttackBuff] == 2;
}

void ataque_ddos(Entity *caster, Entity *target){
  damage(target,  apply_buffs(caster, 1500));
}

void fuerza_bruta(Entity *caster, Entity *target){
  target->buff[BruteForceCharge] += 1;
  if (target->buff[BruteForceCharge] == 3){
    target->buff[BruteForceCharge] = 0;
    damage(target,  apply_buffs(caster, 10000));
  }
}