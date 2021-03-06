#ifndef DUNGEON_H
# define DUNGEON_H

# include "heap.h"
# include "macros.h"
# include "dims.h"

#define DUNGEON_X              80
#define DUNGEON_Y              21
#define MIN_ROOMS              5
#define MAX_ROOMS              9
#define ROOM_MIN_X             4
#define ROOM_MIN_Y             2
#define ROOM_MAX_X             14
#define ROOM_MAX_Y             8
#define SAVE_DIR               ".rlg327"
#define DUNGEON_SAVE_FILE      "dungeon"
#define DUNGEON_SAVE_SEMANTIC  "RLG327-F2018"
#define DUNGEON_SAVE_VERSION   0U

#define mappair(pair) (d->map[pair[dim_y]][pair[dim_x]])
#define mapxy(x, y) (d->map[y][x])
#define hardnesspair(pair) (d->hardness[pair[dim_y]][pair[dim_x]])
#define hardnessxy(x, y) (d->hardness[y][x])

/* This macro was learned from William Whyte
   on Stack Overflow: https://stackoverflow.com/questions/111928/is-there-a
   -printf-converter-to-print-in-binary-format and used to create the 
   following macros*/
#define BYTE_TO_BINARY(byte) \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0')

#define CHECK_ERR(byte) \
  (byte & 0x08 ? '1' : '0')

#define CHECK_TUN(byte) \
  (byte & 0x04 ? '1' : '0')

#define CHECK_TEL(byte) \
  (byte & 0x02 ? '1' : '0')

#define CHECK_INT(byte) \
  (byte & 0x01 ? '1' : '0')

#define MONSTER_NUM(name) \
  (strcmp(name,"f") == 0 ? 15 : strcmp(name,"e") == 0 ? 14 : \
   strcmp(name,"d") == 0 ? 13 : strcmp(name,"c") == 0 ? 12 : \
   strcmp(name,"b") == 0 ? 11 : strcmp(name,"a") == 0 ? 10 : atoi(name))

typedef enum __attribute__ ((__packed__)) terrain_type {
  ter_debug,
  ter_wall,
  ter_wall_immutable,
  ter_floor,
  ter_floor_room,
  ter_floor_hall,
} terrain_type_t;

typedef struct room {
  pair_t position;
  pair_t size;
} room_t;

typedef struct character {
  uint32_t priority;
  pair_t position;
  uint8_t speed;
  uint8_t stats;
  char name[2];
  uint8_t is_alive;
  int32_t turn;
  pair_t last_known_pos;
} character_t;

typedef struct dungeon {
  uint32_t num_rooms;
  room_t *rooms;
  terrain_type_t map[DUNGEON_Y][DUNGEON_X];
  /* Since hardness is usually not used, it would be expensive to pull it *
   * into cache every time we need a map cell, so we store it in a        *
   * parallel array, rather than using a structure to represent the       *
   * cells.  We may want a cell structure later, but from a performanace  *
   * perspective, it would be a bad idea to ever have the map be part of  *
   * that structure.  Pathfinding will require efficient use of the map,  *
   * and pulling in unnecessary data with each map cell would add a lot   *
   * of overhead to the memory system.                                    */
  uint8_t hardness[DUNGEON_Y][DUNGEON_X];
  uint8_t pc_distance[DUNGEON_Y][DUNGEON_X];
  uint8_t pc_tunnel[DUNGEON_Y][DUNGEON_X];
  character_t *characters;
  uint32_t num_monsters;
} dungeon_t;

void init_dungeon(dungeon_t *d);
void delete_dungeon(dungeon_t *d);
int gen_dungeon(dungeon_t *d);
void render_dungeon(dungeon_t *d);
int write_dungeon(dungeon_t *d, char *file);
int read_dungeon(dungeon_t *d, char *file);
int read_pgm(dungeon_t *d, char *pgm);
void render_distance_map(dungeon_t *d);
void render_tunnel_distance_map(dungeon_t *d);
void render_hardness_map(dungeon_t *d);
void render_movement_cost_map(dungeon_t *d);
void setup_pc(dungeon_t *d);

void play_dungeon(dungeon_t *d);
void play(dungeon_t *d, character_t *c);
void move_random(dungeon_t *d, character_t *c);
void move_int_tel(dungeon_t *d, character_t *c);
void move_int(dungeon_t *d, character_t *c);
void move_to_pc(dungeon_t *d, character_t *c);
void is_PC(dungeon_t *d);
int within_vision(dungeon_t *d, character_t *c);
void check_if_kills(dungeon_t *d, character_t *c, pair_t p);
void check_int_tel_tun_err(dungeon_t *d);

#endif
