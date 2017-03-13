#include "dialog_auto.h"
#include "ui_dialog_auto.h"

#include <iostream>
#include <QKeyEvent>
#include <QPainter>
#include <QString>
#include <QTextDocument>
#include <QDebug>
#include <QTimer>
#include <QTime>

dialog_auto::dialog_auto(QWidget *parent) :
    QDialog(parent)//,
    //ui(new Ui::dialog_auto)
{
    //ui->setupUi(this);

    // set the screen size
    resize((WIDTH+1)*SIZE, (HEIGHT+1)*SIZE + SIZE*3);
    setWindowTitle("Snake Game Automation");
    // initialize the game conditions
    //initGame();

    initGame_automative();

    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    QTimer * timer = new QTimer(this);
    // update game data
    // connect(timer,SIGNAL(timeout()),this,SLOT(gameLoop()));
    connect(timer,SIGNAL(timeout()),this,SLOT(gameLoop_automative()));

    // update graphical display
    connect(timer,SIGNAL(timeout()),this,SLOT(update()));
    timer->start(10);
}

dialog_auto::~dialog_auto()
{
    //delete ui;
}

void dialog_auto::initGame()
{
    direction = UP;
    play_status = PLAYING;
    score = 0;

    // initialize snake
    sque.clear();
    for (int i = 0; i < STARTING_L; i++){
        enqueue_snake_head(WIDTH/2, HEIGHT/2+2 - i);
    }

    // initialize egg
    generate_egg();
}

void dialog_auto::enqueue_snake_head(int x, int y)
{
    Coordinate * pos = new Coordinate();
    pos->x = x;
    pos->y = y;
    sque.enqueue(pos);
}

void dialog_auto::dequeue_snake_tail()
{
    if (!sque.isEmpty()){
        Coordinate * tmp = sque.dequeue();
        delete tmp;
    }
}

void dialog_auto::display_snake_coordinate()
{
    for (int i = 0; i < sque.size(); ++i) {
        qDebug() << "(" << sque.at(i)->x << " ," << sque.at(i)->y << ")" << " <- ";
    }
    qDebug() << "\n";
}

bool dialog_auto::is_egg(int x, int y)
{
    if (x == egg.x && y == egg.y){
        return true;
    }
    return false;
}

bool dialog_auto::egg_eaten()
{
    int shead_x = sque.back()->x;
    int shead_y = sque.back()->y;
    if (is_egg(shead_x, shead_y)){
        return true;
    }
    return false;
}

// randomly set the positon of egg.x and egg.y
// re-set if the position is snake or block
void dialog_auto::generate_egg()
{
    int egg_x, egg_y;
    do{
        egg_x = qrand() % WIDTH + 1;
        egg_y = qrand() % HEIGHT + 1;
    }while(is_snake(egg_x, egg_y) || is_block(egg_x, egg_y));

    egg.x = egg_x;
    egg.y = egg_y;
}

// return true if game over, false if game is not over
// check if the snake head position is snake body or block
bool dialog_auto::game_over_check(int x, int y)
{
    if (is_snake_body(x, y) || is_block(x, y)){
        return true;
    }
    return false;
}

// check if the coordinate is a snake (excluding snake head)
bool dialog_auto::is_snake_body(int x, int y)
{
    if (sque.size() != 0){
        for (int i = 0; i < sque.size()-1; ++i) {
            if (x == sque.at(i)->x && y == sque.at(i)->y){
                return true;
            }
        }
    }
    return false;
}
// check if the coordinate is a snake (including snake head)
bool dialog_auto::is_snake(int x, int y)
{
    if (is_snake_body(x, y)){
        return true;
    }
    if (x == sque.back()->x && y == sque.back()->y){
        return true;
    }
    return false;
}

bool dialog_auto::is_block(int x, int y)
{
    if(x == 0 || x == WIDTH || y == 0 || y == HEIGHT){
        return true;
    }
    return false;
}


