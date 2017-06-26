#pragma once
#include "gtest/gtest.h"
#include "board.h"
#include "move.h"

TEST(Move, ApplyQuietMoves)
{
	Board board;
	board.whitePawnBitboard = 2;
	board.nextColour = white;
	Move move = Move(1, 9, quietMove, pawn, &board);
	move.applyMove(&board);
	EXPECT_EQ(board.whitePawnBitboard, 1 << 9);
	EXPECT_EQ(board.allPieces, 1 << 9);

	board = Board();
	board.whiteQueenBitboard = 2;
	board.nextColour = white;
	move = Move(1, 7, quietMove, queen, &board);
	move.applyMove(&board);
	EXPECT_EQ(board.whiteQueenBitboard, 1 << 7);
	EXPECT_EQ(board.allPieces, 1 << 7);
}

TEST(Move, ApplyPawnDoubleMoves)
{
	Board board;
	board.whitePawnBitboard = (uint64_t)1 << 8;
	board.nextColour = white;
	Move move = Move(8, 24, pawnDoubleMove, pawn, &board);
	move.applyMove(&board);
	EXPECT_EQ(board.whitePawnBitboard, (uint64_t)1 << 24);
	EXPECT_EQ(board.allPieces, (uint64_t)1 << 24);
	EXPECT_EQ(board.enPassantSquare, 16);

	board = Board();
	board.blackPawnBitboard = (uint64_t)1 << 48;
	board.nextColour = black;
	move = Move(48, 32, pawnDoubleMove, pawn, &board);
	move.applyMove(&board);
	EXPECT_EQ(board.blackPawnBitboard, (uint64_t)1 << 32);
	EXPECT_EQ(board.allPieces, (uint64_t)1 << 32);
	EXPECT_EQ(board.enPassantSquare, 40);
}

TEST(Move, ApplyEnPassantMoves)
{
	Board board;
	board.whitePawnBitboard = 4294967296;
	board.blackPawnBitboard = 8589934592;
	board.enPassantSquare = 41;
	board.nextColour = white;
	Move move = Move(32, 41, capture, pawn, &board);
	move.applyMove(&board);
	EXPECT_EQ(board.whitePawnBitboard, 2199023255552);
	EXPECT_EQ(board.allPieces, 2199023255552);
	EXPECT_EQ(board.enPassantSquare, -1);

	board = Board();
	board.whitePawnBitboard = 16777216;
	board.blackPawnBitboard = 33554432;
	board.enPassantSquare = 16;
	board.nextColour = black;
	move = Move(25, 16, capture, pawn, &board);
	move.applyMove(&board);
	EXPECT_EQ(board.blackPawnBitboard, 65536);
	EXPECT_EQ(board.allPieces, 65536);
	EXPECT_EQ(board.enPassantSquare, -1);

	board = Board();
	board.whitePawnBitboard = 16908288;
	board.blackPawnBitboard = 33554432;
	board.enPassantSquare = 16;
	board.nextColour = black;
	move = Move(25, 16, capture, pawn, &board);
	move.applyMove(&board);
	EXPECT_EQ(board.blackPawnBitboard, 65536);
	EXPECT_EQ(board.whitePawnBitboard, 131072);
	EXPECT_EQ(board.allPieces, 196608);
	EXPECT_EQ(board.enPassantSquare, -1);


}

TEST(Move, ApplyCaptureMoves)
{
	Board board = Board();
	board.whiteQueenBitboard = 2;
	board.blackPawnBitboard = 128;
	board.update();
	board.nextColour = white;
	Move move = Move(1, 7, capture, queen, &board);
	move.applyMove(&board);
	EXPECT_EQ(board.whiteQueenBitboard, 1 << 7);
	EXPECT_EQ(board.allPieces, 1 << 7);

	board = Board();
	board.whitePawnBitboard = 2;
	board.blackPawnBitboard = 256;
	board.update();
	board.nextColour = white;
	move = Move(1, 8, capture, pawn, &board);
	move.applyMove(&board);
	EXPECT_EQ(board.whitePawnBitboard, 1 << 8);
	EXPECT_EQ(board.blackPawnBitboard, 0);
	EXPECT_EQ(board.allPieces, 1 << 8);

	board = Board();
	board.whitePawnBitboard = 34359738368;
	board.blackPawnBitboard = 4398046511104;
	board.update();
	board.nextColour = white;
	move = Move(35, 42, capture, pawn, &board);
	move.applyMove(&board);
	EXPECT_EQ(board.whitePawnBitboard, 4398046511104);
	EXPECT_EQ(board.blackPawnBitboard, 0);
	EXPECT_EQ(board.allPieces, 4398046511104);


	board = Board();
	board.whiteRookBitboard = 9223372036854775808;
	board.blackPawnBitboard = 72057594037927936;
	board.update();
	board.nextColour = white;
	move = Move(63, 56, capture, rook, &board);
	move.applyMove(&board);
	EXPECT_EQ(board.whiteRookBitboard, 72057594037927936);
	EXPECT_EQ(board.blackPawnBitboard, 0);
	EXPECT_EQ(board.allPieces, 72057594037927936);

	board = Board();
	board.whiteQueenBitboard = 4194304;
	board.blackKingBitboard = 64;
	board.update();
	board.nextColour = white;
	move = Move(22, 6, capture, queen, &board);
	move.applyMove(&board);
	EXPECT_EQ(board.whiteQueenBitboard, 64);
	EXPECT_EQ(board.blackKingBitboard, 0);
	EXPECT_EQ(board.allPieces, 64);
}

