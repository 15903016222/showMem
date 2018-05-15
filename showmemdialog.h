#ifndef SHOWMEMDIALOG_H
#define SHOWMEMDIALOG_H

#include <QDialog>

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

    void get_mem_info();
    int analyze_mem_info();

    void display_mem_info(int, QString, QString);

public slots:
    virtual int exec();

private:
    QPushButton *m_ok;
    QVector<QVector<QLabel *> > m_label;
    QVector<QProgressBar *> m_bar;
    float m_free;
    float m_total;
    QFont m_ft;
    qint8 m_count;
};

#endif // SHOWMEMDIALOG_H
