#include "ServerSettingsDialog.h"

ServerSettingsDialog::ServerSettingsDialog(QWidget *parent) : QDialog(parent)
{
	ui.setupUi(this);
}

ServerSettingsDialog::~ServerSettingsDialog()
{

}

void ServerSettingsDialog::on_startServerPushButton_clicked()
{
	emit startServer(QHostAddress(ui.ipLineEdit->text()), ui.portLineEdit->text().toInt());
}

void ServerSettingsDialog::on_stopServerPushButton_clicked()
{
	emit stopServer();
}

void ServerSettingsDialog::onServerListen(bool state)
{
	if (state)
	{
		this->ui.serverStatusLabel->setText("Server status: online");
		this->ui.startServerPushButton->setEnabled(false);
	}
	else
	{
		this->ui.serverStatusLabel->setText("Server status: offline");
		this->ui.startServerPushButton->setEnabled(true);
	}
}