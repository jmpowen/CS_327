#include <cstdlib>
#include <ncurses.h>
#include <cstring>

#include "dungeon.h"
#include "pc.h"
#include "utils.h"
#include "move.h"
#include "path.h"
#include "io.h"
#include "object.h"

void pc_pick_up_item(dungeon *d)
{
  object *o = d->objmap[d->PC->position[dim_y]][d->PC->position[dim_x]];
  object *temp = o->get_next();
  o->set_next(NULL);
  d->PC->inventory[d->PC->inventory_size] = o;
  d->PC->inventory_size++;
  d->objmap[d->PC->position[dim_y]][d->PC->position[dim_x]] = temp;
}

void pc_equip_item(dungeon *d, int select)
{
  object *o = d->PC->inventory[select];
  uint32_t o_type = o->get_type();
  int i;
  object *temp = NULL;
  if(o_type == objtype_WEAPON) {
    temp = d->PC->equipment[0];
    d->PC->equipment[0] = o;
  } else if (o_type == objtype_OFFHAND) {
    temp = d->PC->equipment[1];
    d->PC->equipment[1] = o;

  } else if (o_type == objtype_RANGED) {
    temp = d->PC->equipment[2];
    d->PC->equipment[2] = o;

  } else if (o_type == objtype_ARMOR) {
    temp = d->PC->equipment[3];
    d->PC->equipment[3] = o;

  } else if (o_type == objtype_HELMET) {
    temp = d->PC->equipment[4];
    d->PC->equipment[4] = o;

  } else if (o_type == objtype_CLOAK) {
    temp = d->PC->equipment[5];
    d->PC->equipment[5] = o;

  } else if (o_type == objtype_GLOVES) {
    temp = d->PC->equipment[6];
    d->PC->equipment[6] = o;

  } else if (o_type == objtype_BOOTS) {
    temp = d->PC->equipment[7];
    d->PC->equipment[7] = o;

  } else if (o_type == objtype_AMULET) {
    temp = d->PC->equipment[8];
    d->PC->equipment[8] = o;

  } else if (o_type == objtype_LIGHT) {
    temp = d->PC->equipment[9];
    d->PC->equipment[9] = o;

  } else if (o_type == objtype_RING) {
    if(d->PC->equipment[10] == NULL) {

      d->PC->equipment[10] = o;
      
    } else if(d->PC->equipment[11] == NULL) {
    
    d->PC->equipment[11] = o;

    } else {
    temp = d->PC->equipment[10];
    d->PC->equipment[10] = o;
      
    }
  }

  if(temp == NULL) {
    d->PC->inventory[select] = NULL;
    d->PC->equipment_size++;
    d->PC->inventory_size--;
    for(i = 0; i < MAX_INVENTORY-1;i++) {
      if(d->PC->inventory[i] == NULL) {
	temp = d->PC->inventory[i+1];
	d->PC->inventory[i] = temp;
	d->PC->inventory[i+1] = NULL;
      }
    }
  } else {
    d->PC->inventory[select] = temp;
  }
}

void pc_drop_item_inventory(dungeon *d, int select)
{
  object *o = d->PC->inventory[select];
  o->set_next(d->objmap[d->PC->position[dim_y]][d->PC->position[dim_x]]);
  d->objmap[d->PC->position[dim_y]][d->PC->position[dim_x]] = o;
  d->PC->inventory[select] = NULL;
  d->PC->inventory_size--;

  o = NULL;
  int i;
  for(i = 0; i < MAX_INVENTORY -1; i++) {
    if(d->PC->inventory[i] == NULL) {
      o = d->PC->inventory[i+1];
      d->PC->inventory[i] = o;
      d->PC->inventory[i+1] = NULL;
    }
  }
}

