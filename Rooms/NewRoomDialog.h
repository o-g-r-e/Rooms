#ifndef NEWROOMDIALOG_H
#define NEWROOMDIALOG_H

#include "ui_NewRoom.h"
#include <QtWidgets/qdialog.h>


class NewRoomDialog : public QDialog
{
	Q_OBJECT
public:
	NewRoomDialog(QWidget *parent = 0);
	~NewRoomDialog();

private:
	Ui::NewRoomClass ui;

signals:
	void addRoom(QString name);

private slots:
	void on_createRoomPushButton_clicked();
};

#endif // NEWROOMDIALOG_H
