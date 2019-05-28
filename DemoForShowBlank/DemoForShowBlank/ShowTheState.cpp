#include "ShowTheState.h"
#include <QtCore/QDateTime>
#include <QtCore/QTimer>
#include <QtCore/QFile>
#include <QtCore/QXmlStreamWriter>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QHeaderView>
#include <QtGui/QPainter>
#include <QDebug>
#include "MyTableView.h"
#include "TimeSelect.h"

ShowTheState::ShowTheState(QWidget *parent)
	: QWidget(parent)
	, m_pModel(NULL)
	, m_pTimeSelect(NULL)
	, m_pBarDelegate(NULL)
	, m_pRepaintTimer(NULL)
{
	ui.setupUi(this);
	InitUI();
	OnModelOpen();
}

void ShowTheState::InitUI()
{
	//设置标题  装配车间老化管理看板
	ui.m_labTittle->setText(tr("Assembly plant burn-in manage plank."));

	//设置时间字段
	SetTime();
	QPalette paletteTime;
	paletteTime.setColor(QPalette::Background, QColor(65, 157, 252));
	paletteTime.setColor(QPalette::WindowText, Qt::white);
	ui.m_labTime->setAutoFillBackground(true);
	ui.m_labTime->setPalette(paletteTime);


	//设置总数
	SetTotal();
	ui.m_labTotal->setAutoFillBackground(true);
	ui.m_labTotal->setPalette(paletteTime);

	m_pModel = new QStandardItemModel(this);
	QStringList strTitleList;
	// 初始化表头
	// 工位 工单 SN 产品 开始时间 结束时间 老化进度
	strTitleList << tr("station") << tr("odd numbers") << tr("SN") << tr("product") << tr("Begin Time") << tr("End Time") << tr("burn-in state");
	m_pModel->setColumnCount(strTitleList.size());
	int nColIndex = 0;
	foreach (QString strItemName , strTitleList)
	{
		m_pModel->setHeaderData(nColIndex, Qt::Horizontal, strItemName);
		nColIndex++;
	}
	ui.m_tabView->setModel(m_pModel);

	//设置表头宽度
	//ui.m_tabView->horizontalHeader()->setMaximumWidth(60);
	//ui.m_tabView->horizontalHeader()->setMinimumWidth(60);
	ui.m_tabView->verticalHeader()->setMaximumWidth(40);
	ui.m_tabView->verticalHeader()->setMinimumWidth(40);

	//初始化时间控件
	m_pTimeSelect = new CTimeSelect(this);

	//初始化定时器
	m_pRepaintTimer = new QTimer(this);
	connect(m_pRepaintTimer, SIGNAL(timeout()), this, SLOT(OnRepaintTimeOut()));
	m_pRepaintTimer->start(1000);

	connect(ui.m_btnAdd, SIGNAL(clicked()), this, SLOT(OnBtnAdd()));
	connect(ui.m_btnDel, SIGNAL(clicked()), this, SLOT(OnBtnDel()));
	connect(ui.m_tabView, SIGNAL(sgnItemTimeDoubleClick()), this, SLOT(OnMouseDoubleClickTimeModel()));
	connect(m_pTimeSelect, SIGNAL(sgnSetCurTimeClicked(const QDateTime&)), this, SLOT(OnSelTimeOkClicked(const QDateTime&)));

}

void ShowTheState::OnModelOpen()
{
	if (NULL == m_pModel)
	{
		return;
	}

	QFile file("MyDbInfo.xml");
	bool bRet = file.open(QIODevice::ReadOnly | QFile::Text);
	if (!bRet)
	{
		return;
	}

	QXmlStreamReader rend(&file);
	while (!rend.atEnd() && !rend.hasError())
	{
		if (rend.isStartElement())
		{
			if (rend.name() == "DetailInfo")
			{
				OnBtnAdd();
			}
			QString strTmp = rend.readElementText();
			if (rend.name() == EnumToString(Station))
			{
				OnAddOneItem(Station, strTmp);
			}
			else if (rend.name() == EnumToString(OddNumbers))
			{
				OnAddOneItem(OddNumbers, strTmp);
			}
			else if (rend.name() == EnumToString(SN))
			{
				OnAddOneItem(SN, strTmp);
			}
			else if (rend.name() == EnumToString(Product))
			{
				OnAddOneItem(Product, strTmp);
			}
			else if (rend.name() == EnumToString(BeginTime))
			{
				OnAddOneItem(BeginTime, strTmp);
			}
			else if (rend.name() == EnumToString(EndTime))
			{
				OnAddOneItem(EndTime, strTmp);
			}
			else if (rend.name() == EnumToString(BurnInState))
			{
				OnAddOneItem(BurnInState, strTmp);
			}
		}
		else if (rend.isEndElement())
		{
			qDebug() << "+++++++End+++++++";
		}
		rend.readNext();
	}

	file.close();
}

