#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tcpClient = new QTcpSocket(this);
    connect(tcpClient, &QTcpSocket::readyRead, this, &MainWindow::tcp_client_readdata);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_connect_clicked()
{
    if (tcpClient->state() != QAbstractSocket::ConnectedState) {
        QString ip = ui->txt_ip->text();
        ushort port = ui->txt_port->text().toUShort();
        tcpClient->connectToHost(ip, port);

        // 等待连接成功
        if (!tcpClient->waitForConnected(30000)) {
            qDebug() << "Connection failed";
            return;
        }

        ui->btn_connect->setText("断开");
    } else {
        tcpClient->disconnectFromHost();
        ui->btn_connect->setText("连接");
    }
}

void MainWindow::on_btn_send_clicked()
{
    if (tcpClient->state() == QAbstractSocket::ConnectedState) {
        QString send_str = ui->txt_input->toPlainText();
        tcpClient->write(send_str.toLocal8Bit());
        ui->txt_log->append("发送数据: " + send_str);
    }
}

void MainWindow::tcp_client_readdata()
{
    QByteArray buffer = tcpClient->readAll();
    QString recv_str = QString::fromLocal8Bit(buffer);
    ui->txt_log->append("接收数据: " + recv_str);
    qDebug() << recv_str;
}