void pc_drop_item_equipment(dungeon *d, int select)
{
  object *o = d->PC->equipment[select];
  o->set_next(d->objmap[d->PC->position[dim_y]][d->PC->position[dim_x]]);
  d->objmap[d->PC->position[dim_y]][d->PC->position[dim_x]] = o;
  d->PC->equipment[select] = NULL;
  d->PC->equipment_size--;
}

void pc_unequip_item(dungeon *d, int select)
{
  object *o = d->PC->equipment[select];
  d->PC->equipment[select] = NULL;
  d->PC->equipment_size--;
  if(d->PC->inventory_size == MAX_INVENTORY) {
    o->set_next(d->objmap[d->PC->position[dim_y]][d->PC->position[dim_x]]);
    d->objmap[d->PC->position[dim_y]][d->PC->position[dim_x]] = o;
    //Item is dropped instead;
  } else {
    d->PC->inventory[d->PC->inventory_size] = o;
    d->PC->inventory_size++;
  }
}

void pc_expunge_item(dungeon *d)
{
  object *o = d->objmap[d->PC->position[dim_y]][d->PC->position[dim_x]];
  object *temp = o->get_next();
  o->set_next(NULL);
  delete o;
  d->objmap[d->PC->position[dim_y]][d->PC->position[dim_x]] = temp;
}

uint32_t pc_is_alive(dungeon *d)
{
  return d->PC->alive;
}

int pc_get_damage(dungeon *d)
{
  int total, i;
  for(i = 0; i < MAX_EQUIPMENT; i++) {
    if(d->PC->equipment[i] != NULL) {
      total += d->PC->equipment[i]->roll_dice();
    }
  }
  return total;
}

void place_pc(dungeon *d)
{
  d->PC->position[dim_y] = rand_range(d->rooms->position[dim_y],
                                     (d->rooms->position[dim_y] +
                                      d->rooms->size[dim_y] - 1));
  d->PC->position[dim_x] = rand_range(d->rooms->position[dim_x],
                                     (d->rooms->position[dim_x] +
                                      d->rooms->size[dim_x] - 1));

  pc_init_known_terrain(d->PC);
  pc_observe_terrain(d->PC, d);
}

void config_pc(dungeon *d)
{
  static dice pc_dice(0, 1, 4);

  d->PC = new pc;

  d->PC->symbol = '@';

  place_pc(d);

  d->PC->speed = PC_SPEED;
  d->PC->alive = 1;
  d->PC->sequence_number = 0;
  d->PC->kills[kill_direct] = d->PC->kills[kill_avenged] = 0;
  d->PC->color.push_back(COLOR_WHITE);
  d->PC->damage = new dice(0,1,4);
  d->PC->name = "Isabella Garcia-Shapiro";
  d->PC->hp = 400;
  

  d->PC->inventory_size = 0;
  d->PC->equipment_size = 0;

  d->character_map[character_get_y(d->PC)][character_get_x(d->PC)] = d->PC;

  dijkstra(d);
  dijkstra_tunnel(d);
}

