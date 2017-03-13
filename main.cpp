#include "dialog.h"
#include <QApplication>

#include "startingscreen.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //Dialog w;
    //w.setWindowTitle("Snake Game");
    //w.show();
    StartingScreen start;
    start.setWindowTitle("ECE 368 Snake Game");
    start.show();

    return a.exec();
}
