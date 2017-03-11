#pragma once

#include "gtest/gtest.h"
#include "board.h"
#include "scoring.h"
#include "move.h"
#include "moveGeneration.h"
#include "magicBitboards.h"

#include <iostream>
#include <stdint.h>


TEST(Board, UpdateFunction)
{
	Board board;
	board.whitePawnBitboard = 2;
	board.whiteBishopBitboard = 4;
	board.update();
	EXPECT_EQ(board.allPieces, 6);

	board.blackBishopBitboard = 8;
	board.update();
	EXPECT_EQ(board.allPieces, 14);
	EXPECT_EQ(board.whitePieces, 6);
	EXPECT_EQ(board.blackPieces, 8);
}
TEST(Board, Defaults)
{
	Board board;
	board.defaults();
	EXPECT_EQ(board.whitePawnBitboard, 65280);
	EXPECT_EQ(board.whiteRookBitboard, 129);
	EXPECT_EQ(board.whiteKnightBitboard, 66);
	EXPECT_EQ(board.whiteBishopBitboard, 36);
	EXPECT_EQ(board.whiteQueenBitboard, 8);
	EXPECT_EQ(board.whiteKingBitboard, 16);

	EXPECT_EQ(board.blackPawnBitboard, 71776119061217280);
	EXPECT_EQ(board.blackRookBitboard, 9295429630892703744);
	EXPECT_EQ(board.blackKnightBitboard, 4755801206503243776);
	EXPECT_EQ(board.blackBishopBitboard, 2594073385365405696);
	EXPECT_EQ(board.blackQueenBitboard, 576460752303423488);
	EXPECT_EQ(board.blackKingBitboard, 1152921504606846976);

}

TEST(Bitboard, bitsum)
{
	EXPECT_EQ(bitSum(15), 4);
	EXPECT_EQ(bitSum(17), 2);
	EXPECT_EQ(bitSum(emptyBitboard), 0);
	EXPECT_EQ(bitSum(universalBitboard), 64);
	EXPECT_EQ(bitSum(255), 8);
}

TEST(Bitboard, pop)
{
	uint64_t b = 15;
	EXPECT_EQ(pop(b), 1);
	EXPECT_EQ(pop(b), 2);
	EXPECT_EQ(pop(b), 4);
	EXPECT_EQ(pop(b), 8);
	EXPECT_EQ(pop(b), 0);

	b = 263;
	EXPECT_EQ(pop(b), 1);
	EXPECT_EQ(pop(b), 2);
	EXPECT_EQ(pop(b), 4);
	EXPECT_EQ(pop(b), 256);
	EXPECT_EQ(pop(b), 0);
}

TEST(Bitboard, bitScanForward)
{
	EXPECT_EQ(bitScanForward(2), 1);
	EXPECT_EQ(bitScanForward(256), 8);
	EXPECT_EQ(bitScanForward(64), 6);
}

TEST(Scoring, material)
{
	Board board;
	board.whitePawnBitboard = 1;
	EXPECT_EQ(calculateMaterialScore(&board, white), 100);
	EXPECT_EQ(calculateMaterialScore(&board, black), -100);

	board.whitePawnBitboard = 0;
	board.blackKnightBitboard = 5;
	EXPECT_EQ(calculateMaterialScore(&board, white), -640);
	EXPECT_EQ(calculateMaterialScore(&board, black), 640);

	board.blackKnightBitboard = 0;
	board.whiteQueenBitboard= 8;
	EXPECT_EQ(calculateMaterialScore(&board, white), 900);
	EXPECT_EQ(calculateMaterialScore(&board, black), -900);

	board.defaults();
	EXPECT_EQ(calculateMaterialScore(&board, black), 0);
	EXPECT_EQ(calculateMaterialScore(&board, white), 0);
}

