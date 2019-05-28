#include "MyTableView.h"

MyTableView::MyTableView(QWidget *parent /*= NULL*/) : QTableView(parent)
{

}

MyTableView::~MyTableView()
{

}

void MyTableView::mouseDoubleClickEvent(QMouseEvent *event)
{
	QModelIndex index = currentIndex();
	if (index.column() == BeginTime || index.column() == EndTime)
	{
		emit sgnItemTimeDoubleClick();
	}

	QTableView::mouseDoubleClickEvent(event);
}
