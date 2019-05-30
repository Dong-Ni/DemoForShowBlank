#ifndef MYTABLEVIEW_H__ 
#define MYTABLEVIEW_H__

#include <QtWidgets/QTableView>

typedef enum ColInfo
{
	SelectModel = 0,
	Station = 1,
	OddNumbers = 2,
	SN = 3,
	Product = 4,
	BeginTime = 5,
	EndTime = 6,
	BurnInState = 7,

}ColInfo_e;

class MyTableView : public QTableView
{
	Q_OBJECT

public:
	MyTableView(QWidget *parent = NULL);
	~MyTableView();


signals:
	void sgnItemTimeDoubleClick();

protected:
	virtual void mouseDoubleClickEvent(QMouseEvent *event);
};




#endif // MYTABLEVIEW_H__
