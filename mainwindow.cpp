#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Init();
}

MainWindow::~MainWindow()
{
    delete ui;
}
const unsigned char HeaderMsg[]={0xfe,0xfe,0xfe,0xfe,0x68,0x88,0x88,0x88,0x88,0x88,0x88,0x68};
const unsigned char SetPswMsg[]={0x33,0x46,0x12,0x37,0x35,0x33,0x33,0x33,0x22,0xd2,0x22,0x40};
void MainWindow::doProcessSerialOpenpBtnSlot()
{
    if( mySerialPort->isOpen() == true ) {
        ui->pushButton->setText("打开串口");
        mySerialPort->clear();
        mySerialPort->close();
    }
    else {
        mySerialPort->clear();
        mySerialPort->close();

        mySerialPort->setPortName(ui->comboBox->currentText());
        switch (ui->baudcomboBox->currentIndex())
        {
            case 0:
                 mySerialPort->setBaudRate(QSerialPort::Baud1200);
                 break;
            case 1:
                 mySerialPort->setBaudRate(QSerialPort::Baud2400);
                 break;
            case 2:
                 mySerialPort->setBaudRate(QSerialPort::Baud4800);
                 break;
            case 3:
                 mySerialPort->setBaudRate(QSerialPort::Baud9600);
                 break;
            case 4:
                 mySerialPort->setBaudRate(QSerialPort::Baud115200);
                 break;
        }
        switch (ui->datacomboBox->currentIndex())
        {
             case 0:
                 mySerialPort->setDataBits(QSerialPort::Data8);
                 qDebug()<<"Data8";
                 break;
             case 1:
                 mySerialPort->setDataBits(QSerialPort::Data7);
                 qDebug()<<"Data7";
                 break;

        }
        mySerialPort->setFlowControl(QSerialPort::NoFlowControl);//无流控制
        switch (ui->crccomboBox->currentIndex())
        {
             case 0:
                 mySerialPort->setParity(QSerialPort::NoParity);
                 qDebug()<<"NoParity";
                 break;
             case 1:
                 mySerialPort->setParity(QSerialPort::EvenParity);
                 qDebug()<<"EvenParity";
                 break;
             case 2:
                 mySerialPort->setParity(QSerialPort::OddParity);
                 qDebug()<<"OddParity";
                 break;
        }
        mySerialPort->setStopBits(QSerialPort::OneStop); //一位停止位
        if(!mySerialPort->open(QIODevice::ReadWrite))
        {
            QMessageBox::information(this,"Warning","串口已被占用，打开失败",QMessageBox::Ok);
            return;
        }
        else
        {
            ui->pushButton->setText("关闭串口");

            if( mySerialPort->isOpen() && mySerialPort->portName() == ui->comboBox->currentText())
            {
                qDebug()<<"打开了";
            }
        }
    }
}

void MainWindow::doProcessSerialSendDataSlot(int Type)
{
    char a=0x00;
    SendArr.clear();
    if( Type == 14)
    {
        int DataLen=DataArr.length();
        if( DataLen== 0 )
        {
            QMessageBox::information(this,"Warning","数据不能为空",QMessageBox::Ok);
            return;
        }

        SendArr.append((char *)&HeaderMsg,12);
        SendArr.append(20);
        SendArr.append(DataLen+12);
        SendArr.append((char *)&SetPswMsg,12);
        SendArr.append(DataArr);
        int SendLen=SendArr.length();
        SendArr.append(a);
        for(int i=4;i<SendLen;i++)
        {
            SendArr[SendLen] =SendArr[SendLen] +SendArr[i];
        }
        SendArr.append(22);
        qDebug()<<SendArr;
    }

    if( mySerialPort->isOpen() ) {
        mySerialPort->write(SendArr);
    }
}

void MainWindow::doProcessSerialReciDataSlot()
{
    ReciArr = mySerialPort->readAll();
    QString reciStr=QString(ReciArr.toHex());
    QString dispReciStr=QString("Reci: ");
    int len=reciStr.length();
    int wlen=len/2-1;
    for (int var=1;var<=wlen;var++) {
        //dispReciStr.append(reciStr.chopped(2)+" ");
        reciStr.insert(var*2+var-1," ");
    }
    ui->textEdit->setText(reciStr);
}

void MainWindow::doProcessPBtnSlot()
{
    QPushButton *pBtn=qobject_cast<QPushButton*>(this->sender());
    if( pBtn == ui->Setbtn ) {
       emit SignalSerialSendData(14);
    }
    else if( pBtn == ui->Getbtn ) {
       emit SignalSerialSendData(11);
    }
}

