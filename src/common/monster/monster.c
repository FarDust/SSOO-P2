#include "monster.h"

// GreatJagRuz
void ruzgar(Monster* monster, Player* target)
{
	printf("Great JagRuz usó Ruzgar");
	dmg_player(targets[i], 1000, monster->name)
}; // 50
void coletazo(Monster* monster, Player** targets, int targets_n)
{
	printf("Great JagRuz usó Coletazo");
	for (int i = 0; i < targets_n; ++i)
	{
		dmg_player(targets[i], 500, monster->name)
	}
}; // 50
// Ruzalos
void salto(Monster* monster, Player* target)
{
	printf("Ruzalos usó Salto");
	dmg_player(target, 1500, monster->name);
	monster->can_jump = false;
}; // 40
void espina_venenosa(Monster* monster, Player* target)
{
	if (target->toxic){
		dmg_player(target, 500, monster)
	} else {
		target->toxic = true;
	}
}; // 60
// Ruiz
void caso_de_copia(Monster* monster, Player* player_to_steal, Player* target)
{
	printf("Ruiz usó Caso de Copia");
}; // 40
void reprobatron(Monster* monster, Player* target)
{
	printf("Ruiz usó Reprobatron");
	target->desmoralized = true;
}; // 20
void sudoRmRf(Monster* monster, int rounds, Player** targets, int targets_n)
{
	printf("Ruiz usó sudo rm -rf");
	for (int i = 0; i < targets_n; ++i)
	{
		dmg_player(targets[i], 100*rounds, monster->name);
	}
}; // 40


void dmg_player(Player* target, int dmg, int uuid)
{
	if (target->health < dmg)
	{
		target->health = 0;
		printf("Great JagRuz hizo %i puntos de daño a %s\n", dmg - target->health, target->uuid);
	} else {
		target->health -= dmg;
		printf("%i hizo %i puntos de daño a %s\n", uuid, dmg, target->uuid); // hacer esto un mensaje(?) buscar name?
	}
}