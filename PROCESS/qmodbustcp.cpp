#include "qmodbustcp.h"

#include <QMutexLocker>
#include <QDebug>
#include <errno.h>

#undef __log
#ifdef _LOG_MODBUSCOM_
#define __log qDebug
#else
#define __log QT_NO_QDEBUG_MACRO
#endif

QModbusTCP::QModbusTCP() :
    _pModBus(NULL),
    _remoteHost("127.0.0.1"),
    _remotePort(502),
    _needThreadStop(false)
{
    _pMutexRawData = new QMutex(QMutex::Recursive);
    _pMutexDataRead = new QMutex(QMutex::Recursive);
    _pMutexAcqData = new QMutex(QMutex::Recursive);
    _pMutexQueue = new QMutex(QMutex::Recursive);

    _lastError = MB_ENOERR;
    _accessingParamsQueue.clear();
    _isConnected = false;
    _isPresent = false;
    _noComError = true;
    _dataRead.clear();
    _moduleIdentList.clear();

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
    QMutexLocker locker(_pMutexQueue);

    if(_isConnected)
    {
        s_AccessingElm tmp;
        tmp.command = e_CommandList_Disconnect;
        tmp.moduleId = 0;
        _accessingParamsQueue.enqueue(tmp);
    }
    else
    {
        s_AccessingElm tmp;
        tmp.command = e_CommandList_Connection;
        tmp.moduleId = 0;
        _accessingParamsQueue.enqueue(tmp);
    }
}

void QModbusTCP::disconnectFromDev(void)
{
    s_AccessingElm tmp;
    QMutexLocker locker(_pMutexQueue);

    tmp.command = e_CommandList_Disconnect;
    tmp.moduleId = 0;
    _accessingParamsQueue.enqueue(tmp);
}

bool QModbusTCP::readRegisters(uint16_t moduleId, uint16_t startAddr, uint16_t length)
{
    bool ret = false;
    s_AccessingElm tmp;
    QMutexLocker locker(_pMutexQueue);

    tmp.moduleId = moduleId;
    tmp.startAddr = startAddr;
    tmp.length = length;
    tmp.command = e_CommandList_ReadRegisters;
    __log() << "MODBUS Request Reading Register at " << QString::number(startAddr);
    _accessingParamsQueue.enqueue(tmp);

    tmp.moduleId = 0;
    tmp.command = e_CommandList_Default;
    _accessingParamsQueue.enqueue(tmp);

    return ret;
}

void QModbusTCP::getDataRead(QList<uint16_t> &dataRead)
{
    QMutexLocker locker(_pMutexDataRead);
    dataRead = _dataRead;
}

void QModbusTCP::getDataRead(uint16_t* dataRead, uint16_t count)
{
    QList<uint16_t> _locDataRead;
    QMutexLocker locker(_pMutexDataRead);

    _locDataRead = _dataRead;

    count = _locDataRead.count();
    for(int i=0;i<count;i++)
        dataRead[i] = _locDataRead.at(i);
}

void QModbusTCP::getAcqData(QList<uint16_t> &acqData)
{
    QMutexLocker locker(_pMutexAcqData);
    acqData = _acqData;
}

bool QModbusTCP::writeRegisters(uint16_t moduleId, uint16_t startAddr, uint16_t length, QList<uint16_t> &data)
{
    bool ret = false;
    s_AccessingElm tmp;
    QMutexLocker locker(_pMutexQueue);

    tmp.moduleId = moduleId;
    tmp.startAddr = startAddr;
    tmp.length = length;
    tmp.command = e_CommandList_WriteRegisters;
    tmp.dataToWrite = data;
    __log() << "MODBUS Request Writing Register at " << QString::number(startAddr);
    _accessingParamsQueue.enqueue(tmp);

    tmp.moduleId = 0;
    tmp.command = e_CommandList_Default;
    _accessingParamsQueue.enqueue(tmp);

    return ret;
}

bool QModbusTCP::writeRegisters(uint16_t moduleId, uint16_t startAddr, QList<uint16_t> &data)
{
    bool ret = false;
    s_AccessingElm tmp;
    QMutexLocker locker(_pMutexQueue);

    tmp.moduleId = moduleId;
    tmp.startAddr = startAddr;
    tmp.length = data.length();
    tmp.command = e_CommandList_WriteRegisters;
    tmp.dataToWrite = data;
    __log() << "MODBUS Request Writing Register at " << QString::number(startAddr);
    _accessingParamsQueue.enqueue(tmp);

    tmp.moduleId = 0;
    tmp.command = e_CommandList_Default;
    _accessingParamsQueue.enqueue(tmp);

    return ret;
}

int QModbusTCP::getFreeModuleIdentifier(void)
{
    int ret = 1;

    while(_moduleIdentList.indexOf(ret) != -1 && ret < 255)
    {
        ret++;
    }

    if(ret != 255)
        _moduleIdentList.append(ret);

    return ret;
}

