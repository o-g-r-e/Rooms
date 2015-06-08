#include <qtabwidget.h>

class RoomsTabWidget : public QTabWidget
{
	Q_OBJECT
public:
	RoomsTabWidget(QWidget* parent);
	~RoomsTabWidget();

signals:
	void tabClosed(QString roomName);

public slots:
	void onCloseTab(int index);
};