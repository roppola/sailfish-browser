/****************************************************************************
**
** Copyright (C) 2013 Jolla Ltd.
** Contact: Dmitry Rozhkov <dmitry.rozhkov@jollamobile.com>
**
****************************************************************************/

#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QObject>
#include <QHash>
#include <QString>
#include <QVariant>
#include "browserservice.h"
#include <transferengineinterface.h>
#include <transfertypes.h>


class DownloadManager : public QObject
{
    Q_OBJECT

public:
    explicit DownloadManager(BrowserService *service, QObject *parent = 0);

public slots:
    void cancelActiveTransfers();

private slots:
    void recvObserve(const QString message, const QVariant data);
    void cancelTransfer(int transferId);
    void restartTransfer(int transferId);

private:
    enum Status {
        DownloadStarted,
        DownloadDone,
        DownloadFailed,
        DownloadCanceled
    };

    // TODO: unlike Gecko downloads and Sailfish transfers these mappings
    //       are not persistent -> after user has browser closed transfers can't be
    //       restarted.
    QHash<qulonglong, int> m_download2transferMap;
    QHash<int, qulonglong> m_transfer2downloadMap;
    QHash<qulonglong, Status> m_statusCache;

    TransferEngineInterface *m_transferClient;
    BrowserService *m_service;
};

#endif
