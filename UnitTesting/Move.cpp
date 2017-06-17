#pragma once
#include "gtest/gtest.h"
#include "board.h"
#include "move.h"

TEST(Move, ApplyQuietMoves)
{
	Move move = Move(1, 9, quietMove, pawn);
	Board board;
	board.whitePawnBitboard = 2;
	board.nextColour = white;
	Board moveBoard = move.applyMove(&board);
	EXPECT_EQ(moveBoard.whitePawnBitboard, 1 << 9);
	EXPECT_EQ(moveBoard.allPieces, 1 << 9);

	board = Board();
	move = Move(1, 7, quietMove, queen);
	board.whiteQueenBitboard = 2;
	board.nextColour = white;
	moveBoard = move.applyMove(&board);
	EXPECT_EQ(moveBoard.whiteQueenBitboard, 1 << 7);
	EXPECT_EQ(moveBoard.allPieces, 1 << 7);
}

TEST(Move, ApplyPawnDoubleMoves)
{
	Move move = Move(8, 24, pawnDoubleMove, pawn);
	Board board;
	board.whitePawnBitboard = (uint64_t)1 << 8;
	board.nextColour = white;
	Board moveBoard = move.applyMove(&board);
	EXPECT_EQ(moveBoard.whitePawnBitboard, (uint64_t)1 << 24);
	EXPECT_EQ(moveBoard.allPieces, (uint64_t)1 << 24);
	EXPECT_EQ(moveBoard.enPassantSquare, 16);

	move = Move(48, 32, pawnDoubleMove, pawn);
	board = Board();
	board.blackPawnBitboard = (uint64_t)1 << 48;
	board.nextColour = black;
	moveBoard = move.applyMove(&board);
	EXPECT_EQ(moveBoard.blackPawnBitboard, (uint64_t)1 << 32);
	EXPECT_EQ(moveBoard.allPieces, (uint64_t)1 << 32);
	EXPECT_EQ(moveBoard.enPassantSquare, 40);
}

TEST(Move, ApplyEnPassantMoves)
{
	Move move = Move(32, 41, capture, pawn);
	Board board;
	board.whitePawnBitboard = 4294967296;
	board.blackPawnBitboard = 8589934592;
	board.enPassantSquare = 41;
	board.nextColour = white;
	Board moveBoard = move.applyMove(&board);
	EXPECT_EQ(moveBoard.whitePawnBitboard, 2199023255552);
	EXPECT_EQ(moveBoard.allPieces, 2199023255552);
	EXPECT_EQ(moveBoard.enPassantSquare, -1);

	move = Move(25, 16, capture, pawn);
	board = Board();
	board.whitePawnBitboard = 16777216;
	board.blackPawnBitboard = 33554432;
	board.enPassantSquare = 16;
	board.nextColour = black;
	moveBoard = move.applyMove(&board);
	EXPECT_EQ(moveBoard.blackPawnBitboard, 65536);
	EXPECT_EQ(moveBoard.allPieces, 65536);
	EXPECT_EQ(moveBoard.enPassantSquare, -1);

	move = Move(25, 16, capture, pawn);
	board = Board();
	board.whitePawnBitboard = 16908288;
	board.blackPawnBitboard = 33554432;
	board.enPassantSquare = 16;
	board.nextColour = black;
	moveBoard = move.applyMove(&board);
	EXPECT_EQ(moveBoard.blackPawnBitboard, 65536);
	EXPECT_EQ(moveBoard.whitePawnBitboard, 131072);
	EXPECT_EQ(moveBoard.allPieces, 196608);
	EXPECT_EQ(moveBoard.enPassantSquare, -1);


}

