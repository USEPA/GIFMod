#ifdef GIFMOD
#include "reactionwindow.h"
#include <QStringList>
#include "reactiontablewidget.h"
#include "GWidget.h"



ReactionWindow::ReactionWindow(QWidget *parent) : QMainWindow(parent), d(new ReactionWindowPri)
{   
	GraphWidget *mainGraphWidget; 
    ReactionTableWidget* rtw = new ReactionTableWidget(mainGraphWidget, d->Constituents, d->Parameters, d->Functions, d->Physical);
    setCentralWidget(rtw);
    resize( 800, 200);

    connect(rtw, SIGNAL(expressionConverted(QList<QStringList>)), SLOT(onExpressionConverted(QList<QStringList>)));
}
ReactionWindow::ReactionWindow(GraphWidget *gWidget, QWidget *parent) : QMainWindow(parent), d(new ReactionWindowPri)
{
	mainGraphWidget = gWidget;
	d->Constituents = gWidget->EntityNames("Constituent");
	d->Parameters = gWidget->EntityNames("Reaction parameter");
	d->Physical = gWidget->PhysicalCharacteristicsList;
	d->Functions = gWidget->functionList;
	ReactionTableWidget* rtw = new ReactionTableWidget(gWidget, d->Constituents, d->Parameters, d->Functions, d->Physical);
	setCentralWidget(rtw);
	resize(800, 200);

	connect(rtw, SIGNAL(expressionConverted(QList<QStringList>)), SLOT(onExpressionConverted(QList<QStringList>)));
}


ReactionWindow::~ReactionWindow() {delete d;}


void ReactionWindow::onExpressionConverted(const QList<QStringList>& data)
{
    ReactionTableWidget* rtw = new ReactionTableWidget(mainGraphWidget, d->Constituents, d->Parameters, d->Functions, d->Physical, data);
    rtw->setAttribute(Qt::WA_DeleteOnClose);
    rtw->show();
}

#endif
