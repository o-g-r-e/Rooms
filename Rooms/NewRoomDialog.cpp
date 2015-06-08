#include "NewRoomDialog.h"

NewRoomDialog::NewRoomDialog(QWidget *parent) : QDialog(parent)
{
	ui.setupUi(this);
}

NewRoomDialog::~NewRoomDialog()
{

}

void NewRoomDialog::on_createRoomPushButton_clicked()
{
	emit addRoom(ui.roomNameLineEdit->text());
	this->close();
}