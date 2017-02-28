#pragma once
#include "qabstractitemmodel.h"
#include "QString.h"
//#include "node.h"
//#include "GWidget.h"

class Entity;
class GraphWidget;
class QTableView;

class ExchangeParametersTableModel:	public QAbstractTableModel
{
public:
	ExchangeParametersTableModel(Entity *entity, QTableView *qTable, GraphWidget *gWidget);
	QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
	bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
	int rowCount(const QModelIndex & index = QModelIndex()) const;
	int columnCount(const QModelIndex & parent = QModelIndex()) const { 
		return 3; };
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const{
		if (role == Qt::DisplayRole)
		{
			if (orientation == Qt::Horizontal) {
				switch (section)
				{
				case 0:
					return QString("Particle/Solid phase");
				case 1:
					return QString("Exchange rate");
				case 2:
					return QString("Partitioning coefficient");
				}
			}
		}
		return QVariant();
	};
	Qt::ItemFlags flags(const QModelIndex & index) const{
		int row = index.row();
		int col = index.column();
		if (row == rowCount() - 1 && col > 0)return 0;
		return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
	};
	~ExchangeParametersTableModel(){};
	bool insertRows(int row, int count, const QModelIndex & parent = QModelIndex());
	bool removeRows(int firstRow, int lastRow, const QModelIndex & parent = QModelIndex());
	void removeCurrentItem();
public slots:

private:
	GraphWidget *gWidget;
	Entity *entity;
	QTableView *qTable;
	bool addingRow = false;
};

