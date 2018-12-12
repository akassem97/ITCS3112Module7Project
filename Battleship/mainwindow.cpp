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

/*
void MainWindow::on_pvpButton_clicked()
{


    PvpWindow *pvp = new PvpWindow();

    pvp->show();
    this->close();
    qDebug() << "test";


}

void MainWindow::on_pvbButton_clicked()
{
    qDebug() << "test";
    this->close();

}
*/


void MainWindow::on_start_released()
{
    SelectShipsWindow *s = new SelectShipsWindow();
    s->show();
    this->close();
}
