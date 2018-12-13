#ifndef PVBWINDOW_H
#define PVBWINDOW_H

#include <QMainWindow>
#include <QMainWindow>
#include <QDebug>
#include "User.h"
#include "BotAi.h"
namespace Ui {
class PvbWindow;
}

class PvbWindow : public QMainWindow
{
    Q_OBJECT
    User* user;
    BotAi b;

public:
    explicit PvbWindow(QWidget *parent = nullptr);
    explicit PvbWindow(User* u, QWidget *parent = nullptr);
    User* getUser()
    {
        return user;
    }
    BotAi getBot()
    {
        return b;
    }
    BotAi setBot( BotAi b1)
    {
        b = b1;
    }
    ~PvbWindow();
    void startGame();

private slots:
    void on_pushButton_e_1_1_released();

    void on_pushButton_e_1_2_released();

    void on_pushButton_e_1_3_released();

    void on_pushButton_e_1_4_released();

    void on_pushButton_e_1_5_released();

    void on_pushButton_e_1_6_released();

    void on_pushButton_e_1_7_released();

    void on_pushButton_e_1_8_released();

    void on_pushButton_e_1_9_released();

    void on_pushButton_e_1_10_released();

    void on_pushButton_e_2_1_released();

    void on_pushButton_e_2_2_released();

    void on_pushButton_e_2_3_released();

    void on_pushButton_e_2_4_released();

    void on_pushButton_e_2_5_released();

    void on_pushButton_e_2_6_released();

    void on_pushButton_e_2_7_released();

    void on_pushButton_e_2_8_released();

    void on_pushButton_e_2_9_released();

    void on_pushButton_e_2_10_released();

    void on_pushButton_e_3_1_released();

    void on_pushButton_e_3_2_released();

    void on_pushButton_e_3_3_released();

    void on_pushButton_e_3_4_released();

    void on_pushButton_e_3_5_released();

    void on_pushButton_e_3_6_released();

    void on_pushButton_e_3_7_released();

    void on_pushButton_e_3_8_released();

    void on_pushButton_e_3_9_released();

    void on_pushButton_e_3_10_released();

    void on_pushButton_e_4_1_released();

    void on_pushButton_e_4_2_released();

    void on_pushButton_e_4_3_released();

    void on_pushButton_e_4_4_released();

    void on_pushButton_e_4_5_released();

    void on_pushButton_e_4_6_released();

    void on_pushButton_e_4_7_released();

    void on_pushButton_e_4_8_released();

    void on_pushButton_e_4_9_released();

    void on_pushButton_e_4_10_released();

    void on_pushButton_e_5_1_released();

    void on_pushButton_e_5_2_released();

    void on_pushButton_e_5_3_released();

    void on_pushButton_e_5_4_released();

    void on_pushButton_e_5_5_released();

    void on_pushButton_e_5_6_released();

    void on_pushButton_e_5_7_released();

    void on_pushButton_e_5_8_released();

    void on_pushButton_e_5_9_released();

    void on_pushButton_e_5_10_released();

    void on_pushButton_e_6_1_released();

    void on_pushButton_e_6_2_released();

    void on_pushButton_e_6_3_released();

    void on_pushButton_e_6_4_released();

    void on_pushButton_e_6_5_released();

    void on_pushButton_e_6_6_released();

    void on_pushButton_e_6_7_released();

    void on_pushButton_e_6_8_released();

    void on_pushButton_e_6_9_released();

    void on_pushButton_e_6_10_released();

    void on_pushButton_e_7_1_released();

    void on_pushButton_e_7_2_released();

    void on_pushButton_e_7_3_released();

    void on_pushButton_e_7_4_released();

    void on_pushButton_e_7_5_released();

    void on_pushButton_e_7_6_released();

    void on_pushButton_e_7_7_released();

    void on_pushButton_e_7_8_released();

    void on_pushButton_e_7_9_released();

    void on_pushButton_e_7_10_released();

    void on_pushButton_e_8_1_released();

    void on_pushButton_e_8_2_released();

    void on_pushButton_e_8_3_released();

    void on_pushButton_e_8_4_released();

    void on_pushButton_e_8_5_released();

    void on_pushButton_e_8_6_released();

    void on_pushButton_e_8_7_released();

    void on_pushButton_e_8_8_released();

    void on_pushButton_e_8_9_released();

    void on_pushButton_e_8_10_released();

    void on_pushButton_e_9_1_released();

    void on_pushButton_e_9_2_released();

    void on_pushButton_e_9_3_released();

    void on_pushButton_e_9_4_released();

    void on_pushButton_e_9_5_released();

    void on_pushButton_e_9_6_released();

    void on_pushButton_e_9_7_released();

    void on_pushButton_e_9_8_released();

    void on_pushButton_e_9_9_released();

    void on_pushButton_e_9_10_released();

    void on_pushButton_e_10_1_released();

    void on_pushButton_e_10_2_released();

    void on_pushButton_e_10_3_released();

    void on_pushButton_e_10_4_released();

    void on_pushButton_e_10_5_released();

    void on_pushButton_e_10_6_released();

    void on_pushButton_e_10_7_released();

    void on_pushButton_e_10_8_released();

    void on_pushButton_e_10_9_released();

    void on_pushButton_e_10_10_released();

private:
    Ui::PvbWindow *ui;
};

#endif // PVBWINDOW_H
