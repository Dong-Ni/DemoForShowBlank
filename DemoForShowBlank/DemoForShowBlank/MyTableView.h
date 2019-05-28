#ifndef MYTABLEVIEW_H__ 
#define MYTABLEVIEW_H__

#include <QtWidgets/QTableView>

typedef enum ColInfo
{
	Station = 0,
	OddNumbers = 1,
	SN = 2,
	Product = 3,
	BeginTime = 4,
	EndTime = 5,
	BurnInState = 6,

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
