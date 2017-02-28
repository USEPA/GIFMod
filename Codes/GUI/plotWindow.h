#pragma once

#include <QMainWindow>
#include <QTimer>
#include "customplotzoom.h"
#include "qvector.h"
#include "BTCSet.h"
#include "qstring.h"
#include "qnamespace.h"
#include "qvariant.h"

class GraphWidget;
namespace Ui {
class plotWindow;
};

struct plotformat{
	Qt::GlobalColor color = Qt::red;
	QBrush brush = QBrush(QColor(240, 255, 200));
	QCPGraph *fillGraph = 0;
	QCPGraph::LineStyle lineStyle = QCPGraph::lsLine;
	QCPScatterStyle::ScatterShape scatterStyle = QCPScatterStyle::ssNone;// ssDisc;
	bool legend = true;
	Qt::PenStyle penStyle = Qt::DotLine;
	int penWidth = 2;
	QCPAxis::ScaleType xAxisType = QCPAxis::stLinear;
	QCPAxis::ScaleType yAxisType = QCPAxis::stLinear;
	QString xAxisLabel = "", yAxisLabel = "";
	bool xAxisTimeFormat = false;
	QMap<QString, int> lineStyles = QMap<QString, int>{ { "Line", QCPGraph::lsLine }, { "None", QCPGraph::lsNone } };
	QMap<QString, int> scatterStyles = QMap<QString, int>{ { "None", QCPScatterStyle::ssNone }, { "Cross", QCPScatterStyle::ssCross }, { "Cross Circle", QCPScatterStyle::ssCrossCircle }, { "Dot", QCPScatterStyle::ssDot } };
	QMap<QString, int> axisTypes = QMap<QString, int>{ { "Normal", QCPAxis::ScaleType::stLinear }, { "Log", QCPAxis::ScaleType::stLogarithmic } };
	QMap<QString, int> axisTimeFormats = QMap<QString, int>{ { "Number", 0 },{ "Time", 1 } };
	QMap<QString, int> penStyles = QMap<QString, int>{ { "Solid Line", Qt::SolidLine }, { "Dot Line", Qt::DotLine }, { "Dash Line", Qt::DashLine }, { "Dash Dot Line", Qt::DashDotLine }, { "Dash Dot Dot Line", Qt::DashDotDotLine } };
	QMap<QString, int> colors = QMap<QString, int> { { "Red", Qt::red }, { "Black", Qt::black }, { "Blue", Qt::blue }, { "Green", Qt::green }, { "Cyan", Qt::cyan }, { "Dark Green", Qt::darkGreen },
	{ "Dark Red", Qt::darkRed }, { "Dark Blue", Qt::darkBlue }, { "Dark Gray", Qt::darkGray }, { "Magenta", Qt::magenta }, { "Light Gray", Qt::lightGray }, { "Yellow", Qt::yellow } };
	QMap<QString, int> penWidths = QMap<QString, int>{ { "1", 1 }, { "2", 2 }, { "3", 3 }, { "4", 4 }, { "5", 5 } };
	QMap<QString, int> legends = QMap<QString, int>{ { "Show", 1 }, { "Hide", 0 } };

};

