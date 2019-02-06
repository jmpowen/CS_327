#include <ncurses.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>

#include "dungeon.h"
#include "interaction.h"

void init_interaction(void)
{
  initscr();
  raw();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
}

void draw_interaction(dungeon_t *d)
{
  clear();
  int x, y;

  for(y = 1; y < DUNGEON_Y; y++) {
    for(x = 0; x < DUNGEON_X; x++) {
      if(charxy(x,y)) {
	mvaddch(y,x,charxy(x,y)->symbol);
      } else if(x == d->stairs[0].position[dim_x]
		&& y == d->stairs[0].position[dim_y]) {
	mvaddch(y,x,d->stairs[0].symbol);
      } else if(x == d->stairs[1].position[dim_x]
		&& y == d->stairs[1].position[dim_y]) {
	mvaddch(y,x,d->stairs[1].symbol);
      } else {
	switch (mapxy(x,y)) {
	case ter_wall:
	case ter_wall_immutable:
	  mvaddch(y,x,' ');
	  break;
        case ter_floor:
	case ter_floor_room:
	  mvaddch(y,x,'.');
	  break;
	case ter_floor_hall:
	  mvaddch(y,x,'#');
	  break;
	case ter_debug:
	  mvaddch(y,x,'*');
	  fprintf(stderr, "Debug character at %d, %d\n", x, y);
	  break;
	}
	refresh();
      }
    }
  }
}


static int32_t check_move(dungeon_t *d, int x, int y)
{
  if(mapxy(d->pc.position[dim_x]+x,d->pc.position[dim_y]+y) == ter_floor_room
     || mapxy(d->pc.position[dim_x]+x,d->pc.position[dim_y]+y) == ter_floor_hall) {
    return 1;
  } else {
    mvprintw(0,0,"There is a wall");
    return 0;
  }
}

void pc_interaction(dungeon_t *d)
{
  int move_char, x, y, valid, not_drawn;
  
  valid = not_drawn = 0;
  while(!valid) {
    x = y = 0;
    move_char = getch();
    clear_msg_row_interaction();
    switch(move_char) {
    case '7':
    case 'y':
      x--;
      y--;
      valid = check_move(d,x,y);
      break;
    case '8':
    case 'k':
      y--;
      valid = check_move(d,x,y);
      break;
    case '9':
    case 'u':
      x++;
      y--;
      valid = check_move(d,x,y);
      break;
    case '6':
    case 'l':
      x++;
      valid = check_move(d,x,y);
      break;
    case '3':
    case 'n':
      x++;
      y++;
      valid = check_move(d,x,y);
      break;
    case '2':
    case 'j':
      y++;
      valid = check_move(d,x,y);
      break;
    case '1':
    case 'b':
      x--;
      y++;
      valid = check_move(d,x,y);
      break;
    case '4':
    case 'h':
      x--;
      valid = check_move(d,x,y);
      break;
    case '>':
      if(d->pc.position[dim_x] == d->stairs[0].position[dim_x]
	 && d->pc.position[dim_y] == d->stairs[0].position[dim_y]) {
	valid = check_move(d,x,y);
	new_dungeon(d);
	draw_interaction(d);
	mvprintw(0,0,"Going down stairs.");
	not_drawn = 1;
      } else {
	mvprintw(0,0,"Can't go down. Not on the stairs.");
      }
      break;
    case '<':
      if(d->pc.position[dim_x] == d->stairs[1].position[dim_x]
	 && d->pc.position[dim_y] == d->stairs[1].position[dim_y]) {
	valid = check_move(d,x,y);
	new_dungeon(d);
	draw_interaction(d);
	mvprintw(0,0,"Going up stairs.");
	not_drawn = 1;
      } else {
	mvprintw(0,0,"Can't go up. Not on the stairs.");
      }
      break;
    case '5':
    case ' ':
    case '.':
      //rest;
      valid = check_move(d,x,y);
      break;
    case 'm':
      show_monsters(d);
      draw_interaction(d);
      break;
    case 'q':
      d->pc.alive = 0;
      valid = 1;
      break;
    default:
      mvprintw(0,0,"%c is not a valid move.", move_char);
    }
  }
  d->character[d->pc.position[dim_y]][d->pc.position[dim_x]] = NULL;
  d->pc.position[dim_x] += x;
  d->pc.position[dim_y] += y;
  if(d->character[d->pc.position[dim_y]][d->pc.position[dim_x]] != NULL) {
    d->character[d->pc.position[dim_y]][d->pc.position[dim_x]]->alive = 0;
    d->character[d->pc.position[dim_y]][d->pc.position[dim_x]] = NULL;
    d->num_monsters--;
    d->pc.kills[kill_direct]++;
  }
  d->character[d->pc.position[dim_y]][d->pc.position[dim_x]] = &d->pc;
  if(!(not_drawn)) {
    draw_interaction(d);
  }
}

