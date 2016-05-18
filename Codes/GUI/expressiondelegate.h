#ifndef EXPRESSIONDELEGATE_H
#define EXPRESSIONDELEGATE_H

#include <QItemDelegate>

class StatusViewer;
class ReactionTableModel;

class ExpressionDelegate : public QItemDelegate
{
public:
    ExpressionDelegate(ReactionTableModel* model, StatusViewer* statusbar, QObject * p = 0);

    QWidget* createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const;
    void setEditorData(QWidget * editor, const QModelIndex & index) const;
    void setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const;

private:
    ReactionTableModel* model;
    StatusViewer* statusbar;
};

#endif // EXPRESSIONDELEGATE_H
