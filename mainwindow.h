#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "webcamcap.h"

class Gameplay;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void addScore(int count);

    void on_checkBox_clicked(bool checked);

    void on_Start_clicked(bool checked);

private:
    Ui::MainWindow *ui;
    Gameplay *iLoop;
    int iScore;

    webcamcap::MyFifo fifo;
};

#endif // MAINWINDOW_H
