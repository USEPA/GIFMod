#ifdef GIFMOD
#ifndef REACTIONTABLEWIDGET_H
#define REACTIONTABLEWIDGET_H

#include <QWidget>
class GraphWidget;

struct ReactionTableWidgetPri;

class ReactionTableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ReactionTableWidget(GraphWidget *gw, const QStringList& constituents, const QStringList& parameters,
		const QStringList& functions, const QStringList& physical, const QList<QStringList>& data = QList<QStringList>(), QWidget* p = 0);
    ~ReactionTableWidget();

    QList<QStringList> convertedExpression() const;

signals:
    void expressionConverted(const QList<QStringList>& data);

private slots:
    void onAddProcessClicked();
    void onRemoveProcessClicked();
    void onOkClicked();
    void onInvalidExpresstionDetected(const QString& msg);

private:
    ReactionTableWidgetPri* d;
};

#endif // REACTIONTABLEWIDGET_H
#endif