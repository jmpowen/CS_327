#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <stdint.h>
#include <endian.h>

void makeDungeon();
void printDungeon();
void load();
void save();

char grid[21][80];
int hardness[21][80];
int coordinates[200][4];
int timeStamp;
int numRooms;
FILE *f;

/* char file_type[13]; */
/* uint32_t file_version_marker; */
/* uint32_t file_size; */

struct Jimbob {
  int xpos;
  int ypos;
  int health;
};

int main(int argc, char* argv[])
{
  if(argc > 1) {
    int i = 1;
    if(strcmp(argv[1], "--save") == 0) {
      makeDungeon();
    }
    while(i < argc) {
      if(strcmp(argv[i], "--load") == 0) {
	load();
      } else if(strcmp(argv[i], "--save") == 0) {
	save();
      } else {
	printf("Bad arguments.\n");
	return -1;
      }
      i++;
    }
    return 0;
  }

  makeDungeon();
  return 0;
}

void makeDungeon()
{ 
  timeStamp = time(NULL);
  srand(timeStamp);
  numRooms = rand() % 5 + 5;

  for(int j = 0; j < 80; j++) {
    for(int k = 0; k < 21; k++) {
      grid[k][j] = ' ';
      hardness[k][j] = rand() % 253 + 1;
    }
  }

  for(int j = 0; j < 80; j++) {
    grid[0][j] = '_';
    grid[20][j] = '_';
    hardness[0][j] = 255;
    hardness[20][j] = 255;
  }
  
  for(int k = 0; k < 21; k++) {
    grid[k][0] = '|';
    grid[k][79] = '|';
    hardness[k][0] = 255;
    hardness[k][79] = 255;
  }
  
  int i = 0;
  while(i < numRooms) {
    int x = rand() % 10 + 3;
    int y = rand() % 10 + 2;
    
    int randX = rand() % 80;
    int randY = rand() % 21;

    bool breakOut = false;
    for(int j = -1; j < y+1; j++) {
      for(int k = -1; k < x+1; k++) {
	if(grid[randY+j][randX+k] == '.' || grid[randY+j][randX+k] == '|' || grid[randY+j][randX+k] == '_') {
	  breakOut = true;
	}
      }
    }
    
    if(breakOut) {
      continue;
    }

    coordinates[i][0] = randX;
    coordinates[i][1] = randY;
    coordinates[i][2] = x;
    coordinates[i][3] = y;

    for(int j = 0; j < y; j++) {
      for(int k = 0; k < x; k++) {
	grid[randY+j][randX+k] = '.';
	hardness[randY+j][randX+k] = 0;
      }
    }
    i++;
  }

  
  //struct Jimbob player1;

  //player1.xpos = coordinates[0][0];
  //player1.ypos = coordinates[0][1];
  //player1.health = 100;
  //grid[coordinates[0][1]][coordinates[0][0]] = '@';

  i = 0;
  while(i < numRooms-1) {
    int startX, startY, endX, endY;
    if(coordinates[i][0] < coordinates[i+1][0] && coordinates[i][1] < coordinates[i+1][1]) {
      startX = coordinates[i][0];
      endX = coordinates[i+1][0];
      startY = coordinates[i][1];
      endY = coordinates[i+1][1];

      for(int j = 0; j <= abs(startY-endY); j++) {
	if(grid[startY+j][startX] == ' ') {
	  grid[startY+j][startX] = '#';
	  hardness[startY+j][startX] = 0;
	}
      }
   
      for(int k = 0; k <= abs(startX-endX); k++) {
	if(grid[endY][startX+k] == ' ') {
	  grid[endY][startX+k] = '#';
	  hardness[endY][startX+k] = 0;
	}
      }
    } else if(coordinates[i][0] < coordinates[i+1][0]){
      startX = coordinates[i][0];
      endX = coordinates[i+1][0];
      startY = coordinates[i+1][1];
      endY = coordinates[i][1];

      for(int j = 0; j <= abs(startY-endY); j++) {
	if(grid[startY+j][startX] == ' ') {
	  grid[startY+j][startX] = '#';
	  hardness[startY+j][startX] = 0;
	}
      }
   
      for(int k = 0; k <= abs(startX-endX); k++) {
	if(grid[startY][startX+k] == ' ') {
	  grid[startY][startX+k] = '#';
	  hardness[startY][startX+k] = 0;
	}
      }
    } else if(coordinates[i][1] < coordinates[i+1][1]) {
      startX = coordinates[i+1][0];
      endX = coordinates[i][0];
      startY = coordinates[i][1];
      endY = coordinates[i+1][1];

      for(int j = 0; j <= abs(startY-endY); j++) {
	if(grid[startY+j][startX] == ' ') {
	  grid[startY+j][startX] = '#';
	  hardness[startY+j][startX] = 0;
	}
      }
   
      for(int k = 0; k <= abs(startX-endX); k++) {
	if(grid[startY][startX+k] == ' ') {
	  grid[startY][startX+k] = '#';
	  hardness[startY][startX+k] = 0;
	}
      }
    } else {
      startX = coordinates[i+1][0];
      endX = coordinates[i][0];
      startY = coordinates[i+1][1];
      endY = coordinates[i][1];

      for(int j = 0; j <= abs(startY-endY); j++) {
	if(grid[startY+j][startX] == ' ') {
	  grid[startY+j][startX] = '#';
	  hardness[startY+j][startX] = 0;
	}
      }
   
      for(int k = 0; k <= abs(startX-endX); k++) {
	if(grid[endY][startX+k] == ' ') {
	  grid[endY][startX+k] = '#';
	  hardness[endY][startX+k] = 0;
	}
      }
    }

    i++;
  }
  
  printDungeon();
}

