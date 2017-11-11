#pragma once
#include "gtest/gtest.h"
#include "move.h"
#include "moveGeneration.h"
#include "magicBitboards.h"
#include "utils.h"

TEST(MoveGeneration, PawnMoves)
{
	Board board = Board();
	board.setBitboard(white, pawn, 2);
	board.update();
	board.nextColour = white;
	std::array<Move, 150> Movelist;
	int arraySize = generatePawnMoves(&board, &Movelist, 0, ~0, ~0, 0);
	EXPECT_EQ(arraySize, 1);
	EXPECT_EQ(Movelist[0].from, 1);
	EXPECT_EQ(Movelist[0].to, 9);
	EXPECT_EQ(Movelist[0].piece, pawn);
	EXPECT_EQ(Movelist[0].moveType, quietMove);

	board = Board();
	board.setBitboard(white, pawn, 256);
	board.update();
	board.nextColour = white;
	arraySize = generatePawnMoves(&board, &Movelist, 0, ~0, ~0, 0);
	EXPECT_EQ(arraySize, 2);
	EXPECT_EQ(Movelist[0].from, 8);
	EXPECT_EQ(Movelist[1].from, 8);
	if (Movelist[0].to == 16)
	{
		EXPECT_EQ(Movelist[0].piece, pawn);
		EXPECT_EQ(Movelist[0].moveType, quietMove);

		EXPECT_EQ(Movelist[1].piece, pawn);
		EXPECT_EQ(Movelist[1].moveType, quietMove);
		EXPECT_EQ(Movelist[1].to, 24);
	}
	else
	{
		EXPECT_EQ(Movelist[1].piece, pawn);
		EXPECT_EQ(Movelist[1].moveType, quietMove);

		EXPECT_EQ(Movelist[0].piece, pawn);
		EXPECT_EQ(Movelist[0].moveType, quietMove);
		EXPECT_EQ(Movelist[0].to, 24);
	}

	board = Board();
	board.setBitboard(white, pawn, 1);
	board.setBitboard(white, king, 256);
	board.setBitboard(black, pawn, 512);
	board.update();
	board.nextColour = white;
	arraySize = generatePawnMoves(&board, &Movelist, 0, ~0, ~0, 0);
	EXPECT_EQ(arraySize, 1);
	EXPECT_EQ(Movelist[0].from, 0);
	EXPECT_EQ(Movelist[0].to, 9);
	EXPECT_EQ(Movelist[0].piece, pawn);
	EXPECT_EQ(Movelist[0].moveType, capture);

	board = Board();
	board.setBitboard(white, pawn, 32768);
	board.setBitboard(black, king, 4194304);
	board.setBitboard(black, pawn, 8388608);
	board.update();
	board.nextColour = white;
	arraySize = generatePawnMoves(&board, &Movelist, 0, ~0, ~0, 0);
	EXPECT_EQ(arraySize, 1);
	EXPECT_EQ(Movelist[0].from, 15);
	EXPECT_EQ(Movelist[0].to, 22);
	EXPECT_EQ(Movelist[0].piece, pawn);
	EXPECT_EQ(Movelist[0].moveType, capture);

	board = Board();
	board.setBitboard(white, pawn, 281474976710656);
	board.update();
	board.nextColour = white;
	arraySize = generatePawnMoves(&board, &Movelist, 0, ~0, ~0, 0);
	EXPECT_EQ(arraySize, 4);
	EXPECT_EQ(Movelist[0].from, 48);
	EXPECT_EQ(Movelist[0].to, 56);
	EXPECT_EQ(Movelist[0].piece, pawn);

	board = Board();
	board.setBitboard(black, pawn, 256);
	board.update();
	board.nextColour = black;
	arraySize = generatePawnMoves(&board, &Movelist, 0, ~0, ~0, 0);
	EXPECT_EQ(arraySize, 4);
	EXPECT_EQ(Movelist[0].from, 8);
	EXPECT_EQ(Movelist[0].to, 0);
	EXPECT_EQ(Movelist[0].piece, pawn);

}

