#include "UserSettingsDialog.h"

UserSettingsDialog::UserSettingsDialog(QWidget *parent) : QDialog(parent)
{
	ui.setupUi(this);
}

UserSettingsDialog::~UserSettingsDialog()
{

}

void UserSettingsDialog::onUserNickNameChanged(const QString & userNickName)
{
	emit userNickNameValid(!userNickName.isEmpty());
}