#include "ConnectToRoomDialog.h"

ConnectToRoomDialog::ConnectToRoomDialog(QWidget *parent) : QDialog(parent)
{
	ui.setupUi(this);
}

ConnectToRoomDialog::~ConnectToRoomDialog()
{

}

void ConnectToRoomDialog::on_connectToRoomPushButton_clicked()
{
	emit newRoom(QHostAddress(ui.ipLineEdit->text()), ui.portLineEdit->text().toInt(), ui.roomNameLineEdit->text());
	//this->close();
}