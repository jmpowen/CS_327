#include <stdlib.h>
#include <cstring>
#include "dungeon.h"
#include "item.h"

void generate_items(dungeon *d)
{
  uint32_t i;
  item *m;
  uint32_t room;
  pair_t p;
  uint32_t the_item;

  for(i = 0; i < d->num_monsters; i++) {
    m = new item;
    the_item = rand_range(0, (d->object_descriptions).size()-1);
    memset(m, 0, sizeof(*m));

    do {
      room = rand_range(1, d->num_rooms - 1);
      p[dim_y] = rand_range(d->rooms[room].position[dim_y],
                            (d->rooms[room].position[dim_y] +
                             d->rooms[room].size[dim_y] - 1));
      p[dim_x] = rand_range(d->rooms[room].position[dim_x],
                            (d->rooms[room].position[dim_x] +
                             d->rooms[room].size[dim_x] - 1));
    } while (d->item_map[p[dim_y]][p[dim_x]]);
    m->position[dim_y] = p[dim_y];
    m->position[dim_x] = p[dim_x];
    d->item_map[p[dim_y]][p[dim_x]] = m;

    m->name = d->object_descriptions[the_item].get_name();
    m->description = d->object_descriptions[the_item].get_description();
    m->type = d->object_descriptions[the_item].get_type();
    m->color = d->object_descriptions[the_item].get_color();
    m->hit = d->object_descriptions[the_item].get_hit().roll();
    m->damage = d->object_descriptions[the_item].get_damage();
    m->dodge = d->object_descriptions[the_item].get_dodge().roll();
    m->defence = d->object_descriptions[the_item].get_defence().roll();
    m->weight = d->object_descriptions[the_item].get_weight().roll();
    m->speed = d->object_descriptions[the_item].get_speed().roll();
    m->attribute = d->object_descriptions[the_item].get_attribute().roll();
    m->value = d->object_descriptions[the_item].get_value().roll();
  }
  
}

char item_get_symbol(const item *i)
{
  return object_symbol[i->type];
}

int16_t *item_get_pos(item *i)
{
  return i->position;
}

uint32_t item_get_color(const item *i)
{
  return i->color;
}
