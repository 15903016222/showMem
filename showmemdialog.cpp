#include "showmemdialog.h"
#include "ui_showmemdialog.h"
#include <QtXml>
#include <QLabel>

showMemDialog::showMemDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::showMemDialog)
{
    ui->setupUi(this);
    ui->buttonBox->setGeometry(ui->buttonBox->pos().x(),
                               ui->buttonBox->pos().y(),
                               178, 32);
    ui->progressBar->setTextVisible(false);
    qDebug() << ui->buttonBox->pos() <<endl
             << ui->buttonBox->width() << endl
             << ui->buttonBox->height() << endl;
    //ui->buttonBox->setGeometry(400, 300, ui->buttonBox->width(), ui->buttonBox->height());
    QProcess *proc = new QProcess;
    proc->start("/bin/bash /tmp/getMem.sh");
    proc->waitForFinished();
    delete proc;

    QFile file ("/tmp/demo.xml");
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

    QDomElement root = doc.documentElement();
    QDomNode node = root.firstChild();
    while (!node.isNull()) {
        if (node.isElement()) {
            QDomElement e = node.toElement();
            QDomNodeList list = e.childNodes();
            for (int i = 0; i < list.count(); ++i) {
                QDomNode n = list.at(i);
                if (node.isElement()) {
                    //qDebug() << n.nodeName() << ":" << n.toElement().text();
                }
            }
        }
        node = node.nextSibling();
    }

}

showMemDialog::~showMemDialog()
{
    delete ui;
}
