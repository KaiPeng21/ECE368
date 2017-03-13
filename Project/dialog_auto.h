#ifndef DIALOG_AUTO_H
#define DIALOG_AUTO_H

#include <QDialog>
#include "constants.h"
#include <QQueue>

namespace Ui {
class dialog_auto;
}

class dialog_auto : public QDialog
{
   Q_OBJECT

public:
   explicit dialog_auto(QWidget *parent = 0);
   ~dialog_auto();

   void initGame();
   void enqueue_snake_head(int x, int y);
   void dequeue_snake_tail();
   void display_snake_coordinate();
   bool is_egg(int x, int y);
   bool egg_eaten();
   void generate_egg();
   bool game_over_check(int x, int y);
   bool is_snake_body(int x, int y);
   bool is_snake(int x, int y);
   bool is_block(int x, int y);

   void enqueue_path(int direction, int);
   void dequeue_path();
   void initGame_automative();

   void CreatePath(int j, int i, int count, int delay);
   void AutomatedSnake();

protected:
   void keyPressEvent(QKeyEvent *event);
   void paintEvent(QPaintEvent * event);

public slots:
   void gameLoop_automative();


private:
   Ui::dialog_auto *ui;

   int direction;
   int play_status;
   int score;
   Coordinate egg;
   QQueue<Coordinate *> sque;
   QQueue<Coordinate *> pathq;
   int array[WIDTH+1][HEIGHT+1];

};

#endif // DIALOG_AUTO_H

