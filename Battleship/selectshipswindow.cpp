#include "selectshipswindow.h"
#include "ui_selectshipswindow.h"
#include <QDebug>
#include "User.h"
#include "Ship.h"
SelectShipsWindow::SelectShipsWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SelectShipsWindow)
{
    ui->setupUi(this);


}

SelectShipsWindow::~SelectShipsWindow()
{
    delete ui;
}

void SelectShipsWindow::on_submit_released()
{
    QString shipx1 = ui->ship_x->currentText();
    QString shipx2 = ui->ship_x_2->currentText();
    QString shipx3 = ui->ship_x_3->currentText();
    QString shipx4 = ui->ship_x_4->currentText();
    QString shipx5 = ui->ship_x_5->currentText();
    QString shipx6 = ui->ship_x_6->currentText();
    QString shipx7 = ui->ship_x_7->currentText();


    QString shipy1 = ui->ship_y->currentText();
    QString shipy2 = ui->ship_y_2->currentText();
    QString shipy3 = ui->ship_y_3->currentText();
    QString shipy4 = ui->ship_y_4->currentText();
    QString shipy5 = ui->ship_y_5->currentText();
    QString shipy6 = ui->ship_y_6->currentText();
    QString shipy7 = ui->ship_y_7->currentText();

    QString *dir = new QString[7];
    dir[0] = ui->ship_1_d_1->currentText();
    dir[1] = ui->ship_1_d_2->currentText();
    dir[2] = ui->ship_1_d_3->currentText();
    dir[3] = ui->ship_1_d_4->currentText();
    dir[4] = ui->ship_1_d_5->currentText();
    dir[5] = ui->ship_1_d_6->currentText();
    dir[6] = ui->ship_1_d_7->currentText();

    for(int i = 0; i < 7; i++)
    {
        if(dir[i] == "North")
            dir[i] = "1";
        else if(dir[i] == "South")
            dir[i] = "2";
        else if(dir[i] == "East")
            dir[i] = "3";
        else {
            dir[i] = "4";
        }


    }

    Ship* s = new Ship[7];
    User u;
    s[0] = Ship(2,dir[0].toInt(),shipx1.toInt(),shipy1.toInt());
    s[1] = Ship(2,dir[1].toInt(),shipx2.toInt(),shipy2.toInt());
    s[2] = Ship(3,dir[2].toInt(),shipx3.toInt(),shipy3.toInt());
    s[3] = Ship(3,dir[3].toInt(),shipx4.toInt(),shipy4.toInt());
    s[4] = Ship(2,dir[4].toInt(),shipx5.toInt(),shipy5.toInt());
    s[5] = Ship(3,dir[5].toInt(),shipx6.toInt(),shipy6.toInt());
    s[6] = Ship(3,dir[6].toInt(),shipx7.toInt(),shipy7.toInt());

    qDebug() << s[0].getDirection();

    s[0].setSize(2);
    s[1].setSize(2);
    s[2].setSize(3);
    s[3].setSize(3);
    s[4].setSize(3);
    s[5].setSize(5);
    s[6].setSize(5);

    bool check = true;
    for(int i = 0; i < 7; i++)
    {
        if(u.checkIfValid(s[i].getY(),s[i].getX(),s[i].getDirection(), s[i].getSize()))
            u.getGrid().addShip( s[i].getSize(), s[i].getDirection(), s[i].getY(), s[i].getX());
        else
             check = false;
    }


    if(check == false)
    {
        qDebug() << "Invalid ship positions... Re Enter....";
        ui->errorLabel->setText("Invalid Ship Placements... Try Again...");
    }
    else {
        //open new window and send User
    }



}


