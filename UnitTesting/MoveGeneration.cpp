#pragma once
#include "gtest/gtest.h"
#include "move.h"
#include "moveGeneration.h"
#include "magicBitboards.h"
#include "utils.h"

TEST(MoveGeneration, PawnMoves)
{
	Board board = Board();
	board.whitePawnBitboard = 2;
	board.update();
	board.nextColour = white;
	std::vector<Move> Movelist = searchForMoves(&board);
	EXPECT_EQ(Movelist.size(), 1);
	EXPECT_EQ(Movelist[0].from, 1);
	EXPECT_EQ(Movelist[0].to, 9);
	EXPECT_EQ(Movelist[0].piece, pawn);
	EXPECT_EQ(Movelist[0].moveType, quietMove);

	board = Board();
	board.whitePawnBitboard = 256;
	board.update();
	board.nextColour = white;
	Movelist = searchForMoves(&board);
	EXPECT_EQ(Movelist.size(), 2);
	EXPECT_EQ(Movelist[0].from, 8);
	EXPECT_EQ(Movelist[1].from, 8);
	if (Movelist[0].to == 16)
	{
		EXPECT_EQ(Movelist[0].piece, pawn);
		EXPECT_EQ(Movelist[0].moveType, quietMove);

		EXPECT_EQ(Movelist[1].piece, pawn);
		EXPECT_EQ(Movelist[1].moveType, pawnDoubleMove);
		EXPECT_EQ(Movelist[1].to, 24);
	}
	else
	{
		EXPECT_EQ(Movelist[1].piece, pawn);
		EXPECT_EQ(Movelist[1].moveType, quietMove);

		EXPECT_EQ(Movelist[0].piece, pawn);
		EXPECT_EQ(Movelist[0].moveType, pawnDoubleMove);
		EXPECT_EQ(Movelist[0].to, 24);
	}

	board = Board();
	board.whitePawnBitboard = 1;
	board.blackKingBitboard = 256;
	board.blackPawnBitboard = 512;
	board.update();
	board.nextColour = white;
	Movelist = searchForMoves(&board);
	EXPECT_EQ(Movelist.size(), 1);
	EXPECT_EQ(Movelist[0].from, 0);
	EXPECT_EQ(Movelist[0].to, 9);
	EXPECT_EQ(Movelist[0].piece, pawn);
	EXPECT_EQ(Movelist[0].moveType, capture);

	board = Board();
	board.whitePawnBitboard = 32768;
	board.blackKingBitboard = 4194304;
	board.blackPawnBitboard = 8388608;
	board.update();
	board.nextColour = white;
	Movelist = searchForMoves(&board);
	EXPECT_EQ(Movelist.size(), 1);
	EXPECT_EQ(Movelist[0].from, 15);
	EXPECT_EQ(Movelist[0].to, 22);
	EXPECT_EQ(Movelist[0].piece, pawn);
	EXPECT_EQ(Movelist[0].moveType, capture);

	board = Board();
	board.whitePawnBitboard = 281474976710656;
	board.update();
	board.nextColour = white;
	Movelist = searchForMoves(&board);
	EXPECT_EQ(Movelist.size(), 4);
	EXPECT_EQ(Movelist[0].from, 48);
	EXPECT_EQ(Movelist[0].to, 56);
	EXPECT_EQ(Movelist[0].piece, pawn);

	board = Board();
	board.blackPawnBitboard = 256;
	board.update();
	board.nextColour = black;
	Movelist = searchForMoves(&board);
	EXPECT_EQ(Movelist.size(), 4);
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
	board.nextColour = white;
	generateKingMoves(&board, movelist, board.whitePieces, board.blackPieces);

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
	board.nextColour = white;
	generateKingMoves(&board, movelist, board.whitePieces, board.blackPieces);

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
	board.nextColour = black;
	generateKingMoves(&board, movelist, board.blackPieces, board.whitePieces);

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
	board.nextColour = white;

	generateKnightMoves(&board, movelist, board.whitePieces, board.blackPieces);
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
	board.nextColour = white;

	generateKnightMoves(&board, movelist, board.whitePieces, board.blackPieces);
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
	board.nextColour = white;

	generateRookMoves(&board, movelist, board.whitePieces, board.blackPieces);
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
	board.nextColour = white;

	generateRookMoves(&board, movelist, board.whitePieces, board.blackPieces);
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
	board.nextColour = white;

	generateBishopMoves(&board, movelist, board.whitePieces, board.blackPieces);
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
	board.nextColour = white;

	generateBishopMoves(&board, movelist, board.whitePieces, board.blackPieces);
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
	board.nextColour = white;

	generateBishopMoves(&board, movelist, board.whitePieces, board.blackPieces);
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
	board.nextColour = white;

	generateBishopMoves(&board, movelist, board.whitePieces, board.blackPieces);
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
	board.nextColour = white;

	generateQueenMoves(&board, movelist, board.whitePieces, board.blackPieces);
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
	board.nextColour = white;

	generateQueenMoves(&board, movelist, board.whitePieces, board.blackPieces);
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
	board.nextColour = white;

	generateQueenMoves(&board, movelist, board.whitePieces, board.blackPieces);
	EXPECT_EQ(movelist.size(), 1);
	EXPECT_EQ(movelist[0].from, 0);
	EXPECT_EQ(movelist[0].to, 1);
	EXPECT_EQ(movelist[0].piece, queen);
	EXPECT_EQ(movelist[0].moveType, quietMove);

	movelist = std::vector<Move>();
	board = Board();
	board.whiteQueenBitboard = 2097152;
	board.whitePawnBitboard = 1615884288;
	board.blackKnightBitboard = 268435456;
	board.update();
	board.nextColour = white;

	generateQueenMoves(&board, movelist, board.whitePieces, board.blackPieces);
	EXPECT_EQ(movelist.size(), 1);
	EXPECT_EQ(movelist[0].from, 21);
	EXPECT_EQ(movelist[0].to, 28);
	EXPECT_EQ(movelist[0].piece, queen);
	EXPECT_EQ(movelist[0].moveType, capture);

	movelist = std::vector<Move>();
	board = Board();
	board.whiteQueenBitboard = 67108864;
	board.whitePawnBitboard = 60264677376;
	board.blackKnightBitboard = 16777216;
	board.update();
	board.nextColour = white;

	generateQueenMoves(&board, movelist, board.whitePieces, board.blackPieces);
	EXPECT_EQ(movelist.size(), 2);
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


	movelist = std::vector<Move>();
	board = Board();
	board.whiteQueenBitboard = 4194304;
	board.whitePawnBitboard = 3768623104;
	board.blackKingBitboard = 64;
	board.update();
	board.nextColour = white;

	generateQueenMoves(&board, movelist, board.whitePieces, board.blackPieces);
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

TEST(MoveGeneration, CastlingMoves)
{
	std::vector<Move> movelist = std::vector<Move>();
	Board board = Board();
	board.whiteKingBitboard = 16;
	board.whiteRookBitboard = 128;
	board.canWhiteCastleKingSide = true;
	board.update();
	board.nextColour = white;
	generateCastlingMoves(&board, movelist, board.whitePieces, board.blackPieces);
	EXPECT_EQ(movelist.size(), 1);
	EXPECT_EQ(movelist[0].from, 4);
	EXPECT_EQ(movelist[0].to, 6);
	EXPECT_EQ(movelist[0].piece, king);
	EXPECT_EQ(movelist[0].moveType, kingSideCastling);

	movelist = std::vector<Move>();
	board = Board();
	board.whiteKingBitboard = 16;
	board.whiteRookBitboard = 1;
	board.canWhiteCastleQueenSide = true;
	board.update();
	board.nextColour = white;
	generateCastlingMoves(&board, movelist, board.whitePieces, board.blackPieces);
	EXPECT_EQ(movelist.size(), 1);
	EXPECT_EQ(movelist[0].from, 4);
	EXPECT_EQ(movelist[0].to, 2);
	EXPECT_EQ(movelist[0].piece, king);
	EXPECT_EQ(movelist[0].moveType, queenSideCastling);

	movelist = std::vector<Move>();
	board = Board();
	board.blackKingBitboard = 1152921504606846976;
	board.blackRookBitboard = 9223372036854775808;
	board.canBlackCastleKingSide = true;
	board.update();
	board.nextColour = black;
	generateCastlingMoves(&board, movelist, board.whitePieces, board.blackPieces);
	EXPECT_EQ(movelist.size(), 1);
	EXPECT_EQ(movelist[0].from, 60);
	EXPECT_EQ(movelist[0].to, 62);
	EXPECT_EQ(movelist[0].piece, king);
	EXPECT_EQ(movelist[0].moveType, kingSideCastling);

	movelist = std::vector<Move>();
	board = Board();
	board.blackKingBitboard = 1152921504606846976;
	board.blackRookBitboard = 72057594037927936;
	board.canBlackCastleQueenSide = true;
	board.update();
	board.nextColour = black;
	generateCastlingMoves(&board, movelist, board.whitePieces, board.blackPieces);
	EXPECT_EQ(movelist.size(), 1);
	EXPECT_EQ(movelist[0].from, 60);
	EXPECT_EQ(movelist[0].to, 58);
	EXPECT_EQ(movelist[0].piece, king);
	EXPECT_EQ(movelist[0].moveType, queenSideCastling);
}

uint64_t perft(int depth, Board* board, int divide)
{
	std::vector<Move> moveList;

	moveList = searchForMoves(board);
	int perftVal;
	uint64_t nodes = 0;

	if (depth == 0) return 1;

	for (int x = 0; x < moveList.size(); x++)
	{
		Board newBoard = moveList[x].applyMove(board);
		if (board->nextColour == white)
		{
			if (!newBoard.isPieceAttacked(bitScanForward(newBoard.whiteKingBitboard),board->nextColour))
			{
				perftVal = perft(depth - 1, &newBoard, divide);
				nodes += perftVal;
				if (depth == divide) std::cout << notationFromMove(moveList[x]) << " " << perftVal << "\n";
			}
		}
		else
		{
			if (!newBoard.isPieceAttacked(bitScanForward(newBoard.blackKingBitboard), board->nextColour))
			{
				perftVal = perft(depth - 1, &newBoard, divide);
				nodes += perftVal;
				if (depth == divide) std::cout << notationFromMove(moveList[x]) << " " << perftVal << "\n";
			}
		}
	}
	return nodes;
}

TEST(MoveGeneration, Perft)
{
	Board board;
	
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
}