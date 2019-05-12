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
    if(mySerialPort->isOpen())
    {
        mySerialPort->clear();
        mySerialPort->close();
    }
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
    int b[2]={0};
    int Data;
    double dData;
    SendArr.clear();
    ReciArr.clear();
    SendArr.append((char *)&HeaderMsg,12);
    if( Type == 14)
    {
        DataArr.clear();
        switch( rBtnType )
        {
            case 0:
                Data=g_Data.allP;
                b[0]=Data/100;
                b[1]=Data%100;
                DataArr.append(b[1]/10*16+b[1]%10 + 51);
                DataArr.append(b[0]/10*16+b[0]%10 + 51);


                Data=g_Data.newP;
                b[0]=Data/100;
                b[1]=Data%100;
                DataArr.append(b[1]/10*16+b[1]%10 + 51);
                DataArr.append(b[0]/10*16+b[0]%10 + 51);

                dData=g_Data.newC;
                Data=(int)(dData*10000);
                b[0]=Data/100;
                b[1]=Data%100;
                DataArr.append(b[1]/10*16+b[1]%10 + 51);
                DataArr.append(b[0]/10*16+b[0]%10 + 51);

                Data=g_Data.allowTime;
                b[0]=Data/100;
                b[1]=Data%100;
                DataArr.append(b[1]/10*16+b[1]%10 + 51);
                DataArr.append(b[0]/10*16+b[0]%10 + 51);

                Data=g_Data.autoTime;
                b[0]=Data/100;
                b[1]=Data%100;
                DataArr.append(b[1]/10*16+b[1]%10 + 51);
                DataArr.append(b[0]/10*16+b[0]%10 + 51);

                Data=g_Data.offTimes;
                b[0]=Data/100;
                b[1]=Data%100;
                DataArr.append(b[1]/10*16+b[1]%10 + 51);
                DataArr.append(b[0]/10*16+b[0]%10 + 51);
                break;
            case 1:
                qDebug()<<"总有功";
                Data=g_Data.allP;
                b[0]=Data/100;
                b[1]=Data%100;
                DataArr.append(b[1]/10*16+b[1]%10 + 51);
                DataArr.append(b[0]/10*16+b[0]%10 + 51);
                break;
            case 2:
                qDebug()<<"新有功";
                DataArr.clear();
                Data=g_Data.newP;
                b[0]=Data/100;
                b[1]=Data%100;
                DataArr.append(b[1]/10*16+b[1]%10 + 51);
                DataArr.append(b[0]/10*16+b[0]%10 + 51);
                break;
            case 3:
                qDebug()<<"新功率因数";
                dData=g_Data.newC;
                Data=(int)(dData*10000);
                b[0]=Data/100;
                b[1]=Data%100;
                DataArr.append(b[1]/10*16+b[1]%10 + 51);
                DataArr.append(b[0]/10*16+b[0]%10 + 51);
                break;
            case 4:
                qDebug()<<"允许时间";

                Data=g_Data.allowTime;
                b[0]=Data/100;
                b[1]=Data%100;
                DataArr.append(b[1]/10*16+b[1]%10 + 51);
                DataArr.append(b[0]/10*16+b[0]%10 + 51);
                break;
            case 5:
                qDebug()<<"自动时间";
                Data=g_Data.autoTime;
                b[0]=Data/100;
                b[1]=Data%100;
                DataArr.append(b[1]/10*16+b[1]%10 + 51);
                DataArr.append(b[0]/10*16+b[0]%10 + 51);
                break;
            case 6:
                qDebug()<<"跳闸次数";
                Data=g_Data.offTimes;
                b[0]=Data/100;
                b[1]=Data%100;
                DataArr.append(b[1]/10*16+b[1]%10 + 51);
                DataArr.append(b[0]/10*16+b[0]%10 + 51);
                break;
        }
        int DataLen=DataArr.length();
        if( DataLen== 0 )
        {
            QMessageBox::information(this,"Warning","数据不能为空",QMessageBox::Ok);
            return;
        }
        SendArr.append(20);
        SendArr.append(DataLen+12);
        SendArr.append((char *)&SetPswMsg,12);
        SendArr.append(DataArr);
    }
    else if( Type == 11 )
    {
        if(rBtnType <0 || rBtnType >6)
        {
            QMessageBox::information(this,"Warning","请选择要读取的选项",QMessageBox::Ok);
            return;
        }
        SendArr.append(17);
        SendArr.append(4);
        SendArr.append(51+rBtnType);
        SendArr.append(70);
        SendArr.append(18);
        SendArr.append(55);


    }
    int SendLen=SendArr.length();
    SendArr.append(a);
    for(int i=4;i<SendLen;i++)
    {
        SendArr[SendLen] =SendArr[SendLen] +SendArr[i];
    }
    SendArr.append(22);
    qDebug()<<SendArr;
    if( mySerialPort->isOpen() ) {
        mySerialPort->write(SendArr);
        QString sendStr=QString(SendArr.toHex());
        int len=sendStr.length();
        int wlen=len/2-1;
        for (int var=1;var<=wlen;var++) {
            sendStr.insert(var*2+var-1," ");
        }
        QDateTime current_date_time =QDateTime::currentDateTime();
        QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz");
        ui->textEdit->append(QString("Send: ")+sendStr+QString("--")+current_date);


    }
}

