#include "pvbwindow.h"
#include "ui_pvbwindow.h"
#include "User.h"
#include "BotAi.h"
PvbWindow::PvbWindow(QWidget *parent, User u) :
    QMainWindow(parent),
    ui(new Ui::PvbWindow)
{
    ui->setupUi(this);

}

PvbWindow::~PvbWindow()
{
    delete ui;
}
