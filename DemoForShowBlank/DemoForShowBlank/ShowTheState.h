#pragma once

#include <QtWidgets/QWidget>
#include "ui_ShowTheState.h"
#include <QtGui/QStandardItemModel>
#include <mutex>

class CTimeSelect;

class BarDelegate : public QAbstractItemDelegate
{
	Q_OBJECT
public:
	BarDelegate(QObject * parent = NULL);
	virtual void paint(QPainter *painter,
		const QStyleOptionViewItem &option,
		const QModelIndex &index) const;

	virtual QSize sizeHint(const QStyleOptionViewItem &option,
		const QModelIndex &index) const;
};

class ShowTheState : public QWidget
{
    Q_OBJECT

public:
    ShowTheState(QWidget *parent = Q_NULLPTR);

	void InitUI();
	void OnModelOpen();
	void OnAddOneItem(ColInfo_e iEnum, const QString& str);
	void OnModelClose();  //退出模块
	QString EnumToString(ColInfo_e iEnum);

protected:
	virtual void resizeEvent(QResizeEvent *event);
	virtual void closeEvent(QCloseEvent *event);

signals:
	void sgnItemTimeDoubleClick();

public slots:
	//刷新界面上显示时间
	void SetTime();
	//刷新界面上总数
	void SetTotal(int nTotal = 0);
	//添加或者删除按钮
	void OnBtnAdd();
	void OnBtnDel();
	//时间按钮切换
	void OnMouseDoubleClickTimeModel();
	void OnSelTimeOkClicked(const QDateTime& dateTime);
	//定时器触发，刷新界面
	void OnRepaintTimeOut();

private:
    Ui::ShowTheStateClass ui;
	QStandardItemModel    *m_pModel;
	CTimeSelect           *m_pTimeSelect;
	BarDelegate           *m_pBarDelegate;
	QTimer                *m_pRepaintTimer;
	std::mutex            m_myMutex;
};
