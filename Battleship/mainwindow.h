#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pvpButton_clicked();

    void on_pvbButton_clicked();

    void on_pushButton_clicked();

    void on_start_released();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
