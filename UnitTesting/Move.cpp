#pragma once
#include "gtest/gtest.h"
#include "board.h"
#include "move.h"
#include "utils.h"

TEST(Move, ApplyQuietMoves)
{
	Board board;
	board.setBitboard(white, pawn, 2);
	board.nextColour = white;
	Move move = Move(1, 9, quietMove, pawn, &board);
	move.applyMove(&board);
	EXPECT_EQ(board.getPieceBitboard(white,pawn), 1 << 9);
	EXPECT_EQ(board.getPieceBitboard(white, pawn), 1 << 9);

	board = Board();
	board.setBitboard(white, queen, 2);
	board.nextColour = white;
	move = Move(1, 7, quietMove, queen, &board);
	move.applyMove(&board);
	EXPECT_EQ(board.getPieceBitboard(white, queen), 1 << 7);
	EXPECT_EQ(board.getPieceBitboard(white, queen), 1 << 7);
}

TEST(Move, ApplyPawnDoubleMoves)
{
	Board board;
	board.setBitboard(white, pawn, (uint64_t)1 << 8);
	board.nextColour = white;
	Move move = Move(8, 24, quietMove, pawn, &board);
	move.applyMove(&board);
	EXPECT_EQ(board.getPieceBitboard(white, pawn), (uint64_t)1 << 24);
	EXPECT_EQ(board.allPieces, (uint64_t)1 << 24);
	EXPECT_EQ(board.enPassantSquare, 16);

	board = Board();
	board.setBitboard(black, pawn, (uint64_t)1 << 48);
	board.nextColour = black;
	move = Move(48, 32, quietMove, pawn, &board);
	move.applyMove(&board);
	EXPECT_EQ(board.getPieceBitboard(black, pawn), (uint64_t)1 << 32);
	EXPECT_EQ(board.allPieces, (uint64_t)1 << 32);
	EXPECT_EQ(board.enPassantSquare, 40);
}

TEST(Move, ApplyEnPassantMoves)
{
	Board board;
	board.setBitboard(white, pawn, 4294967296);
	board.setBitboard(black, pawn, 8589934592);
	board.enPassantSquare = 41;
	board.nextColour = white;
	Move move = Move(32, 41, capture, pawn, &board);
	move.applyMove(&board);
	EXPECT_EQ(board.getPieceBitboard(white, pawn), 2199023255552);
	EXPECT_EQ(board.allPieces, 2199023255552);
	EXPECT_EQ(board.enPassantSquare, -1);

	board = Board();
	board.setBitboard(white, pawn, 16777216);
	board.setBitboard(black, pawn, 33554432);
	board.enPassantSquare = 16;
	board.nextColour = black;
	move = Move(25, 16, capture, pawn, &board);
	move.applyMove(&board);
	EXPECT_EQ(board.getPieceBitboard(black, pawn), 65536);
	EXPECT_EQ(board.allPieces, 65536);
	EXPECT_EQ(board.enPassantSquare, -1);

	board = Board();
	board.setBitboard(white, pawn, 16908288);
	board.setBitboard(black, pawn, 33554432);
	board.enPassantSquare = 16;
	board.nextColour = black;
	move = Move(25, 16, capture, pawn, &board);
	move.applyMove(&board);
	EXPECT_EQ(board.getPieceBitboard(black, pawn), 65536);
	EXPECT_EQ(board.getPieceBitboard(white, pawn), 131072);
	EXPECT_EQ(board.allPieces, 196608);
	EXPECT_EQ(board.enPassantSquare, -1);


}

