#ifndef PC_H
# define PC_H

# include <stdint.h>

# include "dims.h"
# include "character.h"
# include "dungeon.h"

#define MAX_INVENTORY 10
#define MAX_EQUIPMENT 12

class pc : public character {
 public:
  ~pc() {}
  terrain_type known_terrain[DUNGEON_Y][DUNGEON_X];
  uint8_t visible[DUNGEON_Y][DUNGEON_X];

  object *inventory[10] = {0};
  object *equipment[12] = {0};

  uint32_t inventory_size;
  uint32_t equipment_size;


};

void pc_delete(pc *pc);
uint32_t pc_is_alive(dungeon *d);
void config_pc(dungeon *d);
uint32_t pc_next_pos(dungeon *d, pair_t dir);
void place_pc(dungeon *d);
uint32_t pc_in_room(dungeon *d, uint32_t room);
void pc_learn_terrain(pc *p, pair_t pos, terrain_type ter);
terrain_type pc_learned_terrain(pc *p, int16_t y, int16_t x);
void pc_init_known_terrain(pc *p);
void pc_observe_terrain(pc *p, dungeon *d);
int32_t is_illuminated(pc *p, int16_t y, int16_t x);
void pc_reset_visibility(pc *p);

void pc_pick_up_item(dungeon *d);
void pc_equip_item(dungeon *d, int select);
void pc_drop_item_inventory(dungeon *d, int select);
void pc_drop_item_equipment(dungeon *d, int select);
void pc_unequip_item(dungeon *d, int select);
void pc_expunge_item(dungeon *d);
int pc_get_damage(dungeon *d);

#endif
