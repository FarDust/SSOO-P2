#include "spell.h"
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

char* do_dmg(Entity* target, int dmg)
{
  int dmg_done;
  cahr msg[100];
	if (target->health < dmg)
	{
    dmg_done = target->health;
		target->health = 0;
    sprintf(msg, "%s hit by %d hp\n", target->name, amount);
	} else {
		target->health -= dmg;
    printf("Entity %ld - hit by %d hp\n", target->uuid, dmg);
    sprintf(msg, "%s hit by %d hp\n", target->name, amount);
	}
  return msg;
}
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
    "Fuerza Bruta",
    "Salto",
    "Espina Venenosa",
    "Caso de copia",
    "Reprobatron-9000",
    "sudo rm-rf",
    "Ruzgar",
    "Coletazo"
  };
  return SPELL_NAME[spell];
}

bool is_harm_spell(Spell spell){
  bool is_harmfull[SPELL_COUNT] = {0};

  is_harmfull[Estocada] = true;
  is_harmfull[CorteCruzado] = true;
  is_harmfull[Distraer] = true;

  is_harmfull[Curar] = false;
  is_harmfull[DestelloRegenerador] = true;
  is_harmfull[DescargaVital] = true;

  is_harmfull[InyeccionSQL] = false;
  is_harmfull[AtaqueDDOS] = true;
  is_harmfull[FuerzaBruta] = true;

  return is_harmfull[spell];
}

int apply_buffs(Entity *caster, int amount){
  int new_amount = amount;
  if (caster->buff[AttackBuff] > 0)
  {
    new_amount = 2 * new_amount;
    printf("Entity %ld Damage buff x2\n", caster->uuid);
  } else if (caster->buff[Desmoralized])
  {
    new_amount = 0.5 * new_amount;
    printf("Entity %ld Damage buff x0.5\n", caster->uuid);
  }
  return new_amount;
}

char* damage(Entity *target, int amount){
  target->health = target->health - (size_t)fmin(target->health, amount);
  target->health = (size_t)fmax(fmin(target->health, target->max_health), 0);
  printf("Entity %ld hit by %d hp\n", target->uuid, amount);
  char msg[100];
  sprintf(msg, "%s hit by %d hp\n", target->name, amount);
  return msg;
}

char* heal(Entity *target, int amount){
  target->health = (size_t)fmin(fmax(0, target->health + amount), target->max_health);
  printf("Entity %ld heal by %d hp\n", target->uuid, amount);
  char msg[100];
  sprintf(msg, "%s healed by %d hp\n", target->name, amount);
  return msg;
}

char* estocada(Entity *caster, Entity *target){
  damage(target, apply_buffs(caster, 1000));
  target->buff[Sangrado] += 1;
  target->buff[Sangrado] = (unsigned int)fmin(target->buff[Sangrado], 3);
  char msg[100];
  sprintf(msg, "%s ha usado estocada\n%s", caster->name, damage(target, apply_buffs(caster, 1000)));
  return msg;
};

void sangrado(Entity *target){
  damage(target, target->buff[Sangrado] * 500);
};

char* corte_cruzado(Entity *caster, Entity *target){
  char msg[100];
  sprintf(msg, "%s ha usado corte cruzado\n%s", caster->name, damage(target, apply_buffs(caster, 3000)));
  return msg;
}

char* distraer(Entity *caster, Entity *target){
  /*set TauntedBy to uuid of caster and Taunted to true*/
  target->buff[TauntedBy] = caster->uuid;
  target->buff[Taunted] = true;
  char msg[100];
  sprintf(msg, "%s ha usado distraer sobre %s\n", caster->name, target->name);
  return msg;
}

char* curar(Entity *caster, Entity *target){
  size_t RECOVERY_AMOUNT = 2000;
  if(caster->buff[Desmoralized]){
    RECOVERY_AMOUNT = 1000;
  }
  char msg[100];
  if (caster->buff[CasoDeCopiaStatus] == true)
  {
    sprintf(msg, "%s ha usado curar\n%s", caster->name, heal(caster, RECOVERY_AMOUNT));
  }
  else
  {
    sprintf(msg, "%s ha usado curar\n%s", caster->name, heal(target, RECOVERY_AMOUNT));
  }
  return msg;
}

size_t destello_regenerador(Entity *caster, Entity *target){
  time_t t;
  srand((unsigned) time(&t));

  size_t light_damage = 750 + rand() % (1250 + 1);
  damage(target,  apply_buffs(caster, light_damage));
  return light_damage;
}

char* destello_regenerador_side_effect(Entity *caster, Entity *target, size_t amount){
  char msg[100];
  if (caster->buff[CasoDeCopiaStatus] == true)
  {
    sprintf(msg, "%s ha usado destello regenerador\n%s", caster->name, heal(caster, amount));
  }
  else
  {
    sprintf(msg, "%s ha usado destello regenerador\n%s", caster->name, heal(target, amount));
  }
  return msg;
}

char* descarga_vital(Entity *caster, Entity *target){
  size_t damage_dealt = 2 * (caster->max_health - caster->health);
  char msg[100];
  sprintf(msg, "%s ha usado descarga vital\n%s", caster->name, damage(target,  apply_buffs(caster, damage_dealt)));
  return msg;
}

char* inyeccion_sql(Entity *caster, Entity *target){
  /* set turns of attack buff */
  Entity *real_target;
  if (caster->buff[CasoDeCopiaStatus] == true)
  {
    real_target = caster;
  }
  else
  {
    real_target = target;
  }
  real_target->buff[AttackBuff] = 2;
  char msg[100];
  sprintf(msg, "%s ha usado inyeccion_sql sobre %s\n", caster->name, target->name);
  return msg;
}

char* ataque_ddos(Entity *caster, Entity *target){
  char msg[100];
  sprintf(msg, "%s ha usado ataque DDos\n%s", caster->name, damage(target, apply_buffs(caster, 1500)));
  return msg;
}

char* fuerza_bruta(Entity *caster, Entity *target){
  target->buff[BruteForceCharge] += 1;
  char msg[100];
  if (target->buff[BruteForceCharge] == 3){
    target->buff[BruteForceCharge] = 0;
    damage(target,  apply_buffs(caster, 10000));
    sprintf(msg, "%s ha usado fuerza bruta\n%s", caster->name, damage(target, apply_buffs(caster, 10000)));
  }else{
    sprintf(msg, "%s ha usado fuerza bruta\n", caster->name);
  }
  return msg;
}