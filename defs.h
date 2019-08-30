#ifndef DEFS_H
#define DEFS_H
#include<stdlib.h>
#include<stdio.h>


#define DEBUG

#ifndef DEBUG
#define ASSERT(n)
#else
#define ASSERT(n)\
if(!(n)){\
printf("%s - Failed", #n);\
printf("On %s ", __DATE__);\
printf("At %s ", __TIME__);\
printf("In File %s",__FILE__);\
printf("At Line %d\n",__LINE__);\
exit(1);}
#endif
 // For debugging and assert
typedef unsigned long long U64;

#define NAME "Chess"
#define Board_sq_nos 120
#define MAXGAMEMOVES 2048// Here considered only half moves
#define MAXPOSITIONMOVES 256
#define MAXDEPTH 64
//Starting defination of FEN notation
#define START_FEN  "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"


enum { EMPTY, wP, wN, wB, wR, wQ, wK, bP, bN, bB, bR, bQ, bK };
enum { FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, FILE_NONE };
enum { RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8, RANK_NONE };
enum { WHITE, BLACK,BOTH};
enum {
	A1 = 21,B1, C1, D1, E1,F1, G1,H1,
	A2 = 31,B2, C2, D2, E2,F2, G2,H2,
	A3 = 41,B3, C3, D3, E3,F3, G3,H3,
	A4 = 51,B4, C4, D4, E4,F4, G4,H4,
	A5 = 61,B5, C5, D5, E5,F5, G5,H5,
	A6 = 71,B6, C6, D6, E6,F6, G6,H6,
	A7 = 81,B7, C7, D7, E7,F7, G7,H7,
	A8 = 91,B8, C8, D8, E8,F8, G8,H8, NO_SQ,OFFBOARD };
enum {FALSE, TRUE };
enum {WKCA=1, WQCA=2, BKCA=4, BQCA=8};

typedef struct {
	int move;//Store the move
	int score;
} S_MOVE;

typedef struct {
	S_MOVE moves[MAXPOSITIONMOVES];
	int count;
} S_MOVELIST;

//Board Structure
typedef struct{

	int move;
	int Cast_Perm;
	int enPass;
	int fiftyMove;
	U64 POS_KEY;

}S_Undo;
typedef struct {
	int pieces[Board_sq_nos];
	U64 pawns[3];// 64 bit values will be used for black and white
	int Kingsq[2];
	int side;
	int enPass;
	int fiftyMove;
	int ply;
	int his_ply;
	U64 POS_KEY;
	int Cast_Perm;
	int PCE_NUM[13];// Includes 13 types pieces includes empty
	int bigPCE[2];
	int majPCE[2];
	int	minPCE[2];
	int	material[2];

	S_Undo history[MAXGAMEMOVES];
	//piece list
	int pList[13][10];

}S_Board;


//Global variables
extern int Sq120to64[Board_sq_nos];
extern int Sq64to120[64];
extern U64 SetMask[64];
extern U64 ClearMask[64];
extern U64 PieceKeys[13][120];
extern U64 SideKey;
extern U64 CastleKeys[16];
extern char PceChar[];
extern char SideChar[];
extern char RankChar[];
extern char FileChar[];

extern int PieceBig[13];
extern int PieceMaj[13];
extern int PieceMin[13];
extern int PieceVal[13];
extern int PieceCol[13];
extern int PiecePawn[13];
//To check the Pieces
extern int PieceKnight[13];
extern int PieceKing[13];
extern int PieceRookQueen[13];
extern int PieceBishopQueen[13];
extern int PieceSlides[13];

extern int FilesBrd[Board_sq_nos];
extern int RanksBrd[Board_sq_nos];

#define FROMSQ(m) ((m) & 0x7F)
#define TOSQ(m) (((m)>>7) & 0x7F)
#define CAPTURED(m) (((m)>>14) & 0xF)
#define PROMOTED(m) (((m)>>20) & 0xF)

#define MFLAGEP 0x40000
#define MFLAGPS 0x80000
#define MFLAGCA 0x1000000

#define MFLAGCAP 0x7C000
#define MFLAGPROM 0xF00000

#define NOMOVE 0
// General MACROS 
#define SQ64(sq120) Sq120to64[sq120]
#define SQ120(sq64) Sq64to120[(sq64)]
#define File2SQ(f,r) ( (21 + (f) ) + ( (r) * 10 ) )
#define POP(a) PopBit(a)
#define CNT(a) CountBits(a)
#define CLRBIT(cc,sq) ((cc) &= ClearMask[sq])
#define SETBIT(bb,sq)  ((bb) |= SetMask[sq])

#define IsBQ(p) (PieceBishopQueen[(p)])
#define IsRQ(p) (PieceRookQueen[(p)])
#define IsKn(p) (PieceKnight[(p)])
#define IsKi(p) (PieceKing[(p)])

// Functions on all the files 
extern void InitBitMask();
extern void InitAll();
extern void PrintBitBoard(U64 bb);
extern int PopBit(U64 *bb);
extern int CountBits(U64 b);
extern U64 GeneratePosKey(const S_Board *pos);
extern void ResetBoard(S_Board *pos);
extern int ParseFen(char *fen, S_Board *pos);
extern void PrintBoard(const S_Board *pos);
extern int CheckBoard(const S_Board *pos);
extern int SqAttacked(const int sq, const int side, const S_Board *pos);
extern char *PrMove(const int move);
extern char *PrSq(const int sq);
extern int SqOnBoard(const int sq);
extern int SideValid(const int side);
extern int FileRankValid(const int fr);
extern int PieceValidEmpty(const int pce);
extern int PieceValid(const int pce);
extern void GenerateAllMoves(const S_Board *pos, S_MOVELIST *list);
extern int MakeMove(S_Board *pos, int move);
extern void TakeMove(S_Board *pos);

#endif
