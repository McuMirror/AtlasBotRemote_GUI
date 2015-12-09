#ifndef QMODBUSTCP_H
#define QMODBUSTCP_H

#include <QStringList>
#include <QThread>
#include <QTimer>
#include <QQueue>
#include <QMutex>

//#include "dicoModbus.h"
#include "../LIB/modbus.h"

class QModbusTCP : public QThread
{
        Q_OBJECT

#define MODBUS_DELAY_REFRESH        100
#define MODBUS_NB_RAM_ACQUIS        36

        enum eMBErrorCode
        {
            MB_ENOERR,                  /*!< no error. */
            MB_ENOREG,                  /*!< illegal register address. */
            MB_EINVAL,                  /*!< illegal argument. */
            MB_EPORTERR,                /*!< porting layer error. */
            MB_ENORES,                  /*!< insufficient resources. */
            MB_EIO,                     /*!< I/O error. */
            MB_EILLSTATE,               /*!< protocol stack in illegal state. */
            MB_ETIMEDOUT                /*!< timeout error occurred. */
        } ;

        enum e_CommandList
        {
            e_CommandList_Default,
            e_CommandList_Connection,
            e_CommandList_AdvIdent,
            e_CommandList_ReadRegisters,
            e_CommandList_WriteRegisters,
            e_CommandList_Disconnect
        };

        struct s_AccessingElm
        {
            uint16_t moduleId;
            uint16_t startAddr;
            uint16_t length;
            e_CommandList command;
            QList<uint16_t> dataToWrite;
        };

    public:
        QModbusTCP();
        ~QModbusTCP();

        void setRemoteHost(QString remoteHost);
        void setRemotePort(int remotePort);

        void openTCPAndConnect(void);
        void disconnect(void);
        bool isConnected(void);
        bool isNoComError(void);
        bool readRegisters(uint16_t moduleId, uint16_t startAddr, uint16_t length);
        void getDataRead(QList<uint16_t> &dataRead);
        void getDataRead(uint16_t* dataRead, uint16_t count);
        void getAcqData(QList<uint16_t> &acqData);
        bool writeRegisters(uint16_t moduleId, uint16_t startAddr, uint16_t length, QList<uint16_t> &data);
        bool writeRegisters(uint16_t moduleId, uint16_t startAddr, QList<uint16_t> &data);

        int getFreeModuleIdentifier(void);
        inline void releaseModuleIdentifier(int id) {_moduleIdentList.removeOne(id);}

        inline eMBErrorCode GetLastError(void) const {return _lastError;}

    public slots:
        void run();
        void DeviceConnected(QString);
        void DeviceDisconnected();

    private:
        void rawData_safeClear(void);
        int rawData_safeCount(void);
        QByteArray &rawData_safeAppend(const QByteArray &a);

    signals:
        void Done(int);
        void ComError(QString);
        void BlueConnected(QString);
        void BlueDisconnected();

    private slots:
        void RepeatingProcess(void);
        void RawDataReceived(QByteArray);

    private:
        modbus_t* _pModBus;
        QString _remoteHost;
        int _remotePort;
        bool _noComError;
        QMutex *_pMutexRawData, *_pMutexDataRead, *_pMutexAcqData;
        eMBErrorCode _lastError;
        bool _isConnected;
        bool _isProcessing;
        QByteArray _rawData;
        QQueue<s_AccessingElm> _accessingParamsQueue;
        s_AccessingElm _curAccessingParams;
        QList<uint16_t> _dataRead;
        QList<uint16_t> _acqData;
        QList<int> _moduleIdentList;
};

#endif // QMODBUSTCP_H