TEST(Scoring, positionalScore)
{
	Board board;
	board.whiteRookBitboard = 1;
	board.update();
	EXPECT_EQ(calculatePositionalScore(&board, white), 0);
	EXPECT_EQ(calculatePositionalScore(&board, black), 0);
	
	board = Board();
	board.whitePawnBitboard = 512;
	board.update();
	EXPECT_EQ(calculatePositionalScore(&board, white) , 10);
	EXPECT_EQ(calculatePositionalScore(&board, black), -10);
	
}

TEST(Move, ApplyMove)
{
	Move move = Move(1, 9, quietMove, pawn);
	Board board;
	board.whitePawnBitboard = 2;
	Board moveBoard = move.applyMove(&board, white);
	EXPECT_EQ(moveBoard.whitePawnBitboard, 1 << 9);
	EXPECT_EQ(moveBoard.allPieces, 1 << 9);

	board = Board();
	move = Move(1, 7, quietMove, queen);
	board.whiteQueenBitboard = 2;
	moveBoard = move.applyMove(&board, white);
	EXPECT_EQ(moveBoard.whiteQueenBitboard, 1 << 7);
	EXPECT_EQ(moveBoard.allPieces, 1 << 7);


	board = Board();
	move = Move(1, 7, capture, queen);
	board.whiteQueenBitboard = 2;
	board.blackPawnBitboard = 128;
	board.update();
	moveBoard = move.applyMove(&board, white);
	EXPECT_EQ(moveBoard.whiteQueenBitboard, 1 << 7);
	EXPECT_EQ(moveBoard.allPieces, 1 << 7);

	board = Board();
	move = Move(1, 8, capture, pawn);
	board.whitePawnBitboard = 2;
	board.blackPawnBitboard = 256;
	board.update();
	moveBoard = move.applyMove(&board, white);
	EXPECT_EQ(moveBoard.whitePawnBitboard, 1 << 8);
	EXPECT_EQ(moveBoard.allPieces, 1 << 8);
}

TEST(MoveGeneration, PawnMoves)
{
	Board board = Board();
	board.whitePawnBitboard = 2;
	board.update();
	std::vector<Move> Movelist = searchForMoves(&board, white);
	EXPECT_EQ(Movelist.size(), 1);
	EXPECT_EQ(Movelist[0].from, 1);
	EXPECT_EQ(Movelist[0].to, 9);
	EXPECT_EQ(Movelist[0].piece, pawn);
	EXPECT_EQ(Movelist[0].moveType, quietMove);

	board = Board();
	board.whitePawnBitboard = 256;
	board.update();
	Movelist = searchForMoves(&board, white);
	EXPECT_EQ(Movelist.size(), 2);
	EXPECT_EQ(Movelist[0].from, 8);
	EXPECT_EQ(Movelist[0].to + Movelist[1].to, 40); 
	EXPECT_EQ(Movelist[0].piece, pawn);
	EXPECT_EQ(Movelist[0].moveType, quietMove);

	board = Board();
	board.whitePawnBitboard = 1;
	board.blackKingBitboard = 256;
	board.blackPawnBitboard = 512;
	board.update();
	Movelist = searchForMoves(&board, white);
	EXPECT_EQ(Movelist.size(), 1);
	EXPECT_EQ(Movelist[0].from, 0);
	EXPECT_EQ(Movelist[0].to, 9);
	EXPECT_EQ(Movelist[0].piece, pawn);
	EXPECT_EQ(Movelist[0].moveType, capture);
}

