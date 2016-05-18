#include "ExchangeParametersTableModel.h"
#include "node.h"
#include "qdebug.h"
#include "GWidget.h"
#include "qtableview.h"
#include "entity.h"

ExchangeParametersTableModel::ExchangeParametersTableModel(Entity *entity, QTableView *qTable, GraphWidget *gWidget)
{
	this->entity = entity;
	this->gWidget = gWidget;
	this->qTable = qTable;
}
QVariant ExchangeParametersTableModel::data(const QModelIndex & index, int role) const
{
	if (!index.isValid())
		return QVariant();
	int row = index.row();
	int col = index.column();
	qDebug() << row << col << role;
	if (row == 1 && col == 2 && role == 8)
		int i = 0;
	if (role == TypeRole)
		if (col < 1) return "ComboBox";
	if (role == DefaultValuesListRole)
	{
		if (col == 0) return QStringList() << "Soil" << gWidget->EntityNames("Particle");
	}
	if (role != Qt::EditRole && role != Qt::DisplayRole) return QVariant();

	if (row == rowCount()-1 && col == 0)
		return "Add...";
	if (row == rowCount()-1)
		return QVariant();
	if (col == 0)
		return entity->solidAqueousExchangeParameters[row].Particle;
	if (col == 1)
		return entity->solidAqueousExchangeParameters[row].ExchangeRate;
	if (col == 2)
		return entity->solidAqueousExchangeParameters[row].PartitioningCoefficient;
	qDebug() << "returned nothing.";
	return QVariant();
}

bool ExchangeParametersTableModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	static int rows;
	int row = index.row();
	int col = index.column();
	if (row < rowCount() - 1)
	{
		if (col == 0)
			entity->solidAqueousExchangeParameters[row].Particle = value.toString();
		if (col == 1)
			entity->solidAqueousExchangeParameters[row].ExchangeRate = value.toString();
		if (col == 2)
			entity->solidAqueousExchangeParameters[row].PartitioningCoefficient = value.toString();
		return true;
	}
	if (value.toString() == "Add..." || value.toString() == "") return false;
	SolidAqueousExchangeParameterItem item;
	item.Particle = value.toString();
	item.ExchangeRate = "1";// data(value.toString(), DefaultValuesListRole).toStringList()[0];
	item.PartitioningCoefficient = "0";

	//	insertRows(rowCount(), 1);
	entity->solidAqueousExchangeParameters.append(item);
	insertRows(rowCount(), 1);
	gWidget->log(QString("Solid aqueous exchange parameters (%1, %2, %3) added to constituent: %4.").arg(item.Particle).arg(item.ExchangeRate).arg(item.PartitioningCoefficient).arg(entity->Name()));
	return true;

}
bool ExchangeParametersTableModel::insertRows(int row, int count, const QModelIndex & parent)
{
	beginInsertRows(parent, row , row + count-1);
	endInsertRows();
	return true;
}
bool ExchangeParametersTableModel::removeRows(int firstRow, int lastRow, const QModelIndex & parent)
{
	beginRemoveRows(parent, firstRow, lastRow);
	entity->solidAqueousExchangeParameters.removeAt(firstRow);
	endRemoveRows();
	return true;
}
void ExchangeParametersTableModel::removeCurrentItem()
{
	if (rowCount() == 1) return;
	int row = qTable->currentIndex().row();
	if (row == rowCount() - 1) return;
	removeRows(row, row);
}

int ExchangeParametersTableModel::rowCount(const QModelIndex & index) const
{
	return entity->solidAqueousExchangeParameters.count() + 1;
}
