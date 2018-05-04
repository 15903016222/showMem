#include "showmemdialog.h"
#include "ui_showmemdialog.h"

showMemDialog::showMemDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::showMemDialog)
{
    ui->setupUi(this);
}

showMemDialog::~showMemDialog()
{
    delete ui;
}
