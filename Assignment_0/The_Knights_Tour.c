#include <stdio.h>
#include <stdbool.h>

int knightsTour(int, int, int);
void printAns();
int  nextMove(int, int, _Bool);

int grid[5][5] = {0};
int count = 0;

//This calls knightsTour from all starting coordinates.
int main(int argc, char* argv[])
{
  for(int i = 0; i < 5; i++)
    {
      for(int j = 0; j < 5; j++)
	{
	  knightsTour(i, j, 0);
	}
    }
  printf("Number of outputs: %d", count);
  return 0;
}

//The function knightsTour recursively solves the tours by leaving each grid numbered then solving
//with the next move. It prints the answer once the base case is met and returns a 1.
int knightsTour(int x, int y, int number)
{
  int move = 0;

  if(number == 25)
    {
      return 1;
    }

  if(grid[x][y] == 0)
    {
      while(move < 8)
	{
	  If(nextMove(move, x, 1) !=  -1 && nextMove(move, y, 0) != -1)
	    {
	      grid[x][y] = number + 1;
	      if(knightsTour(nextMove(move, x, 1), nextMove(move, y, 0), number + 1))
		{
		  printAns();
		  
		  move = 8;
		}
	    }
	  move++;
	}
      grid[x][y] = 0;
    }

  return 0;
  
}
 
//The parameter move is the integer passed to choose which move will be given to xy.
//The parameter xy is an int that is either x or y. 
//The parameter isX just tells the function if it is X being passed as xy.
int nextMove(int move, int  xy, _Bool isX)
{
  if(isX) {
      if(move == 0) {
	  xy += 2;
	} else if(move == 1) {
	  xy += 2;
	} else if(move == 2) {
	  xy += 1;
	} else if(move == 3) {
	  xy += -1;
	} else if(move == 4) {
	  xy += -2;
	} else if(move == 5) {
	  xy += -2;
	} else if(move == 6) {
	  xy += 1;
        } else {
	  xy += -1;
        }
  } else {
      if(move == 0) {
	  xy += 1;
	} else if(move == 1) {
	  xy += -1;
	} else if(move == 2) {
	  xy += 2;
	} else if(move == 3) {
	  xy += 2;
	} else if(move == 4) {
	  xy += 1;
	} else if(move == 5) {
	  xy += -1;
	} else if(move == 6) {
	  xy += -2;
        } else {
	  xy += -2;
        }
    }


   if(xy < 0 || xy > 4)
     {
       return -1;
     }
   else
     {
       return xy;
     }
}

//This prints the ans with some light formatting to make it look nice.
void printAns()
{
  count++;
  for(int i = 0; i < 5; i++)
    {
      for(int j = 0; j < 5; j++)
	{
	  printf("%4d", grid[j][i]);
	}
      printf("\n");
    }
  printf("\n\n");
}