TEST(Move, ApplyCaptureMoves)
{
	Board board = Board();
	Move move = Move(1, 7, capture, queen);
	board.whiteQueenBitboard = 2;
	board.blackPawnBitboard = 128;
	board.update();
	board.nextColour = white;
	Board moveBoard = move.applyMove(&board);
	EXPECT_EQ(moveBoard.whiteQueenBitboard, 1 << 7);
	EXPECT_EQ(moveBoard.allPieces, 1 << 7);

	board = Board();
	move = Move(1, 8, capture, pawn);
	board.whitePawnBitboard = 2;
	board.blackPawnBitboard = 256;
	board.update();
	board.nextColour = white;
	moveBoard = move.applyMove(&board);
	EXPECT_EQ(moveBoard.whitePawnBitboard, 1 << 8);
	EXPECT_EQ(moveBoard.blackPawnBitboard, 0);
	EXPECT_EQ(moveBoard.allPieces, 1 << 8);

	board = Board();
	move = Move(35, 42, capture, pawn);
	board.whitePawnBitboard = 34359738368;
	board.blackPawnBitboard = 4398046511104;
	board.update();
	board.nextColour = white;
	moveBoard = move.applyMove(&board);
	EXPECT_EQ(moveBoard.whitePawnBitboard, 4398046511104);
	EXPECT_EQ(moveBoard.blackPawnBitboard, 0);
	EXPECT_EQ(moveBoard.allPieces, 4398046511104);


	board = Board();
	move = Move(63, 56, capture, rook);
	board.whiteRookBitboard = 9223372036854775808;
	board.blackPawnBitboard = 72057594037927936;
	board.update();
	board.nextColour = white;
	moveBoard = move.applyMove(&board);
	EXPECT_EQ(moveBoard.whiteRookBitboard, 72057594037927936);
	EXPECT_EQ(moveBoard.blackPawnBitboard, 0);
	EXPECT_EQ(moveBoard.allPieces, 72057594037927936);

	board = Board();
	move = Move(22, 6, capture, queen);
	board.whiteQueenBitboard = 4194304;
	board.blackKingBitboard = 64;
	board.update();
	board.nextColour = white;
	moveBoard = move.applyMove(&board);
	EXPECT_EQ(moveBoard.whiteQueenBitboard, 64);
	EXPECT_EQ(moveBoard.blackKingBitboard, 0);
	EXPECT_EQ(moveBoard.allPieces, 64);
}

TEST(Move, ApplyPromotionMoves)
{
	Board board = Board();
	Move move = Move(48, 56, queenPromotion, pawn);
	board.whitePawnBitboard = 281474976710656;
	board.update();
	board.nextColour = white;
	Board moveBoard = move.applyMove(&board);
	EXPECT_EQ(moveBoard.whiteQueenBitboard, 72057594037927936);
	EXPECT_EQ(moveBoard.whitePawnBitboard, 0);
	EXPECT_EQ(moveBoard.allPieces, 72057594037927936);

	board = Board();
	move = Move(8, 0, knightPromotion, pawn);
	board.blackPawnBitboard = 256;
	board.update();
	board.nextColour = black;
	moveBoard = move.applyMove(&board);
	EXPECT_EQ(moveBoard.blackKnightBitboard, 1);
	EXPECT_EQ(moveBoard.blackPawnBitboard, 0);
	EXPECT_EQ(moveBoard.allPieces, 1);
}