void printDungeon() {
  for(int i = 0; i < 21; i++) {
   for(int j = 0; j < 80; j++) {
    printf("%c", grid[i][j]);
   }
  }
}


void load() {
  char* path;
  path = malloc(strlen(getenv("HOME")) + strlen("/.rlg327/dungeon") + 1);
  strcpy(path, getenv("HOME"));
  strcat(path, "/.rlg327/dungeon");
  
  f = fopen(path, "r");
  free(path);

  /* char *test_path; */
  /* test_path = malloc(strlen(getenv("HOME")) + strlen("/Downloads/test_dungeon_files/102.rlg327") + 1); */
  /* strcpy(test_path, getenv("HOME")); */
  /* strcat(test_path, "/Downloads/test_dungeon_files/102.rlg327"); */
  /* f = fopen(test_path, "r"); */
  /* free(test_path); */
  if(f == NULL) {
    makeDungeon(); //in Piazza schaeffer said that it does nothing but the sheet with the specs
    //says that it creats a new dungeon if there is nothing to load from. This is what I chose
    //to do.
  }

  char file_type[13];
  fread(&file_type, 1, 12, f);

  uint32_t file_version_marker;
  fread(&file_version_marker, 4, 1, f);

  uint32_t file_size;
  fread(&file_size, 4, 1, f);
  file_size = be32toh(file_size);

  uint8_t PC_x;
  uint8_t PC_y;
  fread(&PC_x, 1, 1, f);
  fread(&PC_y, 1, 1, f);

  unsigned char grid_hardness[21][80];
  fread(&grid_hardness, 1, 1680, f);
  for(int j = 0; j < 80; j++) {
    for(int k = 0; k < 21; k++) {
      hardness[k][j] = (int) grid_hardness[k][j];
    }
  }
  
  for(int j = 0; j < 80; j++) {
    for(int k = 0; k < 21; k++) {
      if(hardness[k][j] == 255) {
	if(k == 0 || (k == 20 && !(j == 0 || j == 79))) {
	  grid[k][j] = '_';
	} else {
	  grid[k][j] = '|';
	}
      } else if(hardness[k][j] == 0) {
	grid[k][j] = '#';
      } else {
	grid[k][j] = ' ';
      }
    }
  }
  int x = (int) PC_x;
  int y = (int) PC_y;
  printf("%d %d\n", x, y);
  grid[y][x] = '@';

  int size_left = ((int) file_size)-1702;
  size_left = size_left/4;
  uint8_t rooms[size_left][4];
  fread(&rooms, 1, size_left*4, f);

  for(int j = 0; j < size_left; j++) {
    for(int k = 0; k < 4; k++) {
      coordinates[j][k] = (int) rooms[j][k];
    }
  }

  for(int i = 0; i < size_left; i++) {
    for(int j = 0; j < coordinates[i][2]; j++) {
      for(int k = 0; k < coordinates[i][3]; k++) {
	grid[coordinates[i][1]+k][coordinates[i][0]+j] = '.';
      }
    }
  }

  printDungeon();
  
  fclose(f);
}

void save() {
  char *path;
  path = malloc(strlen(getenv("HOME")) + strlen("/.rlg327/dungeon") + 1);
  strcpy(path, getenv("HOME"));
  strcat(path, "/.rlg327/dungeon");
  f = fopen(path, "w");
  free(path);

  /* char *test_path; */
  /* test_path = malloc(strlen(getenv("HOME")) + strlen("/Downloads/test_dungeon_files/106.rlg327") + 1); */
  /* strcpy(test_path, getenv("HOME")); */
  /* strcat(test_path, "/Downloads/test_dungeon_files/106.rlg327"); */
  /* f = fopen(test_path, "w"); */
  /* free(test_path); */

  char file_type[] = "RLG327-F2018";
  fwrite(&file_type, 1, 12, f);

  uint32_t file_version_mark = 0;
  fwrite(&file_version_mark, 4, 1, f);

  uint32_t file_size = 1702 + 4*numRooms;
  file_size = htobe32(file_size);
  fwrite(&file_size, 4, 1, f);

  uint8_t PC_x = coordinates[0][0];
  uint8_t PC_y = coordinates[0][1];
  fwrite(&PC_x, 1, sizeof(PC_x), f);
  fwrite(&PC_y, 1, sizeof(PC_y), f);

  unsigned char grid_hardness[21][80];
  
  for(int j = 0; j < 80; j++) {
    for(int k = 0; k < 21; k++) {
      grid_hardness[k][j] = (char) hardness[k][j];
    }
  }
  fwrite(&grid_hardness, 1, 1680, f);

  uint8_t rooms[numRooms][4];
  for(int j = 0; j < numRooms; j++) {
    for(int k = 0; k < 4; k++) {
      rooms[j][k] = (char) coordinates[j][k];
    }
  }
  
  fwrite(&rooms, 1, 4*numRooms, f);
  
  fclose(f); 
}
