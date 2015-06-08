#pragma once
#include <QtCore\qstring.h>
#include <QtNetwork\QTcpSocket>
#include <QtNetwork\qhostaddress.h>
#include <QtWidgets\qtextedit.h>

class ClientRoom : public QObject
{
	Q_OBJECT
public:
	ClientRoom(QString roomName, QHostAddress address, qint16 port);
	~ClientRoom();

public:
	QTcpSocket* getSocket();
	QString getName();
	QString getTypedMessage();
	QHostAddress getAddress();
	qint16 getPort();
	void connect();

private:
	QString roomName;
	QTcpSocket* inoutSocket;
	QString typedMessage;
	QHostAddress address;
	qint16 port;

signals:
	void socketStateChanged(QAbstractSocket::SocketState socketState);
	void sendTypedMessage(QString typedMessage);

private slots:
	void onSocketStateChanged(QAbstractSocket::SocketState socketState);
	void onSetTypedMessage();
	void onSendTypedMessage();
};