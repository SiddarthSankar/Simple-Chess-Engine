#include<stdio.h>
#include "defs.h"


int CheckBoard(const S_Board *pos) {

	int t_pceNum[13] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int t_bigPce[2] = { 0, 0};
	int t_majPce[2] = { 0, 0};
	int t_minPce[2] = { 0, 0};
	int t_material[2] = { 0, 0};

	int sq64,t_piece,t_pce_num,sq120,colour,pcount;

	U64 t_pawns[3] = {0ULL, 0ULL, 0ULL};

	t_pawns[WHITE] = pos->pawns[WHITE];
	t_pawns[BLACK] = pos->pawns[BLACK];
	t_pawns[BOTH] = pos->pawns[BOTH];
		// check piece lists
	for(t_piece = wP; t_piece <= bK; ++t_piece) {
		for(t_pce_num = 0; t_pce_num < pos->PCE_NUM[t_piece]; ++t_pce_num) {
			sq120 = pos->pList[t_piece][t_pce_num];
			ASSERT(pos->pieces[sq120]==t_piece);
		}
	}
	// check piece count and other counters
	for(sq64 = 0; sq64 < 64; ++sq64) {
		sq120 = SQ120(sq64);
		t_piece = pos->pieces[sq120];
		t_pceNum[t_piece]++;
		colour = PieceCol[t_piece];
		if( PieceBig[t_piece] == TRUE) t_bigPce[colour]++;
		if( PieceMin[t_piece] == TRUE) t_minPce[colour]++;
		if( PieceMaj[t_piece] == TRUE) t_majPce[colour]++;

		t_material[colour] += PieceVal[t_piece];
	}
	for(t_piece = wP; t_piece <= bK; ++t_piece) {
		ASSERT(t_pceNum[t_piece]==pos->PCE_NUM[t_piece]);
	}
	// check bitboards count
	pcount = CNT(t_pawns[WHITE]);
	ASSERT(pcount == pos->PCE_NUM[wP]);
	pcount = CNT(t_pawns[BLACK]);
	ASSERT(pcount == pos->PCE_NUM[bP]);
	pcount = CNT(t_pawns[BOTH]);
	ASSERT(pcount == (pos->PCE_NUM[bP] + pos->PCE_NUM[wP]));

	// check bitboards squares
	while(t_pawns[WHITE]) {
		sq64 = POP(&t_pawns[WHITE]);
		ASSERT(pos->pieces[SQ120(sq64)] == wP);
	}

	while(t_pawns[BLACK]) {
		sq64 = POP(&t_pawns[BLACK]);
		ASSERT(pos->pieces[SQ120(sq64)] == bP);
	}

	while(t_pawns[BOTH]) {
		sq64 = POP(&t_pawns[BOTH]);
		ASSERT( (pos->pieces[SQ120(sq64)] == bP) || (pos->pieces[SQ120(sq64)] == wP) );
	}

	ASSERT(t_material[WHITE]==pos->material[WHITE] && t_material[BLACK]==pos->material[BLACK]);
	ASSERT(t_minPce[WHITE]==pos->minPCE[WHITE] && t_minPce[BLACK]==pos->minPCE[BLACK]);
	ASSERT(t_majPce[WHITE]==pos->majPCE[WHITE] && t_majPce[BLACK]==pos->majPCE[BLACK]);
	ASSERT(t_bigPce[WHITE]==pos->bigPCE[WHITE] && t_bigPce[BLACK]==pos->bigPCE[BLACK]);

	ASSERT(pos->side==WHITE || pos->side==BLACK);
	ASSERT(GeneratePosKey(pos)==pos->POS_KEY);

	ASSERT(pos->enPass==NO_SQ || ( RanksBrd[pos->enPass]==RANK_6 && pos->side == WHITE)
		 || ( RanksBrd[pos->enPass]==RANK_3 && pos->side == BLACK));

	ASSERT(pos->pieces[pos->Kingsq[WHITE]] == wK);
	ASSERT(pos->pieces[pos->Kingsq[BLACK]] == bK);

	//ASSERT(pos->Cast_Perm >= 0 && pos->Cast_Perm <= 15);

	//ASSERT(PceListOk(pos));

	return TRUE;

}
void UpdateListsMaterial(S_Board *pos) {

	int piece,sq,index,colour;

	for(index = 0; index < Board_sq_nos; ++index) {
		sq = index;
		piece = pos->pieces[index];
		//ASSERT(PceValidEmptyOffbrd(piece));
		if(piece!=OFFBOARD && piece!= EMPTY) {
			colour = PieceCol[piece];

			if( PieceBig[piece] == TRUE) pos->bigPCE[colour]++;
		    if( PieceMin[piece] == TRUE) pos->minPCE[colour]++;
		    if( PieceMaj[piece] == TRUE) pos->majPCE[colour]++;

			pos->material[colour] += PieceVal[piece];
			//Piece List
			pos->pList[piece][pos->PCE_NUM[piece]] = sq;
			pos->PCE_NUM[piece]++;


			if(piece==wK) pos->Kingsq[WHITE] = sq;
			if(piece==bK) pos->Kingsq[BLACK] = sq;

			if(piece==wP) {
				SETBIT(pos->pawns[WHITE],SQ64(sq));
				SETBIT(pos->pawns[BOTH],SQ64(sq));
			} else if(piece==bP) {
				SETBIT(pos->pawns[BLACK],SQ64(sq));
				SETBIT(pos->pawns[BOTH],SQ64(sq));
			}

		}

	}
}
int ParseFen(char *fen, S_Board *pos) {

	//ASSERT(fen!=NULL);
//	ASSERT(pos!=NULL);

	int  rank = RANK_8;
    int  file = FILE_A;
    int  piece = 0;
    int  count = 0;
    int  i = 0;
	int  sq64 = 0;
	int  sq120 = 0;

	ResetBoard(pos);

	while ((rank >= RANK_1) && *fen) {
	    count = 1;
		switch (*fen) {
            case 'p': piece = bP; break;
            case 'r': piece = bR; break;
            case 'n': piece = bN; break;
            case 'b': piece = bB; break;
            case 'k': piece = bK; break;
            case 'q': piece = bQ; break;
            case 'P': piece = wP; break;
            case 'R': piece = wR; break;
            case 'N': piece = wN; break;
            case 'B': piece = wB; break;
            case 'K': piece = wK; break;
            case 'Q': piece = wQ; break;

            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
                piece = EMPTY;
                count = *fen - '0';
                break;

            case '/':
            case ' ':
                rank--;
                file = FILE_A;
                fen++;
                continue;

            default:
                printf("FEN error \n");
                return -1;
        }

		for (i = 0; i < count; i++) {
            sq64 = rank * 8 + file;
			sq120 = SQ120(sq64);
            if (piece != EMPTY) {
                pos->pieces[sq120] = piece;
            }
			file++;
        }
		fen++;
	}

	ASSERT(*fen == 'w' || *fen == 'b');

	pos->side = (*fen == 'w') ? WHITE : BLACK;
	fen += 2;

	for (i = 0; i < 4; i++) {
        if (*fen == ' ') {
            break;
        }
		switch(*fen) {
			case 'K': pos->Cast_Perm |= WKCA; break;
			case 'Q': pos->Cast_Perm |= WQCA; break;
			case 'k': pos->Cast_Perm |= BKCA; break;
			case 'q': pos->Cast_Perm |= BQCA; break;
			default:	     break;
        }
		fen++;
	}
	fen++;

	ASSERT(pos->Cast_Perm>=0 && pos->Cast_Perm <= 15);

	if (*fen != '-') {
		file = fen[0] - 'a';
		rank = fen[1] - '1';

		ASSERT(file>=FILE_A && file <= FILE_H);
		ASSERT(rank>=RANK_1 && rank <= RANK_8);

		pos->enPass = File2SQ(file,rank);
    }

	pos->POS_KEY = GeneratePosKey(pos);

	UpdateListsMaterial(pos);

	return 0;
}



