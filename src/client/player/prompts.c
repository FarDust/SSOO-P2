#include "prompts.h"
#include "../../common/player/player.h"

#include <stdlib.h>
#include <stdio.h>

void show_classes(){
  printf("===================\n");
  printf("=     Clases      =\n");
  printf("-------------------\n");
  for (size_t spec = 0; spec < CLASS_NUMBER; spec++)
  {
    printf("Opción: %ld\n", spec);
    printf("Nombre: %s\n", get_class_name(spec));
    printf("MAX HP: %ld\n\n", get_spec_health(spec));
    printf("Hechizos disponibles:\n");
    for (size_t slot = 0; slot < 3; slot++)
    {
      printf("- %s\n", get_spell_name(get_spell_slot(spec, slot)));
    }
    printf("-------------------\n");
  }
  
}