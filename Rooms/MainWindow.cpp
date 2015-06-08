#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
	ui.setupUi(this);
	
	tabs = new RoomsTabWidget(this);
	tabs->setTabsClosable(true);
	tabs->setObjectName("tabsWidget");
	tabs->hide();
	
	setCentralWidget(tabs);

	server = new Server();
	connect(server, SIGNAL(showEventMesage(QString)), this, SLOT(onShowMessageBox(QString)));
	connect(server, SIGNAL(serverListen(bool)), ui.actionNew_room, SLOT(setEnabled(bool)));

	client = new Client();
	connect(client, SIGNAL(showEventMesage(QString)), this, SLOT(onShowMessageBox(QString)));
	connect(client, SIGNAL(successConnected(QString, ClientRoom*)), this, SLOT(onCreateRoomTab(QString, ClientRoom*)));
	//connect(client, SIGNAL(addNewUserToRoom(QString, QString)), this, SLOT(onAddNewUserToRoom(QString, QString)));
	connect(client, SIGNAL(setUserList(QStringList, QString)), this, SLOT(onSetUserList(QStringList, QString)));
	connect(client, SIGNAL(removeUserFromRoom(QString, QString)), this, SLOT(onRemoveUserFromRoom(QString, QString)));
	connect(client, SIGNAL(addNewMessage(QString, QString)), this, SLOT(onAddNewMessageToRoom(QString, QString)));

	connect(server, SIGNAL(createdNewRoom(QHostAddress, qint16, QString)), client, SLOT(onConnectToServer(QHostAddress, qint16, QString)));

	connect(tabs, SIGNAL(tabClosed(QString)), client, SLOT(onDeleteClientRoom(QString)));
	connect(tabs, SIGNAL(tabClosed(QString)), server, SLOT(onDeleteRoom(QString)));

	newRoomDialog = new NewRoomDialog(this);
	connect(newRoomDialog, SIGNAL(addRoom(QString)), server, SLOT(onCreateRoom(QString)));

	serverSettingsDialog = new ServerSettingsDialog(this);
	connect(serverSettingsDialog, SIGNAL(startServer(QHostAddress, qint16)), server, SLOT(onStartListenServer(QHostAddress, qint16)));
	connect(serverSettingsDialog, SIGNAL(stopServer()), server, SLOT(onStopServer()));
	connect(server, SIGNAL(serverListen(bool)), serverSettingsDialog, SLOT(onServerListen(bool)));

	connectToRoomDialog = new ConnectToRoomDialog(this);
	connect(connectToRoomDialog, SIGNAL(newRoom(QHostAddress, qint16, QString)), client, SLOT(onConnectToServer(QHostAddress, qint16, QString)));

	userSettingsDialog = new UserSettingsDialog(this);
	connect(userSettingsDialog->ui.userNameLineEdit, SIGNAL(textChanged(const QString &)), client, SLOT(onSetNickName(const QString &)));
	connect(userSettingsDialog->ui.userNameLineEdit, SIGNAL(textChanged(const QString &)), userSettingsDialog, SLOT(onUserNickNameChanged(const QString &)));
	connect(userSettingsDialog, SIGNAL(userNickNameValid(bool)), ui.actionConnect_to_room, SLOT(setEnabled(bool)));
}

MainWindow::~MainWindow()
{
	server->~Server();
}

void MainWindow::on_actionNew_room_triggered()
{
	if (!client->getNickName().isEmpty())
		showModalDialog(newRoomDialog);
	else
		this->onShowMessageBox("Set your nickname first!");
}

void MainWindow::on_actionServer_settings_triggered()
{
	showModalDialog(serverSettingsDialog);
}

void MainWindow::on_actionConnect_to_room_triggered()
{
	showModalDialog(connectToRoomDialog);
}

void MainWindow::on_actionUser_settings_triggered()
{
	showModalDialog(userSettingsDialog);
}

void MainWindow::deleteWidgetsRecurcivety(QObject* layout)
{
	QObjectList objectList = layout->children();

	if (objectList.count() > 0)
	{
		foreach(QObject* widget, objectList)
		{
			this->deleteWidgetsRecurcivety(widget);
		}
	}
	else
	{
		delete layout;
	}
}

//void MainWindow::onCloseRoomTab(QString roomName)
//{
	////qDebug() << "onCloseRoomTab" << index;
	//client->deleteClientRoom(this->tabs->widget(index)->objectName());
	////QWidget* tab = this->tabs->widget(index);
	////QLayout* tabLayout = tab->layout();
	////deleteWidgetsRecurcivety(tabLayout);
	////tab->deleteLater();
	//this->tabs->removeTab(index);

	////delete tab;
	

	//if (this->tabs->count() == 0)
		//this->tabs->hide();

	//return;
