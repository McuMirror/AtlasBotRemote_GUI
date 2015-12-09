#ifndef ATLASBOTREMOTE_H
#define ATLASBOTREMOTE_H

#include <QMainWindow>

namespace Ui {
class AtlasBotRemote;
}

class AtlasBotRemote : public QMainWindow
{
        Q_OBJECT

    public:
        explicit AtlasBotRemote(QWidget *parent = 0);
        ~AtlasBotRemote();

    private:
        Ui::AtlasBotRemote *ui;
};

#endif // ATLASBOTREMOTE_H
