#ifndef SHOWMEMDIALOG_H
#define SHOWMEMDIALOG_H

#include <QDialog>

#define MAXMEMNUMS 6
#define LABELNUMS  2

namespace Ui {
class showMemDialog;
}

class QLabel;
class QProgressBar;
class QFont;
class showMemDialog : public QDialog
{
    Q_OBJECT

public:
    explicit showMemDialog(QWidget *parent = 0);
    ~showMemDialog();

    void show_volume(int num, QString name, QString value);
    void set_font(int pix);
    void delete_mem_info();

public slots:
    void show();

private:
    Ui::showMemDialog *ui;
    QLabel *m_label[MAXMEMNUMS][LABELNUMS];
    QProgressBar *m_bar[MAXMEMNUMS];
    float m_free;
    float m_total;
    QFont m_ft;
};

#endif // SHOWMEMDIALOG_H
