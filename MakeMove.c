#include "defs.h"
#include "stdio.h"

#define HASH_PCE(pce,sq) (pos->POS_KEY ^= (PieceKeys[(pce)][(sq)]))
#define HASH_CA (pos->POS_KEY ^= (CastleKeys[(pos->Cast_Perm)]))
#define HASH_SIDE (pos->POS_KEY ^= (SideKey))
#define HASH_EP (pos->POS_KEY ^= (PieceKeys[EMPTY][(pos->enPass)]))

const int CastlePerm[120] = {
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 13, 15, 15, 15, 12, 15, 15, 14, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15,  7, 15, 15, 15,  3, 15, 15, 11, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15
};

static void ClearPiece(const int sq, S_Board *pos) {

	ASSERT(SqOnBoard(sq));


    int pce = pos->pieces[sq];

    ASSERT(PieceValid(pce));

	int col = PieceCol[pce];
	int index = 0;
	int t_pceNum = -1;

	ASSERT(SideValid(col));

    HASH_PCE(pce,sq);

	pos->pieces[sq] = EMPTY;
    pos->material[col] -= PieceVal[pce];

	if(PieceBig[pce]) {
			pos->bigPCE[col]--;
		if(PieceMaj[pce]) {
			pos->majPCE[col]--;
		} else {
			pos->minPCE[col]--;
		}
	} else {
		CLRBIT(pos->pawns[col],SQ64(sq));
		CLRBIT(pos->pawns[BOTH],SQ64(sq));
	}

	for(index = 0; index < pos->PCE_NUM[pce]; ++index) {
		if(pos->pList[pce][index] == sq) {
			t_pceNum = index;
			break;
		}
	}

	ASSERT(t_pceNum != -1);
	ASSERT(t_pceNum>=0&&t_pceNum<10);

	pos->PCE_NUM[pce]--;

	pos->pList[pce][t_pceNum] = pos->pList[pce][pos->PCE_NUM[pce]];

}

static void AddPiece(const int sq, S_Board *pos, const int pce) {

    ASSERT(PieceValid(pce));
    ASSERT(SqOnBoard(sq));

	int col = PieceCol[pce];
	ASSERT(SideValid(col));

    HASH_PCE(pce,sq);

	pos->pieces[sq] = pce;

    if(PieceBig[pce]) {
			pos->bigPCE[col]++;
		if(PieceMaj[pce]) {
			pos->majPCE[col]++;
		} else {
			pos->minPCE[col]++;
		}
	} else {
		SETBIT(pos->pawns[col],SQ64(sq));
		SETBIT(pos->pawns[BOTH],SQ64(sq));
	}

	pos->material[col] += PieceVal[pce];
	pos->pList[pce][pos->PCE_NUM[pce]++] = sq;

}

static void MovePiece(const int from, const int to, S_Board *pos) {

    ASSERT(SqOnBoard(from));
    ASSERT(SqOnBoard(to));

	int index = 0;
	int pce = pos->pieces[from];
	int col = PieceCol[pce];
	ASSERT(SideValid(col));
    ASSERT(PieceValid(pce));

#ifdef DEBUG
	int t_PieceNum = FALSE;
#endif

	HASH_PCE(pce,from);
	pos->pieces[from] = EMPTY;

	HASH_PCE(pce,to);
	pos->pieces[to] = pce;

	if(!PieceBig[pce]) {
		CLRBIT(pos->pawns[col],SQ64(from));
		CLRBIT(pos->pawns[BOTH],SQ64(from));
		SETBIT(pos->pawns[col],SQ64(to));
		SETBIT(pos->pawns[BOTH],SQ64(to));
	}

	for(index = 0; index < pos->PCE_NUM[pce]; ++index) {
		if(pos->pList[pce][index] == from) {
			pos->pList[pce][index] = to;
#ifdef DEBUG
			t_PieceNum = TRUE;
#endif
			break;
		}
	}
	ASSERT(t_PieceNum);
}