uint32_t pc_next_pos(dungeon *d, pair_t dir)
{
  static uint32_t have_seen_corner = 0;
  static uint32_t count = 0;

  dir[dim_y] = dir[dim_x] = 0;

  if (in_corner(d, d->PC)) {
    if (!count) {
      count = 1;
    }
    have_seen_corner = 1;
  }

  /* First, eat anybody standing next to us. */
  if (charxy(d->PC->position[dim_x] - 1, d->PC->position[dim_y] - 1)) {
    dir[dim_y] = -1;
    dir[dim_x] = -1;
  } else if (charxy(d->PC->position[dim_x], d->PC->position[dim_y] - 1)) {
    dir[dim_y] = -1;
  } else if (charxy(d->PC->position[dim_x] + 1, d->PC->position[dim_y] - 1)) {
    dir[dim_y] = -1;
    dir[dim_x] = 1;
  } else if (charxy(d->PC->position[dim_x] - 1, d->PC->position[dim_y])) {
    dir[dim_x] = -1;
  } else if (charxy(d->PC->position[dim_x] + 1, d->PC->position[dim_y])) {
    dir[dim_x] = 1;
  } else if (charxy(d->PC->position[dim_x] - 1, d->PC->position[dim_y] + 1)) {
    dir[dim_y] = 1;
    dir[dim_x] = -1;
  } else if (charxy(d->PC->position[dim_x], d->PC->position[dim_y] + 1)) {
    dir[dim_y] = 1;
  } else if (charxy(d->PC->position[dim_x] + 1, d->PC->position[dim_y] + 1)) {
    dir[dim_y] = 1;
    dir[dim_x] = 1;
  } else if (!have_seen_corner || count < 250) {
    /* Head to a corner and let most of the NPCs kill each other off */
    if (count) {
      count++;
    }
    if (!against_wall(d, d->PC) && ((rand() & 0x111) == 0x111)) {
      dir[dim_x] = (rand() % 3) - 1;
      dir[dim_y] = (rand() % 3) - 1;
    } else {
      dir_nearest_wall(d, d->PC, dir);
    }
  }else {
    /* And after we've been there, let's head toward the center of the map. */
    if (!against_wall(d, d->PC) && ((rand() & 0x111) == 0x111)) {
      dir[dim_x] = (rand() % 3) - 1;
      dir[dim_y] = (rand() % 3) - 1;
    } else {
      dir[dim_x] = ((d->PC->position[dim_x] > DUNGEON_X / 2) ? -1 : 1);
      dir[dim_y] = ((d->PC->position[dim_y] > DUNGEON_Y / 2) ? -1 : 1);
    }
  }

  /* Don't move to an unoccupied location if that places us next to a monster */
  if (!charxy(d->PC->position[dim_x] + dir[dim_x],
              d->PC->position[dim_y] + dir[dim_y]) &&
      ((charxy(d->PC->position[dim_x] + dir[dim_x] - 1,
               d->PC->position[dim_y] + dir[dim_y] - 1) &&
        (charxy(d->PC->position[dim_x] + dir[dim_x] - 1,
                d->PC->position[dim_y] + dir[dim_y] - 1) != d->PC)) ||
       (charxy(d->PC->position[dim_x] + dir[dim_x] - 1,
               d->PC->position[dim_y] + dir[dim_y]) &&
        (charxy(d->PC->position[dim_x] + dir[dim_x] - 1,
                d->PC->position[dim_y] + dir[dim_y]) != d->PC)) ||
       (charxy(d->PC->position[dim_x] + dir[dim_x] - 1,
               d->PC->position[dim_y] + dir[dim_y] + 1) &&
        (charxy(d->PC->position[dim_x] + dir[dim_x] - 1,
                d->PC->position[dim_y] + dir[dim_y] + 1) != d->PC)) ||
       (charxy(d->PC->position[dim_x] + dir[dim_x],
               d->PC->position[dim_y] + dir[dim_y] - 1) &&
        (charxy(d->PC->position[dim_x] + dir[dim_x],
                d->PC->position[dim_y] + dir[dim_y] - 1) != d->PC)) ||
       (charxy(d->PC->position[dim_x] + dir[dim_x],
               d->PC->position[dim_y] + dir[dim_y] + 1) &&
        (charxy(d->PC->position[dim_x] + dir[dim_x],
                d->PC->position[dim_y] + dir[dim_y] + 1) != d->PC)) ||
       (charxy(d->PC->position[dim_x] + dir[dim_x] + 1,
               d->PC->position[dim_y] + dir[dim_y] - 1) &&
        (charxy(d->PC->position[dim_x] + dir[dim_x] + 1,
                d->PC->position[dim_y] + dir[dim_y] - 1) != d->PC)) ||
       (charxy(d->PC->position[dim_x] + dir[dim_x] + 1,
               d->PC->position[dim_y] + dir[dim_y]) &&
        (charxy(d->PC->position[dim_x] + dir[dim_x] + 1,
                d->PC->position[dim_y] + dir[dim_y]) != d->PC)) ||
       (charxy(d->PC->position[dim_x] + dir[dim_x] + 1,
               d->PC->position[dim_y] + dir[dim_y] + 1) &&
        (charxy(d->PC->position[dim_x] + dir[dim_x] + 1,
                d->PC->position[dim_y] + dir[dim_y] + 1) != d->PC)))) {
    dir[dim_x] = dir[dim_y] = 0;
  }

  return 0;
}

