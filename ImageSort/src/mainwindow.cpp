#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_chooseSourceDir_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(
                this, tr("Select Source Directory"), ".",
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
                );
    ui->lineEdit_sourceDir->setText(dir);
}

void MainWindow::on_pushButton_chooseTargetDir_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(
                this, tr("Select Target Directory"), ".",
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
                );
    ui->lineEdit_targetDir->setText(dir);
}

void MainWindow::on_checkBox_sortNonExif_clicked()
{
    if (!ui->checkBox_sortNonExif->isChecked())
    {
        ui->checkBox_separateNonExif->setChecked(false);
        ui->checkBox_separateNonExif->setEnabled(false);
    }
    else
    {
        ui->checkBox_separateNonExif->setEnabled(true);
    }

}

void MainWindow::on_checkBox_day_clicked()
{
    if (!ui->checkBox_day->isChecked())
    {
        ui->checkBox_hour->setChecked(false);
        ui->checkBox_minute->setChecked(false);
        ui->checkBox_second->setChecked(false);
        ui->checkBox_hour->setEnabled(false);
        ui->checkBox_minute->setEnabled(false);
        ui->checkBox_second->setEnabled(false);
    }
    else
    {
        ui->checkBox_hour->setEnabled(true);
        ui->checkBox_minute->setEnabled(true);
        ui->checkBox_second->setEnabled(true);
    }
}

void MainWindow::on_pushButton_start_clicked()
{
    QString sourceDir = ui->lineEdit_sourceDir->text();
    QString targetDir = ui->lineEdit_targetDir->text();
    if (sourceDir == "" || !QDir(sourceDir).exists())
    {
        QMessageBox::warning(this, tr("Error"),
                             tr("Cannot find source path:\n") + sourceDir,
                             QMessageBox::Ok, QMessageBox::Ok
                            );
    }
    else if (targetDir == "" || !QDir(targetDir).exists())
    {
        QMessageBox::warning(this, tr("Error"),
                             tr("Cannot find target path:\n")
                             + targetDir,
                             QMessageBox::Ok, QMessageBox::Ok
                            );
    }
    else
    {
        Sorter * sorter = new Sorter(
                    sourceDir.toUtf8().constData(),
                    targetDir.toUtf8().constData()
                    );
        sorter->move            = ui->radioButton_move->isChecked();
        sorter->sortNonExif     = ui->checkBox_sortNonExif->isChecked();
        sorter->separateNonExif = ui->checkBox_separateNonExif->isChecked();
        sorter->sortByYear      = ui->checkBox_year->isChecked();
        sorter->sortByMonth     = ui->checkBox_month->isChecked();
        sorter->sortByDay       = ui->checkBox_day->isChecked();
        sorter->sortByHour      = ui->checkBox_hour->isChecked();
        sorter->sortByMinute    = ui->checkBox_minute->isChecked();
        sorter->sortBySecond    = ui->checkBox_second->isChecked();

        std::thread (&Sorter::Start, sorter).detach();
        QProgressDialog progress("Sorting files...", "Cancel", 0, 100, this);
        progress.setWindowModality(Qt::WindowModal);
        progress.show();

        while (!sorter->completed)
        {
            if (progress.wasCanceled())
                sorter->abort_flag = true;
            else
                progress.setValue(sorter->progress * 100);
            qApp->processEvents();
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        progress.setValue(100);
        delete sorter;
        QMessageBox doneMsg;
        doneMsg.setText("Finished sorting.");
        doneMsg.setStandardButtons(QMessageBox::Ok);
        doneMsg.exec();
    }
}
