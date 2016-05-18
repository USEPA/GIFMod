#pragma once

#include <QStyledItemDelegate>
#include <mProplist.h>
#include "qcombobox.h"
#include "qfiledialog.h"
#include <QCalendarWidget>

class GraphWidget;
class Node;
//! [0]
class Delegate : public QStyledItemDelegate
{
    Q_OBJECT

public:

	Delegate(QObject *parent, GraphWidget* gWidget) : QStyledItemDelegate(parent) {
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

	void Delegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
		const QModelIndex &index) const;
	GraphWidget* parent;

public slots:
	void browserClicked();
	void dirBrowserClicked();
	void openParticleInitialCondition();
	void openConstituentInitialCondition();
	void openAqueousExchangeParameters();
	void browserCheck(QString);// const;
//	void datePicked(QWidget *editor = 0, QModelIndex index = QModelIndex());

private:
	QString browserFileName;
	QModelIndex *tempIndex;
	QFileDialog *fileDialog;
	QStyleOptionViewItem option;
};
