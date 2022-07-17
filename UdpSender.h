#ifndef UDPSENDER_H
#define UDPSENDER_H

#include <QObject>
#include <QtNetwork/QUdpSocket>


class UdpSender : public QObject
{
    Q_OBJECT
public:
    explicit UdpSender(QObject *parent = 0);
    void sendUdp(QByteArray readMeFileContent);
signals:

public slots:


private:
    QUdpSocket *sSocket;
    QHostAddress ip;
    quint16 port;

};


#endif // UDPSENDER_H
