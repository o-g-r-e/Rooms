#include "RoomsTabWidget.h"

RoomsTabWidget::RoomsTabWidget(QWidget* parent)
{
	this->setParent(parent);
	connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(onCloseTab(int)));
}

RoomsTabWidget::~RoomsTabWidget()
{

}

void RoomsTabWidget::onCloseTab(int index)
{
	emit tabClosed(this->widget(index)->objectName());
	
	delete this->widget(index);
	//this->removeTab(index);
	
	if (this->count() == 0)
		this->hide();
};