#include "entity.h"
#include "../spells/spell.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

Entity *ENTITIES[MAX_ENTITIES];
size_t current_entity = 0;

size_t get_entities_number(){
  return current_entity;
}

Entity * get_entity_by(size_t uuid){
  for (size_t entity = 0; entity < get_entities_number(); entity++)
  {
    if (ENTITIES[entity] != NULL && ENTITIES[entity]->uuid == uuid){
      return ENTITIES[entity];
    }
  }
  return NULL;
}

Entity * get_entity(size_t entity){
  return ENTITIES[entity];
}

Entity * set_entity(size_t entity, Entity * value){
  return ENTITIES[entity] = value;
}

Entity * spawn_entity(){
  Entity *entity = malloc(sizeof(Entity));
  entity->name = NULL;
  entity->health = 0;
  entity->max_health = 0;
  entity->uuid = current_entity;
  ENTITIES[current_entity] = entity;
  current_entity += 1;
  return entity;
}

void kill_entity(Entity * entity){
  free(entity);
}

void reset_entities(){
  for (size_t entity= 0; entity< get_entities_number(); entity++)
  {
    kill_entity(get_entity(entity));
  }
  current_entity = 0;
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

void show_status(Entity * entity, bool with_uuid){
  printf("=======================\n");
  if (with_uuid){
    printf("Entity UUID: %ld\n", entity->uuid);
  }
  show_health(entity);
  printf("\n");
  show_buffs(entity);
  printf("=======================\n");
}