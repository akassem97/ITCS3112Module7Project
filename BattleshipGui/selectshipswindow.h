#ifndef SELECTSHIPSWINDOW_H
#define SELECTSHIPSWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include "User.h"
#include "Ship.h"
#include "pvpwindow.h"
#include "pvbwindow.h"
namespace Ui {
class SelectShipsWindow;
}

class SelectShipsWindow : public QMainWindow
{
    Q_OBJECT
    User u;
    int* choice;

public:
    explicit SelectShipsWindow(QWidget *parent = nullptr);
    explicit SelectShipsWindow(int* choice = nullptr, QWidget *parent = nullptr);
    void setChoice(int* c)
    {
        choice = c;
    }
    int* getChoice()
    {
        return choice;
    }
    ~SelectShipsWindow();
    User* getUser()
    {
        return &u;
    }


private slots:
    void on_submit_released();

    void on_ship_x_6_activated(const QString &arg1);

    void on_ship_x_currentIndexChanged(const QString &arg1);



private:
    Ui::SelectShipsWindow *ui;
};

#endif // SELECTSHIPSWINDOW_H