TEST(MoveGeneration, KingMoves)
{
	std::array<Move,150> movelist;
	Board board = Board();
	board.setBitboard(white, king, 1);
	board.setBitboard(white, pawn, 768);
	board.setBitboard(black, pawn, 2);
	board.update();
	board.nextColour = white;
	int arraySize = generateKingMoves(&board, &movelist, board.whitePieces, board.blackPieces, 0,0);
	EXPECT_EQ(arraySize, 1);
	EXPECT_EQ(movelist[0].from, 0);
	EXPECT_EQ(movelist[0].to, 1);
	EXPECT_EQ(movelist[0].piece, king);
	EXPECT_EQ(movelist[0].moveType, capture);

	board = Board();
	board.setBitboard(white, king, 1);
	board.setBitboard(white, pawn, 512);
	board.setBitboard(white, rook, 2);
	board.update();
	board.nextColour = white;
	arraySize = generateKingMoves(&board, &movelist, board.whitePieces, board.blackPieces, 0, 0);
	EXPECT_EQ(arraySize, 1);
	EXPECT_EQ(movelist[0].from, 0);
	EXPECT_EQ(movelist[0].to, 8);
	EXPECT_EQ(movelist[0].piece, king);
	EXPECT_EQ(movelist[0].moveType, quietMove);

	board = Board();
	board.setBitboard(black, king, 9223372036854775808);
	board.setBitboard(black, pawn, 4647714815446351872);
	board.setBitboard(white, rook, 18014398509481984);
	board.update();
	board.nextColour = black;
	arraySize = generateKingMoves(&board, &movelist, board.blackPieces, board.whitePieces, 0, 0);
	EXPECT_EQ(arraySize, 1);
	EXPECT_EQ(movelist[0].from, 63);
	EXPECT_EQ(movelist[0].to, 54);
	EXPECT_EQ(movelist[0].piece, king);
	EXPECT_EQ(movelist[0].moveType, capture);
}

TEST(MoveGeneration, KnightMoves)
{
	std::array<Move, 150> movelist;
	Board board = Board();
	board.setBitboard(white, knight, 1);
	board.setBitboard(white, pawn, 1024);
	board.update();
	board.nextColour = white;
	int arraySize = generateKnightMoves(&board, &movelist, board.whitePieces, board.blackPieces, 0, ~0, ~0,0);
	EXPECT_EQ(arraySize, 1);
	EXPECT_EQ(movelist[0].from, 0);
	EXPECT_EQ(movelist[0].to, 17);
	EXPECT_EQ(movelist[0].piece, knight);
	EXPECT_EQ(movelist[0].moveType, quietMove);

	board = Board();
	board.setBitboard(white, knight, 9223372036854775808);
	board.setBitboard(white, pawn, 70368744177664);
	board.setBitboard(black, rook, 9007199254740992);
	board.update();
	board.nextColour = white;
	arraySize = generateKnightMoves(&board, &movelist, board.whitePieces, board.blackPieces, 0, ~0, ~0, 0);
	EXPECT_EQ(arraySize, 1);
	EXPECT_EQ(movelist[0].from, 63);
	EXPECT_EQ(movelist[0].to, 53);
	EXPECT_EQ(movelist[0].piece, knight);
	EXPECT_EQ(movelist[0].moveType, capture);
}

TEST(MoveGeneration, RookMoves)
{
	std::array<Move, 150> movelist;
	Board board = Board();
	board.setBitboard(white, rook, 1);
	board.setBitboard(white, pawn, 260);
	board.update();
	board.nextColour = white;
	int arraySize = generateRookMoves(&board, &movelist, board.whitePieces, board.blackPieces, 0, ~0, ~0, 0);
	EXPECT_EQ(arraySize, 1);
	EXPECT_EQ(movelist[0].from, 0);
	EXPECT_EQ(movelist[0].to, 1);
	EXPECT_EQ(movelist[0].piece, rook);
	EXPECT_EQ(movelist[0].moveType, quietMove);

	board = Board();
	board.setBitboard(white, rook, 1);
	board.setBitboard(white, pawn, 2);
	board.setBitboard(black, pawn, 256);
	board.update();
	board.nextColour = white;
	arraySize = generateRookMoves(&board, &movelist, board.whitePieces, board.blackPieces, 0, ~0, ~0, 0);
	EXPECT_EQ(arraySize, 1);
	EXPECT_EQ(movelist[0].from, 0);
	EXPECT_EQ(movelist[0].to, 8);
	EXPECT_EQ(movelist[0].piece, rook);
	EXPECT_EQ(movelist[0].moveType, capture);
}

