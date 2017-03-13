#include "startingscreen.h"
#include "ui_startingscreen.h"

#include "dialog.h"
#include "dialog_auto.h"

StartingScreen::StartingScreen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartingScreen)
{
    ui->setupUi(this);
}

StartingScreen::~StartingScreen()
{
    delete ui;
}

void StartingScreen::on_pushButton_clicked()
{
    Dialog * w = new Dialog();
    w->show();
}

void StartingScreen::on_pushButton_3_clicked()
{
    close();
}

void StartingScreen::on_pushButton_2_clicked()
{
    dialog_auto  * d = new dialog_auto();
    d->show();
}
