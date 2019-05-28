#include "TimeSelect.h"
#include "ui_TimeSelect.h"
#include <QtCore/QDateTime>

CTimeSelect::CTimeSelect(QWidget* parent /*= NULL*/) : QDialog(parent)
{
	ui = new Ui::TimeSelect;
	ui->setupUi(this);
	InitUI();
}

CTimeSelect::~CTimeSelect()
{

}

void CTimeSelect::InitUI()
{
	ui->m_dateTime->setDateTime(QDateTime::currentDateTime());
	connect(ui->m_btnCancel, SIGNAL(clicked()), this, SLOT(OnBtnCanelClicked()));
	connect(ui->m_btnOk, SIGNAL(clicked()), this, SLOT(OnBtnOkClicked()));
}

void CTimeSelect::OnBtnOkClicked()
{	
	QDateTime dateTime = ui->m_dateTime->dateTime();
	emit sgnSetCurTimeClicked(dateTime);
	hide();
}

void CTimeSelect::OnBtnCanelClicked()
{
	hide();
}

