#ifdef GIFMOD
#ifndef REACTIONWINDOW_H
#define REACTIONWINDOW_H

#include <QMainWindow>
class GraphWidget;

struct ReactionWindowPri;

class ReactionWindow : public QMainWindow
{
    Q_OBJECT
public:
	explicit ReactionWindow(QWidget *parent = 0);
	explicit ReactionWindow(GraphWidget *gWidget, QWidget *parent = 0);
	~ReactionWindow();
	GraphWidget *mainGraphWidget;

private slots:
    void onExpressionConverted(const QList<QStringList>& data);

private:
    ReactionWindowPri* d;
};

#endif // REACTIONWINDOW_H
#endif