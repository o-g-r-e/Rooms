#pragma once
#include <QtCore\qdatastream.h>

static enum MessageType { CONNECT_TO_ROOM = 1, CONNECT_TO_ROOM_ERROR, NEW_USER, REM_USER, USER_LIST, MESSAGE };

class Message
{
public:
	
	QString mes;
	QString destRoom;
	qint32 type;
};

Message instMessage(QString mes, QString destRoom, qint32 type);

/*struct Message
{
	QString mes;
	QString destRoom;
	qint32 type;
};*/

QDataStream &operator<<(QDataStream &out, const Message &message);
QDataStream &operator>>(QDataStream &in, Message &message);