TEST(MoveGeneration, KingMoves)
{
	std::vector<Move> movelist = std::vector<Move>();
	Board board = Board();
	board.whiteKingBitboard = 1;
	board.whitePawnBitboard = 768;
	board.blackPawnBitboard = 2;
	board.update();
	generateKingMoves(&board, white, movelist,board.whitePieces, board.blackPieces);

	EXPECT_EQ(movelist.size(), 1);
	EXPECT_EQ(movelist[0].from, 0);
	EXPECT_EQ(movelist[0].to, 1);
	EXPECT_EQ(movelist[0].piece, king);
	EXPECT_EQ(movelist[0].moveType, capture);

	movelist = std::vector<Move>();
	board = Board();
	board.whiteKingBitboard = 1;
	board.whitePawnBitboard = 512;
	board.whiteRookBitboard = 2;
	board.update();
	generateKingMoves(&board, white, movelist, board.whitePieces, board.blackPieces);
	EXPECT_EQ(movelist.size(), 1);
	EXPECT_EQ(movelist[0].from, 0);
	EXPECT_EQ(movelist[0].to, 8);
	EXPECT_EQ(movelist[0].piece, king);
	EXPECT_EQ(movelist[0].moveType, quietMove);

	movelist = std::vector<Move>();
	board = Board();
	board.blackKingBitboard = 9223372036854775808;
	board.blackPawnBitboard = 4647714815446351872;
	board.whiteRookBitboard = 18014398509481984;
	board.update();
	generateKingMoves(&board, black, movelist, board.blackPieces, board.whitePieces);
	EXPECT_EQ(movelist.size(), 1);
	EXPECT_EQ(movelist[0].from, 63);
	EXPECT_EQ(movelist[0].to, 54);
	EXPECT_EQ(movelist[0].piece, king);
	EXPECT_EQ(movelist[0].moveType, capture);
}

TEST(MoveGeneration, KnightMoves)
{
	std::vector<Move> movelist = std::vector<Move>();
	Board board = Board();
	board.whiteKnightBitboard = 1;
	board.whitePawnBitboard = 1024;
	board.update();
	generateKnightMoves(&board, white, movelist, board.whitePieces, board.blackPieces);
	EXPECT_EQ(movelist.size(), 1);
	EXPECT_EQ(movelist[0].from, 0);
	EXPECT_EQ(movelist[0].to, 17);
	EXPECT_EQ(movelist[0].piece, knight);
	EXPECT_EQ(movelist[0].moveType, quietMove);

	movelist = std::vector<Move>();
	board = Board();
	board.whiteKnightBitboard = 9223372036854775808;
	board.whitePawnBitboard = 70368744177664;
	board.blackRookBitboard = 9007199254740992;
	board.update();
	generateKnightMoves(&board, white, movelist, board.whitePieces, board.blackPieces);
	EXPECT_EQ(movelist.size(), 1);
	EXPECT_EQ(movelist[0].from, 63);
	EXPECT_EQ(movelist[0].to, 53);
	EXPECT_EQ(movelist[0].piece, knight);
	EXPECT_EQ(movelist[0].moveType, capture);
}

TEST(MoveGeneration, RookMoves)
{
	

	std::vector<Move> movelist = std::vector<Move>();
	Board board = Board();
	board.whiteRookBitboard = 1;
	board.whitePawnBitboard = 260;
	board.update();
	generateRookMoves(&board, white, movelist, board.whitePieces, board.blackPieces);
	EXPECT_EQ(movelist.size(), 1);
	EXPECT_EQ(movelist[0].from, 0);
	EXPECT_EQ(movelist[0].to, 1);
	EXPECT_EQ(movelist[0].piece, rook);
	EXPECT_EQ(movelist[0].moveType, quietMove);

	movelist = std::vector<Move>();
	board = Board();
	board.whiteRookBitboard = 1;
	board.whitePawnBitboard = 2;
	board.blackPawnBitboard = 256;
	board.update();
	generateRookMoves(&board, white, movelist, board.whitePieces, board.blackPieces);
	EXPECT_EQ(movelist.size(), 1);
	EXPECT_EQ(movelist[0].from, 0);
	EXPECT_EQ(movelist[0].to, 8);
	EXPECT_EQ(movelist[0].piece, rook);
	EXPECT_EQ(movelist[0].moveType, capture);
}

