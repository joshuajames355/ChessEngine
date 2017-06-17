#pragma once
#include "gtest/gtest.h"
#include "board.h"

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

TEST(Board, loadFromFen)
{
	Board defaultBoard;
	defaultBoard.defaults();
	Board fenBoard;
	fenBoard.loadFromFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

	EXPECT_EQ(defaultBoard.allPieces, fenBoard.allPieces);
	EXPECT_EQ(defaultBoard.blackBishopBitboard, fenBoard.blackBishopBitboard);
	EXPECT_EQ(defaultBoard.blackKingBitboard, fenBoard.blackKingBitboard);
	EXPECT_EQ(defaultBoard.blackKnightBitboard, fenBoard.blackKnightBitboard);
	EXPECT_EQ(defaultBoard.blackPawnBitboard, fenBoard.blackPawnBitboard);
	EXPECT_EQ(defaultBoard.blackPieces, fenBoard.blackPieces);
	EXPECT_EQ(defaultBoard.blackQueenBitboard, fenBoard.blackQueenBitboard);
	EXPECT_EQ(defaultBoard.blackRookBitboard, fenBoard.blackRookBitboard);
	EXPECT_EQ(defaultBoard.whiteBishopBitboard, fenBoard.whiteBishopBitboard);
	EXPECT_EQ(defaultBoard.whiteKingBitboard, fenBoard.whiteKingBitboard);
	EXPECT_EQ(defaultBoard.whiteKnightBitboard, fenBoard.whiteKnightBitboard);
	EXPECT_EQ(defaultBoard.whitePawnBitboard, fenBoard.whitePawnBitboard);
	EXPECT_EQ(defaultBoard.whitePieces, fenBoard.whitePieces);
	EXPECT_EQ(defaultBoard.whiteQueenBitboard, fenBoard.whiteQueenBitboard);
	EXPECT_EQ(defaultBoard.whiteRookBitboard, fenBoard.whiteRookBitboard);
	EXPECT_EQ(fenBoard.enPassantSquare, -1);

	Board board = Board();
	board.whitePawnBitboard = 1;
	board.update();
	fenBoard = Board();
	fenBoard.loadFromFen("8/8/8/8/8/8/8/P7 w KQkq - 0 1");

	EXPECT_EQ(board.allPieces, fenBoard.allPieces);
	EXPECT_EQ(board.blackBishopBitboard, fenBoard.blackBishopBitboard);
	EXPECT_EQ(board.blackKingBitboard, fenBoard.blackKingBitboard);
	EXPECT_EQ(board.blackKnightBitboard, fenBoard.blackKnightBitboard);
	EXPECT_EQ(board.blackPawnBitboard, fenBoard.blackPawnBitboard);
	EXPECT_EQ(board.blackPieces, fenBoard.blackPieces);
	EXPECT_EQ(board.blackQueenBitboard, fenBoard.blackQueenBitboard);
	EXPECT_EQ(board.blackRookBitboard, fenBoard.blackRookBitboard);
	EXPECT_EQ(board.whiteBishopBitboard, fenBoard.whiteBishopBitboard);
	EXPECT_EQ(board.whiteKingBitboard, fenBoard.whiteKingBitboard);
	EXPECT_EQ(board.whiteKnightBitboard, fenBoard.whiteKnightBitboard);
	EXPECT_EQ(board.whitePawnBitboard, fenBoard.whitePawnBitboard);
	EXPECT_EQ(board.whitePieces, fenBoard.whitePieces);
	EXPECT_EQ(board.whiteQueenBitboard, fenBoard.whiteQueenBitboard);
	EXPECT_EQ(fenBoard.enPassantSquare, -1);

	board = Board();
	board.whitePawnBitboard = 16777216;
	board.blackPawnBitboard = 33554432;
	board.update();
	fenBoard = Board();
	fenBoard.loadFromFen("8/8/8/8/Pp6/8/8/8 b - a3 0 1");

	EXPECT_EQ(board.allPieces, fenBoard.allPieces);
	EXPECT_EQ(board.blackBishopBitboard, fenBoard.blackBishopBitboard);
	EXPECT_EQ(board.blackKingBitboard, fenBoard.blackKingBitboard);
	EXPECT_EQ(board.blackKnightBitboard, fenBoard.blackKnightBitboard);
	EXPECT_EQ(board.blackPawnBitboard, fenBoard.blackPawnBitboard);
	EXPECT_EQ(board.blackPieces, fenBoard.blackPieces);
	EXPECT_EQ(board.blackQueenBitboard, fenBoard.blackQueenBitboard);
	EXPECT_EQ(board.blackRookBitboard, fenBoard.blackRookBitboard);
	EXPECT_EQ(board.whiteBishopBitboard, fenBoard.whiteBishopBitboard);
	EXPECT_EQ(board.whiteKingBitboard, fenBoard.whiteKingBitboard);
	EXPECT_EQ(board.whiteKnightBitboard, fenBoard.whiteKnightBitboard);
	EXPECT_EQ(board.whitePawnBitboard, fenBoard.whitePawnBitboard);
	EXPECT_EQ(board.whitePieces, fenBoard.whitePieces);
	EXPECT_EQ(board.whiteQueenBitboard, fenBoard.whiteQueenBitboard);
	EXPECT_EQ(board.whiteRookBitboard, fenBoard.whiteRookBitboard);
	EXPECT_EQ(board.whiteRookBitboard, fenBoard.whiteRookBitboard);
	EXPECT_EQ(fenBoard.enPassantSquare, 16);
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

