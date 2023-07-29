#include "mainwindow.h"
#include <QApplication>
#include <QFileDialog>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <unistd.h>
#include "UdpSender.h"
#include "UdpReceiver.h"




class QFileDialogTester : public QWidget
{
public:
  QString openFile()
  {
    QString filename =  QFileDialog::getOpenFileName(
          this,
          "Open Document",
          QDir::currentPath(),
          "All files (*.*) ;; Document files (*.doc *.rtf);; PNG files (*.png)");

    if( !filename.isNull() )
    {
      qDebug() << "selected file path : " << filename.toUtf8();
    }
    return QString(filename.toUtf8());
  }
};

void readMeFileFunction(QString readMeFilePath){
    QFile file(readMeFilePath);

    if(!file.open(QIODevice::ReadWrite)){
        qCritical() << "Could not open file!";
        qCritical() << file.errorString();
        return;
    }

//    file.seek(0);
    qInfo() << "Reading file content :-\n"<< file.readAll();
    file.close();
}

QByteArray fileContentIntoQString(QString filePath){

    QFile file(filePath);

    if(!file.open(QIODevice::ReadWrite)){
        qCritical() << "Could not open file!";
        qCritical() << file.errorString();
        return "";
    }
    file.seek(0);
    QByteArray fileContent = file.readAll();
    file.close();
    return fileContent;
}

void writeMeFileFunction(QString writeMeData, QString writeMeFileName){
    QFile file(writeMeFileName);

    if(!file.open(QIODevice::ReadWrite)){
        qCritical() << "Could not open file!";
        qCritical() << file.errorString();
    }

    QTextStream stream(& file);
    stream << writeMeData;


    file.close();

}

QString chooseFile(){
    QFileDialogTester test;
    QString readMeFilePath = test.openFile();
    return readMeFilePath;
}

QString extractFileName(QString readMeFilePath){
    QString readMeFileName="";
    for(int i=readMeFilePath.length()-1; i>=0; i--){
        if(readMeFilePath[i]=='/'){
            break;
        }
        readMeFileName=readMeFilePath[i]+readMeFileName;
    }
    return readMeFileName;
}

void sndBtnGui(QString readMeFilePath){
    QByteArray readMeFileContent = fileContentIntoQString(readMeFilePath);
    QFile readMeFile(readMeFilePath);
    long long int readMeFileSize=readMeFile.size();
    long long int numberOfPackets=(readMeFileSize/1000)+1;
    if(readMeFileSize%1000==0){numberOfPackets++;}
    UdpSender client;
    QByteArray msgHeader("");
    for(int i=1; i<=14; i++){
        msgHeader.append("0");
    }

    QString readMeFileName=extractFileName(readMeFilePath);
    QByteArray firstMessage;
    firstMessage.append(msgHeader);
    firstMessage.append(readMeFileName.toUtf8());
    long long int firstActualMessageSize = readMeFileName.toUtf8().size();
    for(int i=firstActualMessageSize+1; i<=1000; i++){
        firstMessage.append('/');
    }

    firstMessage.append(numberOfPackets);
    client.sendUdp(firstMessage);
    QByteArray substr;
    int packetNumber=1;
    for(long long int i=0; i<readMeFileContent.length(); i++){
        int count=0;
        substr.clear();
        int pkt = 0;
        while(count<1000 && i<readMeFileContent.length()){
            substr.append(readMeFileContent.at(i)); //substr + readMeFileContent.at(i);
            i++;
            count++;
        }
        pkt++;

        if(substr.size()<1000){
            for(int i=substr.size()+1; i<=1000; i++){
                substr.append('/');
            }
        }
        substr.prepend(msgHeader);
        substr.append(packetNumber);
        client.sendUdp(substr);
        packetNumber++;
        i--;
        usleep(1000*100);
    }
    client.sendUdp("00000000000000FILE KHATAM");

}

int main( int argc, char **argv )
{


    QApplication a( argc, argv );

    MainWindow w;
    w.show();


    UdpReceiver client;



    return a.exec();

}
