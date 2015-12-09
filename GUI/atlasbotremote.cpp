#include "atlasbotremote.h"
#include "ui_atlasbotremote.h"

AtlasBotRemote::AtlasBotRemote(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AtlasBotRemote)
{
    ui->setupUi(this);
}

AtlasBotRemote::~AtlasBotRemote()
{
    delete ui;
}
