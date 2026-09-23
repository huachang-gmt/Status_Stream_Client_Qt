#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include "status_data.h"

class QLabel;
class TcpWorker;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    StatusData current_status;
    QThread *tcp_thread;
    TcpWorker *tcp_worker;
    void updateStatusDisplay(const StatusData &status);

    void updatePacketStats(
            uint32_t sequence,
            uint32_t packet_loss,
            uint32_t latest_missing_sequence);

    void updateConnectionStatus(const QString &status);

    QLabel *connection_status_value_label;
    QLabel *sequence_value_label;
    QLabel *loss_value_label;
    QLabel *missing_value_label;

    QLabel *connect_value_label;
    QLabel *voltage_value_label;
    QLabel *moving_value_label;
    QLabel *fa_value_label;
    QLabel *homing_value_label;
    QLabel *error_value_label;

    QLabel *ai_value_label[8];

    QLabel *x_value_label;
    QLabel *y_value_label;
    QLabel *z_value_label;
    QLabel *rx_value_label;
    QLabel *ry_value_label;
    QLabel *rz_value_label;

};

#endif // MAINWINDOW_H
