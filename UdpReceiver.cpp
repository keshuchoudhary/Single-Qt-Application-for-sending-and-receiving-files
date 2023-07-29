#include "UdpReceiver.h"
#include <QFile>
#include <QDataStream>

int tempCount=0;
QString fileName="";


QString UdpReceiver::getIp(){
    QFile file("iniReceive.txt");

    if(!file.open(QIODevice::ReadWrite)){
        qCritical() << "Could not open file!";
        qCritical() << file.errorString();
        return "";
    }

    file.seek(0);

    QString strIp = file.readLine(1024);
    strIp.resize(strIp.length()-1);

    return strIp;


}

quint16 UdpReceiver::getPort(){
    QFile file("iniReceive.txt");

    if(!file.open(QIODevice::ReadWrite)){
        qCritical() << "Could not open file!";
        qCritical() << file.errorString();
        return 0;
    }

    file.seek(0);

    QString strIp = file.readLine(1024);
    QString strPort = file.readLine(1024);
    strPort.resize(strPort.length()-1);


    QTextStream ts(&strPort);
    quint16 num = 0;
    ts >> num;
    return num;
}

void writeMeFileFunction(QByteArray writeMeData, int len, QString writeMeFileName){


        QFile file(writeMeFileName);
        fflush(stdout);
        fflush(stderr);
//        printf("\nbefore file size %llu bytes ", file.size());
        // For Append
        if(!file.open(QIODevice::Append)){
            qCritical() << "Could not open file!";
            qCritical() << file.errorString();
            return;
        }
        file.write(writeMeData);
//        printf("\nafter file size %llu bytes ", file.size());
        file.close();




    fflush(stdout);
    fflush(stderr);
}





UdpReceiver::UdpReceiver(QObject *parent) :
    QObject(parent)
{
    QString tempIp=getIp();
    ip=QHostAddress(tempIp);
    port=getPort();


    rSocket = new QUdpSocket(this);
    bool ret = rSocket->bind(ip, port);
    if(ret==false) { printf("\nCould not bind .%s:%d %s ", __FILE__, __LINE__, __func__); exit(0);}

    connect(rSocket, SIGNAL(readyRead()), this, SLOT(receiveUdp()));

}






QByteArray cnvrtRealMsg(QByteArray actualMessage){
    QByteArray realMsg="";


    int i=999;
    while(actualMessage.at(i)=='/'){
        i--;
    }

    realMsg=actualMessage;
    realMsg.truncate(i+1);
    return realMsg;
}

void UdpReceiver::receiveUdp()
{
    QByteArray buffer;
    buffer.resize(rSocket->pendingDatagramSize());

    QHostAddress sender;
    quint16 senderPort;
    rSocket->readDatagram(buffer.data(), buffer.size(), &sender, &senderPort);

    qDebug() << "Message from: " << sender.toString();
    qDebug() << "Message port: " << senderPort;
    qDebug() << "Message size: " << buffer.size() << " bytes";
    fflush(stdout);
    fflush(stderr);

    QByteArray actualMessage=buffer.mid(14,1013);
    if(actualMessage=="FILE KHATAM"){
        tempCount=0;
        return;
    }
    QByteArray realMsg=cnvrtRealMsg(actualMessage);
    if(realMsg=="FILE KHATAM"){
        tempCount=0;
        return;
    }
    if(tempCount==0){
        fileName=realMsg;
        QFile file(fileName);
        if(!file.open(QIODevice::ReadWrite | QFile::Truncate))
        {
            qDebug() << "File could not be opened\n";
        }
        tempCount++;
    }
    else
        writeMeFileFunction(realMsg, realMsg.size(), fileName);

}

