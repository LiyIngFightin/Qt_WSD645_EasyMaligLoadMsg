#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QStringList>
#include <QMessageBox>
#include <QtDebug>
#include <QByteArray>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
signals:
    void SignalSerialSendData(int);
    void SignalSerialReciData();
private slots:
    void doProcessSerialOpenpBtnSlot();
    void doProcessSerialSendDataSlot(int);
    void doProcessSerialReciDataSlot();
    void doProcessPBtnSlot();
    void doProcessRBtnSlot(bool);
private:
    void Init();
    void SerialInit();
    void SelectDataInit();
    QStringList getPortNameList();
private:
    Ui::MainWindow *ui;
    QSerialPort* mySerialPort;
    QStringList myPortNameList;
    QByteArray SendArr;
    QByteArray ReciArr;
    QByteArray DataArr;
    int rBtnType;
};

#endif // MAINWINDOW_H
