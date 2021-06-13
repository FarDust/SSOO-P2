#include "spell.h"

int do_dmg(Entity* target, int dmg)
{
    int dmg_done;
	if (target->health < dmg)
	{
        dmg_done = target->health;
		target->health = 0;
        return dmg_done;
	} else {
		target->health -= dmg;
        return dmg;
	}
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