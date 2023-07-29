#include "UdpSender.h"
#include <QFile>
#include <QDataStream>



QString getIp(){
    QFile file("iniSend.txt");

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

quint16 getPort(){
    QFile file("iniSend.txt");

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



void UdpSender::sendUdp(QByteArray Data)
{
    sSocket->writeDatagram(Data, ip, port);
    qDebug() << "Message sent :- size " << Data.size()<<" "<<ip<<" "<<port;
}




UdpSender::UdpSender(QObject *parent) :
    QObject(parent)
{
    QString tempIp=getIp();
    ip=QHostAddress(tempIp);
    port=getPort();

    sSocket = new QUdpSocket(this);

}