TEST(MoveGeneration, BishopMoves)
{
	std::array<Move, 150> movelist;
	Board board = Board();
	board.setBitboard(white, bishop, 1);
	board.setBitboard(white, pawn, 262144);
	board.update();
	board.nextColour = white;
	int arraySize = generateBishopMoves(&board, &movelist, board.whitePieces, board.blackPieces, 0, ~0, ~0, 0);
	EXPECT_EQ(arraySize, 1);
	EXPECT_EQ(movelist[0].from, 0);
	EXPECT_EQ(movelist[0].to, 9);
	EXPECT_EQ(movelist[0].piece, bishop);
	EXPECT_EQ(movelist[0].moveType, quietMove);

	board = Board();
	board.setBitboard(white,bishop, 128);
	board.setBitboard(white, pawn, 2097216);
	board.update();
	board.nextColour = white;
	arraySize = generateBishopMoves(&board, &movelist, board.whitePieces, board.blackPieces, 0, ~0, ~0, 0);
	EXPECT_EQ(arraySize, 1);
	EXPECT_EQ(movelist[0].from, 7);
	EXPECT_EQ(movelist[0].to, 14);
	EXPECT_EQ(movelist[0].piece, bishop);
	EXPECT_EQ(movelist[0].moveType, quietMove);

	board = Board();
	board.setBitboard(white, bishop, 134217728);
	board.setBitboard(white, pawn, 68720787456);
	board.setBitboard(black, pawn, 17179869184);
	board.update();
	board.nextColour = white;
	arraySize = generateBishopMoves(&board, &movelist, board.whitePieces, board.blackPieces, 0, ~0, ~0, 0);
	EXPECT_EQ(arraySize, 1);
	EXPECT_EQ(movelist[0].from, 27);
	EXPECT_EQ(movelist[0].to, 34);
	EXPECT_EQ(movelist[0].piece, bishop);
	EXPECT_EQ(movelist[0].moveType, capture);

	board = Board();
	board.setBitboard(white, bishop, 4);
	board.setBitboard(white, pawn, 1536);
	board.setBitboard(black, knight, 1048576);
	board.update();
	board.nextColour = white;
	arraySize = generateBishopMoves(&board, &movelist, board.whitePieces, board.blackPieces, 0, ~0, ~0, 0);
	EXPECT_EQ(arraySize, 2);
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
	std::array<Move, 150> movelist;
	Board board = Board();
	board.setBitboard(white, queen, 1);
	board.setBitboard(white, pawn, 262402);
	board.update();
	board.nextColour = white;
	int arraySize = generateQueenMoves(&board, &movelist, board.whitePieces, board.blackPieces, 0, ~0, ~0, 0);
	EXPECT_EQ(arraySize, 1);
	EXPECT_EQ(movelist[0].from, 0);
	EXPECT_EQ(movelist[0].to, 9);
	EXPECT_EQ(movelist[0].piece, queen);
	EXPECT_EQ(movelist[0].moveType, quietMove);

	board = Board();
	board.setBitboard(white, queen, 128);
	board.setBitboard(white, pawn, 2129984);
	board.update();
	board.nextColour = white;
	arraySize = generateQueenMoves(&board, &movelist, board.whitePieces, board.blackPieces, 0, ~0, ~0, 0);
	EXPECT_EQ(arraySize, 1);
	EXPECT_EQ(movelist[0].from, 7);
	EXPECT_EQ(movelist[0].to, 14);
	EXPECT_EQ(movelist[0].piece, queen);
	EXPECT_EQ(movelist[0].moveType, quietMove);

	board = Board();
	board.setBitboard(white, queen, 1);
	board.setBitboard(white, pawn, 772);
	board.update();
	board.nextColour = white;
	arraySize = generateQueenMoves(&board, &movelist, board.whitePieces, board.blackPieces, 0, ~0, ~0, 0);
	EXPECT_EQ(arraySize, 1);
	EXPECT_EQ(movelist[0].from, 0);
	EXPECT_EQ(movelist[0].to, 1);
	EXPECT_EQ(movelist[0].piece, queen);
	EXPECT_EQ(movelist[0].moveType, quietMove);

	board = Board();
	board.setBitboard(white, queen, 2097152);
	board.setBitboard(white, pawn, 1615884288);
	board.setBitboard(black, knight, 268435456);
	board.update();
	board.nextColour = white;
	arraySize = generateQueenMoves(&board, &movelist, board.whitePieces, board.blackPieces, 0, ~0, ~0, 0);
	EXPECT_EQ(arraySize, 1);
	EXPECT_EQ(movelist[0].from, 21);
	EXPECT_EQ(movelist[0].to, 28);
	EXPECT_EQ(movelist[0].piece, queen);
	EXPECT_EQ(movelist[0].moveType, capture);

	board = Board();
	board.setBitboard(white, queen, 67108864);
	board.setBitboard(white, pawn, 60264677376);
	board.setBitboard(black, knight, 16777216);
	board.update();
	board.nextColour = white;
	arraySize = generateQueenMoves(&board, &movelist, board.whitePieces, board.blackPieces, 0, ~0, ~0, 0);
	EXPECT_EQ(arraySize, 2);
	if (movelist[0].to == 24)
	{
		EXPECT_EQ(movelist[0].from, 26);
		EXPECT_EQ(movelist[0].to, 24);
		EXPECT_EQ(movelist[0].piece, queen);
		EXPECT_EQ(movelist[0].moveType, capture);
	}
	else
	{
		EXPECT_EQ(movelist[1].from, 26);
		EXPECT_EQ(movelist[1].to, 24);
		EXPECT_EQ(movelist[1].piece, queen);
		EXPECT_EQ(movelist[1].moveType, capture);
	}


	board = Board();
	board.setBitboard(white, queen, 4194304);
	board.setBitboard(white, pawn, 3768623104);
	board.setBitboard(black, queen, 64);
	board.update();
	board.nextColour = white;
	arraySize = generateQueenMoves(&board, &movelist, board.whitePieces, board.blackPieces, 0, ~0, ~0, 0);
	EXPECT_EQ(arraySize, 2);
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

TEST(MoveGeneration, CastlingMoves)
{
	std::array<Move, 150> movelist;
	Board board = Board();
	board.setBitboard(white, king, 16);
	board.setBitboard(white, rook, 128);
	board.canWhiteCastleKingSide = true;
	board.update();
	board.nextColour = white;
	int arraySize = generateCastlingMoves(&board, &movelist, board.whitePieces, board.blackPieces, 0, 0);
	EXPECT_EQ(arraySize, 1);
	EXPECT_EQ(movelist[0].from, 4);
	EXPECT_EQ(movelist[0].to, 6);
	EXPECT_EQ(movelist[0].piece, king);
	EXPECT_EQ(movelist[0].moveType, kingSideCastling);

	board = Board();
	board.setBitboard(white, king, 16);
	board.setBitboard(white, rook, 1);
	board.canWhiteCastleQueenSide = true;
	board.update();
	board.nextColour = white;
	arraySize = generateCastlingMoves(&board, &movelist, board.whitePieces, board.blackPieces, 0, 0);
	EXPECT_EQ(arraySize, 1);
	EXPECT_EQ(movelist[0].from, 4);
	EXPECT_EQ(movelist[0].to, 2);
	EXPECT_EQ(movelist[0].piece, king);
	EXPECT_EQ(movelist[0].moveType, queenSideCastling);

	board = Board();
	board.setBitboard(black, king, 1152921504606846976);
	board.setBitboard(black, rook, 9223372036854775808);
	board.canBlackCastleKingSide = true;
	board.update();
	board.nextColour = black;
	arraySize = generateCastlingMoves(&board, &movelist, board.whitePieces, board.blackPieces, 0, 0);
	EXPECT_EQ(arraySize, 1);
	EXPECT_EQ(movelist[0].from, 60);
	EXPECT_EQ(movelist[0].to, 62);
	EXPECT_EQ(movelist[0].piece, king);
	EXPECT_EQ(movelist[0].moveType, kingSideCastling);

	board = Board();
	board.setBitboard(black, king, 1152921504606846976);
	board.setBitboard(black, rook, 72057594037927936);
	board.canBlackCastleQueenSide = true;
	board.update();
	board.nextColour = black;
	arraySize = generateCastlingMoves(&board, &movelist, board.whitePieces, board.blackPieces, 0, 0);
	EXPECT_EQ(arraySize, 1);
	EXPECT_EQ(movelist[0].from, 60);
	EXPECT_EQ(movelist[0].to, 58);
	EXPECT_EQ(movelist[0].piece, king);
	EXPECT_EQ(movelist[0].moveType, queenSideCastling);
}

TEST(MoveGeneration, getPinnedPieces)
{
	Board board = Board("3k4/3p4/8/8/8/8/8/3R3K b - - 1 8 ");
	EXPECT_EQ(getPinnedPieces(&board), (uint64_t)1 << 51);

	board = Board("4k3/3p4/8/8/8/8/8/3R3K b - - 1 8 ");
	EXPECT_EQ(getPinnedPieces(&board), 0);

	board = Board("3k4/4p3/8/8/7B/8/8/7K b - - 1 8 ");
	EXPECT_EQ(getPinnedPieces(&board), (uint64_t)1 << 52);

	board = Board("3k4/3p4/8/8/8/8/8/3Q3K b - - 1 8 ");
	EXPECT_EQ(getPinnedPieces(&board), (uint64_t)1 << 51);

	board = Board("3k4/4p3/8/8/7Q/8/8/7K b - - 1 8 ");
	EXPECT_EQ(getPinnedPieces(&board), (uint64_t)1 << 52);
}

uint64_t perft(int depth, Board* board, int divide)
{
	std::array<Move,150> moveList;

	int arraySize = searchForMoves(board,&moveList);
	int perftVal;
	uint64_t nodes = 0;

	if (depth == 0) return 1;

	for (int x = 0; x < arraySize; x++)
	{
		moveList[x].applyMove(board);
		perftVal = perft(depth - 1, board, divide);
		nodes += perftVal;
		if (depth == divide) std::cout << notationFromMove(moveList[x]) << " " << perftVal << "\n";
		moveList[x].undoMove(board);
	}
	return nodes;
}
/*
TEST(MoveGeneration, Perft)
{
	Board board;
	
	//time_t startTime = time(0);
	//board.defaults();
	//perft(6, &board, -1);
	//std::cout << (uint64_t)((119060324) / difftime(time(NULL), startTime)) << "\n";
	
	board.defaults();
	EXPECT_EQ(perft(0, &board, -1), 1);
	EXPECT_EQ(perft(1, &board, -1), 20);
	EXPECT_EQ(perft(2, &board, -1), 400);
	EXPECT_EQ(perft(3, &board, -1), 8902);
	EXPECT_EQ(perft(4, &board, -1), 197281);
	
	board = Board();
	board.loadFromFen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
	EXPECT_EQ(perft(0, &board, -1), 1);
	EXPECT_EQ(perft(1, &board, -1), 48);
	EXPECT_EQ(perft(2, &board, -1), 2039);
	EXPECT_EQ(perft(3, &board, -1), 97862);
	EXPECT_EQ(perft(4, &board, -1), 4085603); 
	
	board = Board();
	board.loadFromFen("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -");
	EXPECT_EQ(perft(0, &board, -1), 1);
	EXPECT_EQ(perft(1, &board, -1), 14);
	EXPECT_EQ(perft(2, &board, -1), 191);
	EXPECT_EQ(perft(3, &board, -1), 2812);
	EXPECT_EQ(perft(4, &board, -1), 43238);
	
	board = Board();
	board.loadFromFen("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
	EXPECT_EQ(perft(0, &board, -1), 1);
	EXPECT_EQ(perft(1, &board, -1), 6);
	EXPECT_EQ(perft(2, &board, -1), 264);
	EXPECT_EQ(perft(3, &board, -1), 9467);
	EXPECT_EQ(perft(4, &board, -1), 422333);
	
	board = Board();
	board.loadFromFen("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
	EXPECT_EQ(perft(0, &board, -1), 1);
	EXPECT_EQ(perft(1, &board, -1), 44);
	EXPECT_EQ(perft(2, &board, -1), 1486);
	EXPECT_EQ(perft(3, &board, -1), 62379); 
	EXPECT_EQ(perft(4, &board, -1), 2103487); 
	
	board = Board();
	board.loadFromFen("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");
	EXPECT_EQ(perft(0, &board, -1), 1);
	EXPECT_EQ(perft(1, &board, -1), 46);
	EXPECT_EQ(perft(2, &board, -1), 2079);
	EXPECT_EQ(perft(3, &board, -1), 89890);
	EXPECT_EQ(perft(4, &board, -1), 3894594);
}*/