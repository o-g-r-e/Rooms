#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_MainWindow.h"
#include "Server.h"
#include "Client.h"
#include "ClientRoom.h"
#include "NewRoomDialog.h"
#include "ServerSettingsDialog.h"
#include "ConnectToRoomDialog.h"
#include "UserSettingsDialog.h"
#include "RoomsTabWidget.h"
#include <QtWidgets/QMainWindow>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QtWidgets\qtextedit.h>
#include <QtWidgets\qlistwidget.h>

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	Ui::MainWindowClass ui;
	RoomsTabWidget* tabs;
	Server* server;
	Client* client;
	

	NewRoomDialog* newRoomDialog;
	ServerSettingsDialog* serverSettingsDialog;
	ConnectToRoomDialog* connectToRoomDialog;
	UserSettingsDialog* userSettingsDialog;

private:
	void showModalDialog(QDialog* dialog);
	//void showServerSettingsDialog();
	void deleteWidgetsRecurcivety(QObject* layout);

public slots:
	void on_actionNew_room_triggered();
	void on_actionServer_settings_triggered();
	void on_actionConnect_to_room_triggered();
	void on_actionUser_settings_triggered();
	void onCreateRoomTab(QString welcomeMessage, ClientRoom* clientRoom);
	//void onAddNewUserToRoom(QString userName, QString roomName);
	void onSetUserList(QStringList userNickNames, QString roomName);
	void onRemoveUserFromRoom(QString userName, QString roomName);
	void onAddNewMessageToRoom(QString message, QString roomName);
	void onShowMessageBox(QString message);
	//void onCloseRoomTab(QString roomName);
};

#endif // MAINWINDOW_H