void ShowTheState::OnAddOneItem(ColInfo_e iEnum, const QString& str)
{
	if (str.isEmpty())
	{
		return;
	}

	QStandardItem *item = new QStandardItem();
	item->setData(str, Qt::DisplayRole);
	m_pModel->setItem(m_pModel->rowCount() - 1, iEnum, item);
}

void ShowTheState::OnModelClose()
{
	if (NULL != m_pRepaintTimer)
	{
		m_pRepaintTimer->stop();
	}


	if (NULL == m_pModel)
	{
		return;
	}

	// 工位 工单 SN 产品 开始时间 结束时间 老化进度
	//strTitleList << tr("station") << tr("odd numbers") << tr("SN") << tr("product") << tr("Begin Time") << tr("End Time") << tr("burn-in state");
	QFile file("MyDbInfo.xml");
	file.open(QIODevice::WriteOnly);
	QXmlStreamWriter xmlWirte(&file);
	xmlWirte.setAutoFormatting(true);
	//开始写文件
	xmlWirte.writeStartDocument();
	xmlWirte.writeStartElement("BeginDBInfo");
	QString  strTmp;
	for (int i = 0; i < m_pModel->rowCount(); i++)
	{
		//第一个字段信息
		xmlWirte.writeStartElement("DetailInfo");
		xmlWirte.writeAttribute("id", QString::number(i));
		QStandardItem *item = NULL;
		for (int j = Station; j <= BurnInState; j++)
		{
			strTmp.clear();
			item = m_pModel->item(i, j);
			if (NULL != item)
			{
				strTmp = item->data(Qt::DisplayRole).toString();
			}
			xmlWirte.writeTextElement(EnumToString(ColInfo_e(j)), strTmp);
		}
		xmlWirte.writeEndElement();
	}
	//结束
	xmlWirte.writeEndDocument();
}

QString  ShowTheState::EnumToString(ColInfo_e iEnum)
{
	QString strRet;
	switch (iEnum)
	{
	case Station:
		strRet = "Station";
		break;
	case OddNumbers:
		strRet = "OddNumbers";
		break;
	case SN:
		strRet = "SN";
		break;
	case Product:
		strRet = "Product";
		break;
	case BeginTime:
		strRet = "BeginTime";
		break;
	case EndTime:
		strRet = "EndTime";
		break;
	case BurnInState:
		strRet = "BurnInState";
		break;
	default:
		break;
	}

	return strRet;
}

void ShowTheState::resizeEvent(QResizeEvent *event)
{
	QHeaderView *header = ui.m_tabView->verticalHeader();

	int nHeadWidth = header->width();
	int nTotalWidth = ui.m_wgtTotal->width()- nHeadWidth;

	ui.m_tabView->setColumnWidth(Station, 50 * nTotalWidth / 1000);
	ui.m_tabView->setColumnWidth(OddNumbers, 75 * nTotalWidth / 1000);
	ui.m_tabView->setColumnWidth(SN, 75 * nTotalWidth / 1000);
	ui.m_tabView->setColumnWidth(Product, 100 * nTotalWidth / 1000);
	ui.m_tabView->setColumnWidth(BeginTime, 160 * nTotalWidth / 1000);
	ui.m_tabView->setColumnWidth(EndTime, 160 * nTotalWidth / 1000);
	ui.m_tabView->setColumnWidth(BurnInState, 380 * nTotalWidth / 1000);

	QWidget::resizeEvent(event);
}

void ShowTheState::closeEvent(QCloseEvent *event)
{
	OnModelClose();
	QWidget::closeEvent(event);
}

void ShowTheState::SetTime()
{
	QDateTime curTime = QDateTime::currentDateTime();
	QString strTimeFormat = QString("yyyy-MM-dd hh:mm:ss");
	QString strTime = curTime.toString(strTimeFormat);
	ui.m_labTime->setText(strTime);
}

void ShowTheState::SetTotal(int nTotal)
{
	QString strTotal = QString("%1%2").arg(tr("Total: ")).arg(QString::number(nTotal));
	ui.m_labTotal->setText(strTotal);
}