void ResetBoard(S_Board *pos){

	int index=0;
	for(index = 0; index < Board_sq_nos; ++index) {
		pos->pieces[index] = OFFBOARD;
	}

	for(index = 0; index < 64; ++index) {
		pos->pieces[SQ120(index)] = EMPTY;
	}

	for(index = 0; index < 2; ++index) {
		pos->bigPCE[index] = 0;
		pos->majPCE[index] = 0;
		pos->minPCE[index] = 0;
		pos->material[index] = 0;
		
	}
	for(index = 0; index < 3; ++index) {
		pos->pawns[index] = 0ULL;
	}

	for(index = 0; index < 13; ++index) {
		pos->PCE_NUM[index] = 0;
	}

	pos->Kingsq[WHITE] = pos->Kingsq[BLACK] = NO_SQ;

	pos->side = BOTH;
	pos->enPass = NO_SQ;
	pos->fiftyMove = 0;

	pos->ply = 0;
	pos->his_ply = 0;

	pos->Cast_Perm = 0;

	pos->POS_KEY = 0ULL;
}

void PrintBoard(const S_Board *pos) {

	int sq,file,rank,piece;

	printf("\nChess Board:\n\n");

	for(rank = RANK_8; rank >= RANK_1; rank--) {
		printf("%d  ",rank+1);
		for(file = FILE_A; file <= FILE_H; file++) {
			sq = File2SQ(file,rank);
			piece = pos->pieces[sq];
			printf("%3c",PceChar[piece]);
		}
		printf("\n");
	}

	printf("\n   ");
	for(file = FILE_A; file <= FILE_H; file++) {
		printf("%3c",'a'+file);
	}
	printf("\n");
	printf("side:%c\n",SideChar[pos->side]);
	printf("enPas:%d\n",pos->enPass);
	printf("castle:%c%c%c%c\n",
			pos->Cast_Perm & WKCA ? 'K' : '-',
			pos->Cast_Perm & WQCA ? 'Q' : '-',
			pos->Cast_Perm & BKCA ? 'k' : '-',
			pos->Cast_Perm & BQCA ? 'q' : '-'
			);
	printf("PosKey:%llx\n",pos->POS_KEY);
}
