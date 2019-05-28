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
	void OnModelClose();  //�˳�ģ��
	QString EnumToString(ColInfo_e iEnum);

protected:
	virtual void resizeEvent(QResizeEvent *event);
	virtual void closeEvent(QCloseEvent *event);

signals:
	void sgnItemTimeDoubleClick();

public slots:
	//ˢ�½�������ʾʱ��
	void SetTime();
	//ˢ�½���������
	void SetTotal(int nTotal = 0);
	//��ӻ���ɾ����ť
	void OnBtnAdd();
	void OnBtnDel();
	//ʱ�䰴ť�л�
	void OnMouseDoubleClickTimeModel();
	void OnSelTimeOkClicked(const QDateTime& dateTime);
	//��ʱ��������ˢ�½���
	void OnRepaintTimeOut();

private:
    Ui::ShowTheStateClass ui;
	QStandardItemModel    *m_pModel;
	CTimeSelect           *m_pTimeSelect;
	BarDelegate           *m_pBarDelegate;
	QTimer                *m_pRepaintTimer;
	std::mutex            m_myMutex;
};
