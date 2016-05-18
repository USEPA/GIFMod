#include "ConstituentTableModel.h"
#include "node.h"
#include "qdebug.h"
#include "GWidget.h"
#include "qtableview.h"
#include "entity.h"

ConstituentTableModel::ConstituentTableModel(Node *node, QTableView *qTable, GraphWidget *gWidget)
{
	this->node = node;
	this->gWidget = gWidget;
	this->qTable = qTable;
}
QVariant ConstituentTableModel::data(const QModelIndex & index, int role) const
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
	if (row == rowCount()-1 && col == 0)
		return "Add...";
	if (row == rowCount()-1)
		return QVariant();
	if (col == 0)
		return node->constituentInitialCondition()[row].Constituent;
	if (col == 1)
		return node->constituentInitialCondition()[row].Particle;
	if (col == 2)
		return node->constituentInitialCondition()[row].Model;
	if (col == 3)
		return node->constituentInitialCondition()[row].Value;
}

bool ConstituentTableModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	int row = index.row();
	int col = index.column();
	if (row < rowCount() - 1)
	{
		QStringList experimentstobeUpdated;
		experimentstobeUpdated.append(node->experimentName());
		if (node->experimentName() == "Global")
			experimentstobeUpdated.append(node->parent->experimentsList());

		if (col == 0)
		{
			for each (QString experiment in experimentstobeUpdated)
				node->constituentInitialCondition(experiment)[row].Constituent = value.toString();
			for each (QString experiment in experimentstobeUpdated)
			{
				if (data(createIndex(row, 1)).toString() == "Soil" || data(createIndex(row, 1)).toString() == "Aqueous")
					node->constituentInitialCondition(experiment)[row].Model = "";
				else if (!data(createIndex(row, 2), DefaultValuesListRole).toStringList().contains(node->constituentInitialCondition(experiment)[row].Model))
					node->constituentInitialCondition(experiment)[row].Model = data(createIndex(row, 2), DefaultValuesListRole).toStringList()[0];
			}
		}
		if (col == 1)
		{
			for each (QString experiment in experimentstobeUpdated)
				node->constituentInitialCondition(experiment)[row].Particle = value.toString();
			for each (QString experiment in experimentstobeUpdated)
			{
				
				if (value.toString() == "Soil" || value.toString() == "Aqueous")
					node->constituentInitialCondition(experiment)[row].Model = "";
				else
				{
					QString modelValue = node->constituentInitialCondition(experiment)[row].Model;
					QStringList modelDafaultValues = data(createIndex(row, 2), DefaultValuesListRole).toStringList();
					if (!modelDafaultValues.contains(modelValue))
						for each (QString experiment in experimentstobeUpdated)
							node->constituentInitialCondition(experiment)[row].Model = modelDafaultValues[0];
				}
			}
		}
		if (col == 2)
			for each (QString experiment in experimentstobeUpdated)
				node->constituentInitialCondition(experiment)[row].Model = value.toString();
		if (col == 3)
			for each (QString experiment in experimentstobeUpdated)
				node->constituentInitialCondition(experiment)[row].Value = value.toString();

		return true;
	}
	if (value.toString() == "Add..." || value.toString() == "") return false;
	ConstituentInitialConditionItem item;
	item.Constituent = value.toString();
	item.Particle = "Aqueous";
	item.Model = "";// data(value.toString(), DefaultValuesListRole).toStringList()[0];
	item.Value = "0";// value.toFloat();
	
	
	node->constituentInitialCondition().append(item);
	insertRows(rowCount(), 1);
	if (node->experimentName() != "Global")
	{
		//		node->particleInitialCondition().append(item);
		gWidget->log(QString("Constituent initial condition (%1, %2, %3, %4) added to block: %5 for experiment %6.").arg(item.Constituent).arg(item.Particle).arg(item.Model).arg(item.Value).arg(node->Name()).arg(node->experimentName()));
		return true;
	}
	for each(QString experiment in node->parent->experimentsList())
		node->constituentInitialCondition(experiment).append(item);
	gWidget->log(QString("Constituent initial condition (%1, %2, %3, %4) added to block: %5 for all experiments.").arg(item.Constituent).arg(item.Particle).arg(item.Model).arg(item.Value).arg(node->Name()));
	return true;
}
bool ConstituentTableModel::insertRows(int row, int count, const QModelIndex & parent)
{
	beginInsertRows(parent, row , row + count-1);
	endInsertRows();
	return true;
}
bool ConstituentTableModel::removeRows(int firstRow, int lastRow, const QModelIndex & parent)
{
	if (firstRow == -1 || lastRow == -1) return false;
	beginRemoveRows(parent, firstRow, lastRow);
	if (node->experimentName() == "Global")
		for each(QString experiment in node->parent->experimentsList())
			node->constituentInitialCondition(experiment).removeAt(firstRow);
	else
		node->constituentInitialCondition().removeAt(firstRow);
	endRemoveRows();
	return true;
}
void ConstituentTableModel::removeCurrentItem()
{
	if (rowCount() == 1) return;
	int row = qTable->currentIndex().row();
	if (row == rowCount() - 1) return;
	removeRows(row, row);// , createIndex(0, 0));
}

int ConstituentTableModel::rowCount(const QModelIndex & index) const
{
	return node->constituentInitialCondition().count()+1;
}
