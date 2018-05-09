#include "showmemdialog.h"
#include <QPushButton>
#include <QLabel>
#include <QProgressBar>
#include <QProcess>
#include <QtXml>
#include <QMessageBox>

#define CALCU_MEM(x) ((x) * 1000 / 1024 / 1024 / 1024)

// dialog 600x400
#define X_OFFSET 20
#define Y_OFFSET 60
#define WIDTH  270
#define HEIGHT 20

#define VDIVISION ((400 - Y_OFFSET - 40) / 3)
#define HDIVISION ((600 - X_OFFSET * 3) / 2)

#define DISK_NAME   0
#define DISK_VOLUME 1

#define PROGRESSBAR_STYLE ("QProgressBar::chunk {background-color: #FF0000; width: 10px;} \
                             QProgressBar { border: 1px solid black;}")

#define FILE_NAME ("/tmp/mem.xml")
#define RM_MEM_FILE ("rm /tmp/mem.xml")
#define GET_MEM_CMD1 ("/bin/bash /usr/bin/getMem.sh")
#define GET_MEM_CMD2 ("/bin/bash /home/ws/project/phascan-II/showMem/getMem.sh")

void get_mem_info()
{
    QProcess *proc = new QProcess;
    proc->start(GET_MEM_CMD1);
    if (proc->waitForFinished()) {
        proc->start(GET_MEM_CMD2);
        proc->waitForFinished();
    }
    delete proc;
}

showMemDialog::showMemDialog(QWidget *parent) :
    QDialog(parent),
    m_ok(new QPushButton(this))
{
    this->setUi();
    m_free  = 0.0;
    m_total = 0.0;
}

showMemDialog::~showMemDialog()
{
    init_label_progreebar();
    delete m_ok;
}

void showMemDialog::init_label_progreebar()
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

void showMemDialog::setUi()
{
    setWindowFlags(this->windowFlags() & ~(Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint));
    setFixedSize(600, 400);
    m_ok->setText("确定");
    m_ok->setGeometry(496, 360, 89, 25);
    connect(m_ok, SIGNAL(clicked(bool)), this, SLOT(close()));
    m_ft.setPointSize(14);

    this->setStyleSheet(PROGRESSBAR_STYLE);

    for (int i = 0; i < MAXMEMNUMS; ++i) {
        m_bar[i] = NULL;
        for (int j = 0; j < LABELNUMS; ++j) {
            m_label[i][j] = NULL;
        }
    }
}

void showMemDialog::display_mem_info(int num, QString name, QString value)
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
    }
    else if (name == QString("free")) {
        m_free = CALCU_MEM(value.toFloat());
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
    }

    return ;
}

int open_file(QString file_name, QDomDocument *doc)
{
    QFile file (file_name);
    if (!file.open(QFile::ReadOnly)) {
        return -1;
    }

    if (!doc->setContent(&file)) {
        file.close();
        return -1;
    }
    file.close();

    // delete xml file
    QProcess *proc = new QProcess;
    proc->start(RM_MEM_FILE);
    proc->waitForFinished();
    delete proc;

    return 0;
}

int showMemDialog::analyze_mem_info(QString file_name)
{
    QDomDocument *doc = new QDomDocument;
    if (0 != open_file(file_name, doc)) {
        return -1;
    }

    QDomElement root = doc->documentElement();
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
                    display_mem_info(count, n.nodeName(), n.toElement().text());
                }
            }
            ++count;
        }
        node = node.nextSibling();
    }

    return 0;
}

int showMemDialog::exec()
{
    // 初始化显示模块
    init_label_progreebar();

    // 获取系统存储信息
    get_mem_info();

    // 解析数据
    if (-1 == analyze_mem_info(FILE_NAME)) {
        QMessageBox::critical(this,
                              "Error",
                              "不能获取存储信息",
                              QMessageBox::Abort);
        return -1;
    }

    this->show();
}
