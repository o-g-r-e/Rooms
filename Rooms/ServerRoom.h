#include <QtCore\qstring.h>
#include <QtCore\qhash.h>
#include <QtNetwork\qtcpsocket.h>

class ServerRoom : public QObject
{
	Q_OBJECT
public:
	ServerRoom(QString roomName);
	~ServerRoom();

private:
	QString roomName;
	QHash<int, QTcpSocket*> clientsSockets;
	QHash<int, QString> userNames;
	QString welcomeMessage;

public:
	QHash<int, QTcpSocket*> getClientSockets();
	QHash<int, QString> getUserNames();
	QString getRoomName();
	void addNewUser(int socketDescriptor, QString userNickName, QTcpSocket* clientSocket);
	void removeUser(int socketDescriptor);
	void clearUsers();
	QString getUserNamesString();
	QString getWelcomeMessage();

signals:
	void clientDisconnect(QString userNickName);

private slots:
	void onDisconnectClient(QAbstractSocket::SocketError socketState);
};