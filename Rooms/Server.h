#include "ServerRoom.h"
#include "Message.h"
#include <QtNetwork/qtcpserver.h>
#include <QtNetwork/qtcpsocket.h>
//#include <QtCore/qdatetime.h>

class Server : public QObject
{
	Q_OBJECT
public:
	Server();
	~Server();

private:
	QTcpServer* tcpServer;
	QHostAddress address;
	qint16 port;
	QHash<int, QTcpSocket*> clientsSockets;
	QHash<QString, ServerRoom*> rooms;
	
public:
	QHash<QString, ServerRoom*> getRooms();
	QHostAddress getAddress();
	qint16 getPort();
	QTcpServer* getTcpServer();

private:
	void sendMessage(QTcpSocket* socket, Message message);
	Message readMessage(QTcpSocket* socket);
	void sendToAll(ServerRoom* serverRoom, Message message);
	void removeClient(QTcpSocket* socket);
	void stopListenServer();
	void checkSockets(bool open);
	

signals:
	void createdNewRoom(QHostAddress address, qint16 port, QString name);
	void showEventMesage(QString message);
	void serverListen(bool state);
	//void roomClientDisconnect(QString roomName, QString clientNickName);
	
private slots:
	void onStartListenServer(QHostAddress address, qint16 port);
	void onStopServer();
	void onAddConnection();
	//void onClientConnectToRoom();
	void onRead();
	void onCreateRoom(QString name);
	void onDeleteRoom(QString name);
	void onDisconnectRoomClient(QString userNickName);
	void onDisconnectClient();
	void onServerAccessError(QAbstractSocket::SocketError socketError);
};