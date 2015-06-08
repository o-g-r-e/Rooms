#include "ClientRoom.h"
#include "Message.h"
#include <QtCore\qhash.h>

class Client : public QObject
{
	Q_OBJECT
public:
	Client();
	~Client();

private:
	QHash<QString, ClientRoom*> rooms;
	QString nickName;

public:
	QHash<QString, ClientRoom*> getRooms();
	QString getNickName();
	void sendMessage(QTcpSocket* socket, Message message);
	QList<Message> readMessage(QTcpSocket* socket);

private slots:
	void onConnectToServer(QHostAddress address, qint16 port, QString name);
	void onRead();
	void onSetNickName(const QString & nickName);
	void onSocketStateChanged(QAbstractSocket::SocketState socketState);
	void onSendTypedMessage(QString typedMessage);
	void onSocketError(QAbstractSocket::SocketError state);
	//void connectToRoom();
	void onDeleteClientRoom(QString roomName);

signals:
	void successConnected(QString name, ClientRoom* client);
	//void addNewUserToRoom(QString userName, QString roomName);
	void setUserList(QStringList userNickNames, QString roomName);
	void removeUserFromRoom(QString userName, QString roomName);
	void addNewMessage(QString message, QString roomName);
	void showEventMesage(QString);
};