#ifndef SHOWMEMDIALOG_H
#define SHOWMEMDIALOG_H

#include <QDialog>
#include <QObject>
#include <QThread>
#include <QProcess>
#include <QtXml>
#include <QDomElement>
#include <QMessageBox>
#include <QDebug>

#define FILE_NAME ("/tmp/mem.xml")
#define RM_MEM_FILE ("rm /tmp/mem.xml")
#define GET_MEM_CMD1 ("/bin/bash /usr/bin/getMem.sh")
#define GET_MEM_CMD2 ("/bin/bash /home/ws/project/phascan-II/showMem/getMem.sh")

class  getMemInfo : public QObject
{
    Q_OBJECT

public:
    getMemInfo() {}
    ~getMemInfo() {}

    void get_mem_info() {
        QProcess *proc = new QProcess;
        proc->start(GET_MEM_CMD1);
        if (proc->waitForFinished()) {
            proc->start(GET_MEM_CMD2);
            proc->waitForFinished();
        }
        delete proc;
    }

    int open_file(QString file_name, QDomDocument *doc) {
        QFile file (file_name);
        if (!file.open(QFile::ReadOnly)) {
            return -1;
        }

        if (!doc->setContent(&file)) {
            file.close();
            return -1;
        }
        file.close();

        return 0;
    }

    void analyze_mem_info () {
        QDomDocument *doc = new QDomDocument;
        if (0 != open_file(FILE_NAME, doc)) {
            return;
        }

        QDomElement root = doc->documentElement();
        QDomNode node = root.firstChild();

        while (!node.isNull()) {
            if (node.isElement()) {
                QDomElement e = node.toElement();
                QDomNodeList list = e.childNodes();
                for (int i = 0; i < list.count(); ++i) {
                    QDomNode n = list.at(i);
                    QString content;
                    content = n.nodeName() + \
                                QString("+") + \
                                n.toElement().text();
                    emit send_mem_info_content(content);
                }
            }
            node = node.nextSibling();
        }
    }

signals:
    void send_mem_info_content(QString);

public slots:
    void receive_get_mem_info_sig() {
        // 获取存储信息
        get_mem_info();

        // 解析数据 并发送数据到dialog
        analyze_mem_info();
    }
};

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

signals:
    void send_get_mem_info_sig();

public slots:
    void receive_mem_info_content(QString);
    virtual int exec();

private:
    getMemInfo *m_getmem;
    QPushButton *m_ok;
    QVector<QVector<QLabel *> > m_label;
    QVector<QProgressBar *> m_bar;
    float m_free;
    float m_total;
    QFont m_ft;
    qint8 m_count;
};

#endif // SHOWMEMDIALOG_H
