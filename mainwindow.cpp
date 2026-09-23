#include "mainwindow.h"
#include "tcp_worker.h"
#include <QGroupBox>
#include <QLabel>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      tcp_thread(new QThread(this)),
      tcp_worker(new TcpWorker())
{
    setWindowTitle("GMT Status Stream Client");
    resize(900, 700);

    
    QWidget *central_widget = new QWidget(this);
    setCentralWidget(central_widget);

    QVBoxLayout *main_layout = new QVBoxLayout(central_widget);

    // ----------------- CONNECTION -------------------
    QGroupBox *connection_group = new QGroupBox("Connection");
    QVBoxLayout *connection_layout = new QVBoxLayout(connection_group);

    QHBoxLayout *server_layout = new QHBoxLayout();
    QLabel *server_name_label = new QLabel("Server");
    QLabel *server_value_label = new QLabel("192.168.137.10:8888");
    server_name_label->setFixedWidth(100);
    server_layout->addWidget(server_name_label);
    server_layout->addWidget(server_value_label);
    connection_layout->addLayout(server_layout);

    QHBoxLayout *status_layout = new QHBoxLayout();
    QLabel *status_name_label = new QLabel("Status");
    connection_status_value_label = new QLabel("Connecting...");
    status_name_label->setFixedWidth(100);
    status_layout->addWidget(status_name_label);
    status_layout->addWidget(connection_status_value_label);
    connection_layout->addLayout(status_layout);

    main_layout->addWidget(connection_group);

    // ----------------- Packet -------------------
    QGroupBox *packet_group = new QGroupBox("Packet");
    QVBoxLayout *packet_layout = new QVBoxLayout(packet_group);

    QHBoxLayout *sequence_layout = new QHBoxLayout();
    QLabel *sequence_name_label = new QLabel("Current Sequence");
    sequence_value_label = new QLabel("0");
    sequence_name_label->setFixedWidth(100);
    sequence_layout->addWidget(sequence_name_label);
    sequence_layout->addWidget(sequence_value_label);
    packet_layout->addLayout(sequence_layout);

    QHBoxLayout *loss_layout = new QHBoxLayout();
    QLabel *loss_name_label = new QLabel("Packet Loss");
    loss_value_label = new QLabel("0");
    loss_name_label->setFixedWidth(100);
    loss_layout->addWidget(loss_name_label);
    loss_layout->addWidget(loss_value_label);
    packet_layout->addLayout(loss_layout);

    QHBoxLayout *missing_layout = new QHBoxLayout();
    QLabel *missing_name_label = new QLabel("Latest Missing Sequence");
    missing_value_label = new QLabel("N/A");
    missing_name_label->setFixedWidth(100);
    missing_layout->addWidget(missing_name_label);
    missing_layout->addWidget(missing_value_label);
    packet_layout->addLayout(missing_layout);

    main_layout->addWidget(packet_group);


    // ----------------- Controller -------------------
    QGroupBox *controller_group = new QGroupBox("Controller");
    QVBoxLayout *controller_layout = new QVBoxLayout(controller_group);
        
    QHBoxLayout *connect_layout = new QHBoxLayout();
    QLabel *connect_name_label = new QLabel("CONNECT");
    connect_value_label = new QLabel("ON");
    connect_name_label->setFixedWidth(100);
    connect_layout->addWidget(connect_name_label);
    connect_layout->addWidget(connect_value_label);
    controller_layout->addLayout(connect_layout);

    QHBoxLayout *voltage_layout = new QHBoxLayout();
    QLabel *voltage_name_label = new QLabel("VOLTAGEON");
    voltage_value_label = new QLabel("ON");
    voltage_name_label->setFixedWidth(100);
    voltage_layout->addWidget(voltage_name_label);
    voltage_layout->addWidget(voltage_value_label);
    controller_layout->addLayout(voltage_layout);

    QHBoxLayout *moving_layout = new QHBoxLayout();
    QLabel *moving_name_label = new QLabel("ISMOVING");
    moving_value_label = new QLabel("OFF");
    moving_name_label->setFixedWidth(100);
    moving_layout->addWidget(moving_name_label);
    moving_layout->addWidget(moving_value_label);
    controller_layout->addLayout(moving_layout);

    QHBoxLayout *fa_layout = new QHBoxLayout();
    QLabel *fa_name_label = new QLabel("ISFA");
    fa_value_label = new QLabel("OFF");
    fa_name_label->setFixedWidth(100);
    fa_layout->addWidget(fa_name_label);
    fa_layout->addWidget(fa_value_label);
    controller_layout->addLayout(fa_layout);

    QHBoxLayout *homing_layout = new QHBoxLayout();
    QLabel *homing_name_label = new QLabel("HOMINGEND");
    homing_value_label = new QLabel("OFF");
    homing_name_label->setFixedWidth(100);
    homing_layout->addWidget(homing_name_label);
    homing_layout->addWidget(homing_value_label);
    controller_layout->addLayout(homing_layout);

    QHBoxLayout *error_layout = new QHBoxLayout();
    QLabel *error_name_label = new QLabel("ERROR");
    error_value_label = new QLabel("OFF");
    error_name_label->setFixedWidth(100);
    error_layout->addWidget(error_name_label);
    error_layout->addWidget(error_value_label);
    controller_layout->addLayout(error_layout);

    main_layout->addWidget(controller_group);

    // ----------------- Analog Input -------------------
    QGroupBox *analog_group = new QGroupBox("Analog Input");
    QVBoxLayout *analog_layout = new QVBoxLayout(analog_group);

    QHBoxLayout *ai00_layout = new QHBoxLayout();
    QLabel *ai00_name_label = new QLabel("AI00");
    ai_value_label[0] = new QLabel("-2.500 V");
    ai00_name_label->setFixedWidth(100);
    ai00_layout->addWidget(ai00_name_label);
    ai00_layout->addWidget(ai_value_label[0]);
    analog_layout->addLayout(ai00_layout);

    QHBoxLayout *ai01_layout = new QHBoxLayout();
    QLabel *ai01_name_label = new QLabel("AI01");
    ai_value_label[1] = new QLabel("RAW 0");
    ai01_name_label->setFixedWidth(100);
    ai01_layout->addWidget(ai01_name_label);
    ai01_layout->addWidget(ai_value_label[1]);
    analog_layout->addLayout(ai01_layout);

    QHBoxLayout *ai02_layout = new QHBoxLayout();
    QLabel *ai02_name_label = new QLabel("AI02");
    ai_value_label[2] = new QLabel("RAW 13107");
    ai02_name_label->setFixedWidth(100);
    ai02_layout->addWidget(ai02_name_label);
    ai02_layout->addWidget(ai_value_label[2]);
    analog_layout->addLayout(ai02_layout);

    QHBoxLayout *ai03_layout = new QHBoxLayout();
    QLabel *ai03_name_label = new QLabel("AI03");
    ai_value_label[3] = new QLabel("RAW 19660");
    ai03_name_label->setFixedWidth(100);
    ai03_layout->addWidget(ai03_name_label);
    ai03_layout->addWidget(ai_value_label[3]);
    analog_layout->addLayout(ai03_layout);

    QHBoxLayout *ai04_layout = new QHBoxLayout();
    QLabel *ai04_name_label = new QLabel("AI04");
    ai_value_label[4] = new QLabel("4.000 V");
    ai04_name_label->setFixedWidth(100);
    ai04_layout->addWidget(ai04_name_label);
    ai04_layout->addWidget(ai_value_label[4]);
    analog_layout->addLayout(ai04_layout);

    QHBoxLayout *ai05_layout = new QHBoxLayout();
    QLabel *ai05_name_label = new QLabel("AI05");
    ai_value_label[5] = new QLabel("5.000 V");
    ai05_name_label->setFixedWidth(100);
    ai05_layout->addWidget(ai05_name_label);
    ai05_layout->addWidget(ai_value_label[5]);
    analog_layout->addLayout(ai05_layout);

    QHBoxLayout *ai06_layout = new QHBoxLayout();
    QLabel *ai06_name_label = new QLabel("AI06");
    ai_value_label[6] = new QLabel("6.000 V");
    ai06_name_label->setFixedWidth(100);
    ai06_layout->addWidget(ai06_name_label);
    ai06_layout->addWidget(ai_value_label[6]);
    analog_layout->addLayout(ai06_layout);

    QHBoxLayout *ai07_layout = new QHBoxLayout();
    QLabel *ai07_name_label = new QLabel("AI07");
    ai_value_label[7] = new QLabel("7.000 V");
    ai07_name_label->setFixedWidth(100);
    ai07_layout->addWidget(ai07_name_label);
    ai07_layout->addWidget(ai_value_label[7]);
    analog_layout->addLayout(ai07_layout);

    main_layout->addWidget(analog_group);

    // ----------------- Position -------------------
    QGroupBox *position_group = new QGroupBox("Position");
    QVBoxLayout *position_layout = new QVBoxLayout(position_group);

    QHBoxLayout *x_layout = new QHBoxLayout();
    QLabel *x_name_label = new QLabel("X");
    x_value_label = new QLabel("10.000");
    x_name_label->setFixedWidth(100);
    x_layout->addWidget(x_name_label);
    x_layout->addWidget(x_value_label);
    position_layout->addLayout(x_layout);

    QHBoxLayout *y_layout = new QHBoxLayout();
    QLabel *y_name_label = new QLabel("Y");
    y_value_label = new QLabel("20.000");
    y_name_label->setFixedWidth(100);
    y_layout->addWidget(y_name_label);
    y_layout->addWidget(y_value_label);
    position_layout->addLayout(y_layout);

    QHBoxLayout *z_layout = new QHBoxLayout();
    QLabel *z_name_label = new QLabel("Z");
    z_value_label = new QLabel("30.000");
    z_name_label->setFixedWidth(100);
    z_layout->addWidget(z_name_label);
    z_layout->addWidget(z_value_label);
    position_layout->addLayout(z_layout);

    QHBoxLayout *rx_layout = new QHBoxLayout();
    QLabel *rx_name_label = new QLabel("RX");
    rx_value_label = new QLabel("1.000");
    rx_name_label->setFixedWidth(100);
    rx_layout->addWidget(rx_name_label);
    rx_layout->addWidget(rx_value_label);
    position_layout->addLayout(rx_layout);

    QHBoxLayout *ry_layout = new QHBoxLayout();
    QLabel *ry_name_label = new QLabel("RY");
    ry_value_label = new QLabel("2.000");
    ry_name_label->setFixedWidth(100);
    ry_layout->addWidget(ry_name_label);
    ry_layout->addWidget(ry_value_label);
    position_layout->addLayout(ry_layout);

    QHBoxLayout *rz_layout = new QHBoxLayout();
    QLabel *rz_name_label = new QLabel("RZ");
    rz_value_label = new QLabel("3.000");
    rz_name_label->setFixedWidth(100);
    rz_layout->addWidget(rz_name_label);
    rz_layout->addWidget(rz_value_label);
    position_layout->addLayout(rz_layout);

    main_layout->addWidget(position_group);


    tcp_worker->moveToThread(tcp_thread);

    connect(tcp_thread,
            &QThread::finished,
            tcp_worker,
            &QObject::deleteLater);

    connect(tcp_thread,
            &QThread::started,
            tcp_worker,
            &TcpWorker::Start);

    connect(tcp_worker,
            &TcpWorker::statusReceived,
            this,
            &MainWindow::updateStatusDisplay);

    connect(tcp_worker,
            &TcpWorker::packetStatsUpdated,
            this,
            &MainWindow::updatePacketStats);           

    connect(tcp_worker,
            &TcpWorker::connectionStatusChanged,
            this,
            &MainWindow::updateConnectionStatus);

    tcp_thread->start();

}

