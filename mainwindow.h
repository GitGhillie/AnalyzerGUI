#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts>

#include <QtBluetooth/qbluetoothdeviceinfo.h>
#include <QtBluetooth/qbluetoothlocaldevice.h>
#include <QtBluetooth/qbluetoothuuid.h>

#include "BTclient.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void sendMessage(const QString &message);

private slots:
    void on_connectButton_clicked();

    void reactOnSocketError(const QString &error);
    void showMessage(const QString &sender, const QString &message);
    void clientDisconnected(const QString &name);
    void clientDisconnected();
    void connected(const QString &name);

private:
    Ui::MainWindow *ui;
    int adapterFromUserSelection() const;
    int currentAdapterIndex = 0;

    QList<QBluetoothHostInfo> localAdapters;
    QList<BtClient *> clients;
};
#endif // MAINWINDOW_H