TEST(Move, ApplyCastlingMoves)
{
	Board board = Board();
	Move move = Move(4, 2, queenSideCastling, king);
	board.whiteKingBitboard = 16;
	board.whiteRookBitboard = 1;
	board.update();
	board.nextColour = white;
	Board moveBoard = move.applyMove(&board);
	EXPECT_EQ(moveBoard.whiteKingBitboard, 4);
	EXPECT_EQ(moveBoard.whiteRookBitboard, 8);
	EXPECT_EQ(moveBoard.allPieces, 12);

	board = Board();
	move = Move(4, 6, kingSideCastling, king);
	board.whiteKingBitboard = 16;
	board.whiteRookBitboard = 128;
	board.update();
	board.nextColour = white;
	moveBoard = move.applyMove(&board);
	EXPECT_EQ(moveBoard.whiteKingBitboard, 64);
	EXPECT_EQ(moveBoard.whiteRookBitboard, 32);
	EXPECT_EQ(moveBoard.allPieces, 96);

	board = Board();
	move = Move(60, 58, queenSideCastling, king);
	board.blackKingBitboard = 1152921504606846976;
	board.blackRookBitboard = 72057594037927936;
	board.update();
	board.nextColour = black;
	moveBoard = move.applyMove(&board);
	EXPECT_EQ(moveBoard.blackKingBitboard, 288230376151711744);
	EXPECT_EQ(moveBoard.blackRookBitboard, 576460752303423488);
	EXPECT_EQ(moveBoard.allPieces, 864691128455135232);

	board = Board();
	move = Move(60, 62, kingSideCastling, king);
	board.blackKingBitboard = 1152921504606846976;
	board.blackRookBitboard = 9223372036854775808;
	board.update();
	board.nextColour = black;
	moveBoard = move.applyMove(&board);
	EXPECT_EQ(moveBoard.blackKingBitboard, 4611686018427387904);
	EXPECT_EQ(moveBoard.blackRookBitboard, 2305843009213693952);
	EXPECT_EQ(moveBoard.allPieces, 6917529027641081856);
}

TEST(Move, UpdateCastlingAvaliability)
{
	Board board = Board();
	Move move = Move(0, 8, quietMove, rook);
	board.whiteRookBitboard = 1;
	board.whiteKingBitboard = 16;
	board.canWhiteCastleQueenSide = true;
	board.update();
	board.nextColour = white;
	Board moveBoard = move.applyMove(&board);
	EXPECT_EQ(moveBoard.canWhiteCastleQueenSide, false);

	board = Board();
	move = Move(7, 15, quietMove, rook);
	board.whiteRookBitboard = 128;
	board.whiteKingBitboard = 16;
	board.canWhiteCastleKingSide = true;
	board.update();
	board.nextColour = white;
	moveBoard = move.applyMove(&board);
	EXPECT_EQ(moveBoard.canWhiteCastleKingSide, false);

	board = Board();
	move = Move(4, 5, quietMove, king);
	board.whiteRookBitboard = 129;
	board.whiteKingBitboard = 16;
	board.canWhiteCastleQueenSide = true;
	board.canWhiteCastleKingSide = true;
	board.update();
	board.nextColour = white;
	moveBoard = move.applyMove(&board);
	EXPECT_EQ(moveBoard.canWhiteCastleKingSide, false);
	EXPECT_EQ(moveBoard.canWhiteCastleQueenSide, false);

	board = Board();
	move = Move(56, 57, quietMove, rook);
	board.whiteRookBitboard = 72057594037927936;
	board.whiteKingBitboard = 1152921504606846976;
	board.canBlackCastleQueenSide = true;
	board.update();
	board.nextColour = black;
	moveBoard = move.applyMove(&board);
	EXPECT_EQ(moveBoard.canBlackCastleQueenSide, false);

	board = Board();
	move = Move(63, 62, quietMove, rook);
	board.whiteRookBitboard = 9223372036854775808;
	board.whiteKingBitboard = 1152921504606846976;
	board.canBlackCastleKingSide = true;
	board.update();
	board.nextColour = black;
	moveBoard = move.applyMove(&board);
	EXPECT_EQ(moveBoard.canBlackCastleKingSide, false);

	board = Board();
	move = Move(60, 59, quietMove, king);
	board.whiteRookBitboard = 9295429630892703744;
	board.whiteKingBitboard = 1152921504606846976;
	board.canBlackCastleKingSide = true;
	board.canBlackCastleQueenSide = true;
	board.update();
	board.nextColour = black;
	moveBoard = move.applyMove(&board);
	EXPECT_EQ(moveBoard.canBlackCastleKingSide, false);
	EXPECT_EQ(moveBoard.canBlackCastleQueenSide, false);
}
