#ifndef SERVERSETTINGSDIALOG_H
#define SERVERSETTINGSDIALOG_H

#include "ui_ServerSettings.h"
#include <QtWidgets/qdialog.h>
#include <QtNetwork/qhostaddress.h>

class ServerSettingsDialog : public QDialog
{
	Q_OBJECT

public:
	ServerSettingsDialog(QWidget *parent = 0);
	~ServerSettingsDialog();

private:
	Ui::ServerSettingsClass ui;

signals:
	void startServer(QHostAddress address, qint16 port);
	void stopServer();

private slots:
	void on_startServerPushButton_clicked();
	void on_stopServerPushButton_clicked();

public slots:
	void onServerListen(bool state);
};

#endif // SERVERSETTINGSDIALOG_H
