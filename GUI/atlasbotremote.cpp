#include "atlasbotremote.h"
#include "ui_atlasbotremote.h"

#include <QDir>

AtlasBotRemote::AtlasBotRemote(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AtlasBotRemote)
{
    ui->setupUi(this);

    /* Settings */
    _pSettings = new QSettings(QDir::homePath() + "/.atlasBotRemote", QSettings::NativeFormat);
    _pSettings->beginGroup("MODBUS_TCP_SETTINGS");
    _remoteHost     = _pSettings->value("remoteHost", "192.168.1.90").toString();
    _remotePort     = _pSettings->value("remotePort", 502).toInt();
    _pSettings->endGroup();

    /* Dialog Preferences */
    _pDialogPref = new DialogPreferences(this);
    connect(ui->action_Settings, SIGNAL(triggered(bool)), this, SLOT(ShowPrefDialog()));

}

void AtlasBotRemote::ShowPrefDialog()
{
    _pDialogPref->setRemoteHost(_remoteHost);
    _pDialogPref->setRemotePort(_remotePort);

    if(_pDialogPref->exec())
    {
        _remoteHost = _pDialogPref->remoteHost();
        _remotePort = _pDialogPref->remotePort();

        _pSettings->beginGroup("MODBUS_TCP_SETTINGS");
        _pSettings->setValue("remoteHost", _remoteHost);
        _pSettings->setValue("remotePort", _remotePort);
        _pSettings->endGroup();
    }
}

AtlasBotRemote::~AtlasBotRemote()
{
    delete ui;
}
