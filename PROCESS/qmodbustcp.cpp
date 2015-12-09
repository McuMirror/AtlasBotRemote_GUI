#include "qmodbustcp.h"

QModbusTCP::QModbusTCP() :
    _pModBus(NULL),
    _remoteHost("127.0.0.1"),
    _remotePort(502)
{
    _pMutexRawData = new QMutex(QMutex::Recursive);
    _pMutexDataRead = new QMutex(QMutex::Recursive);
    _pMutexAcqData = new QMutex(QMutex::Recursive);

    _lastError = MB_ENOERR;
    _accessingParamsQueue.clear();
    _isConnected = false;
    _noComError = true;
    _isProcessing = false;
    rawData_safeClear();
    _dataRead.clear();
    _moduleIdentList.clear();

    connect(this, SIGNAL(finished()), this, SLOT(RepeatingProcess()));
    _isProcessing = true;
    start(QThread::TimeCriticalPriority);
}

void QModbusTCP::setRemoteHost(QString remoteHost)
{
    _remoteHost = remoteHost;
}

void QModbusTCP::setRemotePort(int remotePort)
{
    _remotePort = remotePort;
}

void QModbusTCP::openTCPAndConnect(void)
{
    if(_isConnected)
    {
        s_AccessingElm tmp;
        tmp.command = e_CommandList_Disconnect;
        _accessingParamsQueue.enqueue(tmp);
    }
    else
    {
        s_AccessingElm tmp;
        tmp.command = e_CommandList_Connection;
        _accessingParamsQueue.enqueue(tmp);
    }
}

void QModbusTCP::disconnect(void)
{
    s_AccessingElm tmp;
    tmp.command = e_CommandList_Disconnect;
    _accessingParamsQueue.enqueue(tmp);
}

bool QModbusTCP::isConnected(void)
{
    return _isConnected;
}

bool QModbusTCP::isNoComError(void)
{

}

bool QModbusTCP::readRegisters(uint16_t moduleId, uint16_t startAddr, uint16_t length)
{

}

void QModbusTCP::getDataRead(QList<uint16_t> &dataRead)
{

}

void QModbusTCP::getDataRead(uint16_t* dataRead, uint16_t count)
{

}

void QModbusTCP::getAcqData(QList<uint16_t> &acqData)
{

}

bool QModbusTCP::writeRegisters(uint16_t moduleId, uint16_t startAddr, uint16_t length, QList<uint16_t> &data)
{

}

bool QModbusTCP::writeRegisters(uint16_t moduleId, uint16_t startAddr, QList<uint16_t> &data)
{

}

int QModbusTCP::getFreeModuleIdentifier(void)
{

}

void QModbusTCP::run()
{

}

void QModbusTCP::DeviceConnected(QString)
{

}

void QModbusTCP::DeviceDisconnected()
{

}

void QModbusTCP::rawData_safeClear(void)
{

}

int QModbusTCP::rawData_safeCount(void)
{

}

QByteArray& QModbusTCP::rawData_safeAppend(const QByteArray &a)
{

}

void QModbusTCP::RepeatingProcess(void)
{

}

void QModbusTCP::RawDataReceived(QByteArray)
{

}

QModbusTCP::~QModbusTCP()
{
    modbus_free(_pModBus);
}
