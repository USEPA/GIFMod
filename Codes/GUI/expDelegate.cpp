#include "expdelegate.h"
#include "expeditor.h"
#include "statusviewer.h"
#include "ReactionTableModel.h"

expDelegate::expDelegate(QStringList words, StatusViewer* statusbar, QObject * p)
    : QItemDelegate(p)
{
    this->statusbar = statusbar;
	this->words = words;
}


QWidget *expDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &opt, const QModelIndex &mi) const
{
    expEditor *ee = new expEditor(words, statusbar, parent);
    return ee;
}


void expDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    expEditor *ee = static_cast<expEditor*>(editor);
    ee->setText(model->data(index, Qt::EditRole).toString());
}


void expDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    expEditor *ee = static_cast<expEditor*>(editor);
    model->setData(index, ee->text(), Qt::EditRole);
}