void MainWindow::doProcessSerialReciDataSlot()
{
    ReciArr = mySerialPort->readAll();
    QString reciStr=QString(ReciArr.toHex());
    int len=reciStr.length();
    int wlen=len/2-1;
    for (int var=1;var<=wlen;var++) {
        reciStr.insert(var*2+var-1," ");
    }
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz");
    ui->textEdit->append(QString("Reci: ")+reciStr+"--"+current_date);
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
    else if( pBtn == ui->ClearpBtn ) {
        ui->textEdit->clear();
    }
    else if( pBtn == ui->LoadpBtn ) {
        QString fileName = QFileDialog::getSaveFileName(this, tr("保存为"),
                                     QDir::currentPath(),
                                     tr("Text files (*.txt)"));
        if(fileName.isEmpty()) {
            QMessageBox::warning(this,"警告--保存失败","请输入文件名",QMessageBox::Ok);
            return;
        }
        QFile file(fileName);
        if(!file.open(QIODevice::WriteOnly)) {
            QMessageBox::warning(this,"警告","保存失败",QMessageBox::Ok);
            return;
        }
        QTextStream out(&file);
        out<<ui->textEdit->toPlainText();
        file.close();
        ui->statusBar->showMessage("保存"+
                                   QFileInfo(fileName).fileName()+"文件成功",2000);
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
    }
    else if ( rBtn == ui->AllPrbtn ) {
        qDebug()<<"总有功";
        rBtnType=1;
    }
    else if ( rBtn == ui->NewPrbtn ) {
        qDebug()<<"新有功";
        rBtnType=2;
    }
    else if ( rBtn == ui->NewCrbtn ) {
        qDebug()<<"新功率因数";
        rBtnType=3;
    }
    else if ( rBtn == ui->AllowTimerbtn ) {
        qDebug()<<"允许时间";
        rBtnType=4;
    }
    else if ( rBtn == ui->AutoTimerbtn ) {
        qDebug()<<"自动时间";
        rBtnType=5;
    }
    else if ( rBtn == ui->OffTimesrbtn ) {
        qDebug()<<"跳闸次数";
        DataArr.clear();
        rBtnType=6;
    }
}
int char2num(char ch)
{
    if (ch>='a')
    return ch-'a'+10;
    else if (ch>='A')
    return ch-'A'+10;
    else
    return ch-'0';
}
int Hex2BCD(int ch){
    int result=0;
    result=ch/16*10+ch%16;
    return result;
}
void MainWindow::doProcessMsgDealSlot()
{
    qDebug()<<"报文处理";
    unsigned char b[2]={0};
    int Data=0;
    unsigned char cc;
    unsigned char* byte=nullptr;
    if( ui->textEdit->toPlainText().isEmpty() )
    {
        qDebug()<<"2";
        return;
    }
    if(ReciArr.isEmpty())
    {
        qDebug()<<"3";
        return;
    }
    QByteArray ReciMsgArr=ReciArr.toHex();
    byte=(unsigned char *)ReciMsgArr.data();

    if( char2num(ReciMsgArr.at(0)) == 0xF && char2num(ReciMsgArr.at(1)) == 0x0E )
    {
        qDebug()<<"未判断";
        if( char2num(ReciMsgArr.at(24)) == 0x09 )
        {
            if( char2num(ReciMsgArr.at(25)) == 0x01 )
            {
                ui->statusBar->showMessage("读取成功!!!",4000);
                switch( char2num(ReciMsgArr.at(29)) )
                {
                    case 0x03:
                        b[1]=char2num(ReciMsgArr.at(36))*16+char2num(ReciMsgArr.at(37))-51;
                        b[0]=char2num(ReciMsgArr.at(38))*16+char2num(ReciMsgArr.at(39))-51;
                        Data=Hex2BCD(b[0])*100+Hex2BCD(b[1]);
                        ui->AllPspinBox->setValue(Data);
                        b[1]=char2num(ReciMsgArr.at(40))*16+char2num(ReciMsgArr.at(41))-51;
                        b[0]=char2num(ReciMsgArr.at(42))*16+char2num(ReciMsgArr.at(43))-51;
                        Data=Hex2BCD(b[0])*100+Hex2BCD(b[1]);
                        ui->NewPspinBox->setValue(Data);
                        b[1]=char2num(ReciMsgArr.at(44))*16+char2num(ReciMsgArr.at(45))-51;
                        b[0]=char2num(ReciMsgArr.at(46))*16+char2num(ReciMsgArr.at(47))-51;
                        Data=Hex2BCD(b[0])*100+Hex2BCD(b[1]);
                        ui->NewCdoubleSpinBox->setValue(Data/10000.0);
                        b[1]=char2num(ReciMsgArr.at(48))*16+char2num(ReciMsgArr.at(49))-51;
                        b[0]=char2num(ReciMsgArr.at(50))*16+char2num(ReciMsgArr.at(51))-51;
                        Data=Hex2BCD(b[0])*100+Hex2BCD(b[1]);
                        ui->AllowTimespinBox->setValue(Data);
                        b[1]=char2num(ReciMsgArr.at(52))*16+char2num(ReciMsgArr.at(53))-51;
                        b[0]=char2num(ReciMsgArr.at(54))*16+char2num(ReciMsgArr.at(55))-51;
                        Data=Hex2BCD(b[0])*100+Hex2BCD(b[1]);
                        ui->AutoTimespinBox->setValue(Data);
                        b[1]=char2num(ReciMsgArr.at(56))*16+char2num(ReciMsgArr.at(57))-51;
                        b[0]=char2num(ReciMsgArr.at(58))*16+char2num(ReciMsgArr.at(59))-51;
                        Data=Hex2BCD(b[0])*100+Hex2BCD(b[1]);
                        ui->OffTimesspinBox->setValue(Data);
                        ui->AllSelectrbtn->setChecked(true);
                        break;
                    case 0x04:
                        b[1]=char2num(ReciMsgArr.at(36))*16+char2num(ReciMsgArr.at(37))-51;
                        b[0]=char2num(ReciMsgArr.at(38))*16+char2num(ReciMsgArr.at(39))-51;
                        Data=Hex2BCD(b[0])*100+Hex2BCD(b[1]);
                        ui->AllPspinBox->setValue(Data);
                        ui->AllPrbtn->setChecked(true);
                        qDebug()<<Data;
                        break;
                    case 0x05:
                        b[1]=char2num(ReciMsgArr.at(36))*16+char2num(ReciMsgArr.at(37))-51;
                        b[0]=char2num(ReciMsgArr.at(38))*16+char2num(ReciMsgArr.at(39))-51;
                        Data=Hex2BCD(b[0])*100+Hex2BCD(b[1]);
                        ui->NewPspinBox->setValue(Data);
                        ui->NewPrbtn->setChecked(true);
                        qDebug()<<Data;
                        break;
                    case 0x06:
                        b[1]=char2num(ReciMsgArr.at(36))*16+char2num(ReciMsgArr.at(37))-51;
                        b[0]=char2num(ReciMsgArr.at(38))*16+char2num(ReciMsgArr.at(39))-51;
                        Data=Hex2BCD(b[0])*100+Hex2BCD(b[1]);
                        ui->NewCdoubleSpinBox->setValue(Data/10000.0);
                        ui->NewCrbtn->setChecked(true);
                        qDebug()<<Data;
                        break;
                    case 0x07:
                        b[1]=char2num(ReciMsgArr.at(36))*16+char2num(ReciMsgArr.at(37))-51;
                        b[0]=char2num(ReciMsgArr.at(38))*16+char2num(ReciMsgArr.at(39))-51;
                        Data=Hex2BCD(b[0])*100+Hex2BCD(b[1]);
                        ui->AllowTimespinBox->setValue(Data);
                        ui->AllowTimerbtn->setChecked(true);
                        qDebug()<<Data;
                        break;
                    case 0x08:
                        b[1]=char2num(ReciMsgArr.at(36))*16+char2num(ReciMsgArr.at(37))-51;
                        b[0]=char2num(ReciMsgArr.at(38))*16+char2num(ReciMsgArr.at(39))-51;
                        Data=Hex2BCD(b[0])*100+Hex2BCD(b[1]);
                        ui->AutoTimespinBox->setValue(Data);
                        ui->AutoTimerbtn->setChecked(true);
                        qDebug()<<Data;
                        break;
                    case 0x09:
                        b[1]=char2num(ReciMsgArr.at(36))*16+char2num(ReciMsgArr.at(37))-51;
                        b[0]=char2num(ReciMsgArr.at(38))*16+char2num(ReciMsgArr.at(39))-51;
                        Data=Hex2BCD(b[0])*100+Hex2BCD(b[1]);
                        ui->OffTimesspinBox->setValue(Data);
                        ui->OffTimesrbtn->setChecked(true);
                        qDebug()<<Data;
                        break;
                }
            }
            else if( ReciArr.toHex()[12] & 0x0F == 0x04 )
            {
                ui->statusBar->showMessage("设置成功!!!",4000);
            }
        }
        else
        {
            if( ReciArr.toHex()[12] & 0x0F == 0x01 )
            {
                QMessageBox::warning(this,"警告","读取失败",QMessageBox::Ok);
                return;
            }
            else if( ReciArr.toHex()[12] & 0x0F == 0x04 )
            {
                QMessageBox::warning(this,"警告","设置失败",QMessageBox::Ok);
                return;
            }
        }
    }
    else if( ReciArr.toHex().at(0) == 0x68 )
    {

    }
    SendArr.clear();
    ReciArr.clear();
}

