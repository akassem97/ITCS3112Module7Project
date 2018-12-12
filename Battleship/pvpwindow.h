#ifndef PVPWINDOW_H
#define PVPWINDOW_H

#include <QMainWindow>

namespace Ui {
class PvpWindow;
}

class PvpWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PvpWindow(QWidget *parent = nullptr);
    ~PvpWindow();

private:
    Ui::PvpWindow *ui;
};

#endif // PVPWINDOW_H