TEST(Move, ApplyCaptureMoves)
{
	Board board = Board();
	board.setBitboard(white, queen, 2);
	board.setBitboard(black, pawn, 128);
	board.update();
	board.nextColour = white;
	Move move = Move(1, 7, capture, queen, &board);
	move.applyMove(&board);
	EXPECT_EQ(board.getPieceBitboard(white,queen), 1 << 7);
	EXPECT_EQ(board.allPieces, 1 << 7);

	board = Board();
	board.setBitboard(white, pawn, 2);
	board.setBitboard(black, pawn, 256);
	board.update();
	board.nextColour = white;
	move = Move(1, 8, capture, pawn, &board);
	move.applyMove(&board);
	EXPECT_EQ(board.getPieceBitboard(white, pawn), 1 << 8);
	EXPECT_EQ(board.getPieceBitboard(black, pawn), 0);
	EXPECT_EQ(board.allPieces, 1 << 8);

	board = Board();
	board.setBitboard(white, pawn, 34359738368);
	board.setBitboard(black, pawn, 4398046511104);
	board.update();
	board.nextColour = white;
	move = Move(35, 42, capture, pawn, &board);
	move.applyMove(&board);
	EXPECT_EQ(board.getPieceBitboard(white, pawn), 4398046511104);
	EXPECT_EQ(board.getPieceBitboard(black, pawn), 0);
	EXPECT_EQ(board.allPieces, 4398046511104);


	board = Board();
	board.setBitboard(white, rook, 9223372036854775808);
	board.setBitboard(black, pawn, 72057594037927936);
	board.update();
	board.nextColour = white;
	move = Move(63, 56, capture, rook, &board);
	move.applyMove(&board);
	EXPECT_EQ(board.getPieceBitboard(white,rook), 72057594037927936);
	EXPECT_EQ(board.getPieceBitboard(black, pawn), 0);
	EXPECT_EQ(board.allPieces, 72057594037927936);

	board = Board();
	board.setBitboard(white, queen, 4194304);
	board.setBitboard(black, king, 64);
	board.update();
	board.nextColour = white;
	move = Move(22, 6, capture, queen, &board);
	move.applyMove(&board);
	EXPECT_EQ(board.getPieceBitboard(white, queen), 64);
	EXPECT_EQ(board.getPieceBitboard(black, king), 0);
	EXPECT_EQ(board.allPieces, 64);
}

TEST(Move, ApplyPromotionMoves)
{
	Board board = Board();
	board.setBitboard(white, pawn, 281474976710656);
	board.update();
	board.nextColour = white;
	Move move = Move(48, 56, queenPromotion, pawn, &board);
	move.applyMove(&board);
	EXPECT_EQ(board.getPieceBitboard(white, queen), 72057594037927936);
	EXPECT_EQ(board.getPieceBitboard(white, pawn), 0);
	EXPECT_EQ(board.allPieces, 72057594037927936);

	board = Board();
	board.setBitboard(black, pawn, 256);
	board.update();
	board.nextColour = black;
	move = Move(8, 0, knightPromotion, pawn, &board);
	move.applyMove(&board);
	EXPECT_EQ(board.getPieceBitboard(black, knight), 1);
	EXPECT_EQ(board.getPieceBitboard(black, pawn), 0);
	EXPECT_EQ(board.allPieces, 1);
}

TEST(Move, ApplyCastlingMoves)
{
	Board board = Board();
	board.setBitboard(white, king, 16);
	board.setBitboard(white, rook, 1);
	board.update();
	board.nextColour = white;
	Move move = Move(4, 2, queenSideCastling, king, &board);
	move.applyMove(&board);
	EXPECT_EQ(board.getPieceBitboard(white, king), 4);
	EXPECT_EQ(board.getPieceBitboard(white, rook), 8);
	EXPECT_EQ(board.allPieces, 12);

	board = Board();
	board.setBitboard(white, king, 16);
	board.setBitboard(white, rook, 128);
	board.update();
	board.nextColour = white;
	move = Move(4, 6, kingSideCastling, king, &board);
	move.applyMove(&board);
	EXPECT_EQ(board.getPieceBitboard(white, king), 64);
	EXPECT_EQ(board.getPieceBitboard(white, rook), 32);
	EXPECT_EQ(board.allPieces, 96);

	board = Board();
	board.setBitboard(black, king, 1152921504606846976);
	board.setBitboard(black, rook, 72057594037927936);
	board.update();
	board.nextColour = black;
	move = Move(60, 58, queenSideCastling, king, &board);
	move.applyMove(&board);
	EXPECT_EQ(board.getPieceBitboard(black, king), 288230376151711744);
	EXPECT_EQ(board.getPieceBitboard(black, rook), 576460752303423488);
	EXPECT_EQ(board.allPieces, 864691128455135232);

	board = Board();
	board.setBitboard(black, king, 1152921504606846976);
	board.setBitboard(black, rook, 9223372036854775808);
	board.update();
	board.nextColour = black;
	move = Move(60, 62, kingSideCastling, king, &board);
	move.applyMove(&board);
	EXPECT_EQ(board.getPieceBitboard(black, king), 4611686018427387904);
	EXPECT_EQ(board.getPieceBitboard(black, rook), 2305843009213693952);
	EXPECT_EQ(board.allPieces, 6917529027641081856);
}

