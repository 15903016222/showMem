#ifndef SHOWMEMDIALOG_H
#define SHOWMEMDIALOG_H

#include <QDialog>

namespace Ui {
class showMemDialog;
}

class showMemDialog : public QDialog
{
    Q_OBJECT

public:
    explicit showMemDialog(QWidget *parent = 0);
    ~showMemDialog();

private:
    Ui::showMemDialog *ui;
};

#endif // SHOWMEMDIALOG_H
