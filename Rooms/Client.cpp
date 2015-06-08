#include "Client.h"

Client::Client() : QObject()
{
	
}

Client::~Client()
{

}

QHash<QString, ClientRoom*> Client::getRooms()
{
	return this->rooms;
}

void Client::onSocketStateChanged(QAbstractSocket::SocketState socketState)
{
	//qDebug() << "client: socket state" << socketState;
	ClientRoom* room = (ClientRoom*)sender();
	switch (socketState)
	{
	case QAbstractSocket::ConnectedState:
		rooms.insert(room->getName(), room);
		this->sendMessage(room->getSocket(), instMessage(nickName, room->getName(), MessageType::CONNECT_TO_ROOM));
		connect(room->getSocket(), SIGNAL(readyRead()), this, SLOT(onRead()));
		break;
	case QAbstractSocket::UnconnectedState:
		emit addNewMessage("{System}: Room offline", room->getName());
		break;
	}
}

void Client::onSocketError(QAbstractSocket::SocketError socketError)
{
	//qDebug() << "client: socket error" << socketError;
	switch (socketError)
	{
	case QAbstractSocket::ConnectionRefusedError:
		emit showEventMesage("Connection error");
		break;

	case QAbstractSocket::HostNotFoundError:
		emit showEventMesage("Host not found");
		break;
	}
}

void Client::onDeleteClientRoom(QString roomName)
{
	ClientRoom* clientRoom = this->rooms.value(roomName);
	clientRoom->getSocket()->close();
	this->rooms.remove(roomName);
}

void Client::onConnectToServer(QHostAddress address, qint16 port, QString name)
{
	ClientRoom* newClientRoom = new ClientRoom(name, address, port);

	connect(newClientRoom->getSocket(), SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onSocketError(QAbstractSocket::SocketError)));
	connect(newClientRoom->getSocket(), SIGNAL(stateChanged(QAbstractSocket::SocketState)), newClientRoom, SLOT(onSocketStateChanged(QAbstractSocket::SocketState)));
	connect(newClientRoom, SIGNAL(socketStateChanged(QAbstractSocket::SocketState)), this, SLOT(onSocketStateChanged(QAbstractSocket::SocketState)));
	connect(newClientRoom, SIGNAL(sendTypedMessage(QString)), this, SLOT(onSendTypedMessage(QString)));
	newClientRoom->connect();
}

void Client::onRead()
{
	QTcpSocket* clientSocket = (QTcpSocket*)sender();
	QList<Message> messageList = readMessage(clientSocket);
	foreach(Message m, messageList)
	{
		switch (m.type)
		{
		case MessageType::CONNECT_TO_ROOM:
			this->sendMessage(clientSocket, instMessage(this->nickName, m.destRoom, MessageType::NEW_USER));
			emit successConnected(m.mes, rooms.value(m.destRoom));
			break;

		case MessageType::CONNECT_TO_ROOM_ERROR:
			emit showEventMesage("Room not found");
			break;

		/*case MessageType::NEW_USER:
			emit addNewUserToRoom(m.mes, m.destRoom);
			break;*/

		case MessageType::USER_LIST:
			emit setUserList(m.mes.split(";"), m.destRoom);
			break;

		case MessageType::REM_USER:
			emit removeUserFromRoom(m.mes, m.destRoom);
			break;

		case MessageType::MESSAGE:
			emit addNewMessage(m.mes, m.destRoom);
			break;
		default:
			break;
		}
	}
}

void Client::onSetNickName(const QString & nickName)
{
	this->nickName = nickName;
}

QString Client::getNickName()
{
	return this->nickName;
}

void Client::sendMessage(QTcpSocket* socket, Message message)
{
	if (socket->state() == QAbstractSocket::UnconnectedState)
	{
		ClientRoom* room = rooms.value(message.destRoom);
		room->connect();
	}
	else
	{
		QDataStream dataStream(socket);
		//qDebug() << "client send" << message.mes << message.destRoom << message.type;
		dataStream << message;
	}
}

QList<Message> Client::readMessage(QTcpSocket* socket)
{
	QList<Message> messageList;

	Message message;
	while (socket->bytesAvailable() > 0)
	{
		QDataStream dataStream(socket);
		dataStream >> message;
		//qDebug() << "client receive" << message.mes << message.destRoom << message.type;
		messageList.append(message);
	}
	return messageList;
}

/*void Client::connectRoomToHost(ClientRoom* room, QHostAddress address, qint16 port)
{
	room->getSocket()->connectToHost(address, port);
}*/

void Client::onSendTypedMessage(QString typedMessage)
{
	ClientRoom* clientRoom = (ClientRoom*)sender();

	this->sendMessage(clientRoom->getSocket(), instMessage("["+this->nickName+"]: "+typedMessage, clientRoom->getName(), MessageType::MESSAGE));
}