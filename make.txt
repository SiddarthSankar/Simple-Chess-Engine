all:
	gcc chess.c init.c Bitboards.c hash.c board.c data.c attack.c io.c MoveGEN.c Validate.c MakeMove.c -o chess -O2 -s