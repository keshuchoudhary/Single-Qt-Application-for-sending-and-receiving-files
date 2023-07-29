#include <iostream>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtNetwork/QUdpSocket>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString readMeFilePath;
QString chooseFile();
void MainWindow::on_pushButton_clicked()
{
    readMeFilePath=chooseFile();
    ui->lineEdit->setText(readMeFilePath);
}




void sndBtnGui(QString x);
void MainWindow::on_pushButton_2_clicked()
{
    sndBtnGui(readMeFilePath);
}