void dialog_auto::initGame_automative()
{
    direction = UP;
    play_status = AUTO;
    score = 0;

    //
    pathq.clear();

    // initialize snake
    sque.clear();
    for (int i = 0; i < STARTING_L; i++){
        enqueue_snake_head(WIDTH/2, HEIGHT/2+2 - i);
    }

    // initialize egg
    generate_egg();
    AutomatedSnake();
}

void dialog_auto::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Up && direction != DOWN && play_status == PLAYING){
        direction = UP;
        //qDebug() << direction;
    }
    else if (event->key() == Qt::Key_Down && direction != UP && play_status == PLAYING){
        direction = DOWN;
        //qDebug() << direction;
    }
    else if (event->key() == Qt::Key_Left && direction != RIGHT && play_status == PLAYING){
        direction = LEFT;
        //qDebug() << direction;
    }
    else if (event->key() == Qt::Key_Right && direction != LEFT && play_status == PLAYING){
        direction = RIGHT;
        //qDebug() << direction;
    }
    // quit game
    else if (event->key() == Qt::Key_Escape){
        if (play_status != OVER){
            play_status = OVER;
        }else{
            // close screen
            this->close();
        }

    }else if (event->key() == Qt::Key_Space){
        // pause and resume
        if(play_status == PLAYING){play_status = PAUSE;}
        else if (play_status == PAUSE){play_status = PLAYING;}

        else if (play_status == OVER){
            sque.clear();
            //initGame();
            initGame_automative();
        }

    }
}

void dialog_auto::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
   // if (play_status == PLAYING || play_status == PAUSE){
        QBrush white(Qt::white,Qt::SolidPattern);
        QRect background(0,0,SIZE*WIDTH, SIZE*HEIGHT);
        painter.fillRect(background, white);

        // draw egg
        QBrush ebrush(Qt::red, Qt::SolidPattern);
        QRect erect(SIZE*egg.x, SIZE*egg.y, SIZE, SIZE);
        painter.fillRect(erect, ebrush);

        // draw snake
        QBrush sbrush(Qt::darkGreen, Qt::SolidPattern);
        for (int i = 0; i < sque.size(); ++i) {
            int tmp_x = sque.at(i)->x;
            int tmp_y = sque.at(i)->y;
            QRect srect(SIZE*tmp_x, SIZE*tmp_y, SIZE, SIZE);
            painter.fillRect(srect, sbrush);
        }
        // draw block
        QBrush bbrush(Qt::black, Qt::SolidPattern);
        QRect brect1(0,0,SIZE,SIZE*(HEIGHT+1));
        QRect brect2(0,0,SIZE*(WIDTH+1),SIZE);
        QRect brect3(SIZE*WIDTH,0,SIZE, SIZE*(HEIGHT+1));
        QRect brect4(0,SIZE*HEIGHT, SIZE*(WIDTH+1),SIZE);
        painter.fillRect(brect1, bbrush);
        painter.fillRect(brect2, bbrush);
        painter.fillRect(brect3, bbrush);
        painter.fillRect(brect4, bbrush);

       // draw score
        QString score_v = QString::number(score);
        QString score_text = "Score: ";
        score_text.append(score_v);
        painter.drawText(SIZE, SIZE*(HEIGHT+1) + 2*SIZE, score_text);

        // draw instructions
        QString instruct = "Aumotation Mode Esc - Quit";
        painter.drawText((WIDTH/2)*SIZE, SIZE*(HEIGHT+1) + 2*SIZE, instruct);

   // }else if (play_status == OVER){ // gameover
     if(play_status == OVER){
        QRect endrect(SIZE*WIDTH/4,SIZE, SIZE*WIDTH/2, (HEIGHT-1)*SIZE);
        QBrush endbrush(Qt::green, Qt::Dense6Pattern);
        painter.fillRect(endrect,endbrush);

        QString score_v = QString::number(score);
        QString score_text = "Game Over! Your Score: ";
        score_text.append(score_v);
        painter.drawText(SIZE*WIDTH/2 - 6*SIZE, SIZE*HEIGHT/2, score_text);

        QString instruct = "Space - Play Again, Esc - Exit";
        painter.drawText(SIZE*WIDTH/2 - 7*SIZE, SIZE*HEIGHT/2 + 3*SIZE, instruct);
    }
}


