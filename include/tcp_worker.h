#ifndef TCP_WORKER_H
#define TCP_WORKER_H

#include <QObject>
#include <QString>
#include "tcp_client.h"

class TcpWorker : public QObject
{
    Q_OBJECT

public:
    explicit TcpWorker(QObject *parent = nullptr);
    ~TcpWorker() override;

signals:
    void statusReceived(const StatusData &status);
    void packetStatsUpdated(
        uint32_t sequence,
        uint32_t packet_loss,
        uint32_t latest_missing_sequence);

    void connectionStatusChanged(const QString &status);

public slots:
    void Start();
    void Stop();

private:
    TcpClient tcp_client;
    bool stopping;
    bool has_sequence;
    uint32_t last_sequence;
    uint32_t packet_loss;
    uint32_t latest_missing_sequence;
};

#endif