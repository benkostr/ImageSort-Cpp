#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#pragma once

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QProgressDialog>
#define SORTER_EXPORTS
#include "Sorter.h"

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
    void on_pushButton_chooseSourceDir_clicked();

    void on_pushButton_chooseTargetDir_clicked();

    void on_checkBox_sortNonExif_clicked();

    void on_checkBox_day_clicked();

    void on_pushButton_start_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
