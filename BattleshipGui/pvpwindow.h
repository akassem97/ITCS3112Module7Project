#ifndef PVPWINDOW_H
#define PVPWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include "User.h"
namespace Ui {
class PvpWindow;
}

class PvpWindow : public QMainWindow
{
    Q_OBJECT
    User* user;
public:
    explicit PvpWindow(QWidget *parent = nullptr);
    explicit PvpWindow( User *u = nullptr, QWidget *parent = nullptr);

    void getUi();
    User* getUser()
    {
        return user;
    }
    ~PvpWindow();

private:
    Ui::PvpWindow *ui;
};

#endif // PVPWINDOW_H
