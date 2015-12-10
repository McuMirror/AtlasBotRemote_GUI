#ifndef ATLASBOTREMOTE_H
#define ATLASBOTREMOTE_H

#include <QMainWindow>
#include <QSettings>

#include "dialogpreferences.h"

#include "../PROCESS/qmodbustcp.h"

namespace Ui {
class AtlasBotRemote;
}

class AtlasBotRemote : public QMainWindow
{
        Q_OBJECT

    public:
        explicit AtlasBotRemote(QWidget *parent = 0);
        ~AtlasBotRemote();

    private slots:
        void ShowPrefDialog(void);
        void ModbusDone(int result);
        void ConnectedToDevice(QString peerName);
        void DisconnectedFromDevice();

    private:
        Ui::AtlasBotRemote *ui;

        QSettings *_pSettings;
        DialogPreferences *_pDialogPref;
        QModbusTCP *_pModbusTCP;
};

#endif // ATLASBOTREMOTE_H
