#ifdef GIFMOD

#include "reactiontablewidget.h"
#include <QStringList>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableView>
#include <QtAlgorithms>
#include <QPushButton>
#include <QDebug>
#include "expressioneditor.h"
#include "expressiondelegate.h"
#include "reactiontablemodel.h"
#include "freezetablewidget.h"
#include "statusviewer.h"
#include "GWidget.h"

struct ReactionTableWidgetPri
{
    QStringList Constituents, Parameters, Functions, Physical;
    ReactionTableModel* Model;
    FreezeTableWidget* View;
    QList<QStringList> ConvertedExpression;

    StatusViewer* Statusbar;
};

ReactionTableWidget::ReactionTableWidget(GraphWidget* mainGraphWidget, const QStringList& constituents, const QStringList& parameters,
	const QStringList& functions, const QStringList& physical, const QList<QStringList>& data, QWidget* parent) : QWidget(parent), d(new ReactionTableWidgetPri)
{
	d->Constituents = constituents; d->Parameters = parameters; d->Functions = functions; d->Physical = physical;
    d->Statusbar = new StatusViewer;

    d->Model = new ReactionTableModel(mainGraphWidget, d->Constituents, d->Parameters, d->Functions, d->Physical, data, this);
    d->View = new FreezeTableWidget(d->Model);
    d->View->setColumnWidth(0, 200);
    d->View->setSelectionMode(QAbstractItemView::SingleSelection);
    d->View->frozenView()->setSelectionMode(QAbstractItemView::SingleSelection);
	d->View->setEditTriggers(QAbstractItemView::AllEditTriggers | QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked);


    ExpressionDelegate* delegate = new ExpressionDelegate(d->Model, d->Statusbar, d->View);
    d->View->frozenView()->setItemDelegateForColumn(1, delegate);
    for (int i = 1; i < d->Model->columnCount(); i++) {
        d->View->setItemDelegateForColumn(i, delegate);
    }
    d->View->frozenView()->setColumnWidth(0, d->View->columnWidth(0));
    d->View->frozenView()->setColumnWidth(1, d->View->columnWidth(1));

    QVBoxLayout* vbox = new QVBoxLayout(this);
    vbox->addWidget(d->View);    

    QPushButton *btnAddRow = new QPushButton("Add Process");
    connect(btnAddRow, SIGNAL(clicked(bool)), SLOT(onAddProcessClicked()));
    QPushButton *btnRemoveRow = new QPushButton("Remove Process");
    connect(btnRemoveRow, SIGNAL(clicked(bool)), SLOT(onRemoveProcessClicked()));
/*    QPushButton *btnOk = new QPushButton("OK");
    connect(btnOk, SIGNAL(clicked(bool)), SLOT(onOkClicked()));*/
    QHBoxLayout* hbox = new QHBoxLayout;
    hbox->addWidget(btnAddRow);hbox->addWidget(btnRemoveRow);
    hbox->addWidget(d->Statusbar, 1); //hbox->addWidget(btnOk);
    vbox->addLayout(hbox);

    connect(d->Model, SIGNAL(invalidExpressoinDetected(QString)), SLOT(onInvalidExpresstionDetected(QString)));
}


ReactionTableWidget::~ReactionTableWidget() {delete d;}


void ReactionTableWidget::onAddProcessClicked()
{
    d->Model->addItem(-1, true);
}


void ReactionTableWidget::onRemoveProcessClicked()
{
    QList<int> rows;
    QModelIndex index = d->View->selectionModel()->selectedIndexes().first();
	QString name = index.sibling(index.row(), 0).data().toString();
	d->Model->removeRow(name, index.row());
	/*
    for (int i = 0; i < index.length(); i++) {
        QModelIndex mi = index.at(i);
		QStrigng name = index.sibling.data
        if (mi.isValid() && mi.row() >= 0) {
            if (!rows.contains(mi.row())) {
                rows.append(mi.row());
            }
        }
    }

    qSort(rows.begin(), rows.end());

    for (int i = rows.length() - 1; i >= 0; i--) {
        d->Model->removeItem(rows[i]);
    }*/
}


void ReactionTableWidget::onOkClicked()
{
	hide();
	d->ConvertedExpression = d->Model->exportToExpession();
    emit expressionConverted(d->ConvertedExpression);
}


QList<QStringList> ReactionTableWidget::convertedExpression() const {return d->ConvertedExpression;}

void ReactionTableWidget::onInvalidExpresstionDetected(const QString &msg)
{
    d->Statusbar->showError(msg, 5);
}
#endif