int MakeMove(S_Board *pos, int move) {

	ASSERT(CheckBoard(pos));

	int from = FROMSQ(move);
    int to = TOSQ(move);
    int side = pos->side;

	ASSERT(SqOnBoard(from));
    ASSERT(SqOnBoard(to));
    ASSERT(SideValid(side));
    ASSERT(PieceValid(pos->pieces[from]));
	ASSERT(pos->his_ply >= 0 && pos->his_ply < MAXGAMEMOVES);
	ASSERT(pos->ply >= 0 && pos->ply < MAXDEPTH);

	pos->history[pos->his_ply].POS_KEY = pos->POS_KEY;

	if(move & MFLAGEP) {
        if(side == WHITE) {
            ClearPiece(to-10,pos);
        } else {
            ClearPiece(to+10,pos);
        }
    } else if (move & MFLAGCA) {
        switch(to) {
            case C1:
                MovePiece(A1, D1, pos);
			break;
            case C8:
                MovePiece(A8, D8, pos);
			break;
            case G1:
                MovePiece(H1, F1, pos);
			break;
            case G8:
                MovePiece(H8, F8, pos);
			break;
            default: ASSERT(FALSE); break;
        }
    }

	if(pos->enPass != NO_SQ) HASH_EP;
    HASH_CA;

	pos->history[pos->his_ply].move = move;
    pos->history[pos->his_ply].fiftyMove = pos->fiftyMove;
    pos->history[pos->his_ply].enPass = pos->enPass;
    pos->history[pos->his_ply].Cast_Perm = pos->Cast_Perm;

    pos->Cast_Perm &= CastlePerm[from];
    pos->Cast_Perm &= CastlePerm[to];
    pos->enPass = NO_SQ;

	HASH_CA;

	int captured = CAPTURED(move);
    pos->fiftyMove++;

	if(captured != EMPTY) {
        ASSERT(PieceValid(captured));
        ClearPiece(to, pos);
        pos->fiftyMove = 0;
    }

	pos->his_ply++;
	pos->ply++;

	ASSERT(pos->his_ply >= 0 && pos->his_ply < MAXGAMEMOVES);
	ASSERT(pos->ply >= 0 && pos->ply < MAXDEPTH);

	if(PiecePawn[pos->pieces[from]]) {
        pos->fiftyMove = 0;
        if(move & MFLAGPS) {
            if(side==WHITE) {
                pos->enPass=from+10;
                ASSERT(RanksBrd[pos->enPass]==RANK_3);
            } else {
                pos->enPass=from-10;
                ASSERT(RanksBrd[pos->enPass]==RANK_6);
            }
            HASH_EP;
        }
    }

	MovePiece(from, to, pos);

	int prPce = PROMOTED(move);
    if(prPce != EMPTY)   {
        ASSERT(PieceValid(prPce) && !PiecePawn[prPce]);
        ClearPiece(to, pos);
        AddPiece(to, pos, prPce);
    }

	if(PieceKing[pos->pieces[to]]) {
        pos->Kingsq[pos->side] = to;
    }

	pos->side ^= 1;
    HASH_SIDE;

    ASSERT(CheckBoard(pos));


	if(SqAttacked(pos->Kingsq[side],pos->side,pos))  {
        TakeMove(pos);
        return FALSE;
    }

	return TRUE;

}

void TakeMove(S_Board *pos) {

	ASSERT(CheckBoard(pos));

	pos->his_ply--;
    pos->ply--;

	ASSERT(pos->his_ply >= 0 && pos->his_ply < MAXGAMEMOVES);
	ASSERT(pos->ply >= 0 && pos->ply < MAXDEPTH);

    int move = pos->history[pos->his_ply].move;
    int from = FROMSQ(move);
    int to = TOSQ(move);

	ASSERT(SqOnBoard(from));
    ASSERT(SqOnBoard(to));

	if(pos->enPass != NO_SQ) HASH_EP;
    HASH_CA;

    pos->Cast_Perm = pos->history[pos->his_ply].Cast_Perm;
    pos->fiftyMove = pos->history[pos->his_ply].fiftyMove;
    pos->enPass = pos->history[pos->his_ply].enPass;

    if(pos->enPass != NO_SQ) HASH_EP;
    HASH_CA;

    pos->side ^= 1;
    HASH_SIDE;

	if(MFLAGEP & move) {
        if(pos->side == WHITE) {
            AddPiece(to-10, pos, bP);
        } else {
            AddPiece(to+10, pos, wP);
        }
    } else if(MFLAGCA & move) {
        switch(to) {
            case C1: MovePiece(D1, A1, pos); break;
            case C8: MovePiece(D8, A8, pos); break;
            case G1: MovePiece(F1, H1, pos); break;
            case G8: MovePiece(F8, H8, pos); break;
            default: ASSERT(FALSE); break;
        }
    }

	MovePiece(to, from, pos);

	if(PieceKing[pos->pieces[from]]) {
        pos->Kingsq[pos->side] = from;
    }

	int captured = CAPTURED(move);
    if(captured != EMPTY) {
        ASSERT(PieceValid(captured));
        AddPiece(to, pos, captured);
    }

	if(PROMOTED(move) != EMPTY)   {
        ASSERT(PieceValid(PROMOTED(move)) && !PiecePawn[PROMOTED(move)]);
        ClearPiece(from, pos);
        AddPiece(from, pos, (PieceCol[PROMOTED(move)] == WHITE ? wP : bP));
    }

    ASSERT(CheckBoard(pos));

}



