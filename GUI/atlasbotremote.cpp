#include "atlasbotremote.h"
#include "ui_atlasbotremote.h"

#include <QDir>

AtlasBotRemote::AtlasBotRemote(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AtlasBotRemote)
{
    ui->setupUi(this);

    /* Init Modbus TCP */
    _pModbusTCP = new QModbusTCP();
    connect(_pModbusTCP, SIGNAL(Done(int)), this, SLOT(ModbusDone(int)));
    connect(_pModbusTCP, SIGNAL(TCPConnected(QString)), this, SLOT(ConnectedToDevice(QString)));
    connect(_pModbusTCP, SIGNAL(TCPDisconnected()), this, SLOT(DisconnectedFromDevice()));

    /* Settings */
    _pSettings = new QSettings(QDir::homePath() + "/.atlasBotRemote", QSettings::NativeFormat);
    _pSettings->beginGroup("MODBUS_TCP_SETTINGS");
    _pModbusTCP->setRemoteHost(_pSettings->value("remoteHost", "192.168.1.90").toString());
    _pModbusTCP->setRemotePort(_pSettings->value("remotePort", 502).toInt());
    _pSettings->endGroup();

    /* Dialog Preferences */
    _pDialogPref = new DialogPreferences(this);
    connect(ui->action_Settings, SIGNAL(triggered(bool)), this, SLOT(ShowPrefDialog()));
}

void AtlasBotRemote::ShowPrefDialog()
{
    _pDialogPref->setRemoteHost(_pModbusTCP->remoteHost());
    _pDialogPref->setRemotePort(_pModbusTCP->remotePort());

    if(_pDialogPref->exec())
    {
        _pModbusTCP->setRemoteHost(_pDialogPref->remoteHost());
        _pModbusTCP->setRemotePort(_pDialogPref->remotePort());

        _pSettings->beginGroup("MODBUS_TCP_SETTINGS");
        _pSettings->setValue("remoteHost", _pDialogPref->remoteHost());
        _pSettings->setValue("remotePort", _pDialogPref->remotePort());
        _pSettings->endGroup();
    }
}

void AtlasBotRemote::ModbusDone(int result)
{

}

void AtlasBotRemote::ConnectedToDevice(QString peerName)
{
//    _connectedDevName = peerName;
//    _connectedDevAddr = _pModbus->GetDeviceAddr();
}

void AtlasBotRemote::DisconnectedFromDevice()
{

}

AtlasBotRemote::~AtlasBotRemote()
{
    delete _pDialogPref;
    delete _pModbusTCP;
    delete ui;
}
