#include "ConstituentTableModelPlant.h"
#include "node.h"
#include "qdebug.h"
#include "GWidget.h"
#include "qtableview.h"
#include "entity.h"

ConstituentTableModelPlant::ConstituentTableModelPlant(Node *node, QTableView *qTable, GraphWidget *gWidget)
{
	this->node = node;
	this->gWidget = gWidget;
	this->qTable = qTable;
}
QVariant ConstituentTableModelPlant::data(const QModelIndex & index, int role) const
{
	if (!index.isValid())
		return QVariant();
	int row = index.row();
	int col = index.column();
	if (role == TypeRole)
	{
		if (col < 1) 
			return "ComboBox";
		
		return QVariant();
	}

	if (role == DefaultValuesListRole)
	{
		if (col == 0) return gWidget->EntityNames("Constituent");
		
		
		
	}
	if (role != Qt::EditRole && role != Qt::DisplayRole) return QVariant();
//	return QString("%1, %2").arg(row).arg(col);
	if (row == rowCount()-1 && col == 0)
		return "Add...";
	if (row == rowCount()-1)
		return QVariant();
	if (col == 0)
		return node->NutrientHalfSaturationConstant()[row].Constituent;
	if (col == 1)
		return node->NutrientHalfSaturationConstant()[row].Value;
}

bool ConstituentTableModelPlant::setData(const QModelIndex & index, const QVariant & value, int role)
{
	int row = index.row();
	int col = index.column();
	if (row < rowCount() - 1)
	{
		QStringList experimentstobeUpdated;
		experimentstobeUpdated.append(node->experimentName());
		if (node->experimentName() == "All experiments")
			experimentstobeUpdated = node->parent->experimentsList();

		if (col == 0)
		{
            foreach (QString experiment , experimentstobeUpdated)
				node->NutrientHalfSaturationConstant(experiment)[row].Constituent = value.toString();
			
		}
		
		if (col == 1)
            foreach (QString experiment , experimentstobeUpdated)
				node->NutrientHalfSaturationConstant(experiment)[row].Value = value.toString();

		return true;
	}
	if (value.toString() == "Add..." || value.toString() == "") return false;
	NutrientHalfSaturationConstantItem item;
	item.Constituent = value.toString();
	item.Value = "0";// value.toFloat();
	
	
//	node->constituentInitialCondition().append(item);
	insertRows(rowCount(), 1);
	if (node->experimentName() != "All experiments")
	{
		node->NutrientHalfSaturationConstant().append(item);
		gWidget->log(QString("Nutrient Half Saturation constant (%1, %2) added to block: %3 for experiment %6.").arg(item.Constituent).arg(item.Value).arg(node->Name()).arg(node->experimentName()));
		return true;
	}
    foreach(QString experiment , node->parent->experimentsList())
		node->NutrientHalfSaturationConstant(experiment).append(item);
	gWidget->log(QString("Nutrient Half Saturation Constant (%1, %2) added to block: %5 for all experiments.").arg(item.Value).arg(node->Name()));
	return true;
}
bool ConstituentTableModelPlant::insertRows(int row, int count, const QModelIndex & parent)
{
	beginInsertRows(parent, row , row + count-1);
	endInsertRows();
	return true;
}
bool ConstituentTableModelPlant::removeRows(int firstRow, int lastRow, const QModelIndex & parent)
{
	if (firstRow == -1 || lastRow == -1) return false;
	beginRemoveRows(parent, firstRow, lastRow);
	if (node->experimentName() == "All experiments")
        foreach(QString experiment , node->parent->experimentsList())
			node->NutrientHalfSaturationConstant(experiment).removeAt(firstRow);
	else
		node->NutrientHalfSaturationConstant().removeAt(firstRow);
	endRemoveRows();
	return true;
}
void ConstituentTableModelPlant::removeCurrentItem()
{
	if (rowCount() == 1) return;
	int row = qTable->currentIndex().row();
	if (row == rowCount() - 1) return;
	removeRows(row, row);// , createIndex(0, 0));
}

int ConstituentTableModelPlant::rowCount(const QModelIndex & index) const
{
	return node->NutrientHalfSaturationConstant().count()+1;
}
