#pragma once
#include "gtest/gtest.h"
#include "board.h"

TEST(Board, UpdateFunction)
{
	Board board;
	board.setBitboard(white,pawn, 2);
	board.setBitboard(white, bishop, 4);
	board.update();
	EXPECT_EQ(board.allPieces, 6);

	board.setBitboard(black, bishop, 8);
	board.update();
	EXPECT_EQ(board.allPieces, 14);
	EXPECT_EQ(board.whitePieces, 6);
	EXPECT_EQ(board.blackPieces, 8);
}
TEST(Board, Defaults)
{
	Board board;
	board.defaults();
	EXPECT_EQ(board.getPieceBitboard(white, pawn), 65280);
	EXPECT_EQ(board.getPieceBitboard(white, rook), 129);
	EXPECT_EQ(board.getPieceBitboard(white, knight), 66);
	EXPECT_EQ(board.getPieceBitboard(white, bishop), 36);
	EXPECT_EQ(board.getPieceBitboard(white, queen), 8);
	EXPECT_EQ(board.getPieceBitboard(white, king), 16);

	EXPECT_EQ(board.getPieceBitboard(black, pawn), 71776119061217280);
	EXPECT_EQ(board.getPieceBitboard(black, rook), 9295429630892703744);
	EXPECT_EQ(board.getPieceBitboard(black, knight), 4755801206503243776);
	EXPECT_EQ(board.getPieceBitboard(black, bishop), 2594073385365405696);
	EXPECT_EQ(board.getPieceBitboard(black, queen), 576460752303423488);
	EXPECT_EQ(board.getPieceBitboard(black, king), 1152921504606846976);

}
/*
TEST(Board, loadFromFen)
{
	Board defaultBoard;
	defaultBoard.defaults();
	Board fenBoard;
	fenBoard.loadFromFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

	EXPECT_EQ(defaultBoard.allPieces, fenBoard.allPieces);
	EXPECT_EQ(defaultBoard.getPieceBitboard(black, bishop), fenBoard.getPieceBitboard(black, bishop));
	EXPECT_EQ(defaultBoard.getPieceBitboard(black, king), fenBoard.getPieceBitboard(black, king));
	EXPECT_EQ(defaultBoard.getPieceBitboard(black, knight), fenBoard.getPieceBitboard(black, knight));
	EXPECT_EQ(defaultBoard.getPieceBitboard(black, pawn), fenBoard.getPieceBitboard(black, pawn));
	EXPECT_EQ(defaultBoard.blackPieces, fenBoard.blackPieces);
	EXPECT_EQ(defaultBoard.getPieceBitboard(black, queen), fenBoard.getPieceBitboard(black, queen));
	EXPECT_EQ(defaultBoard.getPieceBitboard(black, rook), fenBoard.getPieceBitboard(black, rook));
	EXPECT_EQ(defaultBoard.getPieceBitboard(white, bishop), fenBoard.getPieceBitboard(white, bishop));
	EXPECT_EQ(defaultBoard.getPieceBitboard(white, king), fenBoard.getPieceBitboard(white, king));
	EXPECT_EQ(defaultBoard.getPieceBitboard(white, knight), fenBoard.getPieceBitboard(white, knight));
	EXPECT_EQ(defaultBoard.getPieceBitboard(white, pawn), fenBoard.getPieceBitboard(white, pawn));
	EXPECT_EQ(defaultBoard.whitePieces, fenBoard.whitePieces);
	EXPECT_EQ(defaultBoard.getPieceBitboard(white, queen), fenBoard.getPieceBitboard(white, queen));
	EXPECT_EQ(defaultBoard.getPieceBitboard(white, rook), fenBoard.getPieceBitboard(white, rook));
	EXPECT_EQ(fenBoard.enPassantSquare, -1);
	EXPECT_EQ(fenBoard.canBlackCastleKingSide, true);
	EXPECT_EQ(fenBoard.canBlackCastleQueenSide, true);
	EXPECT_EQ(fenBoard.canWhiteCastleQueenSide, true);
	EXPECT_EQ(fenBoard.canWhiteCastleKingSide, true);
	EXPECT_EQ(fenBoard.getFiftyMoveTimer(), 0);

	Board board = Board();
	board.setBitboard(white,pawn, 1);
	board.update();
	fenBoard = Board();
	fenBoard.loadFromFen("8/8/8/8/8/8/8/P7 w KQkq - 10 1");

	EXPECT_EQ(board.allPieces, fenBoard.allPieces);
	EXPECT_EQ(board.getPieceBitboard(black, bishop), fenBoard.getPieceBitboard(black, bishop));
	EXPECT_EQ(board.getPieceBitboard(black, king), fenBoard.getPieceBitboard(black, king));
	EXPECT_EQ(board.getPieceBitboard(black, knight), fenBoard.getPieceBitboard(black, knight));
	EXPECT_EQ(board.getPieceBitboard(black, pawn), fenBoard.getPieceBitboard(black, pawn));
	EXPECT_EQ(board.blackPieces, fenBoard.blackPieces);
	EXPECT_EQ(board.getPieceBitboard(black, queen), fenBoard.getPieceBitboard(black, queen));
	EXPECT_EQ(board.getPieceBitboard(black, rook), fenBoard.getPieceBitboard(black, rook));
	EXPECT_EQ(board.getPieceBitboard(white, bishop), fenBoard.getPieceBitboard(white, bishop));
	EXPECT_EQ(board.getPieceBitboard(white, king), fenBoard.getPieceBitboard(white, king));
	EXPECT_EQ(board.getPieceBitboard(white, knight), fenBoard.getPieceBitboard(white, knight));
	EXPECT_EQ(board.getPieceBitboard(white, pawn), fenBoard.getPieceBitboard(white, pawn));
	EXPECT_EQ(board.whitePieces, fenBoard.whitePieces);
	EXPECT_EQ(board.getPieceBitboard(white, queen), fenBoard.getPieceBitboard(white, queen));
	EXPECT_EQ(board.getPieceBitboard(white, rook), fenBoard.getPieceBitboard(white, rook));
	EXPECT_EQ(fenBoard.enPassantSquare, -1);
	EXPECT_EQ(fenBoard.canBlackCastleKingSide, true);
	EXPECT_EQ(fenBoard.canBlackCastleQueenSide, true);
	EXPECT_EQ(fenBoard.canWhiteCastleQueenSide, true);
	EXPECT_EQ(fenBoard.canWhiteCastleKingSide, true);
	EXPECT_EQ(fenBoard.getFiftyMoveTimer(), 10);

	board = Board();
	board.setBitboard(white, pawn, 16777216);
	board.setBitboard(black, pawn, 33554432);
	board.update();
	fenBoard = Board();
	fenBoard.loadFromFen("8/8/8/8/Pp6/8/8/8 b - a3 41 1");

	EXPECT_EQ(board.allPieces, fenBoard.allPieces);
	EXPECT_EQ(board.getPieceBitboard(black, bishop), fenBoard.getPieceBitboard(black, bishop));
	EXPECT_EQ(board.getPieceBitboard(black, king), fenBoard.getPieceBitboard(black, king));
	EXPECT_EQ(board.getPieceBitboard(black, knight), fenBoard.getPieceBitboard(black, knight));
	EXPECT_EQ(board.getPieceBitboard(black, pawn), fenBoard.getPieceBitboard(black, pawn));
	EXPECT_EQ(board.blackPieces, fenBoard.blackPieces);
	EXPECT_EQ(board.getPieceBitboard(black, queen), fenBoard.getPieceBitboard(black, queen));
	EXPECT_EQ(board.getPieceBitboard(black, rook), fenBoard.getPieceBitboard(black, rook));
	EXPECT_EQ(board.getPieceBitboard(white, bishop), fenBoard.getPieceBitboard(white, bishop));
	EXPECT_EQ(board.getPieceBitboard(white, king), fenBoard.getPieceBitboard(white, king));
	EXPECT_EQ(board.getPieceBitboard(white, knight), fenBoard.getPieceBitboard(white, knight));
	EXPECT_EQ(board.getPieceBitboard(white, pawn), fenBoard.getPieceBitboard(white, pawn));
	EXPECT_EQ(board.whitePieces, fenBoard.whitePieces);
	EXPECT_EQ(board.getPieceBitboard(white, queen), fenBoard.getPieceBitboard(white, queen));
	EXPECT_EQ(board.getPieceBitboard(white, rook), fenBoard.getPieceBitboard(white, rook));
	EXPECT_EQ(fenBoard.enPassantSquare, 16);
	EXPECT_EQ(fenBoard.canBlackCastleKingSide, false);
	EXPECT_EQ(fenBoard.canBlackCastleQueenSide, false);
	EXPECT_EQ(fenBoard.canWhiteCastleQueenSide, false);
	EXPECT_EQ(fenBoard.canWhiteCastleKingSide, false);
	EXPECT_EQ(fenBoard.getFiftyMoveTimer(), 41);
}*/