MainWindow::~MainWindow()
{
    tcp_worker->Stop();
    tcp_thread->quit();
    tcp_thread->wait();
}

void MainWindow::updateStatusDisplay(const StatusData &status)
{
    current_status = status;

    connect_value_label->setText(status.connect ? "ON" : "OFF");
    voltage_value_label->setText(status.voltage_on ? "ON" : "OFF");
    moving_value_label->setText(status.is_moving ? "ON" : "OFF");
    fa_value_label->setText(status.is_fa ? "ON" : "OFF");
    homing_value_label->setText(status.homing_end ? "ON" : "OFF");
    error_value_label->setText(status.error ? "ON" : "OFF");

    ai_value_label[0]->setText(QString::number(status.ai_voltage[0], 'f', 3) + " V");
    ai_value_label[1]->setText("RAW " + QString::number(status.ai[1]));
    ai_value_label[2]->setText("RAW " + QString::number(status.ai[2]));
    ai_value_label[3]->setText("RAW " + QString::number(status.ai[3]));
    ai_value_label[4]->setText(QString::number(status.ai_voltage[4], 'f', 3) + " V");
    ai_value_label[5]->setText(QString::number(status.ai_voltage[5], 'f', 3) + " V");
    ai_value_label[6]->setText(QString::number(status.ai_voltage[6], 'f', 3) + " V");
    ai_value_label[7]->setText(QString::number(status.ai_voltage[7], 'f', 3) + " V");

    x_value_label->setText(QString::number(status.x, 'f', 3));
    y_value_label->setText(QString::number(status.y, 'f', 3));
    z_value_label->setText(QString::number(status.z, 'f', 3));
    rx_value_label->setText(QString::number(status.rx, 'f', 3));
    ry_value_label->setText(QString::number(status.ry, 'f', 3));
    rz_value_label->setText(QString::number(status.rz, 'f', 3));
}

void MainWindow::updatePacketStats(
    uint32_t sequence,
    uint32_t packet_loss,
    uint32_t latest_missing_sequence)
{
    sequence_value_label->setText(QString::number(sequence));
    loss_value_label->setText(QString::number(packet_loss));

    if (packet_loss == 0)
    {
        missing_value_label->setText("N/A");
    }
    else
    {
        missing_value_label->setText(
            QString::number(latest_missing_sequence));
    }
}

void MainWindow::updateConnectionStatus(const QString &status)
{
    qDebug() << "[MainWindow] Connection status:" << status;
    
    connection_status_value_label->setText(status);
}