#include "pvpwindow.h"
#include "ui_pvpwindow.h"
#include "selectshipswindow.h"


PvpWindow::PvpWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PvpWindow)
{
    ui->setupUi(this);


}

PvpWindow::PvpWindow(User* u, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PvpWindow)
{
    ui->setupUi(this);

    user = u;

   qDebug() << "hai "<< u->getGrid().getGridStream()[0];
}

PvpWindow::~PvpWindow()
{
    delete ui;
}
