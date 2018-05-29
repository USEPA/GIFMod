#pragma once
#include <QStringList>
#include <vector>
#include "qcolor.h"
#include "qrgb.h"
#include "BTC.h"
//#include "Medium.h"
#include "qgraphicsview.h"
#include "qgraphicsscene.h"
#include "qgraphicsitem.h"
#include "qmainwindow.h"

class GraphWidget;

using namespace std;

struct colorlegend
{
	vector<QColor> colors, legendColors;
	vector<QString> legendTexts;
	QGraphicsView * view = 0;
	QGraphicsScene * scene = 0;
	vector<QString> middleText;
	int x = 10, y = 10, dy = 20,
		columnOffset = 120, boxHeight = 20, boxWidth = 50;
	vector<	QGraphicsTextItem *> texts;
	vector<	QGraphicsRectItem *> boxes;

	vector<CBTC> data;
	vector<float> factors;
	vector<float> shifts;
	QString propertyName;
	double time = -1;
	QStringList nodeNames, edgeNames;
	QMainWindow* window = 0;
};

class colorScheme
{
public:
	colorScheme() {}
	~colorScheme(){}
	int numberofColors() {
		return colors.size();
	}
	vector<QColor> colors;
	void setColorTheme(QString theme) {
		colorTheme = theme;
	}
	QString currentColorTheme() {
		return colorTheme;
	}
	void setLogType(bool logType) {
		this->logType = logType;
	}
	QString currentLogType() {
		return (logType) ? "Logged" : "Normal";
	}
//	static vector<QColor> color(vector<CBTC> data, float t, QString theme = "Green", bool logType = false, int numberofGroups = 8, vector<double> factors = vector<double>(), vector<double> shifts = vector<double>());
	static colorlegend colorandLegend(colorlegend &colors, double t, QString theme = "Green", bool logType = false, int numberofGroups = 8);
	static QGraphicsView* showColorandLegend(colorlegend &legend, QString title = "", GraphWidget* gwidget = 0);
private:
	QString colorTheme;
	bool logType;
};
