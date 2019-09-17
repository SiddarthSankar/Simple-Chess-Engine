#include<stdio.h>
#include "defs.h"
#include <stdlib.h>


int main(){
	//int index = 0;
	InitAll();
    S_Board board[1];
	char input[6];
	int Move = NOMOVE;
	printf("\t\t\tINSTRUCTIONS\n");
	printf("----------------------------------------------------------------\n");
	printf("----------------------------------------------------------------\n");
	printf("Consider P as White Pawn and p as Black Pawn\n");
    printf("Enter the position of for example a2a3 to move\n");
    printf("Enter q to quit the game\n");
    printf("Enter t to take back the moves\n");
    printf("----------------------------------------------------------------\n");
    printf("----------------------------------------------------------------\n");
	ParseFen(START_FEN,board);
	while(TRUE){
	PrintBoard(board);
	printf("Please enter the move >");
	fgets(input,6,stdin);

	if(input[0]=='q'){
	break;
	}else if(input[0]=='t'){
	TakeMove(board);
	}else{
		Move= ParseMove(input,board);
		if(Move != NOMOVE){
			MakeMove(board,Move);
			}
	}
	fflush(stdin);
	}


return(0);
}