TEST(MoveGeneration, BishopMoves)
{
	std::vector<Move> movelist = std::vector<Move>();
	Board board = Board();
	board.whiteBishopBitboard = 1;
	board.whitePawnBitboard = 262144;
	board.update();
	generateBishopMoves(&board, white, movelist, board.whitePieces, board.blackPieces);
	EXPECT_EQ(movelist.size(), 1);
	EXPECT_EQ(movelist[0].from, 0);
	EXPECT_EQ(movelist[0].to, 9);
	EXPECT_EQ(movelist[0].piece, bishop);
	EXPECT_EQ(movelist[0].moveType, quietMove);

	movelist = std::vector<Move>();
	board = Board();
	board.whiteBishopBitboard = 128;
	board.whitePawnBitboard = 2097216;
	board.update();
	generateBishopMoves(&board, white, movelist, board.whitePieces, board.blackPieces);
	EXPECT_EQ(movelist.size(), 1);
	EXPECT_EQ(movelist[0].from, 7);
	EXPECT_EQ(movelist[0].to, 14);
	EXPECT_EQ(movelist[0].piece, bishop);
	EXPECT_EQ(movelist[0].moveType, quietMove);

	movelist = std::vector<Move>();
	board = Board();
	board.whiteBishopBitboard = 134217728;
	board.whitePawnBitboard = 68720787456;
	board.blackPawnBitboard = 17179869184; 
	board.update();
	generateBishopMoves(&board, white, movelist, board.whitePieces, board.blackPieces);
	EXPECT_EQ(movelist.size(), 1);
	EXPECT_EQ(movelist[0].from, 27);
	EXPECT_EQ(movelist[0].to, 34);
	EXPECT_EQ(movelist[0].piece, bishop);
	EXPECT_EQ(movelist[0].moveType, capture);
}

TEST(MoveGeneration, QueenMoves)
{
	std::vector<Move> movelist = std::vector<Move>();
	Board board = Board();
	board.whiteQueenBitboard = 1;
	board.whitePawnBitboard = 262402;
	board.update();
	generateQueenMoves(&board, white, movelist, board.whitePieces, board.blackPieces);
	EXPECT_EQ(movelist.size(), 1);
	EXPECT_EQ(movelist[0].from, 0);
	EXPECT_EQ(movelist[0].to, 9);
	EXPECT_EQ(movelist[0].piece, queen);
	EXPECT_EQ(movelist[0].moveType, quietMove);

	movelist = std::vector<Move>();
	board = Board();
	board.whiteQueenBitboard = 128;
	board.whitePawnBitboard = 2129984;
	board.update();
	generateQueenMoves(&board, white, movelist, board.whitePieces, board.blackPieces);
	EXPECT_EQ(movelist.size(), 1);
	EXPECT_EQ(movelist[0].from, 7);
	EXPECT_EQ(movelist[0].to, 14);
	EXPECT_EQ(movelist[0].piece, queen);
	EXPECT_EQ(movelist[0].moveType, quietMove);

	movelist = std::vector<Move>();
	board = Board();
	board.whiteQueenBitboard = 1;
	board.whitePawnBitboard = 772;
	board.update();
	generateQueenMoves(&board, white, movelist, board.whitePieces, board.blackPieces);
	EXPECT_EQ(movelist.size(), 1);
	EXPECT_EQ(movelist[0].from, 0);
	EXPECT_EQ(movelist[0].to, 1);
	EXPECT_EQ(movelist[0].piece, queen);
	EXPECT_EQ(movelist[0].moveType, quietMove);

	movelist = std::vector<Move>();
	board = Board();
	board.whiteQueenBitboard = 1;
	board.whitePawnBitboard = 514;
	board.blackPawnBitboard = 256;
	board.update();
	generateQueenMoves(&board, white, movelist, board.whitePieces, board.blackPieces);
	EXPECT_EQ(movelist.size(), 1);
	EXPECT_EQ(movelist[0].from, 0);
	EXPECT_EQ(movelist[0].to, 8);
	EXPECT_EQ(movelist[0].piece, queen);
	EXPECT_EQ(movelist[0].moveType, capture);
}

int main(int argc, char **argv) 
{
	magicBitboards temp = magicBitboards();
	temp.setupMagicBitboards();
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

