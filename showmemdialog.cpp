#include "showmemdialog.h"
#include "ui_showmemdialog.h"
#include <QtXml>
#include <QLabel>
#include <QProgressBar>

// dialog 600x400
#define X_OFFSET 20
#define Y_OFFSET 60
#define CALCU_MEM(x) ((x) * 1000 / 1024 / 1024 / 1024)
#define WIDTH  270
#define HEIGHT 20
#define VDIVISION ((400 - Y_OFFSET - 40) / 3)
#define HDIVISION ((600 - X_OFFSET * 3) / 2)
#define DISK_NAME   0
#define DISK_VOLUME 1
#define FILE_NAME "/tmp/demo.xml"

void createMemInfo ()
{
    QProcess *proc = new QProcess;
    proc->start("/bin/bash /usr/bin/getMem.sh");
    if (proc->waitForFinished()) {
        proc->start("/bin/bash /home/ws/project/phascan-II/showMem/getMem.sh");
        proc->waitForFinished();
    }
    delete proc;
}

void analyzeMemInfo (showMemDialog *p)
{
    QFile file (FILE_NAME);
    if (!file.open(QFile::ReadOnly)) {
        qDebug() << "open xml failed";
        return;
    }

    QDomDocument doc;
    if (!doc.setContent(&file)) {
        file.close();
        return ;
    }
    file.close();

    QProcess *proc = new QProcess;
    proc->start("rm /tmp/demo.xml");
        proc->waitForFinished();
    delete proc;

    QDomElement root = doc.documentElement();
    QDomNode node = root.firstChild();
    int count = 0;
    while (!node.isNull()) {
        if (node.isElement()) {
            QDomElement e = node.toElement();
            QDomNodeList list = e.childNodes();
            for (int i = 0; i < list.count(); ++i) {
                QDomNode n = list.at(i);
                if (node.isElement()) {
                    //qDebug() << n.nodeName() << ":" << n.toElement().text();
                    p->show_volume(count, n.nodeName(), n.toElement().text());
                }
            }
            ++count;
        }
        node = node.nextSibling();
    }
}

showMemDialog::showMemDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::showMemDialog)
{
    ui->setupUi(this);
    m_ft.setPointSize(14);
    m_free = 0;
    m_total = 0;
    setFixedSize(600, 400);

    this->setStyleSheet("QProgressBar::chunk {background-color: #FF0000; width: 10px;} \
                         QProgressBar { border: 1px solid black;}");

    for (int i = 0; i < MAXMEMNUMS; ++i) {
        m_bar[i] = NULL;
        for (int j = 0; j < LABELNUMS; ++j) {
            m_label[i][j] = NULL;
        }
    }

}

showMemDialog::~showMemDialog()
{
    delete ui;
}

void showMemDialog::show_volume(int num, QString name, QString value)
{
    if (name == QString("name")) {
        m_label[num][DISK_NAME] = new QLabel(this);
        m_label[num][DISK_NAME]->setGeometry(X_OFFSET + (num % 2) * (WIDTH + 20),
                                     Y_OFFSET + (num / 2) * VDIVISION,
                                     WIDTH,
                                     HEIGHT * 2);
        m_label[num][DISK_NAME]->setFont(m_ft);
        m_label[num][DISK_NAME]->setAlignment(Qt::AlignLeft);
        m_label[num][DISK_NAME]->setText(value);
        qDebug() << "name: " << value;
    }
    else if (name == QString("free")) {
        m_free = CALCU_MEM(value.toFloat());
        qDebug() << "free: " << value << " " << m_free;
    }
    else if (name == QString("total")) {
        m_label[num][DISK_VOLUME] = new QLabel(this);
        m_label[num][DISK_VOLUME]->setGeometry(X_OFFSET + (num % 2) * (WIDTH + 20),
                                     Y_OFFSET + 60 + (num / 2) * VDIVISION,
                                     WIDTH,
                                     HEIGHT * 2);
        m_total = CALCU_MEM(value.toFloat());
        m_label[num][DISK_VOLUME]->setFont(m_ft);
        m_label[num][DISK_VOLUME]->setAlignment(Qt::AlignLeft);
        m_label[num][DISK_VOLUME]->setText(QString::number(m_free, 'f', 2) + "/" + QString::number(m_total, 'f', 2) + "GB");
        m_bar[num] = new QProgressBar(this);
        m_bar[num]->setGeometry(X_OFFSET + (num % 2) * (WIDTH + 20),
                                Y_OFFSET + 40 + (num / 2) * VDIVISION,
                                WIDTH,
                                HEIGHT);
        m_bar[num]->setTextVisible(false);
        m_bar[num]->setValue(100 - (m_free / m_total) * 100);
        qDebug() << "total: " << value << " " << m_total;
    }

    return ;
}

void showMemDialog::set_font(int pix)
{
    m_ft.setPointSize(pix);
}

void showMemDialog::delete_mem_info()
{
    for (int i = 0; i < MAXMEMNUMS; ++i) {
        if (m_bar[i]) {
            delete m_bar[i];
            m_bar[i] = NULL;
        }
        for (int j = 0; j < LABELNUMS; ++j) {
            if (m_label[i][j]) {
                delete m_label[i][j];
                m_label[i][j] = NULL;
            }
        }
    }
}

void showMemDialog::show()
{
    createMemInfo();

    analyzeMemInfo(this);

    exec();

    delete_mem_info();
}
