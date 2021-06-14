#include "entity.h"
#include "../spells/spell.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

Entity *ENTITIES[256];
size_t current_entity = 0;

Entity * spawn_entity(){
  Entity *entity = malloc(sizeof(Entity));
  entity->health = 0;
  entity->max_health = 0;
  entity->uuid = current_entity;
  ENTITIES[current_entity] = entity;
  current_entity += 1;
  return entity;
}

void show_health(Entity * entity){
  printf("Current HP: [");
  printf("\033[0;31m");
  double current_life_percentage = ((double)entity->health / entity->max_health) * 100;
  for (size_t i = 0; i < 10; i++)
  {
    if (i <= (current_life_percentage/100)*10){
      printf("=");
    } else {
      printf(" ");
    }
  }
  printf("\033[0m");
  printf("] - %.2f %%\n", current_life_percentage);
  printf("%ld/%ld hp\n", entity->health, entity->max_health);
}

void show_buffs(Entity * entity){
  if(entity->buff[AttackBuff] > 0){
    printf("\033[0;33m");
    printf("Doble ataque ⚔: %d turnos\n", entity->buff[AttackBuff]);
    printf("\033[0m");
  }
  if(entity->buff[Sangrado] > 0){
    printf("\033[0;31m"); 
    printf("Sangrado 💉: %d acumulaciones\n", entity->buff[Sangrado]);
    printf("\033[0m"); 
  }
  if(entity->buff[Toxic] > 0){
    printf("\033[0;32m"); 
    printf("Envenenado ☠: %d turnos\n", entity->buff[Toxic]);
    printf("\033[0m"); 
  }
  if (entity->buff[Taunted] == true){
    printf("Distraído: 😡\n");
  }
  if (entity->buff[Desmoralized] > 0){
    printf("Desmoralizado: 😓\n");
  }
}

void show_status(Entity * entity){
  printf("=======================\n");
  printf("Entity UUID: %ld\n", entity->uuid);
  show_health(entity);
  printf("\n");
  show_buffs(entity);
  printf("=======================\n");
}