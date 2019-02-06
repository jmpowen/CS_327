#ifndef INTERACTION_H
# define INTERACTION_H

#include "dungeon.h"
#include "character.h"
#include "pc.h"
#include "npc.h"

typedef enum NESW {
  North,
  East,
  South,
  West,
} NESW_t;

typedef struct direction {
  int distance;
  NESW_t compass;
} direction_t;

typedef struct drawn_monsters {
  char symbol;
  int x;
  int y;
  int num_directions;
  NESW_t nsew;
} drawn_monsters_t;

void init_interaction(void);
void draw_interaction(dungeon_t *d);
void pc_interaction(dungeon_t *d);
void clear_msg_row_interaction(void);
void new_dungeon(dungeon_t *d);
void show_monsters(dungeon_t *d);
void draw_monsters(int i, dungeon_t *d);

#endif