void QModbusTCP::run()
{
    uint16_t data[128];
    int nbReadWrite;
    QMutexLocker locker(_pMutexQueue);
    locker.unlock();

    while(1)
    {
        if(_needThreadStop)
            break;

        locker.relock();
        if(_accessingParamsQueue.isEmpty())
        {
            _curAccessingParams.moduleId = 0;
            _curAccessingParams.command = e_CommandList_Default;
        }
        else
        {
            _curAccessingParams = _accessingParamsQueue.dequeue();
        }
        locker.unlock();

        switch(_curAccessingParams.command)
        {
            case e_CommandList_Connection:
                /* Si deja connecte */
                if(_isConnected)
                    modbus_close(_pModBus);

                _isConnected = false;
                _pModBus = modbus_new_tcp(_remoteHost.toUtf8().constData(),_remotePort);

                if(modbus_connect(_pModBus) != -1)
                {
                    _isConnected = true;
                    emit TCPConnected();
                }
                break;
            case e_CommandList_Disconnect:
                modbus_close(_pModBus);
                _isConnected = false;
    //            _devAddr = "";
    //            _devName = "";
                _noComError = true;
                emit TCPDisconnected();
                break;
            case e_CommandList_ReadRegisters:
                if(_isConnected)
                {
                    __log() << "MODBUS Reading Registers at " << QString::number(_curAccessingParams.startAddr);
                    nbReadWrite = modbus_read_registers(_pModBus,
                                                        _curAccessingParams.startAddr,
                                                        _curAccessingParams.length,
                                                        data);
                    _noComError = false;
                    if(nbReadWrite > 0)
                    {
                        _noComError = true;
                        QMutexLocker locker(_pMutexDataRead);
                        _dataRead.clear();
                        for(int i = 0; i<_curAccessingParams.length; i++)
                            _dataRead.append(data[i]);
                    }
                }
                break;
            case e_CommandList_WriteRegisters:
                if(_isConnected)
                {
                    __log() << "MODBUS Writing Registers at " << QString::number(_curAccessingParams.startAddr);

                    for(int i=0;i<_curAccessingParams.dataToWrite.count();i++)
                        data[i] = _curAccessingParams.dataToWrite.at(i);

                    nbReadWrite = modbus_write_registers(_pModBus,
                                                        _curAccessingParams.startAddr,
                                                        _curAccessingParams.length,
                                                        data);
                    _noComError = false;
                    if(nbReadWrite > 0)
                    {
                        _noComError = true;
                    }
                }
                break;
            default:
                _isPresent = false;
                if(_isConnected)
                {

                    nbReadWrite = modbus_read_registers(_pModBus, MODBUS_READ_ONLY_REG_START_ADDR, MODBUS_NB_RAM_ACQUIS, data);
                    _noComError = false;
                    if(nbReadWrite > 0)
                    {
                        _noComError = true;
                        QMutexLocker locker(_pMutexAcqData);
                        _isPresent = true;
                        _acqData.clear();
                        for(int i = 0; i<MODBUS_NB_RAM_ACQUIS; i++)
                            _acqData.append(data[i]);
                    }
                }
                break;
        }

        if(_curAccessingParams.command != e_CommandList_Default)
            __log() << "MODBUS Command Done : " << QString::number((int)_curAccessingParams.command);

        if(_curAccessingParams.command != e_CommandList_Default || _isConnected)
        {
            emit Done((int)_curAccessingParams.command +
                      ((int)_curAccessingParams.moduleId << 8) +
                      ((!_noComError) << 15));
        }

        if(!_noComError)
        {
            QString errorString;
            _lastError = (eMBErrorCode)(errno - MODBUS_ENOBASE);
            switch(_lastError)
            {
                case MB_ENOERR:
                    errorString = "No error";
                    break;
                case MB_ENOREG:
                    errorString = "Illegal register address";
                    break;
                case MB_EINVAL:
                    errorString = "Illegal argument";
                    break;
                case MB_EPORTERR:
                    errorString = "Porting layer error";
                    break;
                case MB_ENORES:
                    errorString = "Insufficient resources";
                    break;
                case MB_EIO:
                    errorString = "I/O error";
                    break;
                case MB_EILLSTATE:
                    errorString = "Protocol stack in illegal state";
                    break;
                case MB_ETIMEDOUT:
                    errorString = "Timeout !";
                    break;
                default:
                    _lastError = MB_ETIMEDOUT;
                    errorString = "Unknown error !";
                    break;
            }

            qCritical() << errorString;
            emit ComError(errorString);
        }

//        if(!_isConnected)
//        {
//            _lastError = MB_ENOERR;
//            _accessingParamsQueue.clear();
//            _isConnected = false;
//            _isPresent = false;
//            _dataRead.clear();
//    //        _devAddr.clear();
//        }
    }
}

void QModbusTCP::DeviceConnected()
{
    emit TCPConnected();
}

void QModbusTCP::DeviceDisconnected()
{
    _isConnected = false;
//    _devAddr = "";
    emit TCPDisconnected();
}

QModbusTCP::~QModbusTCP()
{
    _needThreadStop = true;
    QThread::wait();

    if(_pModBus != NULL)
    {
        modbus_close(_pModBus);
        modbus_free(_pModBus);
    }
}
