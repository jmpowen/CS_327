#ifndef ITEM_H
#define ITEM_H

#include <stdint.h>
#include <vector>

#include "dims.h"
#include "character.h"
#include "dice.h"
#include "descriptions.h"
#include "dungeon.h"
#include "utils.h"
#include "npc.h"

using namespace std;

class item {
 public:
  string name;
  string description;
  object_type_t type;
  uint32_t color;
  int32_t hit;
  dice damage;
  int32_t dodge;
  int32_t defence;
  int32_t weight;
  int32_t speed;
  int32_t attribute;
  int32_t value;
  pair_t position;
};

void generate_items(dungeon *d);
char item_get_symbol(const item *i);
int16_t *item_get_pos(item *i);

#endif