TEST(Board, exportAsFen)
{
	Board board;
	board.defaults();

	EXPECT_EQ(board.exportAsFen().substr(0,53), "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - ");

	board.loadFromFen("rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2");
	board.printBoard();

	EXPECT_EQ(board.exportAsFen().substr(0, 59), "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - ");
	
}

TEST(Board, IsPieceAttacked)
{
	Board board;

	board.loadFromFen("r7/8/8/8/8/8/8/K7 w - - 0 1 ");
	EXPECT_EQ(board.isPieceAttacked(0, white), true);

	board = Board();
	board.loadFromFen("1r6/8/8/8/8/8/8/K7 w - - 0 1 ");
	EXPECT_EQ(board.isPieceAttacked(0, white), false);

	board = Board();
	board.loadFromFen("8/8/8/8/8/8/1p6/K7 w - - 0 1 ");
	EXPECT_EQ(board.isPieceAttacked(0, white), true);

	board = Board();
	board.loadFromFen("8/8/8/8/8/8/p7/K7 w - - 0 1 ");
	EXPECT_EQ(board.isPieceAttacked(0, white), false);
}

TEST(Board, isMaterialDraw)
{
	Board board;

	board.defaults();
	EXPECT_EQ(board.isMaterialDraw(), false);

	board = Board("K7/8/k7/8/8/8/8/8 w -- 0 1");
	EXPECT_EQ(board.isMaterialDraw(), true);

	board = Board("K7/8/k7/8/N7/8/8/8 w -- 0 1");
	EXPECT_EQ(board.isMaterialDraw(), true);

	board = Board("K7/8/k7/8/NN6/8/8/8 w -- 0 1");
	EXPECT_EQ(board.isMaterialDraw(), false);

	board = Board("K7/8/k7/8/8/8/8/B7 w -- 0 1");
	EXPECT_EQ(board.isMaterialDraw(), true);

	board = Board("K7/8/k7/8/8/8/8/B1B5 w -- 0 1");
	EXPECT_EQ(board.isMaterialDraw(), true);

	board = Board("K7/8/k7/8/8/8/8/B1B1b1b w -- 0 1");
	EXPECT_EQ(board.isMaterialDraw(), true);

	board = Board("K7/8/k7/8/8/8/8/B1B1bb1 w -- 0 1");
	EXPECT_EQ(board.isMaterialDraw(), false);
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