void dialog_auto::enqueue_path(int direction, int nothing)
{
    Coordinate * pos = new Coordinate();
    pos->x = direction;
    pos->y = nothing;
    pathq.enqueue(pos);
}

void dialog_auto::dequeue_path()
{
    if (!pathq.isEmpty()){
        pathq.dequeue();
    }
}

void dialog_auto::gameLoop_automative()
{
    if (play_status == AUTO){


        direction = UP;


        if(!pathq.isEmpty()){
            direction = pathq.front()->x;
            dequeue_path();
        }

        int tmp_x = sque.back()->x;
        int tmp_y = sque.back()->y;
        switch(direction){
        case UP: enqueue_snake_head(tmp_x, tmp_y - 1); break;
        case DOWN: enqueue_snake_head(tmp_x, tmp_y + 1); break;
        case LEFT: enqueue_snake_head(tmp_x - 1, tmp_y); break;
        case RIGHT: enqueue_snake_head(tmp_x + 1, tmp_y); break;
        }

        // check egg condition
        if(egg_eaten()){
            score++;
            generate_egg();
            AutomatedSnake();
        }else{
            dequeue_snake_tail();
        }

        // check game over
        if (game_over_check(sque.back()->x, sque.back()->y)){
            play_status = OVER;
        }


    }

}


void dialog_auto::AutomatedSnake(){
   int i;
   int j;
   int count = 0;
   int flag = 0;
   int wall = INT_MAX - 1;
   int snakebody = INT_MAX;
   int top;
   int bot;
   int left;
   int right;

   for(i = 0; i <= WIDTH; i++){
        array[i][0] = wall;
   }
   for(i = 0; i <= WIDTH; i++){
        array[i][HEIGHT] = wall;
   }
   for(j = 1; j < HEIGHT; j++){
        array[0][j] = wall;
   }
   for(j = 1; j < HEIGHT; j++){
        array[WIDTH][j] = wall;
   }
   for(i = 1; i < WIDTH; i++){
       for(j = 1; j < HEIGHT; j++){
           array[i][j] = 0;
       }
   }

   for(i = 1; i < sque.length(); i++){
       array[sque.at(i)->x][sque.at(i)->y] = snakebody;
   }
   array[sque.back()->x][sque.back()->y] = 1;

   do{
       top = sque.back()->y - (count + 1);
       if(top < 1){
           top = 1;
       }
       bot = sque.back()->y + (count + 1);
       if(bot > HEIGHT - 1){
           bot = HEIGHT - 1;
       }
       right = sque.back()->x + (count + 1);
       if(right > WIDTH - 1){
           right = WIDTH - 1;
       }
       left = sque.back()->x - (count + 1);
       if(left < 1){
           left = 1;
       }

       for(i = left; i <= right; i++){
           for(j = top; j <= bot; j++){
               if(array[i][j] > 0 && array[i][j] < wall){
                   array[i][j]++;
               }
           }
       }

       for(i = left; i <= right; i++){
           for(j = top; j <= bot; j++){
               if(array[i][j] == 0){
                   if(array[i+1][j] > 1 && array[i+1][j] < wall){
                       array[i][j]++;
                   }
                   else if(array[i-1][j] > 1 && array[i-1][j] < wall){
                       array[i][j]++;
                   }
                   else if(array[i][j+1] > 1 && array[i][j+1] < wall){
                       array[i][j]++;
                   }
                   else if(array[i][j-1] > 1 && array[i][j-1] < wall){
                       array[i][j]++;
                   }
                }
            }
       }

       if(flag != 1 && sque.at(count) != sque.back()){
           array[sque.at(count)->x][sque.at(count)->y] = 0;
       }
       else{
           flag = 1;
       }
       count++;
   }while(array[egg.x][egg.y] == 0);

/*   for(j = 0; j <= HEIGHT; j++){
       for(i = 0; i <= WIDTH; i++){
          if(array[i][j] >= wall){
              printf(" X ");
          }
          else if(i == egg.x && j == egg.y){
              printf(" E ");
          }
          else{
              printf( "%2d ", array[i][j]);
          }
       }
       printf("\n");
   }
*/
   for(i = 0; i < pathq.length();i++){
       dequeue_path();
   }

   CreatePath(egg.x, egg.y, count, 0);

   return;
}

