#include "expressiondelegate.h"
#include "expressioneditor.h"
#include "statusviewer.h"
#include "ReactionTableModel.h"

ExpressionDelegate::ExpressionDelegate(ReactionTableModel* model, StatusViewer* statusbar, QObject * p)
    : QItemDelegate(p)
{
    this->statusbar = statusbar;
    this->model = model;
}


QWidget *ExpressionDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &opt, const QModelIndex &mi) const
{
    ExpressionEditor *ee = new ExpressionEditor(model, statusbar, parent);
    return ee;
}


void ExpressionDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    ExpressionEditor *ee = static_cast<ExpressionEditor*>(editor);
    ee->setText(model->data(index, Qt::EditRole).toString());
}


void ExpressionDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    ExpressionEditor *ee = static_cast<ExpressionEditor*>(editor);
    model->setData(index, ee->text(), Qt::EditRole);
}

