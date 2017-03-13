#ifndef STARTINGSCREEN_H
#define STARTINGSCREEN_H

#include <QDialog>

namespace Ui {
class StartingScreen;
}

class StartingScreen : public QDialog
{
    Q_OBJECT

public:
    explicit StartingScreen(QWidget *parent = 0);
    ~StartingScreen();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::StartingScreen *ui;
};

#endif // STARTINGSCREEN_H
