#include "ServerRoom.h"

ServerRoom::ServerRoom(QString roomName) : QObject()
{
	this->roomName = roomName;
	this->welcomeMessage = "Welcome to \"" + roomName + "\" room";
}

ServerRoom::~ServerRoom()
{

}

QHash<int, QTcpSocket*> ServerRoom::getClientSockets()
{
	return this->clientsSockets;
}

QString ServerRoom::getRoomName()
{
	return this->roomName;
}

void ServerRoom::onDisconnectClient(QAbstractSocket::SocketError socketState)
{
	//qDebug() << "server: client soket change state:" << socketState;
	if (socketState == QAbstractSocket::RemoteHostClosedError)
	{
		QTcpSocket* clientSocket = (QTcpSocket*)sender();
		int socketDescriptor = clientSocket->socketDescriptor();
		QString userNickName = this->userNames.value(socketDescriptor);
		this->removeUser(socketDescriptor);
		emit clientDisconnect(userNickName);
	}
}

QHash<int, QString> ServerRoom::getUserNames()
{
	return this->userNames;
}

void ServerRoom::addNewUser(int socketDescriptor, QString userNickName, QTcpSocket* clientSocket)
{
	this->clientsSockets.insert(socketDescriptor, clientSocket);
	this->userNames.insert(socketDescriptor, userNickName);
}

void ServerRoom::removeUser(int socketDescriptor)
{
	this->clientsSockets.remove(socketDescriptor);
	this->userNames.remove(socketDescriptor);
}

void ServerRoom::clearUsers()
{
	this->clientsSockets.clear();
	this->userNames.clear();
}

QString ServerRoom::getUserNamesString()
{
	QString names;
	int i = 0;
	foreach(QString nickName, this->userNames)
	{
		if (i>0)
			names.append(";");
		names.append(nickName);
		i++;
	}
	return names;
}

QString ServerRoom::getWelcomeMessage()
{
	return this->welcomeMessage;
}