class plotWindow : public QMainWindow
{
  Q_OBJECT
  
public:
	explicit plotWindow(GraphWidget *parent = 0, QString windowTitle = "Plot Window", QString graphTitle = "");
//  explicit plotWindow(QWidget *parent, QVector<double> &x, QVector<double> &y);
  ~plotWindow();
//  QCPGraph* addScatterPlot(QString name, QVector<double> x, QVector<double> y, bool reformatX = true, plotformat format = plotformat());
  QCPGraph* addScatterPlot(QString name, QVector<double> x, QVector<double> y, plotformat format = plotformat());
  QCPGraph* addScatterPlot(QCPGraph *g, plotformat format);
  QCPGraph* addDotPlot(vector<double> &x, vector<double> &y, const QString &name, plotformat format = plotformat());
  QCPGraph* addScatterDotPlot(CBTCSet &ANS, int index, QString name = "", plotformat format = plotformat()) {
	  return addScatterPlot(ANS, index, name, 1, 0, format);
  }
  QCPGraph* addScatterPlot(CBTCSet &ANS, int index, QString name = "", double coefficient = 1, double offset = 0, plotformat format = plotformat()) {
	  if (name == "") name = QString::fromStdString(ANS.names[index]);
//	  std::vector<double> vx, std::vector<double> vy) {
	  QVector<double> x(ANS.BTC[index].n), y(ANS.BTC[index].n);
	  int numberOfTimePoints = ANS.BTC[index].t.size();
	  if (format.xAxisTimeFormat && (ANS.BTC[index].t[numberOfTimePoints - 1] - ANS.BTC[index].t[0]) < 5)
		  format.xAxisTimeFormat = false;
/*	  if (convertXtoTime)
		  for (int i = 0; i < ANS.BTC[index].n; i++)
		  {
			  //qint64 currentDate = ANS.BTC[index].t[i];
			  //currentDate += QDate(1900, 1, 1).toJulianDay();
			  //QDate date = QDate::fromJulianDay(currentDate);
			  //x[i] = date.toJulianDay();

			  x[i] = ANS.BTC[index].t[i] * 86400 - 2209161600;

			  y[i] = ANS.BTC[index].C[i] * coefficient + offset;
		  }
	  else*/ 
		  for (int i = 0; i < ANS.BTC[index].n; i++)
		  {
			  x[i] = ANS.BTC[index].t[i];
			  y[i] = ANS.BTC[index].C[i] * coefficient + offset;
		  }
	  //format.xAxisTimeFormat = convertXtoTime;
	  return addScatterPlot(name, x, y, format);
  };
  QCPGraph* plotWindow::addHistogramPlot(QString name, QVector<double> t, QVector<double> y, plotformat format = plotformat());
  QCPGraph* plotWindow::addHistogramPlot(QString name, vector<double> t, vector<double> y, plotformat format = plotformat()){
	  return addHistogramPlot(name, vec(t), vec(y), format);
  }
  QCPGraph* plotWindow::addPercentilePlot(QString name, QMap<QString, double> data, plotformat format = plotformat());
  QCPGraph* addScatterPlot(CBTC data, QString name, double coefficient = 1, double offset = 0, bool convertXtoTime = true, plotformat format = plotformat()) {
	  QVector<double> x(data.n), y(data.n);
//	  if (convertXtoTime)
//		  for (int i = 0; i < data.n; i++)
//		  {
//			  x[i] = data.t[i] * 86400 - 2209161600;
//			  y[i] = data.C[i] * coefficient + offset;
//		  }
//	  else
		  for (int i = 0; i < data.n; i++)
		  {
			  x[i] = data.t[i];
			  y[i] = data.C[i] * coefficient + offset;
		  }

	  format.xAxisTimeFormat = convertXtoTime;
	  return addScatterPlot(name, x, y, format);
  };
  QCPGraph* addScatterPlot(CBTC &ANS, QString name = "", plotformat format = plotformat()) {
	  //if (name == "") name = QString::fromStdString(ANS.names[index]);
	  QVector<double> x(ANS.n), y(ANS.n);
//	  int numberOfTimePoints = ANS.t.size();
//	  if (format.xAxisTimeFormat && (ANS.t[numberOfTimePoints - 1] - ANS.t[0]) < 5)
//		  format.xAxisTimeFormat = false;
	  for (int i = 0; i < ANS.n; i++)
	  {
//		  if (convertXtoTime)
//			  x[i] = ANS.t[i] * 86400 - 2209161600;
//		  else
			  x[i] = ANS.t[i];
		  y[i] = ANS.C[i];
	  }
	  return addScatterPlot(name, x, y, format);
  };
  double xtoTime(const double &x) {
	  return x * 86400 - 2209161600;
  };
  double timetoX(const double &time) {
	  return (time + 2209161600) / 86400;
  };

