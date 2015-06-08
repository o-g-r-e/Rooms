#include "Message.h"

Message instMessage(QString mes, QString destRoom, qint32 type)
{
	Message m;
	m.mes = mes;
	m.destRoom = destRoom;
	m.type = type;

	return m;
}

QDataStream &operator<<(QDataStream &out, const Message &message)
{
	out << message.mes << message.destRoom << message.type;
	return out;
}

QDataStream &operator>>(QDataStream &in, Message &message)
{
	in >> message.mes >> message.destRoom >> message.type;
	return in;
}