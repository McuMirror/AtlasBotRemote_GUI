#include "dialogpreferences.h"
#include "ui_dialogpreferences.h"

DialogPreferences::DialogPreferences(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogPreferences),
    _remoteHost("192.168.1.90"),
    _remotePort(502)
{
    ui->setupUi(this);

    connect(ui->spinBox_Addr1, SIGNAL(editingFinished()), this, SLOT(RemoteHostChanged()));
    connect(ui->spinBox_Addr2, SIGNAL(editingFinished()), this, SLOT(RemoteHostChanged()));
    connect(ui->spinBox_Addr3, SIGNAL(editingFinished()), this, SLOT(RemoteHostChanged()));
    connect(ui->spinBox_Addr4, SIGNAL(editingFinished()), this, SLOT(RemoteHostChanged()));
    connect(ui->spinBox_Port,  SIGNAL(editingFinished()), this, SLOT(RemotePortChanged()));
}


void DialogPreferences::setRemoteHost(QString host)
{
    QStringList listStr = host.split(".");
    if(listStr.count() == 4)
    {
        _remoteHost = host;
        ui->spinBox_Addr1->setValue(listStr.at(0).toInt());
        ui->spinBox_Addr2->setValue(listStr.at(1).toInt());
        ui->spinBox_Addr3->setValue(listStr.at(2).toInt());
        ui->spinBox_Addr4->setValue(listStr.at(3).toInt());
    }
}

void DialogPreferences::setRemotePort(int port)
{
    _remotePort = port;
    ui->spinBox_Port->setValue(port);
}

void DialogPreferences::RemoteHostChanged(void)
{
    _remoteHost = ui->spinBox_Addr1->text() + "." +
                  ui->spinBox_Addr2->text() + "." +
                  ui->spinBox_Addr3->text() + "." +
                  ui->spinBox_Addr4->text();
}

void DialogPreferences::RemotePortChanged(void)
{
    _remotePort = ui->spinBox_Port->value();
}

DialogPreferences::~DialogPreferences()
{
    delete ui;
}
