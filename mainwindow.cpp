#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "showmemdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_dialog = new showMemDialog(this);
    m_dialog->exec();
}

MainWindow::~MainWindow()
{
    delete ui;
}
