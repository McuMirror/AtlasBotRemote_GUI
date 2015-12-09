#ifndef DIALOGPREFERENCES_H
#define DIALOGPREFERENCES_H

#include <QDialog>

namespace Ui {
class DialogPreferences;
}

class DialogPreferences : public QDialog
{
        Q_OBJECT

    public:
        explicit DialogPreferences(QWidget *parent = 0);
        ~DialogPreferences();

        void setRemoteHost(QString host);
        inline QString remoteHost(void) const {return _remoteHost;}
        void setRemotePort(int port);
        inline int remotePort(void) const {return _remotePort;}

    private slots:
        void RemoteHostChanged(void);
        void RemotePortChanged(void);

    private:
        Ui::DialogPreferences *ui;
        QString _remoteHost;
        int _remotePort;
};

#endif // DIALOGPREFERENCES_H
