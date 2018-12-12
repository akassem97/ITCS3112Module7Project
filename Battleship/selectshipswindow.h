#ifndef SELECTSHIPSWINDOW_H
#define SELECTSHIPSWINDOW_H

#include <QMainWindow>

namespace Ui {
class SelectShipsWindow;
}

class SelectShipsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SelectShipsWindow(QWidget *parent = nullptr);
    ~SelectShipsWindow();

private slots:
    void on_submit_released();

    void on_ship_x_6_activated(const QString &arg1);

    void on_ship_x_currentIndexChanged(const QString &arg1);

private:
    Ui::SelectShipsWindow *ui;
};

#endif // SELECTSHIPSWINDOW_H
