#ifndef SHOWMEMDIALOG_H
#define SHOWMEMDIALOG_H
#include <QDialog>

#define MAXMEMNUMS 6
#define LABELNUMS  2

class QLabel;
class QProgressBar;
class QFont;
class showMemDialog : public QDialog
{
    Q_OBJECT

public:
    showMemDialog(QWidget *parent = 0);
    ~showMemDialog();

    void setupUi();
    void hide_label_progreebar();
    void display_mem_info(int, QString, QString);
    int analyze_mem_info(QString);

public slots:
    virtual int exec();

private:
    QPushButton *m_ok;
    QLabel *m_label[MAXMEMNUMS][LABELNUMS];
    QProgressBar *m_bar[MAXMEMNUMS];
    float m_free;
    float m_total;
    QFont m_ft;
};

#endif // SHOWMEMDIALOG_H
