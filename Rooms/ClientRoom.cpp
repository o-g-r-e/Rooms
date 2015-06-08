#include "ClientRoom.h"

ClientRoom::ClientRoom(QString roomName, QHostAddress address, qint16 port) : QObject()
{
	this->roomName = roomName;
	this->inoutSocket = new QTcpSocket(this);
	this->address = address;
	this->port = port;
}

ClientRoom::~ClientRoom()
{

}

QTcpSocket* ClientRoom::getSocket()
{
	return this->inoutSocket;
}

QString ClientRoom::getName()
{
	return this->roomName;
}

void ClientRoom::onSocketStateChanged(QAbstractSocket::SocketState socketState)
{
	emit socketStateChanged(socketState);
}

QString ClientRoom::getTypedMessage()
{
	return this->typedMessage;
}

void ClientRoom::onSetTypedMessage()
{
	QTextEdit* textEdit = (QTextEdit*)sender();
	this->typedMessage = textEdit->toPlainText();
}

void ClientRoom::onSendTypedMessage()
{
	emit sendTypedMessage(typedMessage);
	typedMessage = "";
}

QHostAddress ClientRoom::getAddress()
{
	return this->address;
}

qint16 ClientRoom::getPort()
{
	return this->port;
}

void ClientRoom::connect()
{
	this->inoutSocket->connectToHost(this->address, this->port);
}