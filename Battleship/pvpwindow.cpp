#include "pvpwindow.h"
#include "ui_pvpwindow.h"

PvpWindow::PvpWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PvpWindow)
{
    ui->setupUi(this);
}

PvpWindow::~PvpWindow()
{
    delete ui;
}