void ShowTheState::OnBtnAdd()
{
	if (NULL == m_pModel)
	{
		return;
	}

	if (NULL == m_pBarDelegate)
	{
		m_pBarDelegate = new BarDelegate(this);
	}

	m_pModel->insertRow(m_pModel->rowCount());
	QStandardItem *itemState = new QStandardItem(QString::number(0));
	itemState->setData(0.0, Qt::DisplayRole);
	m_pModel->setItem(m_pModel->rowCount() - 1, BurnInState, itemState);
	ui.m_tabView->setItemDelegateForColumn(BurnInState, m_pBarDelegate);

	itemState->setFlags(itemState->flags() & (~Qt::ItemIsEditable));
}

void ShowTheState::OnBtnDel()
{
	if (NULL == m_pModel && m_pModel->rowCount()>=0)
	{
		return;
	}

	m_pModel->removeRow(m_pModel->rowCount()-1);
	ui.m_tabView->setItemDelegateForColumn((int)BurnInState, m_pBarDelegate);
}

void ShowTheState::OnMouseDoubleClickTimeModel()
{
	m_pTimeSelect->exec();
}

void ShowTheState::OnSelTimeOkClicked(const QDateTime& dateTime)
{
	if (NULL == m_pModel)
	{
		return;
	}

	QModelIndex index = ui.m_tabView->currentIndex();

	QStandardItem *item = new QStandardItem;
	QString strDateTimeFormat = QString("yyyy-MM-dd hh:mm:ss");
	QString strDateTime = dateTime.toString(strDateTimeFormat);
	item->setData(strDateTime, Qt::DisplayRole);
	m_pModel->setItem(index.row(), index.column(), item);
	item->setFlags(item->flags() & (~Qt::ItemIsEditable));

	QStandardItem *itemBeg = m_pModel->item(index.row(), BeginTime);
	QStandardItem *itemEnd = m_pModel->item(index.row(), EndTime);

	if (itemBeg && itemEnd)
	{
		//设置进度条
		QDateTime dateBeg = QDateTime::fromString(itemBeg->data(Qt::DisplayRole).toString(), strDateTimeFormat);
		QDateTime dateEnd = QDateTime::fromString(itemEnd->data(Qt::DisplayRole).toString(), strDateTimeFormat);

		if (dateEnd > dateBeg)
		{
			QStandardItem *itemBurState = m_pModel->item(index.row(), BurnInState);
			itemBurState->setData(dateBeg.toSecsSinceEpoch(), Qt::UserRole);
			itemBurState->setData(dateEnd.toSecsSinceEpoch(), Qt::UserRole + 1);
		}
	}
}

void ShowTheState::OnRepaintTimeOut()
{
	if (NULL == m_pModel)
	{
		return;
	}

	//刷新标题栏时间
	SetTime();

	//设置总数
	SetTotal(m_pModel->rowCount());

	for (int i = 0; i < m_pModel->rowCount(); i++)
	{
		QStandardItem *itemBurState = m_pModel->item(i, BurnInState);
		if (itemBurState == NULL)
		{
			continue;
		}
		int64_t timeBeg = itemBurState->data(Qt::UserRole).toULongLong();
		int64_t timeEnd = itemBurState->data(Qt::UserRole+1).toULongLong();
		int64_t timeCur = QDateTime::currentDateTime().toSecsSinceEpoch();
		float val = 0.0;
		if (timeBeg <= timeCur && timeCur <= timeEnd)
		{
			int64_t nLast = timeCur - timeBeg;
			int64_t nTol = timeEnd - timeBeg;
			val = (float)nLast / (float)nTol * float(100.0);
		}
		std::unique_lock<std::mutex> myLockGd(m_myMutex);
		itemBurState->setData(val, Qt::DisplayRole);
	}
}

BarDelegate::BarDelegate(QObject * parent /*= NULL*/) : QAbstractItemDelegate(parent)
{

}

void BarDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	if (option.state & QStyle::State_Selected)
	{
		painter->fillRect(option.rect, option.palette.highlight());
	}


	int value = index.model()->data(index, Qt::DisplayRole).toInt();
	double factor = double(value) / 100.0;
	painter->save();

	if (factor > 0.8)
	{
		painter->setBrush(Qt::red);
	}
	else
	{
		painter->setBrush(QColor(0, int(factor * 255), 255 - int(factor * 255)));
	}

	painter->setPen(Qt::black);
	painter->drawRect(option.rect.x() + 2, option.rect.y() + 2, int(factor*(option.rect.width() - 5)), option.rect.height() - 5);

	QString strText = QString("%1/100").arg(value);
	painter->drawText(option.rect, Qt::AlignHCenter | Qt::AlignVCenter, strText);
	painter->restore();
}

QSize BarDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	return QSize(45, 15);
}
