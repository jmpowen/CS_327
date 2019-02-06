#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>

void makeDungeon();
void printDungeon();

char grid[21][80];
int timeStamp;
int numRooms;

int main(int argc, char* argv[])
{
  makeDungeon();
  return 0;
}

void makeDungeon()
{ 
  for(int j = 0; j < 80; j++) {
    for(int k = 0; k < 21; k++) {
      grid[k][j] = ' ';
    }
  }

  timeStamp = time(NULL);
  srand(timeStamp);
  numRooms = rand() % 5 + 5;

  int coordinates[numRooms][2];

  int i = 0;
  while(i < numRooms) {
    int x = rand() % 10 + 3;
    int y = rand() % 10 + 2;
    
    int randX = rand() % 80;
    int randY = rand() % 21;

    bool breakOut = false;
    for(int j = -1; j < y+1; j++) {
      for(int k = -1; k < x+1; k++) {
	if(grid[randY+j][randX+k] == '.') {
	  breakOut = true;
	}
      }
    }
    
    if(breakOut || (randX + x) > 80 || (randY + y) > 21) {
      continue;
    }

    coordinates[i][0] = randX;
    coordinates[i][1] = randY;

    for(int j = 0; j < y; j++) {
      for(int k = 0; k < x; k++) {
	grid[randY+j][randX+k] = '.';
      }
    }
    i++;
  }

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
	}
      }
   
      for(int k = 0; k <= abs(startX-endX); k++) {
	if(grid[endY][startX+k] == ' ') {
	  grid[endY][startX+k] = '#';
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
	}
      }
   
      for(int k = 0; k <= abs(startX-endX); k++) {
	if(grid[startY][startX+k] == ' ') {
	  grid[startY][startX+k] = '#';
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
	}
      }
   
      for(int k = 0; k <= abs(startX-endX); k++) {
	if(grid[startY][startX+k] == ' ') {
	  grid[startY][startX+k] = '#';
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
	}
      }
   
      for(int k = 0; k <= abs(startX-endX); k++) {
	if(grid[endY][startX+k] == ' ') {
	  grid[endY][startX+k] = '#';
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
  printf("\n\n%d  %d\n", numRooms, timeStamp);
}
