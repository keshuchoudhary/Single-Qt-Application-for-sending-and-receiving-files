#ifndef UDPRECEIVER_H
#define UDPRECEIVER_H


#include <QObject>
#include <QtNetwork/QUdpSocket>


class UdpReceiver : public QObject
{
    Q_OBJECT
public:
    explicit UdpReceiver(QObject *parent = 0);
signals:

public slots:
    void receiveUdp();
    QString getIp();
    quint16 getPort();

private:
    QUdpSocket *rSocket;
    QHostAddress ip;
    quint16 port;

};

#endif // UDPRECEIVER_H
