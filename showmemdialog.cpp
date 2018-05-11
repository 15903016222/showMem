#include "showmemdialog.h"
#include <QPushButton>
#include <QLabel>
#include <QProgressBar>
#include <QProcess>
#include <QtXml>
#include <QThread>

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

showMemDialog::showMemDialog(QWidget *parent) :
    QDialog(parent),
    m_ok(new QPushButton(this)),
    m_getmem(new getMemInfo),
    m_free(0.0),
    m_total(0.0),
    m_num(0)
{
    this->setupUi();
    QThread *thread = new QThread;
    connect(this,
            SIGNAL(send_get_mem_info_sig()),
            m_getmem,
            SLOT(receive_get_mem_info_sig()));
    connect(m_getmem,
            SIGNAL(send_mem_info_content(QString)),
            this,
            SLOT(receive_mem_info_content(QString)));

    m_getmem->moveToThread(thread);
    thread->start();
}

showMemDialog::~showMemDialog()
{
    for (int i = 0; i < MAXMEMNUMS; ++i) {
        delete m_bar[i];
        delete m_label[i][DISK_NAME];
        delete m_label[i][DISK_VOLUME];
    }
    delete m_getmem;
    delete m_ok;
}

void showMemDialog::hide_label_progreebar()
{
    for (int i = 0; i < MAXMEMNUMS; ++i) {
        m_bar[i]->setVisible(false);
        for (int j = 0; j < LABELNUMS; ++j) {
            if (m_label[i][j]) {
                m_label[i][j]->setVisible(false);
            }
        }
    }
}

void showMemDialog::setupUi()
{
    setWindowFlags(this->windowFlags() & ~(Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint));
    setFixedSize(600, 400);
    m_ok->setText("确定");
    m_ok->setGeometry(496, 360, 89, 25);
    connect(m_ok, SIGNAL(clicked(bool)), this, SLOT(close()));
    m_ft.setPointSize(14);

    this->setStyleSheet(PROGRESSBAR_STYLE);

    for (int i = 0; i < MAXMEMNUMS; ++i) {
        m_bar[i] = new QProgressBar(this);
        m_bar[i]->setGeometry(X_OFFSET + (i % 2) * (WIDTH + 20),
                                Y_OFFSET + 40 + (i / 2) * VDIVISION,
                                WIDTH,
                                HEIGHT);

        m_bar[i]->setTextVisible(false);
        m_bar[i]->setVisible(false);
        m_label[i][DISK_NAME] = new QLabel(this);
        m_label[i][DISK_NAME]->setGeometry(X_OFFSET + (i % 2) * (WIDTH + 20),
                                     Y_OFFSET + (i / 2) * VDIVISION,
                                     WIDTH,
                                     HEIGHT * 2);
        m_label[i][DISK_NAME]->setFont(m_ft);
        m_label[i][DISK_NAME]->setAlignment(Qt::AlignLeft);
        m_label[i][DISK_NAME]->setVisible(false);
        m_label[i][DISK_VOLUME] = new QLabel(this);
        m_label[i][DISK_VOLUME]->setGeometry(X_OFFSET + (i % 2) * (WIDTH + 20),
                                     Y_OFFSET + 60 + (i / 2) * VDIVISION,
                                     WIDTH,
                                     HEIGHT * 2);
        m_label[i][DISK_VOLUME]->setFont(m_ft);
        m_label[i][DISK_VOLUME]->setAlignment(Qt::AlignLeft);
        m_label[i][DISK_VOLUME]->setVisible(false);
    }
}

void showMemDialog::display_mem_info(int num, QString name, QString value)
{
    qDebug() << "num: " << num;
    if (!name.compare(QString("name"))) {
        m_label[num][DISK_NAME]->setText(value);
        m_label[num][DISK_NAME]->setVisible(true);
    }
    else if (!name.compare(QString("free"))) {
        m_free = CALCU_MEM(value.toFloat());
    }
    else if (!name.compare(QString("total"))) {
        m_total = CALCU_MEM(value.toFloat());

        m_label[num][DISK_VOLUME]->setText(QString::number(m_free, 'f', 2) + "/" + QString::number(m_total, 'f', 2) + "GB");
        m_label[num][DISK_VOLUME]->setVisible(true);

        m_bar[num]->setValue(100 - (m_free / m_total) * 100);
        m_bar[num]->setVisible(true);
    }

    return ;
}

int showMemDialog::exec()
{
    qDebug() << QThread::currentThread();
    emit send_get_mem_info_sig();
    m_num = 0;
    // 初始化显示模块
    hide_label_progreebar();

    this->show();
}

void showMemDialog::receive_mem_info_content(QString content)
{
    qDebug() << __func__ << QThread::currentThread();
    QString name = content;
    name.truncate(name.indexOf('+'));
    QString value;
    value = content.right(content.length() - content.indexOf('+') - 1);
    qDebug() << name << " : " << value;
    display_mem_info(m_num / 3, name, value);
    ++m_num;
}