TEST(Move, UpdateCastlingAvaliability)
{
	Board board = Board();
	board.setBitboard(white, rook, 1);
	board.setBitboard(white, king, 16);
	board.canWhiteCastleQueenSide = true;
	board.update();
	board.nextColour = white;
	Move move = Move(0, 8, quietMove, rook, &board);
	move.applyMove(&board);
	EXPECT_EQ(board.canWhiteCastleQueenSide, false);

	board = Board();
	board.setBitboard(white, rook, 128);
	board.setBitboard(white, king, 16);
	board.canWhiteCastleKingSide = true;
	board.update();
	board.nextColour = white;
	move = Move(7, 15, quietMove, rook, &board);
	move.applyMove(&board);
	EXPECT_EQ(board.canWhiteCastleKingSide, false);

	board = Board();
	board.setBitboard(white, rook, 129);
	board.setBitboard(white, king, 16);
	board.canWhiteCastleQueenSide = true;
	board.canWhiteCastleKingSide = true;
	board.update();
	board.nextColour = white;
	move = Move(4, 5, quietMove, king, &board);
	move.applyMove(&board);
	EXPECT_EQ(board.canWhiteCastleKingSide, false);
	EXPECT_EQ(board.canWhiteCastleQueenSide, false);

	board = Board();
	board.setBitboard(white, rook, 72057594037927936);
	board.setBitboard(white, king, 1152921504606846976);
	board.canBlackCastleQueenSide = true;
	board.update();
	board.nextColour = black;
	move = Move(56, 57, quietMove, rook, &board);
	move.applyMove(&board);
	EXPECT_EQ(board.canBlackCastleQueenSide, false);

	board = Board();
	board.setBitboard(white, rook, 9223372036854775808);
	board.setBitboard(white, king, 1152921504606846976);
	board.canBlackCastleKingSide = true;
	board.update();
	board.nextColour = black;
	move = Move(63, 62, quietMove, rook, &board);
	move.applyMove(&board);
	EXPECT_EQ(board.canBlackCastleKingSide, false);

	board = Board();
	board.setBitboard(white, rook, 9295429630892703744);
	board.setBitboard(white, king, 1152921504606846976);
	board.canBlackCastleKingSide = true;
	board.canBlackCastleQueenSide = true;
	board.update();
	board.nextColour = black;
	move = Move(60, 59, quietMove, king, &board);
	move.applyMove(&board);
	EXPECT_EQ(board.canBlackCastleKingSide, false);
	EXPECT_EQ(board.canBlackCastleQueenSide, false);
}

TEST(Move, IncrementingScores)
{
	Board board = Board("8/8/8/8/4r3/3P4/8/8 w - - 0 1 ");
	EXPECT_EQ(board.getMaterialScore(white), -400);
	EXPECT_EQ(board.getMaterialScore(black), 400);
	EXPECT_EQ(board.getOnlyMaterialScore(black), 500);
	EXPECT_EQ(board.getOnlyMaterialScore(white), 0);

	Move move = moveFromNotation("d3e4", &board);
	move.applyMove(&board);

	EXPECT_EQ(board.getMaterialScore(white), 100);
	EXPECT_EQ(board.getMaterialScore(black), -100);
	EXPECT_EQ(board.getOnlyMaterialScore(black), 0);
	EXPECT_EQ(board.getOnlyMaterialScore(white), 0);

	move.undoMove(&board);

	EXPECT_EQ(board.getMaterialScore(white), -400);
	EXPECT_EQ(board.getMaterialScore(black), 400);
	EXPECT_EQ(board.getOnlyMaterialScore(black), 500);
	EXPECT_EQ(board.getOnlyMaterialScore(white), 0);
}

TEST(Move, IncrementingZorbistKeys)
{
	Board board;
	board.defaults();

	Move move = moveFromNotation("a2a3", &board);
	move.applyMove(&board);
	uint64_t key1 = board.zorbistKey;
	board.generateZorbistKey();
	uint64_t key2 = board.zorbistKey;
	EXPECT_EQ(key1, key2);


	board.defaults();
	move = moveFromNotation("a2a4", &board);
	move.applyMove(&board);
	key1 = board.zorbistKey;
	board.generateZorbistKey();
	key2 = board.zorbistKey;
	EXPECT_EQ(key1, key2);

	board.loadFromFen("8/P7/8/8/8/8/8/8 w - - 0");
	move = moveFromNotation("a7a8q", &board);
	move.applyMove(&board);

	key1 = board.zorbistKey;
	board.generateZorbistKey();
	key2 = board.zorbistKey;
	EXPECT_EQ(key1, key2);
}
