#include "mainwindow.h"
#include "tcp_worker.h"
#include <QGroupBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QGridLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      tcp_thread(new QThread(this)),
      tcp_worker(new TcpWorker())
{
    constexpr int LABEL_WIDTH = 180;

    QFont group_title_font;
    group_title_font.setBold(true);

    QFont value_font;
    value_font.setBold(true);

    setWindowTitle("GMT Status Stream Client");
    resize(760, 600);

    
    QWidget *central_widget = new QWidget(this);
    setCentralWidget(central_widget);

    QVBoxLayout *main_layout = new QVBoxLayout(central_widget);

    main_layout->setContentsMargins(12, 12, 12, 12);
    main_layout->setSpacing(10);
    // ----------------- CONNECTION ------------------- 
    QGroupBox *connection_group = new QGroupBox("Connection"); 
    connection_group->setFont(group_title_font); 
    QVBoxLayout *connection_layout = new QVBoxLayout(connection_group); 
    connection_layout->setContentsMargins(12, 12, 12, 12); 
    connection_layout->setSpacing(6); 
 
    QHBoxLayout *server_layout = new QHBoxLayout(); 
    QLabel *server_name_label = new QLabel("Server"); 
    QLabel *server_value_label = new QLabel("192.168.137.10:8888"); 
    server_value_label->setFont(value_font);
    server_value_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter); 
    server_name_label->setFixedWidth(LABEL_WIDTH); 
    server_layout->addWidget(server_name_label); 
    server_layout->addWidget(server_value_label); 
    connection_layout->addLayout(server_layout); 
 
    QHBoxLayout *status_layout = new QHBoxLayout(); 
    QLabel *status_name_label = new QLabel("Status"); 
    connection_status_value_label = new QLabel("● Connecting..."); 
    connection_status_value_label->setFont(value_font);
    connection_status_value_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter); 
    status_name_label->setFixedWidth(LABEL_WIDTH); 
    status_layout->addWidget(status_name_label); 
    status_layout->addWidget(connection_status_value_label); 
    connection_layout->addLayout(status_layout); 
 
    main_layout->addWidget(connection_group); 
 
    // ----------------- Packet ------------------- 
    QGroupBox *packet_group = new QGroupBox("Packet"); 
    packet_group->setFont(group_title_font); 
    QVBoxLayout *packet_layout = new QVBoxLayout(packet_group); 
    packet_layout->setContentsMargins(12, 12, 12, 12); 
    packet_layout->setSpacing(6); 
 
    QHBoxLayout *sequence_layout = new QHBoxLayout(); 
    QLabel *sequence_name_label = new QLabel("Current Sequence"); 
    sequence_value_label = new QLabel("0"); 
    sequence_value_label->setFont(value_font);
    sequence_value_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter); 
    sequence_name_label->setFixedWidth(LABEL_WIDTH); 
    sequence_layout->addWidget(sequence_name_label); 
    sequence_layout->addWidget(sequence_value_label); 
    packet_layout->addLayout(sequence_layout); 
 
    QHBoxLayout *loss_layout = new QHBoxLayout(); 
    QLabel *loss_name_label = new QLabel("Packet Loss"); 
    loss_value_label = new QLabel("0"); 
    loss_value_label->setFont(value_font);
    loss_value_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter); 
    loss_name_label->setFixedWidth(LABEL_WIDTH); 
    loss_layout->addWidget(loss_name_label); 
    loss_layout->addWidget(loss_value_label); 
    packet_layout->addLayout(loss_layout); 
 
    QHBoxLayout *missing_layout = new QHBoxLayout(); 
    QLabel *missing_name_label = new QLabel("Latest Missing Sequence"); 
    missing_value_label = new QLabel("N/A"); 
    missing_value_label->setFont(value_font);
    missing_value_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter); 
    missing_name_label->setFixedWidth(LABEL_WIDTH); 
    missing_layout->addWidget(missing_name_label); 
    missing_layout->addWidget(missing_value_label); 
    packet_layout->addLayout(missing_layout); 
 
    main_layout->addWidget(packet_group); 
 
 
    // ----------------- Controller ------------------- 
    QGroupBox *controller_group = new QGroupBox("Controller"); 
    controller_group->setFont(group_title_font); 
    QVBoxLayout *controller_layout = new QVBoxLayout(controller_group); 
    controller_layout->setContentsMargins(12, 12, 12, 12); 
    controller_layout->setSpacing(6); 
 
    QHBoxLayout *controller_columns = new QHBoxLayout(); 
    controller_columns->setSpacing(120); 
    controller_layout->addLayout(controller_columns); 
 
    QVBoxLayout *controller_left = new QVBoxLayout(); 
    controller_left->setSpacing(6); 
 
    QVBoxLayout *controller_right = new QVBoxLayout(); 
    controller_right->setSpacing(6); 
 
    controller_columns->addLayout(controller_left); 
    controller_columns->addLayout(controller_right); 
          
    QHBoxLayout *connect_layout = new QHBoxLayout(); 
    QLabel *connect_name_label = new QLabel("CONNECT"); 
    connect_value_label = new QLabel("ON"); 
    connect_value_label->setFont(value_font);
    connect_value_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter); 
    connect_name_label->setFixedWidth(LABEL_WIDTH); 
    connect_layout->addWidget(connect_name_label); 
    connect_layout->addWidget(connect_value_label); 
    controller_left->addLayout(connect_layout); 
 
    QHBoxLayout *voltage_layout = new QHBoxLayout(); 
    QLabel *voltage_name_label = new QLabel("VOLTAGEON"); 
    voltage_value_label = new QLabel("ON"); 
    voltage_value_label->setFont(value_font);
    voltage_value_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter); 
    voltage_name_label->setFixedWidth(LABEL_WIDTH); 
    voltage_layout->addWidget(voltage_name_label); 
    voltage_layout->addWidget(voltage_value_label); 
    controller_right->addLayout(voltage_layout); 
 
    QHBoxLayout *moving_layout = new QHBoxLayout(); 
    QLabel *moving_name_label = new QLabel("ISMOVING"); 
    moving_value_label = new QLabel("OFF"); 
    moving_value_label->setFont(value_font);
    moving_value_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter); 
    moving_name_label->setFixedWidth(LABEL_WIDTH); 
    moving_layout->addWidget(moving_name_label); 
    moving_layout->addWidget(moving_value_label); 
    controller_left->addLayout(moving_layout); 
 
    QHBoxLayout *fa_layout = new QHBoxLayout(); 
    QLabel *fa_name_label = new QLabel("ISFA"); 
    fa_value_label = new QLabel("OFF"); 
    fa_value_label->setFont(value_font);
    fa_value_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter); 
    fa_name_label->setFixedWidth(LABEL_WIDTH); 
    fa_layout->addWidget(fa_name_label); 
    fa_layout->addWidget(fa_value_label); 
    controller_right->addLayout(fa_layout); 
 
    QHBoxLayout *homing_layout = new QHBoxLayout(); 
    QLabel *homing_name_label = new QLabel("HOMINGEND"); 
    homing_value_label = new QLabel("OFF"); 
    homing_value_label->setFont(value_font);
    homing_value_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter); 
    homing_name_label->setFixedWidth(LABEL_WIDTH); 
    homing_layout->addWidget(homing_name_label); 
    homing_layout->addWidget(homing_value_label); 
    controller_left->addLayout(homing_layout); 
 
    QHBoxLayout *error_layout = new QHBoxLayout(); 
    QLabel *error_name_label = new QLabel("ERROR"); 
    error_value_label = new QLabel("OFF"); 
    error_value_label->setFont(value_font);
    error_value_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter); 
    error_name_label->setFixedWidth(LABEL_WIDTH); 
    error_layout->addWidget(error_name_label); 
    error_layout->addWidget(error_value_label); 
    controller_right->addLayout(error_layout); 
 
    main_layout->addWidget(controller_group); 
 
         
    // ----------------- Analog Input ------------------- 
    QGroupBox *analog_group = new QGroupBox("Analog Input"); 
    analog_group->setFont(group_title_font); 
    QVBoxLayout *analog_layout = new QVBoxLayout(analog_group); 
    analog_layout->setContentsMargins(12, 12, 12, 12); 
    analog_layout->setSpacing(6); 
 
    QHBoxLayout *analog_columns = new QHBoxLayout(); 
    analog_columns->setSpacing(120); 
    analog_layout->addLayout(analog_columns); 
 
    QVBoxLayout *analog_left = new QVBoxLayout(); 
    analog_left->setSpacing(6); 
 
    QVBoxLayout *analog_right = new QVBoxLayout(); 
    analog_right->setSpacing(6); 
 
    analog_columns->addLayout(analog_left); 
    analog_columns->addLayout(analog_right); 
 
    // AI00 
    QHBoxLayout *ai00_layout = new QHBoxLayout(); 
    QLabel *ai00_name_label = new QLabel("AI00"); 
    ai_value_label[0] = new QLabel("-2.500 V"); 
    ai_value_label[0]->setFont(value_font);
    ai_value_label[0]->setAlignment(Qt::AlignRight | Qt::AlignVCenter); 
    ai00_name_label->setFixedWidth(LABEL_WIDTH); 
 
    ai00_layout->addWidget(ai00_name_label); 
    ai00_layout->addWidget(ai_value_label[0]); 
    analog_left->addLayout(ai00_layout); 
 
    // AI01 
    QHBoxLayout *ai01_layout = new QHBoxLayout(); 
    QLabel *ai01_name_label = new QLabel("AI01"); 
    ai_value_label[1] = new QLabel("RAW 0"); 
    ai_value_label[1]->setFont(value_font);
    ai_value_label[1]->setAlignment(Qt::AlignRight | Qt::AlignVCenter); 
    ai01_name_label->setFixedWidth(LABEL_WIDTH); 
 
    ai01_layout->addWidget(ai01_name_label); 
    ai01_layout->addWidget(ai_value_label[1]); 
    analog_right->addLayout(ai01_layout); 
 
    // AI02 
    QHBoxLayout *ai02_layout = new QHBoxLayout(); 
    QLabel *ai02_name_label = new QLabel("AI02"); 
    ai_value_label[2] = new QLabel("RAW 13107"); 
    ai_value_label[2]->setFont(value_font);
    ai_value_label[2]->setAlignment(Qt::AlignRight | Qt::AlignVCenter); 
    ai02_name_label->setFixedWidth(LABEL_WIDTH); 
 
    ai02_layout->addWidget(ai02_name_label); 
    ai02_layout->addWidget(ai_value_label[2]); 
    analog_left->addLayout(ai02_layout); 
 
    // AI03 
    QHBoxLayout *ai03_layout = new QHBoxLayout(); 
    QLabel *ai03_name_label = new QLabel("AI03"); 
    ai_value_label[3] = new QLabel("RAW 19660"); 
    ai_value_label[3]->setFont(value_font);
    ai_value_label[3]->setAlignment(Qt::AlignRight | Qt::AlignVCenter); 
    ai03_name_label->setFixedWidth(LABEL_WIDTH); 
 
    ai03_layout->addWidget(ai03_name_label); 
    ai03_layout->addWidget(ai_value_label[3]); 
    analog_right->addLayout(ai03_layout); 
 
    // AI04 
    QHBoxLayout *ai04_layout = new QHBoxLayout(); 
    QLabel *ai04_name_label = new QLabel("AI04"); 
    ai_value_label[4] = new QLabel("4.000 V"); 
    ai_value_label[4]->setFont(value_font);
    ai_value_label[4]->setAlignment(Qt::AlignRight | Qt::AlignVCenter); 
    ai04_name_label->setFixedWidth(LABEL_WIDTH); 
 
    ai04_layout->addWidget(ai04_name_label); 
    ai04_layout->addWidget(ai_value_label[4]); 
    analog_left->addLayout(ai04_layout); 
 
    // AI05 
    QHBoxLayout *ai05_layout = new QHBoxLayout(); 
    QLabel *ai05_name_label = new QLabel("AI05"); 
    ai_value_label[5] = new QLabel("5.000 V"); 
    ai_value_label[5]->setFont(value_font);
    ai_value_label[5]->setAlignment(Qt::AlignRight | Qt::AlignVCenter); 
    ai05_name_label->setFixedWidth(LABEL_WIDTH); 
 
    ai05_layout->addWidget(ai05_name_label); 
    ai05_layout->addWidget(ai_value_label[5]); 
    analog_right->addLayout(ai05_layout); 
 
    // AI06 
    QHBoxLayout *ai06_layout = new QHBoxLayout(); 
    QLabel *ai06_name_label = new QLabel("AI06"); 
    ai_value_label[6] = new QLabel("6.000 V"); 
    ai_value_label[6]->setFont(value_font);
    ai_value_label[6]->setAlignment(Qt::AlignRight | Qt::AlignVCenter); 
    ai06_name_label->setFixedWidth(LABEL_WIDTH); 
 
    ai06_layout->addWidget(ai06_name_label); 
    ai06_layout->addWidget(ai_value_label[6]); 
    analog_left->addLayout(ai06_layout); 
 
    // AI07 
    QHBoxLayout *ai07_layout = new QHBoxLayout(); 
    QLabel *ai07_name_label = new QLabel("AI07"); 
    ai_value_label[7] = new QLabel("7.000 V"); 
    ai_value_label[7]->setFont(value_font);
    ai_value_label[7]->setAlignment(Qt::AlignRight | Qt::AlignVCenter); 
    ai07_name_label->setFixedWidth(LABEL_WIDTH); 
 
    ai07_layout->addWidget(ai07_name_label); 
    ai07_layout->addWidget(ai_value_label[7]); 
    analog_right->addLayout(ai07_layout); 
 
    main_layout->addWidget(analog_group); 
 
 
    // ----------------- Position ------------------- 
    QGroupBox *position_group = new QGroupBox("Position"); 
    position_group->setFont(group_title_font); 
    QVBoxLayout *position_layout = new QVBoxLayout(position_group); 
    position_layout->setContentsMargins(12, 12, 12, 12); 
    position_layout->setSpacing(6); 
 
    QHBoxLayout *position_columns = new QHBoxLayout(); 
    position_columns->setSpacing(120); 
    position_layout->addLayout(position_columns); 
 
    QVBoxLayout *position_left = new QVBoxLayout(); 
    position_left->setSpacing(6); 
 
    QVBoxLayout *position_right = new QVBoxLayout(); 
    position_right->setSpacing(6); 
 
    position_columns->addLayout(position_left); 
    position_columns->addLayout(position_right); 
 
    // X 
    QHBoxLayout *x_layout = new QHBoxLayout(); 
    QLabel *x_name_label = new QLabel("X"); 
    x_value_label = new QLabel("10.000"); 
    x_value_label->setFont(value_font);
    x_value_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter); 
    x_name_label->setFixedWidth(LABEL_WIDTH); 
 
    x_layout->addWidget(x_name_label); 
    x_layout->addWidget(x_value_label); 
    position_left->addLayout(x_layout); 
 
    // Y 
    QHBoxLayout *y_layout = new QHBoxLayout(); 
    QLabel *y_name_label = new QLabel("Y"); 
    y_value_label = new QLabel("20.000"); 
    y_value_label->setFont(value_font);
    y_value_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter); 
    y_name_label->setFixedWidth(LABEL_WIDTH); 
 
    y_layout->addWidget(y_name_label); 
    y_layout->addWidget(y_value_label); 
    position_right->addLayout(y_layout); 
 
    // Z 
    QHBoxLayout *z_layout = new QHBoxLayout(); 
    QLabel *z_name_label = new QLabel("Z"); 
    z_value_label = new QLabel("30.000"); 
    z_value_label->setFont(value_font);
    z_value_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter); 
    z_name_label->setFixedWidth(LABEL_WIDTH); 
 
    z_layout->addWidget(z_name_label); 
    z_layout->addWidget(z_value_label); 
    position_left->addLayout(z_layout); 
 
    // RX 
    QHBoxLayout *rx_layout = new QHBoxLayout(); 
    QLabel *rx_name_label = new QLabel("RX"); 
    rx_value_label = new QLabel("1.000"); 
    rx_value_label->setFont(value_font);
    rx_value_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter); 
    rx_name_label->setFixedWidth(LABEL_WIDTH); 
 
    rx_layout->addWidget(rx_name_label); 
    rx_layout->addWidget(rx_value_label); 
    position_right->addLayout(rx_layout); 
 
    // RY 
    QHBoxLayout *ry_layout = new QHBoxLayout(); 
    QLabel *ry_name_label = new QLabel("RY"); 
    ry_value_label = new QLabel("2.000"); 
    ry_value_label->setFont(value_font);
    ry_value_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter); 
    ry_name_label->setFixedWidth(LABEL_WIDTH); 
 
    ry_layout->addWidget(ry_name_label); 
    ry_layout->addWidget(ry_value_label); 
    position_left->addLayout(ry_layout); 
 
    // RZ 
    QHBoxLayout *rz_layout = new QHBoxLayout(); 
    QLabel *rz_name_label = new QLabel("RZ"); 
    rz_value_label = new QLabel("3.000"); 
    rz_value_label->setFont(value_font);
    rz_value_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter); 
    rz_name_label->setFixedWidth(LABEL_WIDTH); 
 
    rz_layout->addWidget(rz_name_label); 
    rz_layout->addWidget(rz_value_label); 
    position_right->addLayout(rz_layout); 
 
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
    
    connection_status_value_label->setText("● " + status);
}