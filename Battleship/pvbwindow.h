#ifndef PVBWINDOW_H
#define PVBWINDOW_H

#include <QMainWindow>

namespace Ui {
class PvbWindow;
}

class PvbWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PvbWindow(QWidget *parent = nullptr);
    ~PvbWindow();

private:
    Ui::PvbWindow *ui;
};

#endif // PVBWINDOW_H