  vector<QCPGraph*> addScatterPlot(CBTCSet DataSet, plotformat format = plotformat()) {
	  vector<QCPGraph*> r;
	  format.legend = false;
	  format.penWidth = 1;
	  
	  for (int i = 0; i < DataSet.nvars; i++)
	  {
		  QString name = QString::fromStdString(DataSet[i].name);
		  if (name.toLower().contains("mean"))
		  {
			  format.legend = true;
			  format.penWidth = 2;
			  format.color = Qt::black;
			  format.penStyle = Qt::SolidLine;
		  }
		  if (name.toLower().contains("50"))
		  {
			  format.legend = true;
			  format.penWidth = 1;
			  format.color = Qt::blue;
			  format.penStyle = Qt::DashDotLine;
		  }
		  if (name.toLower().contains("97.5"))
		  {
			  format.legend = true;
			  name = "97.5%";
			  format.penWidth = 1;
			  format.color = Qt::red;
			  format.penStyle = Qt::SolidLine;
		  }
		  if (name.toLower().contains("2.5"))
		  {
			  format.legend = true;
			  name = "2.5%";
			  format.penWidth = 1;
			  format.color = Qt::darkRed;
			  format.penStyle = Qt::SolidLine;
		  }
		  DataSet[i].name = name.toStdString();
		  r.push_back(addScatterPlot(DataSet[i], QString::fromStdString(DataSet[i].name), format));
	  }
	  return r;
  }

  void setupDemo(int demoIndex);
  void setupQuadraticDemo(CustomPlotZoom *customPlot);
  void setupSimpleDemo(CustomPlotZoom *customPlot);
  void setupSincScatterDemo(CustomPlotZoom *customPlot);
  void setupScatterStyleDemo(CustomPlotZoom *customPlot);
  void setupLineStyleDemo(CustomPlotZoom *customPlot);
  void setupScatterPixmapDemo(CustomPlotZoom *customPlot);
  void setupDateDemo(CustomPlotZoom *customPlot);
  void setupTextureBrushDemo(CustomPlotZoom *customPlot);
  void setupMultiAxisDemo(CustomPlotZoom *customPlot);
  void setupLogarithmicDemo(CustomPlotZoom *customPlot);
  void setupRealtimeDataDemo(CustomPlotZoom *customPlot);
  void setupParametricCurveDemo(CustomPlotZoom *customPlot);
  void setupBarChartDemo(CustomPlotZoom *customPlot);
  void setupStatisticalDemo(CustomPlotZoom *customPlot);
  void setupSimpleItemDemo(CustomPlotZoom *customPlot);
  void setupItemDemo(CustomPlotZoom *customPlot);
  void setupStyledDemo(CustomPlotZoom *customPlot);
  void setupAdvancedAxesDemo(CustomPlotZoom *customPlot);
  void setupColorMapDemo(CustomPlotZoom *customPlot);
  void setupFinancialDemo(CustomPlotZoom *customPlot);
  
  void setupPlayground(CustomPlotZoom *customPlot);
  GraphWidget* parent;
  vector<plotformat> format, previousFormat;
  void refreshFormat();
private slots:
  void realtimeDataSlot();
  void bracketDataSlot();
  void screenShot();
  void allScreenShots();
  void contextMenuEvent(QContextMenuEvent *event);
   void showMenu(QMouseEvent *event);
 
private:
  Ui::plotWindow *ui;
  QString demoName;
  QTimer dataTimer;
  QCPItemTracer *itemDemoPhaseTracer;
  int currentDemoIndex;
  bool rightClickInitialized = false;
  QVector<double> vec(vector<double> v){
	  QVector<double> r(v.size());
	  for (int i = 0; i < v.size(); i++)
		  r[i] = v[i];
	  return r;
  }
};

QList<QAction *> subActions(const QMap<QString, int> &list, const int &value, QMenu * menuItem, int graphIndex, QVariant val, bool enabled = true);