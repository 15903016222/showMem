#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "showmemdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    showMemDialog *dialog = new showMemDialog(this);
    connect(ui->pushButton, SIGNAL(clicked(bool)), dialog, SLOT(exec()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
