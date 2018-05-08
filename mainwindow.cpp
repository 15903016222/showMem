#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "showmemdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_dialog = new showMemDialog(this);
    m_button = new QPushButton (this);
    connect(m_button, SIGNAL(clicked(bool)), m_dialog, SLOT(show()));
    //m_dialog->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}
