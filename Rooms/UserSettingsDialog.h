#ifndef USERSETTINGSDIALOG_H
#define USERSETTINGSDIALOG_H

#include "ui_UserSettings.h"
#include <QtWidgets/qdialog.h>

class UserSettingsDialog : public QDialog
{
	Q_OBJECT
public:
	UserSettingsDialog(QWidget *parent = 0);
	~UserSettingsDialog();

public:
	Ui::UserSettingsClass ui;

private slots:
	void onUserNickNameChanged(const QString & userNickName);

signals:
	void userNickNameValid(bool valid);
};

#endif // USERSETTINGSDIALOG_H