void clear_msg_row_interaction(void)
{
  move(0,0);
  clrtoeol();
  refresh();
}

void show_monsters(dungeon_t *d)
{
  int i, leave_list, move_char;
  i = 0;
  draw_monsters(i,d);
  leave_list = 0;
  while(!leave_list) {
    move_char = getch();
    switch(move_char) {
    case 27:
      leave_list = 1;
      break;
    case 8:
    case 'k':
    case KEY_UP:
      if(d->num_monsters > 10 && i >= 1) {
	i--;
	draw_monsters(i,d);
	clear_msg_row_interaction();
      } else {
	clear_msg_row_interaction();
	mvprintw(0,0,"This is the top of the monster list.");
      }
      break;
    case 2:
    case 'j':
    case KEY_DOWN:
      if(d->num_monsters > 10 && (i+10) < d->num_monsters) {
	i++;
	draw_monsters(i,d);
	clear_msg_row_interaction();
      } else {
	clear_msg_row_interaction();
	mvprintw(0,0,"This is the bottom of the monster list.");
      }
      break;
    default:
      mvprintw(0,0,"%d is not a valid move.", move_char);
    }
  }
}

void draw_monsters(int i, dungeon_t *d)
{
  int j,k,x,y,n;
  if(d->num_monsters > 10) {
    n = 10;
  } else {
    n = d->num_monsters;
  }
  drawn_monsters_t dm[n];
  k = j = 0;
  for(y = 0; y < DUNGEON_Y; y++) {
    for(x = 0; x < DUNGEON_X; x++) {
      if(charxy(x,y) && charxy(x,y)->symbol != '@' && j++ >= i && k < n) {
	dm[k].x = x;
	dm[k].y = y;
	dm[k].symbol = charxy(x,y)->symbol;
	k++;
      }
    }
  }

  char c[n][25];
  int the_x, the_y;
  char buffer[10];
  for(j = 0; j < n; j++) {
    sprintf(c[j], "%c, ", dm[j].symbol);
    if(dm[j].x != d->pc.position[dim_x]) {
      the_x = dm[j].x - d->pc.position[dim_x];
      if(the_x < 0) {
	the_x *= -1;
	sprintf(buffer, "%d west ", the_x);
	strcat(c[j], buffer);
      } else {
	sprintf(buffer, "%d east ", the_x);
	strcat(c[j], buffer);
      }
    }
    if(dm[j].y != d->pc.position[dim_y] && dm[j].x != d->pc.position[dim_y]) {
      strcat(c[j], "and ");
    }
    if(dm[j].y != d->pc.position[dim_y]) {
      the_y = dm[j].y - d->pc.position[dim_y];
      if(the_y < 0) {
	the_y *= -1;
	sprintf(buffer, "%d north ", the_y);
	strcat(c[j], buffer);
      } else {
	sprintf(buffer, "%d south ", the_y);
	strcat(c[j], buffer);
      }
    }
  }


  
  for(j = 0; j < n; j++) {
    mvprintw(j+1,0,"%s", c[j]);
  }
}

void new_dungeon(dungeon_t *d)
{
  int kd = d->pc.kills[kill_direct];
  int ka = d->pc.kills[kill_avenged];
  delete_dungeon(d);
  init_dungeon(d);
  gen_dungeon(d);
  config_pc(d);
  d->num_monsters = MAX_MONSTERS;
  gen_monsters(d);
  d->pc.kills[kill_direct] = kd;
  d->pc.kills[kill_avenged] = ka;
  draw_interaction(d);
}
