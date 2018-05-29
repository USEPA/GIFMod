#pragma once
#include "qtablewidget.h"
class TableWidgetDateItem :
	public QTableWidgetItem
{
public:
	TableWidgetDateItem(const QString & text, int type = Type) :QTableWidgetItem(text, type) {
		setData(Qt::EditRole, text);
	};
	TableWidgetDateItem(int type = Type) :QTableWidgetItem("", type) {};
	virtual ~TableWidgetDateItem() {};

	void setData(int role, const QVariant & value);
	QVariant data(int role) const;

private:
	QString formattedDate = "01/01/1900 00:00:00";
	double date = -1;
	QString mm = "01", dd = "01", yyyy = "1900";
	QString HH = "00", MM = "00", SS = "00";

};

