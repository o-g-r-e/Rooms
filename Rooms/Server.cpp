#include "Server.h"

Server::Server() : QObject()
{
	tcpServer = new QTcpServer(this);
}

Server::~Server()
{

}

void Server::onServerAccessError(QAbstractSocket::SocketError socketError)
{
	//qDebug() << "server access error" << socketError;
}

void Server::onStartListenServer(QHostAddress address, qint16 port)
{
	if (!tcpServer->isListening())
	{
		this->address = address;
		this->port = port;

		connect(tcpServer, SIGNAL(newConnection()), this, SLOT(onAddConnection()));
		connect(tcpServer, SIGNAL(acceptError(QAbstractSocket::SocketError)), this, SLOT(onServerAccessError(QAbstractSocket::SocketError)));
		bool listen = tcpServer->listen(this->address, this->port);
		if (listen)
		{
			//qDebug() << "server start listen" << address.toString() << ":" << port;
			emit serverListen(listen);
			this->checkSockets(listen);
		}
		else
		{
			emit showEventMesage(tr("Unable to start the server: %1.").arg(tcpServer->errorString()));
		}
	}
}

void Server::checkSockets(bool open)
{
	foreach(ServerRoom* serverRoom, rooms)
	{
		foreach(QTcpSocket* clientSocket, serverRoom->getClientSockets())
		{
			if (open)
				clientSocket->open(QIODevice::ReadOnly | QIODevice::WriteOnly);
			else
				clientSocket->close();
		}

		if (!open)
			serverRoom->clearUsers();
	}
}

void Server::stopListenServer()
{
	disconnect(tcpServer, SIGNAL(newConnection()), this, SLOT(onAddConnection()));
	this->tcpServer->close();
	emit serverListen(false);
}

void Server::onStopServer()
{
	stopListenServer();
	this->checkSockets(false);
}

void Server::onAddConnection()
{
	QTcpSocket* clientSocket = tcpServer->nextPendingConnection();
	
	int key = clientSocket->socketDescriptor();
	clientsSockets.insert(key, clientSocket);
	connect(clientsSockets.value(key), SIGNAL(readyRead()), this, SLOT(onRead()));
	connect(clientsSockets.value(key), SIGNAL(disconnected()), this, SLOT(onDisconnectClient()));
}

void Server::onDisconnectRoomClient(QString userNickName)
{
	ServerRoom* serverRoom = (ServerRoom*)sender();
	if (serverRoom->getClientSockets().count() == 0)
	{
		this->rooms.remove(serverRoom->getRoomName());
	}
	else
	{
		this->sendToAll(serverRoom, instMessage(userNickName, serverRoom->getRoomName(), MessageType::REM_USER));
	}
}

void Server::onDisconnectClient()
{
	QTcpSocket* socket = (QTcpSocket*)sender();
	clientsSockets.remove(socket->socketDescriptor());
}

void Server::onRead()
{
	QTcpSocket* clientSocket = (QTcpSocket*)sender();
	Message m = readMessage(clientSocket);
	ServerRoom* serverRoom = rooms.value(m.destRoom);
	
	switch (m.type)
	{

	case MessageType::NEW_USER:
		//this->sendToAll(serverRoom, instMessage(m.mes, m.destRoom, MessageType::NEW_USER));
		this->sendToAll(serverRoom, instMessage(serverRoom->getUserNamesString(), m.destRoom, MessageType::USER_LIST));
		break;

	case MessageType::MESSAGE:
		this->sendToAll(serverRoom, instMessage(m.mes, m.destRoom, MessageType::MESSAGE));
		break;

	case MessageType::CONNECT_TO_ROOM:
		int socketKey = clientSocket->socketDescriptor();
		if (serverRoom != NULL/*rooms.contains(m.destRoom)*/)
		{
			serverRoom->addNewUser(socketKey, m.mes, clientSocket);
			this->sendMessage(clientSocket, instMessage(serverRoom->getWelcomeMessage(), m.destRoom, MessageType::CONNECT_TO_ROOM));
			
			disconnect(serverRoom->getClientSockets().value(socketKey), SIGNAL(disconnected()), this, SLOT(onDisconnectClient()));
			connect(serverRoom->getClientSockets().value(socketKey), SIGNAL(error(QAbstractSocket::SocketError)), serverRoom, SLOT(onDisconnectClient(QAbstractSocket::SocketError)));
			connect(serverRoom, SIGNAL(clientDisconnect(QString)), this, SLOT(onDisconnectRoomClient(QString)));
		}
		else
		{
			this->sendMessage(clientSocket, instMessage(m.destRoom, m.destRoom, MessageType::CONNECT_TO_ROOM_ERROR));
			clientSocket->close();
		}
		clientsSockets.remove(socketKey);
		break;
	}
}

QHash<QString, ServerRoom*> Server::getRooms()
{
	return this->rooms;
}

QHostAddress Server::getAddress()
{
	return this->address;
}

qint16 Server::getPort()
{
	return this->port;
}

void Server::onCreateRoom(QString name)
{
	this->rooms.insert(name, new ServerRoom(name));
	emit createdNewRoom(address, port, name);
}

void Server::sendMessage(QTcpSocket* socket, Message message)
{
	QDataStream dataStream(socket);
	//qDebug() << "server send to" << socket->socketDescriptor() << "message:" << message.mes << message.destRoom << message.type << "socket state:" << socket->state();
	dataStream << message;
}

Message Server::readMessage(QTcpSocket* socket)
{
	Message message;
	while (socket->bytesAvailable() > 0)
	{
		QDataStream dataStream(socket);
		dataStream >> message;
	}
	//qDebug() << "server receive from" << socket->socketDescriptor() << "message:" << message.mes << message.destRoom << message.type << "socket state:" << socket->state();
	return message;
}

void Server::sendToAll(ServerRoom* serverRoom, Message message)
{
	foreach(QTcpSocket* cSocket, serverRoom->getClientSockets())
	{
		if (cSocket->state() != QAbstractSocket::UnconnectedState)
			this->sendMessage(cSocket, message);
	}
}

QTcpServer* Server::getTcpServer()
{
	return this->tcpServer;
}