//}

void MainWindow::onCreateRoomTab(QString welcomeMessage, ClientRoom* clientRoom)
{
	tabs->show();

	QString roomName = clientRoom->getName();
	QWidget* roomTab = tabs->findChild<QWidget*>(roomName);

	if (roomTab == NULL)
	{
		QWidget* newTab = new QWidget();
		newTab->setObjectName(roomName);
		tabs->addTab(newTab, roomName);

		QHBoxLayout *hLayout = new QHBoxLayout();
		QVBoxLayout *vLayout1 = new QVBoxLayout();
		QVBoxLayout *vLayout2 = new QVBoxLayout();

		hLayout->setObjectName("hLayout");
		vLayout1->setObjectName("vLayout1");
		vLayout2->setObjectName("vLayout2");

		QTextEdit* chatTextEdit = new QTextEdit();
		chatTextEdit->setTextInteractionFlags(Qt::NoTextInteraction);
		chatTextEdit->setObjectName("chatTextEdit");
		vLayout1->addWidget(chatTextEdit);

		QTextEdit* messageTextEdit = new QTextEdit();
		messageTextEdit->setFixedHeight(50);
		messageTextEdit->setObjectName("messageTextEdit");
		connect(messageTextEdit, SIGNAL(textChanged()), clientRoom, SLOT(onSetTypedMessage()));
		vLayout1->addWidget(messageTextEdit);

		QListWidget* usersListWidget = new QListWidget();
		usersListWidget->setFixedWidth(150);
		usersListWidget->setObjectName("usersListWidget");
		vLayout2->addWidget(usersListWidget);
		qDebug() << usersListWidget->objectName();

		QPushButton* sendButton = new QPushButton("Send");
		sendButton->setFixedWidth(150);
		sendButton->setObjectName("sendButton");
		connect(sendButton, SIGNAL(clicked()), clientRoom, SLOT(onSendTypedMessage()));
		connect(sendButton, SIGNAL(clicked()), messageTextEdit, SLOT(clear()));
		vLayout2->addWidget(sendButton);

		hLayout->addLayout(vLayout1);
		hLayout->addLayout(vLayout2);
		newTab->setLayout(hLayout);
	}

	this->onAddNewMessageToRoom(welcomeMessage, roomName);
}

/*void MainWindow::onAddNewUserToRoom(QString userName, QString roomName)
{
	QWidget* roomTab = tabs->findChild<QWidget*>(roomName);
	QListWidget* userListWitget = roomTab->findChild<QListWidget*>("usersListWidget");
	userListWitget->addItem(userName);
}*/

void MainWindow::onSetUserList(QStringList userNickNames, QString roomName)
{
	QWidget* roomTab = tabs->findChild<QWidget*>(roomName);
	QListWidget* userListWitget = roomTab->findChild<QListWidget*>("usersListWidget");
	userListWitget->clear();
	userListWitget->addItems(userNickNames);
}

void MainWindow::onRemoveUserFromRoom(QString userName, QString roomName)
{
	QWidget* roomTab = tabs->findChild<QWidget*>(roomName);
	QListWidget* userListWitget = roomTab->findChild<QListWidget*>("usersListWidget");
	QList<QListWidgetItem*> items = userListWitget->findItems(userName, Qt::MatchExactly);

	//userListWitget->removeItemWidget(items.value(0));
	
	for (int i = userListWitget->count()-1; i >= 0; i--)
	{
		if (userListWitget->item(i)->text() == userName)
		{
			userListWitget->takeItem(i);
			break;
		}
	}
}

void MainWindow::onAddNewMessageToRoom(QString message, QString roomName)
{
	qDebug() << message;
	QWidget* roomTab = tabs->findChild<QWidget*>(roomName);
	if (roomTab != NULL)
	{
		QTextEdit* chatTextEdit = roomTab->findChild<QTextEdit*>("chatTextEdit");
		if (chatTextEdit != NULL)
		{
			chatTextEdit->append(message);
		}
	}
}

void MainWindow::showModalDialog(QDialog* dialog)
{
	//dialog->setWindowModality(Qt::WindowModal);
	//dialog->show();
	dialog->setModal(true);
	dialog->exec();
}

/*void MainWindow::showServerSettingsDialog()
{
	this->serverSettingsDialog->onServerListen(server->getTcpServer()->isListening());
}*/

void MainWindow::onShowMessageBox(QString message)
{
	QMessageBox::critical(this, "Error", message, QMessageBox::Ok, QMessageBox::Ok);
}