void MainWindow::doProcessSpinBoxValuechangedSlot(int data)
{
    QSpinBox *spb=qobject_cast<QSpinBox *>(this->sender());
    if( spb == ui->AllPspinBox )
    {
        g_Data.allP=data;
    }
    else if( spb == ui->NewPspinBox )
    {
        g_Data.newP=data;
    }
    else if( spb == ui->AllowTimespinBox )
    {
        g_Data.allowTime=data;
    }
    else if( spb == ui->AutoTimespinBox )
    {
        g_Data.autoTime=data;
    }
    else if( spb == ui->OffTimesspinBox )
    {
        g_Data.offTimes=data;
    }
}

void MainWindow::doProcessDoubleSBValueChangedSlot(double data)
{
    g_Data.newC=data;
}
void MainWindow::Init()
{
    SerialInit();
    ui->textEdit->setReadOnly(true);
    connect(ui->Setbtn,SIGNAL(clicked()),
            this,SLOT(doProcessPBtnSlot()));
    connect(ui->Getbtn,SIGNAL(clicked()),
            this,SLOT(doProcessPBtnSlot()));
    connect(ui->ClearpBtn,SIGNAL(clicked()),
            this,SLOT(doProcessPBtnSlot()));
    connect(ui->LoadpBtn,SIGNAL(clicked()),
            this,SLOT(doProcessPBtnSlot()));
    connect(ui->textEdit,SIGNAL(textChanged()),
            this,SLOT(doProcessMsgDealSlot()));
    SelectDataInit();
    ui->statusBar->setSizeGripEnabled(false);//去掉状态栏右下角的三角

    ui->statusBar->showMessage(tr("临时信息!"),2000);//显示临时信息2000ms 前面的正常信息被覆盖 当去掉后一项时，会一直显示
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

    connect(ui->AllPspinBox,SIGNAL(valueChanged(int)),
            this,SLOT(doProcessSpinBoxValuechangedSlot(int)));
    connect(ui->NewPspinBox,SIGNAL(valueChanged(int)),
            this,SLOT(doProcessSpinBoxValuechangedSlot(int)));
    connect(ui->AllowTimespinBox,SIGNAL(valueChanged(int)),
            this,SLOT(doProcessSpinBoxValuechangedSlot(int)));
    connect(ui->AutoTimespinBox,SIGNAL(valueChanged(int)),
            this,SLOT(doProcessSpinBoxValuechangedSlot(int)));
    connect(ui->OffTimesspinBox,SIGNAL(valueChanged(int)),
            this,SLOT(doProcessSpinBoxValuechangedSlot(int)));
    connect(ui->NewCdoubleSpinBox,SIGNAL(valueChanged(double)),
            this,SLOT(doProcessDoubleSBValueChangedSlot(double)));
    g_Data.allP=ui->AllPspinBox->value();
    g_Data.newC=ui->NewCdoubleSpinBox->value();
    g_Data.newP=ui->NewPspinBox->value();
    g_Data.autoTime=ui->AutoTimespinBox->value();
    g_Data.allowTime=ui->AllowTimespinBox->value();
    g_Data.offTimes=ui->OffTimesspinBox->value();

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
