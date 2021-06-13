#include "./monster.h"

void select_monster_spell(Monster* monster)
{
  srand(time(NULL));
  int r = rand()%100;
  switch (monster->name)
  {
  case GreatJagRuz:
    if (r < 50 )
    {
      monster->current_spell = Ruzgar;
    } else {
      monster->current_spell = Coletazo;
    }
    
    break;
  case Ruzalos:
    if (monster->properties->buff[CanJump]==0)
    {
      monster->current_spell = EspinaVenenosa;
      monster->properties->buff[CanJump]=1;
      return;
    }
    if (r < 40)
    {
      monster->current_spell = Salto;
    } else {
      monster->current_spell = EspinaVenenosa;
    }
    break;
  case Ruiz:
    if (r < 40)
    {
      monster->current_spell = CasoDeCopia;
    } else if (r < 60)
    {
      monster->current_spell = Reprobatron;
    } else {
      monster->current_spell = SudoRmRf;
    }
    break;

  default:
    break;
  }
}

void cast_monster_spell(Monster *monster, Player** players, int n_players, int rounds)
{
  srand(time(NULL));
  Entity* targets[n_players];
  for (size_t i = 0; i < n_players; i++)
  {
    targets[i] = players[i]->properties;
  }
  int target = rand() % n_players;

  switch (monster->current_spell)
  {
  case Salto:
    salto(monster->properties, targets[target]);
    break;
  case EspinaVenenosa:
    espina_venenosa(targets[target]);
    break;
  case Ruzgar:
    ruzgar(targets[target]);
    break;
  case Coletazo:
    coletazo(targets, n_players);
    break;
  case CasoDeCopia:
    caso_de_copia(monster->properties, players[target]->spec, targets[target]);
    break;
  case Reprobatron:
    reprobatron(targets[target]);
    break;
  case SudoRmRf:
    sudoRmRf(rounds, targets, n_players);
    break;

  default:
    break;
  }
}