void dialog_auto::CreatePath(int i, int j, int count, int delay){
   int temp = array[i][j];
   int left;
   int right;
   int up;
   int down;
   int upleft;
   int upright;
   int downleft;
   int downright;
   int wall = INT_MAX - 1;
   int snakebody = INT_MAX;
   int flag;

//   printf("%d ", array[i][j]);

   if(count > 0 || count > delay){//(i != sque.back()->x) || (j != sque.back()->y)){
       left = array[i-1][j];
       right = array[i+1][j];
       up = array[i][j-1];
       down = array[i][j+1];
       upleft = array[i-1][j-1];
       upright = array[i+1][j-1];
       downleft = array[i-1][j+1];
       downright = array[i+1][j+1];

       flag = 0;

       if(delay > 0 || (left == array[sque.back()->x][sque.back()->y] && left != temp + 1) || (up == array[sque.back()->x][sque.back()->y] && up != temp + 1) || (down == array[sque.back()->x][sque.back()->y] && down != temp + 1) || (right == array[sque.back()->x][sque.back()->y] && right != temp + 1)){
           if((left == array[i][j] - 1) && ((up < wall) && (upleft < wall) && (upright < wall) || (down < wall) && (downleft < wall) && (downright < wall))){
               flag = 1;
               array[i][j] = snakebody;
               CreatePath(i-1, j, count - 1, delay - 2);
               enqueue_path(RIGHT,0);
//               printf("LEFT ");
               return;
           }
           else if((right == array[i][j] - 1) && ((up < wall) && (upleft < wall) && (upright < wall) || (down < wall) && (downleft < wall) && (downright < wall))){
               flag = 1;
               array[i][j] = snakebody;
               CreatePath(i+1, j, count - 1, delay - 2);
               enqueue_path(LEFT,0);
//               printf("RIGHT ");
               return;
           }
           else if((up == array[i][j] - 1) && ((downright < wall) && (upright < wall) && (right < wall) || (left < wall) && (downleft < wall) && (upleft < wall))){
               flag = 1;
               array[i][j] = snakebody;
               CreatePath(i, j-1, count - 1, delay - 2);
               enqueue_path(DOWN,0);
//               printf("UP ");
               return;
           }
           else if((down == array[i][j] - 1) && ((downright < wall) && (upright < wall) && (right < wall) || (left < wall) && (downleft < wall) && (upleft < wall))){
               flag = 1;
               array[i][j] = snakebody;
               CreatePath(i, j+1, count - 1, delay - 2);
               enqueue_path(UP,0);
//               printf("DOWN ");
               return;
           }
       }
       if(flag == 0){
           if((((left > right) || (right >= wall)) && ((left > up) || (up >= wall)) && ((left > down) || (down >= wall))) && (left < wall)){
               array[i][j] = snakebody;
               CreatePath(i-1, j, count - 1, delay + left - temp - 1);
               enqueue_path(RIGHT,0);
//               printf("left ");
               return;
           }
           else if((((right > up) || (up >= wall)) && ((right > down) || (down >= wall))) && (right < wall)){
               array[i][j] = snakebody;
               CreatePath(i+1, j, count - 1, delay + right - temp - 1);
               enqueue_path(LEFT,0);
//               printf("right ");
               return;
           }
           else if(((up > down) || (down >= wall)) && (up < wall)){
               array[i][j] = snakebody;
               CreatePath(i, j-1, count - 1, delay + up - temp - 1);
               enqueue_path(DOWN,0);
//               printf("up ");
               return;
           }
           else{
               array[i][j] = snakebody;
               CreatePath(i, j+1, count - 1, delay + down - temp - 1);
               enqueue_path(UP,0);
//               printf("down ");
               return;
           }
       }
   }
   else{
       return;
   }
}