TEST(Move, ApplyPromotionMoves)
{
	Board board = Board();
	board.whitePawnBitboard = 281474976710656;
	board.update();
	board.nextColour = white;
	Move move = Move(48, 56, queenPromotion, pawn, &board);
	move.applyMove(&board);
	EXPECT_EQ(board.whiteQueenBitboard, 72057594037927936);
	EXPECT_EQ(board.whitePawnBitboard, 0);
	EXPECT_EQ(board.allPieces, 72057594037927936);

	board = Board();
	board.blackPawnBitboard = 256;
	board.update();
	board.nextColour = black;
	move = Move(8, 0, knightPromotion, pawn, &board);
	move.applyMove(&board);
	EXPECT_EQ(board.blackKnightBitboard, 1);
	EXPECT_EQ(board.blackPawnBitboard, 0);
	EXPECT_EQ(board.allPieces, 1);
}

TEST(Move, ApplyCastlingMoves)
{
	Board board = Board();
	board.whiteKingBitboard = 16;
	board.whiteRookBitboard = 1;
	board.update();
	board.nextColour = white;
	Move move = Move(4, 2, queenSideCastling, king, &board);
	move.applyMove(&board);
	EXPECT_EQ(board.whiteKingBitboard, 4);
	EXPECT_EQ(board.whiteRookBitboard, 8);
	EXPECT_EQ(board.allPieces, 12);

	board = Board();
	board.whiteKingBitboard = 16;
	board.whiteRookBitboard = 128;
	board.update();
	board.nextColour = white;
	move = Move(4, 6, kingSideCastling, king, &board);
	move.applyMove(&board);
	EXPECT_EQ(board.whiteKingBitboard, 64);
	EXPECT_EQ(board.whiteRookBitboard, 32);
	EXPECT_EQ(board.allPieces, 96);

	board = Board();
	board.blackKingBitboard = 1152921504606846976;
	board.blackRookBitboard = 72057594037927936;
	board.update();
	board.nextColour = black;
	move = Move(60, 58, queenSideCastling, king, &board);
	move.applyMove(&board);
	EXPECT_EQ(board.blackKingBitboard, 288230376151711744);
	EXPECT_EQ(board.blackRookBitboard, 576460752303423488);
	EXPECT_EQ(board.allPieces, 864691128455135232);

	board = Board();
	board.blackKingBitboard = 1152921504606846976;
	board.blackRookBitboard = 9223372036854775808;
	board.update();
	board.nextColour = black;
	move = Move(60, 62, kingSideCastling, king, &board);
	move.applyMove(&board);
	EXPECT_EQ(board.blackKingBitboard, 4611686018427387904);
	EXPECT_EQ(board.blackRookBitboard, 2305843009213693952);
	EXPECT_EQ(board.allPieces, 6917529027641081856);
}

TEST(Move, UpdateCastlingAvaliability)
{
	Board board = Board();
	board.whiteRookBitboard = 1;
	board.whiteKingBitboard = 16;
	board.canWhiteCastleQueenSide = true;
	board.update();
	board.nextColour = white;
	Move move = Move(0, 8, quietMove, rook, &board);
	move.applyMove(&board);
	EXPECT_EQ(board.canWhiteCastleQueenSide, false);

	board = Board();
	board.whiteRookBitboard = 128;
	board.whiteKingBitboard = 16;
	board.canWhiteCastleKingSide = true;
	board.update();
	board.nextColour = white;
	move = Move(7, 15, quietMove, rook, &board);
	move.applyMove(&board);
	EXPECT_EQ(board.canWhiteCastleKingSide, false);

	board = Board();
	board.whiteRookBitboard = 129;
	board.whiteKingBitboard = 16;
	board.canWhiteCastleQueenSide = true;
	board.canWhiteCastleKingSide = true;
	board.update();
	board.nextColour = white;
	move = Move(4, 5, quietMove, king, &board);
	move.applyMove(&board);
	EXPECT_EQ(board.canWhiteCastleKingSide, false);
	EXPECT_EQ(board.canWhiteCastleQueenSide, false);

	board = Board();
	board.whiteRookBitboard = 72057594037927936;
	board.whiteKingBitboard = 1152921504606846976;
	board.canBlackCastleQueenSide = true;
	board.update();
	board.nextColour = black;
	move = Move(56, 57, quietMove, rook, &board);
	move.applyMove(&board);
	EXPECT_EQ(board.canBlackCastleQueenSide, false);

	board = Board();
	board.whiteRookBitboard = 9223372036854775808;
	board.whiteKingBitboard = 1152921504606846976;
	board.canBlackCastleKingSide = true;
	board.update();
	board.nextColour = black;
	move = Move(63, 62, quietMove, rook, &board);
	move.applyMove(&board);
	EXPECT_EQ(board.canBlackCastleKingSide, false);

	board = Board();
	board.whiteRookBitboard = 9295429630892703744;
	board.whiteKingBitboard = 1152921504606846976;
	board.canBlackCastleKingSide = true;
	board.canBlackCastleQueenSide = true;
	board.update();
	board.nextColour = black;
	move = Move(60, 59, quietMove, king, &board);
	move.applyMove(&board);
	EXPECT_EQ(board.canBlackCastleKingSide, false);
	EXPECT_EQ(board.canBlackCastleQueenSide, false);
}
