#ifndef IO_H
# define IO_H

class dungeon;

void io_init_terminal(void);
void io_reset_terminal(void);
void io_display(dungeon *d);
void io_handle_input(dungeon *d);
void io_queue_message(const char *format, ...);

void io_display_clear(dungeon *d);
void io_handle_inventory(dungeon *d);
void io_display_inventory(dungeon *d, int select);
void io_display_item_info(dungeon *d, int select);
void io_clear_msg_line();

void io_handle_equipment(dungeon *d);
void io_display_equipment(dungeon *d, int select);

void io_handle_monster_lookup(dungeon *d);

#endif
