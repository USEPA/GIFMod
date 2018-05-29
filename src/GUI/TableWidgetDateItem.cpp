#include "TableWidgetDateItem.h"
#include "qdebug.h"
#include "qdatetime.h"
#include "utility_funcs.h"
//#include <QTime.h>

void TableWidgetDateItem::setData(int role, const QVariant & value)
{
	if (role == Qt::EditRole)
	{

		QString text = value.toString().trimmed();
		
		QList<QChar> replaceItems;
		replaceItems << '\\' << '-' << ',';
        foreach (QChar c , replaceItems)
			text.replace(c, "/");

		QString date = "01/01/1900";
		QString time = "00:00:00";
		double floatDate;

		if (text.contains("/") || text.contains(":"))
		{
			date = text.split(" ")[0];
			if (text.split(" ").size() > 1)
				time = text.split(" ")[1];

			mm = "01"; dd = "01"; yyyy = "1900";
			mm = date.split("/")[0];
			if (date.split("/").size() > 1)
				dd = date.split("/")[1];
			if (date.split("/").size() > 2)
				yyyy = date.split("/")[2];
			if (yyyy.toInt() < 0)
				yyyy = 1900;
			if (yyyy.toInt() < 50)
				yyyy = yyyy.toInt() + 2000;
			if (yyyy.toInt() > 50 && yyyy.toInt() < 100)
				yyyy = yyyy.toInt() + 1900;
			if (yyyy.toInt() < 1900 || yyyy.toInt() > 2100)
				yyyy = 1900;

			HH = "00"; MM = "00"; SS = "00";
			HH = time.split(":")[0];
			if (time.split(":").size() > 1)
				MM = time.split(":")[1];
			if (time.split(":").size() > 2)
				SS = time.split(":")[2];

			//		formattedDate = QString("%1/%2/%3 %4:%5:%6").arg(mm).arg(dd).arg(yyyy).arg(HH).arg(MM).arg(SS);

			QDateTime qdatetime;
			qdatetime = QDateTime(QDate(yyyy.toInt(), mm.toInt(), dd.toInt()), QTime(HH.toInt(), MM.toInt(), SS.toInt()));

			QTime qtime = qdatetime.time();
			float fraction = timetodayfraction(qtime.hour(), qtime.minute(), qtime.second());
			//		float fraction = timetodayfraction(HH.toInt(), MM.toInt(), SS.toInt());
			double juliandate = qdatetime.date().toJulianDay();
			juliandate += fraction;
			floatDate = julian2xldate(juliandate);
		}
		else
		{
			floatDate = text.toDouble();
			if (floatDate <= 0)
				floatDate = 0;
		}


		//		formattedDate = qdatetime.toString("MM/dd/yyyy hh:mm:ss");
		formattedDate = float2datetime(floatDate);
		yyyy = float2datetime(floatDate, "yyyy");
		this->date = floatDate;
	}
	
}

QVariant TableWidgetDateItem::data(int role) const
{
	if (role == 1000)
	{
		return date;
	}
	if (date == -1)
	{
		if (role == Qt::DisplayRole)
			return "MM/DD/YYYY hh:mm:ss or decimal";
		if (role == Qt::TextColorRole)
			return QColor(Qt::GlobalColor::gray);
		if (role == Qt::ToolTipRole)
			return "Enter date/time in one of these formats:\nMM/DD/YYYY hh:mm:ss\nMM/DD/YY hh:mm:ss\nMM/DD/YYYY\nMM/DD/YY\nordecimal number to represent the date and time.";

	}
	if (role == Qt::DisplayRole)
	{
		if (yyyy.toInt() > 1950)
			return formattedDate;
		else
			return date;
	}
	if (role == Qt::EditRole)
	{
		return formattedDate;
	}

	return QVariant();
}