uint32_t pc_in_room(dungeon *d, uint32_t room)
{
  if ((room < d->num_rooms)                                     &&
      (d->PC->position[dim_x] >= d->rooms[room].position[dim_x]) &&
      (d->PC->position[dim_x] < (d->rooms[room].position[dim_x] +
                                d->rooms[room].size[dim_x]))    &&
      (d->PC->position[dim_y] >= d->rooms[room].position[dim_y]) &&
      (d->PC->position[dim_y] < (d->rooms[room].position[dim_y] +
                                d->rooms[room].size[dim_y]))) {
    return 1;
  }

  return 0;
}

void pc_learn_terrain(pc *p, pair_t pos, terrain_type ter)
{
  p->known_terrain[pos[dim_y]][pos[dim_x]] = ter;
  p->visible[pos[dim_y]][pos[dim_x]] = 1;
}

void pc_reset_visibility(pc *p)
{
  uint32_t y, x;

  for (y = 0; y < DUNGEON_Y; y++) {
    for (x = 0; x < DUNGEON_X; x++) {
      p->visible[y][x] = 0;
    }
  }
}

terrain_type pc_learned_terrain(pc *p, int16_t y, int16_t x)
{
  if (y < 0 || y >= DUNGEON_Y || x < 0 || x >= DUNGEON_X) {
    io_queue_message("Invalid value to %s: %d, %d", __FUNCTION__, y, x);
  }

  return p->known_terrain[y][x];
}

void pc_init_known_terrain(pc *p)
{
  uint32_t y, x;

  for (y = 0; y < DUNGEON_Y; y++) {
    for (x = 0; x < DUNGEON_X; x++) {
      p->known_terrain[y][x] = ter_unknown;
      p->visible[y][x] = 0;
    }
  }
}

void pc_observe_terrain(pc *p, dungeon *d)
{
  pair_t where;
  int16_t y_min, y_max, x_min, x_max;

  y_min = p->position[dim_y] - PC_VISUAL_RANGE;
  if (y_min < 0) {
    y_min = 0;
  }
  y_max = p->position[dim_y] + PC_VISUAL_RANGE;
  if (y_max > DUNGEON_Y - 1) {
    y_max = DUNGEON_Y - 1;
  }
  x_min = p->position[dim_x] - PC_VISUAL_RANGE;
  if (x_min < 0) {
    x_min = 0;
  }
  x_max = p->position[dim_x] + PC_VISUAL_RANGE;
  if (x_max > DUNGEON_X - 1) {
    x_max = DUNGEON_X - 1;
  }

  for (where[dim_y] = y_min; where[dim_y] <= y_max; where[dim_y]++) {
    where[dim_x] = x_min;
    can_see(d, p->position, where, 1, 1);
    where[dim_x] = x_max;
    can_see(d, p->position, where, 1, 1);
  }
  /* Take one off the x range because we alreay hit the corners above. */
  for (where[dim_x] = x_min - 1; where[dim_x] <= x_max - 1; where[dim_x]++) {
    where[dim_y] = y_min;
    can_see(d, p->position, where, 1, 1);
    where[dim_y] = y_max;
    can_see(d, p->position, where, 1, 1);
  }       
}

int32_t is_illuminated(pc *p, int16_t y, int16_t x)
{
  return p->visible[y][x];
}

void pc_see_object(character *the_pc, object *o)
{
  if (o) {
    o->has_been_seen();
  }
}
