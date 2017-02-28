#pragma once

#include <QMainWindow>
#include <QTimer>
#include "qcustomplot.h"
#include "qvector.h"
#include "BTCSet.h"
#include "qstring.h"
#include "plotWindow.h"
class CMedium;
class GraphWidget;
namespace Ui {
class runtimeWindow;
}

class runtimeWindow : public QMainWindow
{
  Q_OBJECT
  
public:
  explicit runtimeWindow(GraphWidget *parent = 0, QString mode = "forward");
//  explicit runtimeWindow(QWidget *parent, QVector<double> &x, QVector<double> &y);
  ~runtimeWindow();
  void setMode(QString mode);
  void setLabel(QString label);
  void setExperiment(CMedium *model);
  void addScatterPlot(QString name, QVector<double> x, QVector<double> y);
  void addScatterPlot(QCPGraph *g);
  void addScatterDotPlot(CBTCSet &ANS, int index, QString name = "") {
	  addScatterPlot(ANS, index, name);
  }
  void addScatterPlot(CBTCSet &ANS, int index, QString name = "") {
	  if (name == "") name = QString::fromStdString(ANS.names[index]);
//	  std::vector<double> vx, std::vector<double> vy) {
	  QVector<double> x(ANS.maxnumpoints()), y(ANS.maxnumpoints());
	  for (int i = 0; i < ANS.maxnumpoints(); i++)
	  {
		  x[i] = ANS.BTC[index].t[i] * 86400 - 2209161600;
		  y[i] = ANS.BTC[index].C[i];
	  }
	  addScatterPlot(name, x, y);
  };
  void update(QMap<QString, QVariant> &vars);
  void setupDemo(int demoIndex);
  void setupQuadraticDemo(QCustomPlot *customPlot);
  void setupSimpleDemo(QCustomPlot *customPlot);
  void setupSincScatterDemo(QCustomPlot *customPlot);
  void setupScatterStyleDemo(QCustomPlot *customPlot);
  void setupLineStyleDemo(QCustomPlot *customPlot);
  void setupScatterPixmapDemo(QCustomPlot *customPlot);
  void setupDateDemo(QCustomPlot *customPlot);
  void setupTextureBrushDemo(QCustomPlot *customPlot);
  void setupMultiAxisDemo(QCustomPlot *customPlot);
  void setupLogarithmicDemo(QCustomPlot *customPlot);
  void setupRealtimeDataDemo(QCustomPlot *customPlot);
  void setupParametricCurveDemo(QCustomPlot *customPlot);
  void setupBarChartDemo(QCustomPlot *customPlot);
  void setupStatisticalDemo(QCustomPlot *customPlot);
  void setupSimpleItemDemo(QCustomPlot *customPlot);
  void setupItemDemo(QCustomPlot *customPlot);
  void setupStyledDemo(QCustomPlot *customPlot);
  void setupAdvancedAxesDemo(QCustomPlot *customPlot);
  void setupColorMapDemo(QCustomPlot *customPlot);
  void setupFinancialDemo(QCustomPlot *customPlot);
  
  void setupPlayground(QCustomPlot *customPlot);
  GraphWidget* parent;
  bool stopTriggered = false;
  CMedium* experiment = 0; //Medium
  Ui::runtimeWindow *ui;
private slots:
	
	void on_btnStop_clicked();
	void on_button1_clicked(){
		int i = 0;
	};

  void realtimeDataSlot(double x, double y, bool secondPlot = false, QString chartName = "chart", bool commingBackX = false);
  void bracketDataSlot();
  void screenShot();
  void allScreenShots();
  void contextMenuEvent(QContextMenuEvent *event);
  void showMenu(QMouseEvent *event);

private:
  QString demoName;
  QTimer dataTimer;
  QCPItemTracer *itemDemoPhaseTracer;
  int currentDemoIndex;
  QTime start;
  bool useStartFinishTimes = false;
};

