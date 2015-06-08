#ifndef CONNECTTOROOMDIALOG_H
#define CONNECTTOROOMDIALOG_H

#include "ui_ConnectToRoom.h"
#include <QtWidgets/qdialog.h>
#include <QtCore\qstring.h>
#include <QtNetwork\qhostaddress.h>

class ConnectToRoomDialog : public QDialog
{
	Q_OBJECT

public:
	ConnectToRoomDialog(QWidget *parent = 0);
	~ConnectToRoomDialog();

private:
	Ui::ConnectToRoomClass ui;

signals:
	void newRoom(QHostAddress address, qint16 port, QString name);

private slots:
	void on_connectToRoomPushButton_clicked();
};

#endif // CONNECTTOROOMDIALOG_H