void MainWindow::doProcessRBtnSlot(bool)
{
    int Data=0;
    double dData;
    int b[2]={0};
    QRadioButton *rBtn=qobject_cast<QRadioButton *>(this->sender());
    if( rBtn == ui->AllSelectrbtn ) {
        qDebug()<<"全选";
        rBtnType=0;
        DataArr.clear();
        Data=ui->AllPspinBox->value();
        b[0]=Data/100;
        b[1]=Data%100;
        DataArr.append(b[1]/10*16+b[1]%10 + 51);
        DataArr.append(b[0]/10*16+b[0]%10 + 51);


        Data=ui->NewPspinBox->value();
        b[0]=Data/100;
        b[1]=Data%100;
        DataArr.append(b[1]/10*16+b[1]%10 + 51);
        DataArr.append(b[0]/10*16+b[0]%10 + 51);

        dData=ui->NewCdoubleSpinBox->value();
        Data=(int)(dData*10000);
        b[0]=Data/100;
        b[1]=Data%100;
        DataArr.append(b[1]/10*16+b[1]%10 + 51);
        DataArr.append(b[0]/10*16+b[0]%10 + 51);

        Data=ui->AllowTimespinBox->value();
        b[0]=Data/100;
        b[1]=Data%100;
        DataArr.append(b[1]/10*16+b[1]%10 + 51);
        DataArr.append(b[0]/10*16+b[0]%10 + 51);

        Data=ui->AutoTimespinBox->value();
        b[0]=Data/100;
        b[1]=Data%100;
        DataArr.append(b[1]/10*16+b[1]%10 + 51);
        DataArr.append(b[0]/10*16+b[0]%10 + 51);

        Data=ui->OffTimesspinBox->value();
        b[0]=Data/100;
        b[1]=Data%100;
        DataArr.append(b[1]/10*16+b[1]%10 + 51);
        DataArr.append(b[0]/10*16+b[0]%10 + 51);
    }
    else if ( rBtn == ui->AllPrbtn ) {
        DataArr.clear();
        qDebug()<<"总有功";
        rBtnType=1;
        Data=ui->AllPspinBox->value();
        b[0]=Data/100;
        b[1]=Data%100;
        DataArr.append(b[1]/10*16+b[1]%10 + 51);
        DataArr.append(b[0]/10*16+b[0]%10 + 51);
    }
    else if ( rBtn == ui->NewPrbtn ) {
        qDebug()<<"新有功";
        DataArr.clear();
        rBtnType=2;
        Data=ui->NewPspinBox->value();
        b[0]=Data/100;
        b[1]=Data%100;
        DataArr.append(b[1]/10*16+b[1]%10 + 51);
        DataArr.append(b[0]/10*16+b[0]%10 + 51);
    }
    else if ( rBtn == ui->NewCrbtn ) {
        qDebug()<<"新功率因数";
        DataArr.clear();
        rBtnType=3;
        dData=ui->NewCdoubleSpinBox->value();
        Data=(int)(dData*10000);
        b[0]=Data/100;
        b[1]=Data%100;
        DataArr.append(b[1]/10*16+b[1]%10 + 51);
        DataArr.append(b[0]/10*16+b[0]%10 + 51);
    }
    else if ( rBtn == ui->AllowTimerbtn ) {
        qDebug()<<"允许时间";
        rBtnType=4;
        DataArr.clear();
        Data=ui->AllowTimespinBox->value();
        b[0]=Data/100;
        b[1]=Data%100;
        DataArr.append(b[1]/10*16+b[1]%10 + 51);
        DataArr.append(b[0]/10*16+b[0]%10 + 51);
    }
    else if ( rBtn == ui->AutoTimerbtn ) {
        qDebug()<<"自动时间";
        rBtnType=5;
        DataArr.clear();
        Data=ui->AutoTimespinBox->value();
        b[0]=Data/100;
        b[1]=Data%100;
        DataArr.append(b[1]/10*16+b[1]%10 + 51);
        DataArr.append(b[0]/10*16+b[0]%10 + 51);
    }
    else if ( rBtn == ui->OffTimesrbtn ) {
        qDebug()<<"跳闸次数";
        DataArr.clear();
        rBtnType=6;
        Data=ui->OffTimesspinBox->value();
        b[0]=Data/100;
        b[1]=Data%100;
        DataArr.append(b[1]/10*16+b[1]%10 + 51);
        DataArr.append(b[0]/10*16+b[0]%10 + 51);
    }
}
void MainWindow::Init()
{
    SerialInit();

    connect(ui->Setbtn,SIGNAL(clicked()),
            this,SLOT(doProcessPBtnSlot()));
    connect(ui->Getbtn,SIGNAL(clicked()),
            this,SLOT(doProcessPBtnSlot()));
    SelectDataInit();

}

void MainWindow::SerialInit()
{
    mySerialPort=new QSerialPort();
    myPortNameList=getPortNameList();
    ui->comboBox->addItems(myPortNameList);

    connect(ui->pushButton,SIGNAL(clicked()),
            this,SLOT(doProcessSerialOpenpBtnSlot()));

    connect(this,SIGNAL(SignalSerialSendData(int)),
            this,SLOT(doProcessSerialSendDataSlot(int)));

    connect(this,SIGNAL(SignalSerialReciData()),
            this,SLOT(doProcessSerialReciDataSlot()));
}

void MainWindow::SelectDataInit()
{
    rBtnType=-1;
    connect(ui->AllPrbtn,SIGNAL(toggled(bool)),
            this,SLOT(doProcessRBtnSlot(bool)));
    connect(ui->NewPrbtn,SIGNAL(toggled(bool)),
            this,SLOT(doProcessRBtnSlot(bool)));
    connect(ui->NewCrbtn,SIGNAL(toggled(bool)),
            this,SLOT(doProcessRBtnSlot(bool)));
    connect(ui->AllowTimerbtn,SIGNAL(toggled(bool)),
            this,SLOT(doProcessRBtnSlot(bool)));
    connect(ui->AutoTimerbtn,SIGNAL(toggled(bool)),
            this,SLOT(doProcessRBtnSlot(bool)));
    connect(ui->OffTimesrbtn,SIGNAL(toggled(bool)),
            this,SLOT(doProcessRBtnSlot(bool)));
    connect(ui->AllSelectrbtn,SIGNAL(toggled(bool)),
            this,SLOT(doProcessRBtnSlot(bool)));

    connect(mySerialPort,SIGNAL(readyRead()),
            this,SLOT(doProcessSerialReciDataSlot()));

}

QStringList MainWindow::getPortNameList()
{
    QStringList SerialList;
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
        SerialList << info.portName();
    }
    return SerialList;
}
