#ifndef TIMESELECT_H__
#define TIMESELECT_H__

#include <QtWidgets/QDialog>

namespace Ui
{
	class TimeSelect;
}

class CTimeSelect : public QDialog
{
	Q_OBJECT
public:
	CTimeSelect(QWidget* parent = NULL);
	~CTimeSelect();

	void InitUI();

signals:
	void sgnSetCurTimeClicked(const QDateTime& dateTiem);


public slots:
	void OnBtnOkClicked();
	void OnBtnCanelClicked();

private:
	Ui::TimeSelect *ui;
};


#endif //TIMESELECT_H__

