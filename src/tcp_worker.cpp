#include "tcp_worker.h"
#include <QThread>
#include <QDebug>

TcpWorker::TcpWorker(QObject *parent)
    : QObject(parent),
      stopping(false),
      has_sequence(false),
      last_sequence(0),
      packet_loss(0),
      latest_missing_sequence(0)
{
}

TcpWorker::~TcpWorker()
{
}


void TcpWorker::Start()
{
    qDebug() << "[TcpWorker] Start() thread:" << QThread::currentThread();

    constexpr unsigned long RECONNECT_INTERVAL_MS = 1000U;

    while (!stopping)
    {
        emit connectionStatusChanged("Connecting...");

        const bool connected =
            tcp_client.Connect("192.168.137.10", 8888);

        if (!connected)
        {
            qDebug() << "[TcpWorker] Connect failed";
            emit connectionStatusChanged("Connection Failed");

            tcp_client.Close();

            if (!stopping)
            {
                qDebug() << "[TcpWorker] Reconnecting in"
                         << RECONNECT_INTERVAL_MS
                         << "ms";

                QThread::msleep(RECONNECT_INTERVAL_MS);
            }

            continue;
        }

        qDebug() << "[TcpWorker] Connected to STM32";
        emit connectionStatusChanged("Connected");

        uint8_t receive_buffer[4096];

        while (!stopping && tcp_client.IsConnected())
        {
            const int received =
                tcp_client.Receive(receive_buffer, sizeof(receive_buffer));

            if (received > 0)
            {
                qDebug() << "[TcpWorker] Received"
                         << received
                         << "bytes";

                uint8_t packet_buffer[151];

                while (tcp_client.GetStatusPacket(packet_buffer, sizeof(packet_buffer)))
                {
                    qDebug() << "[TcpWorker] Complete status packet received";

                    const uint32_t sequence =
                        static_cast<uint32_t>(packet_buffer[4]) |
                        (static_cast<uint32_t>(packet_buffer[5]) << 8) |
                        (static_cast<uint32_t>(packet_buffer[6]) << 16) |
                        (static_cast<uint32_t>(packet_buffer[7]) << 24);

                    if (!has_sequence)
                    {
                        has_sequence = true;
                    }
                    else
                    {
                        const uint32_t expected_sequence = last_sequence + 1U;

                        if (sequence != expected_sequence)
                        {
                            packet_loss++;
                            latest_missing_sequence = expected_sequence;

                            qDebug() << "[TcpWorker] Packet loss detected. Missing sequence:"
                                     << latest_missing_sequence;
                        }
                    }

                    last_sequence = sequence;

                    qDebug() << "[TcpWorker] Sequence:" << sequence;

                    emit packetStatsUpdated(
                        sequence,
                        packet_loss,
                        latest_missing_sequence);

                    uint8_t raw_data[68];

                    const bool decoded =
                        tcp_client.DecodeStatusPayload(
                            packet_buffer,
                            sizeof(packet_buffer),
                            raw_data,
                            sizeof(raw_data));

                    if (decoded)
                    {
                        qDebug() << "[TcpWorker] Status payload decoded: 68 bytes";

                        StatusData status_data;

                        const bool parsed =
                            tcp_client.ParseStatusData(
                                raw_data,
                                sizeof(raw_data),
                                status_data);

                        if (parsed)
                        {
                            qDebug() << "[TcpWorker] StatusData parsed successfully";

                            emit statusReceived(status_data);
                        }
                        else
                        {
                            qDebug() << "[TcpWorker] StatusData parse failed";
                        }
                    }
                    else
                    {
                        qDebug() << "[TcpWorker] Status payload decode failed";
                    }
                }
            }
            else
            {
                qDebug() << "[TcpWorker] Receive stopped:"
                         << received;

                if (!stopping)
                {
                    emit connectionStatusChanged("Connection Lost");
                }

                break;
            }
        }

        tcp_client.Close();

        if (!stopping)
        {
            qDebug() << "[TcpWorker] Reconnecting in"
                     << RECONNECT_INTERVAL_MS
                     << "ms";

            QThread::msleep(RECONNECT_INTERVAL_MS);
        }
    }

    qDebug() << "[TcpWorker] Receive loop ended";
}


void TcpWorker::Stop()
{
    stopping = true;

    tcp_client.Close();

    qDebug() << "[TcpWorker] Stop()";
}