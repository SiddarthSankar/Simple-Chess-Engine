#include "defs.h"
#include "stdlib.h"
#include<stdio.h>
// Generates a 15 bit random number
#define RAND_64 	((U64)rand() | \
					(U64)rand() << 15 | \
					(U64)rand() << 30 | \
					(U64)rand() << 45 | \
					((U64)rand() & 0xf) << 60 )

int Sq120to64[Board_sq_nos];
int Sq64to120[64];
U64 SetMask[64];
U64 ClearMask[64];

U64 PieceKeys[13][120];
U64 SideKey;
U64 CastleKeys[16];
//0 0 0 0 -1 2 4 8

int FilesBrd[Board_sq_nos];
int RanksBrd[Board_sq_nos];


void InitFilesRanksBrd() {

	int index = 0;
	int file = FILE_A;
	int rank = RANK_1;
	int sq = A1;

	for(index = 0; index < Board_sq_nos; ++index) {
		FilesBrd[index] = OFFBOARD;
		RanksBrd[index] = OFFBOARD;
	}

	for(rank = RANK_1; rank <= RANK_8; ++rank) {
		for(file = FILE_A; file <= FILE_H; ++file) {
			sq = File2SQ(file,rank);
			FilesBrd[sq] = file;
			RanksBrd[sq] = rank;
		}
	}	

}


void InitHashKeys() {

	int index = 0;
	int index2 = 0;
	for(index = 0; index < 13; ++index) {
		for(index2 = 0; index2 < 120; ++index2) {
			PieceKeys[index][index2] = RAND_64;
		}
	}
	SideKey = RAND_64;
	for(index = 0; index < 16; ++index) {
		CastleKeys[index] = RAND_64;
	}

}
void InitBitMask(){
	int index =0;
	for(index=0; index<64;index++){
		SetMask[index]= 0ULL;
		ClearMask[index]=0ULL;
	}
	for(index=0; index<64;index++){
		SetMask[index] |= (1ULL<<index);
		ClearMask[index] = ~SetMask[index];
	}
}


void InitSq120to64(){

	int index = 0;
	int file = FILE_A;
	int rank = RANK_1;
	int sq = A1;
	int sq64 = 0;
	for(index = 0; index < Board_sq_nos; ++index) {
		Sq120to64[index] = 65;
	}

	for(index = 0; index < 64; ++index) {
		Sq64to120[index] = 120;
	}

	for(rank = RANK_1; rank <= RANK_8; ++rank) {
		for(file = FILE_A; file <= FILE_H; ++file) {
			sq = File2SQ(file,rank);
			Sq64to120[sq64] = sq;
			Sq120to64[sq] = sq64;
			sq64++;
		}
	}
}
void InitAll()
{
	InitSq120to64();
	InitBitMask();
	InitHashKeys();
	InitFilesRanksBrd();
}

