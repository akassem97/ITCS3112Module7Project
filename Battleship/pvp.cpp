#include "pvp.h"
#include "ui_pvp.h"

pvp::pvp(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::pvp)
{
    ui->setupUi(this);
}

pvp::~pvp()
{
    delete ui;
}
