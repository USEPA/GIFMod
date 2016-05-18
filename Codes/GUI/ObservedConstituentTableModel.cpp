#include "ObservedConstituentTableModel.h"
//#include "node.h"
#include "qdebug.h"
#include "GWidget.h"
#include "qtableview.h"
#include "entity.h"

ObservedConstituentTableModel::ObservedConstituentTableModel(QTableView *qTable, GraphWidget *gWidget)
{
//	this->node = node;
	this->gWidget = gWidget;
	this->qTable = qTable;
}
QVariant ObservedConstituentTableModel::data(const QModelIndex & index, int role) const
{
	if (!index.isValid())
		return QVariant();
	int row = index.row();
	int col = index.column();
	if (role == TypeRole)
	{
		if (col < 3) 
			return "ComboBox";
		QString particle = data(createIndex(index.row(), 1)).toString();
		if ((col == 2) && particle != "Soil" && particle != "Aqueous") 
			return "ComboBox";
		return QVariant();
	}

	if (role == DefaultValuesListRole)
	{
		if (col == 0) return gWidget->EntityNames("Constituent");
		if (col == 1)
		{
			QStringList r;
			r.append("Aqueous");
			r.append("Soil");
			r.append(gWidget->EntityNames("Particle"));
			return r;
		}
		if (col == 2)
		{
			QString particle = data(createIndex(row, 1)).toString();
			QStringList r;
			if (!(particle == "Soil" || particle == "Aqueous"))
			{
				QString pModel = gWidget->entityByName(particle)->getValue("Model");
				if (pModel.contains("Single")) r.append("Mobile");
				if (pModel.contains("Dual"))
				{
					r.append("Mobile");
					r.append("Attached");
				}
				if (pModel.contains("Triple"))
				{
					r.append("Mobile");
					r.append("Reversible attached");
					r.append("Irreversible attached");
				}
			}
			return r;
		}
	}
	if (role != Qt::EditRole && role != Qt::DisplayRole) return QVariant();
//	return QString("%1, %2").arg(row).arg(col);
	if (row == rowCount() - 1 && col == 0)
		return QVariant();// "Add...";
	if (row == rowCount()-1)
		return QVariant();
/*	if (col == 0)
		return node->constituentInitialCondition[row].Constituent;
	if (col == 1)
		return node->constituentInitialCondition[row].Particle;
	if (col == 2)
		return node->constituentInitialCondition[row].Model;
*/
	//	if (col == 3)
//		return node->constituentInitialCondition[row].Value;
}

bool ObservedConstituentTableModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	int row = index.row();
	int col = index.column();
	if (row < rowCount()-1)
	{
/*		if (col == 0)
		{
			node->constituentInitialCondition[row].Constituent = value.toString();
			if (data(createIndex(row, 1)).toString() == "Soil" || data(createIndex(row, 1)).toString() == "Aqueous")
				node->constituentInitialCondition[row].Model = "";
			else if (!data(createIndex(row, 2), DefaultValuesListRole).toStringList().contains(node->constituentInitialCondition[row].Model))
				node->constituentInitialCondition[row].Model = data(createIndex(row, 2), DefaultValuesListRole).toStringList()[0];
		}
		if (col == 1)
		{
			node->constituentInitialCondition[row].Particle = value.toString();
			if (value.toString() == "Soil" || value.toString() == "Aqueous")
				node->constituentInitialCondition[row].Model = "";
			else if (!data(createIndex(row, 2), DefaultValuesListRole).toStringList().contains(node->constituentInitialCondition[row].Model))
				node->constituentInitialCondition[row].Model = data(createIndex(row, 2), DefaultValuesListRole).toStringList()[0];
		}
		if (col == 2)
			node->constituentInitialCondition[row].Model = value.toString();
		if (col == 3)
			node->constituentInitialCondition[row].Value = value.toString();
*/		return true;
	}
	if (value.toString() == "Add..." || value.toString() == "") return false;
	ConstituentInitialConditionItem item;
	item.Constituent = value.toString();
	item.Particle = "Aqueous";
	item.Model = "";// data(value.toString(), DefaultValuesListRole).toStringList()[0];
	item.Value = "0";// value.toFloat();
	
	
	insertRows(rowCount(), 1);
//	node->constituentInitialCondition.append(item);
	return true;
}
bool ObservedConstituentTableModel::insertRows(int row, int count, const QModelIndex & parent)
{
	beginInsertRows(parent, row , row + count-1);
	endInsertRows();
	return true;
}
bool ObservedConstituentTableModel::removeRows(int firstRow, int lastRow, const QModelIndex & parent)
{
	beginRemoveRows(parent, firstRow, lastRow);
//	node->constituentInitialCondition.removeAt(firstRow);
	endRemoveRows();
	return true;
}
void ObservedConstituentTableModel::removeCurrentItem()
{
	if (rowCount() == 1) return;
	int row = qTable->currentIndex().row();
	if (row == rowCount() - 1) return;
	removeRows(row, row);// , createIndex(0, 0));
}

int ObservedConstituentTableModel::rowCount(const QModelIndex & index) const
{
	return 1;
/*	int size = node->constituentInitialCondition.count();
	static int rows;
	qDebug() << QString("size of Constituent initial conditions is %1").arg(size);

	return node->constituentInitialCondition.count()+1;
*/
}
