#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "GameMech.h"
#include <QDebug>
#include <selectshipswindow.h>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pvpButton_clicked()
{


    int* choice;
    *choice = 1;


    qDebug() << "test";

    SelectShipsWindow *s = new SelectShipsWindow(choice, this);
    s->show();
    this->hide();

}

void MainWindow::on_pvbButton_clicked()
{
    qDebug() << "test";
    int* choice;
    *choice = 2;
    SelectShipsWindow *s = new SelectShipsWindow(choice, this);
    s->show();
    this->hide();


}


/*
void MainWindow::on_start_released()
{
    SelectShipsWindow *s = new SelectShipsWindow();
    s->show();
    this->hide();
}
*/
