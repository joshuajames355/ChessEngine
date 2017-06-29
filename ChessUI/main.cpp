#include "chessui.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ChessUI w;
	w.show();
	return a.exec();
}
