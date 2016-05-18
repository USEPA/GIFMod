#pragma once

#include <QStyledItemDelegate>
#include <mProplist.h>
class GraphWidget;
class Node;
//! [0]
class ParticleDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:

	ParticleDelegate(QObject *parent, GraphWidget* gWidget) : QStyledItemDelegate(parent) {
		this->parent = gWidget; // static_cast<GraphWidget*> (parent);
	}

/*	void paint(QPainter *painter, const QStyleOptionViewItem &option,
		const QModelIndex &index) const Q_DECL_OVERRIDE;
	QSize sizeHint(const QStyleOptionViewItem &option,
		const QModelIndex &index) const Q_DECL_OVERRIDE;
*/	
	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const Q_DECL_OVERRIDE;

    void setEditorData(QWidget *editor, const QModelIndex &index) const Q_DECL_OVERRIDE;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const Q_DECL_OVERRIDE;

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

	void ParticleDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
		const QModelIndex &index) const;
	
	GraphWidget* parent;

public slots:


private:
	
};
