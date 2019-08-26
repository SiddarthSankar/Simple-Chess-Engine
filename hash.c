#include<stdlib.h>
#include "defs.h"
U64 GeneratePosKey(const S_Board *pos) {

	int sq = 0;
	U64 finalKey = 0;
	int piece = EMPTY;

	// pieces
	for(sq = 0; sq < Board_sq_nos; ++sq) {
		piece = pos->pieces[sq];
		if(piece!=NO_SQ && piece!=EMPTY && piece != OFFBOARD) {
			ASSERT(piece>=wP && piece<=bK);
			finalKey ^= PieceKeys[piece][sq];
		}
	}

	if(pos->side == WHITE) {
		finalKey ^= SideKey;
	}

	if(pos->enPass != NO_SQ) {
		ASSERT(pos->enPass>=0 && pos->enPass<Board_sq_nos);
		/* ASSERT(SqOnBoard(pos->enPass));
		ASSERT(RanksBrd[pos->enPass] == RANK_3 || RanksBrd[pos->enPas] == RANK _6);*/
		finalKey ^= PieceKeys[EMPTY][pos->enPass];
	}

	ASSERT(pos->Cast_Perm>=0 && pos->Cast_Perm<=15);

	finalKey ^= CastleKeys[pos->Cast_Perm];

	return finalKey;
}

