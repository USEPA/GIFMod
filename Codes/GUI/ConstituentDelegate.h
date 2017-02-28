#ifdef GIFMOD
#pragma once

#include <QStyledItemDelegate>
#include <mProplist.h>
class GraphWidget;
class Node;

class ConstituentDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:

	ConstituentDelegate(QObject *parent, GraphWidget* gWidget) : QStyledItemDelegate(parent) {
		this->parent = gWidget; // static_cast<GraphWidget*> (parent);
	}

	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const Q_DECL_OVERRIDE;

    void setEditorData(QWidget *editor, const QModelIndex &index) const Q_DECL_OVERRIDE;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const Q_DECL_OVERRIDE;

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

	GraphWidget* parent;

};
#endif