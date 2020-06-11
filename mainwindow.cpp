#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "remoteselector.h"
#include "BTclient.h"

#include <QtBluetooth/qbluetoothdeviceinfo.h>
#include <QtBluetooth/qbluetoothlocaldevice.h>
#include <QtBluetooth/qbluetoothuuid.h>

static const QLatin1String serviceUuid("e8e10f95-1a70-4b27-9ccf-02010264e9c8");

using namespace QtCharts;

//Todo: Put these as class objects
QLineSeries *series;
QChart *chart;
QChartView *chartView;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->connectButton, &QPushButton::clicked, this, &MainWindow::on_connectButton_clicked);

    series = new QLineSeries();
    for(int i = 0; i<8; i++)
        series->append(i,i);

    chart = new QChart();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setTitle("Sensor evolution");

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    //ui->graphLayout->addWidget(chartView);
}

MainWindow::~MainWindow()
{
    delete ui;
}


/*void MainWindow::on_pushButton_clicked()
{
    chart->removeSeries(series); // Clear the chart
    QFile file("data.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;


}*/

void MainWindow::on_connectButton_clicked()
{
    ui->connectButton->setEnabled(false);

    // scan for services
    const QBluetoothAddress adapter = localAdapters.isEmpty() ?
                                           QBluetoothAddress() :
                                           localAdapters.at(currentAdapterIndex).address();

    RemoteSelector remoteSelector(adapter);

    remoteSelector.startDiscovery(QBluetoothUuid(serviceUuid));

    if (remoteSelector.exec() == QDialog::Accepted) {
        QBluetoothServiceInfo service = remoteSelector.service();

        qDebug() << "Connecting to service 2" << service.serviceName()
                 << "on" << service.device().name();

        // Create client
        qDebug() << "Going to create client";
        BtClient *client = new BtClient(this);
qDebug() << "Connecting...";

        connect(client, &BtClient::messageReceived,
                this, &MainWindow::showMessage);
        connect(client, &BtClient::disconnected,
                this, QOverload<>::of(&MainWindow::clientDisconnected));
        connect(client, QOverload<const QString &>::of(&BtClient::connected),
                this, &MainWindow::connected);
        connect(client, &BtClient::socketErrorOccurred,
                this, &MainWindow::reactOnSocketError);
        connect(this, &MainWindow::sendMessage, client, &BtClient::sendMessage);
qDebug() << "Start client";
        client->startClient(service);

        clients.append(client);
    }

    ui->connectButton->setEnabled(true);
}

void MainWindow::reactOnSocketError(const QString &error)
{
    ui->log->insertPlainText(error);
}

void MainWindow::showMessage(const QString &sender, const QString &message)
{
    ui->log->insertPlainText(QString::fromLatin1("%1: %2\n").arg(sender, message));
    ui->log->ensureCursorVisible();
}

void MainWindow::connected(const QString &name)
{
    ui->log->insertPlainText(QString::fromLatin1("Connected to %1.\n").arg(name));
}

void MainWindow::clientDisconnected(const QString &name)
{
    ui->log->insertPlainText(QString::fromLatin1("%1 has left.\n").arg(name));
}

void MainWindow::clientDisconnected()
{
    BtClient *client = qobject_cast<BtClient *>(sender());
    if (client) {
        clients.removeOne(client);
        client->deleteLater();
    }
}
