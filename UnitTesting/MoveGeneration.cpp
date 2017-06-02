#pragma once
#include "gtest/gtest.h"
#include "move.h"
#include "moveGeneration.h"
#include "magicBitboards.h"


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

	board = Board();
	board.whitePawnBitboard = 281474976710656;
	board.update();
	Movelist = searchForMoves(&board, white);
	EXPECT_EQ(Movelist.size(), 5);
	EXPECT_EQ(Movelist[0].from, 48);
	EXPECT_EQ(Movelist[0].to, 56);
	EXPECT_EQ(Movelist[0].piece, pawn);

	board = Board();
	board.blackPawnBitboard = 256;
	board.update();
	Movelist = searchForMoves(&board, black);
	EXPECT_EQ(Movelist.size(), 5);
	EXPECT_EQ(Movelist[0].from, 8);
	EXPECT_EQ(Movelist[0].to, 0);
	EXPECT_EQ(Movelist[0].piece, pawn);

}

TEST(MoveGeneration, KingMoves)
{
	std::vector<Move> movelist = std::vector<Move>();
	Board board = Board();
	board.whiteKingBitboard = 1;
	board.whitePawnBitboard = 768;
	board.blackPawnBitboard = 2;
	board.update();
	generateKingMoves(&board, white, movelist, board.whitePieces, board.blackPieces);

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

	movelist = std::vector<Move>();
	board = Board();
	board.whiteBishopBitboard = 4;
	board.whitePawnBitboard = 1536;
	board.blackKnightBitboard = 1048576;
	board.update();
	generateBishopMoves(&board, white, movelist, board.whitePieces, board.blackPieces);
	EXPECT_EQ(movelist.size(), 2);
	if (movelist[0].to == 11)
	{
		EXPECT_EQ(movelist[1].from, 2);
		EXPECT_EQ(movelist[1].to, 20);
		EXPECT_EQ(movelist[1].piece, bishop);
		EXPECT_EQ(movelist[1].moveType, capture);
	}
	else
	{
		EXPECT_EQ(movelist[0].from, 2);
		EXPECT_EQ(movelist[0].to, 20);
		EXPECT_EQ(movelist[0].piece, bishop);
		EXPECT_EQ(movelist[0].moveType, capture);
	}


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
	board.whiteQueenBitboard = 4194304;
	board.whitePawnBitboard = 3768623104;
	board.blackKingBitboard = 64;
	board.update();
	generateQueenMoves(&board, white, movelist, board.whitePieces, board.blackPieces);
	EXPECT_EQ(movelist.size(), 2);
	if (movelist[0].to == 6)
	{
		EXPECT_EQ(movelist[0].from, 22);
		EXPECT_EQ(movelist[0].to, 6);
		EXPECT_EQ(movelist[0].piece, queen);
		EXPECT_EQ(movelist[0].moveType, capture);
	}
	else
	{
		EXPECT_EQ(movelist[1].from, 22);
		EXPECT_EQ(movelist[1].to, 6);
		EXPECT_EQ(movelist[1].piece, queen);
		EXPECT_EQ(movelist[1].moveType, capture);
	}

}