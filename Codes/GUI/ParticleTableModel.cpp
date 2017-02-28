#include "ParticleTableModel.h"
#include "node.h"
#include "qdebug.h"
#include "GWidget.h"
#include "qtableview.h"
#include "entity.h"

ParticleTableModel::ParticleTableModel(Node *node, QTableView *qTable, GraphWidget *gWidget)
{
	this->node = node;
	this->gWidget = gWidget;
	this->qTable = qTable;
}
QVariant ParticleTableModel::data(const QModelIndex & index, int role) const
{
	if (!index.isValid())
		return QVariant();
	int row = index.row();
	int col = index.column();
	qDebug() << row << col << role;
	if (row == 1 && col == 2 && role == 8)
		int i = 0;
	if (role == TypeRole)
		if (col < 2) return "ComboBox";
	if (role == DefaultValuesListRole)
	{
		if (col == 0) return gWidget->EntityNames("Particle");
		if (col == 1)
		{
			QString pModel = gWidget->entityByName(data(createIndex(row, 0)).toString())->getValue("Model");
			QStringList r;
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
		return node->particleInitialCondition()[row].Particle;
	if (col == 1)
		return node->particleInitialCondition()[row].Model;
	if (col == 2)
		return node->particleInitialCondition()[row].Value;
	qDebug() << "returned nothing.";
	return QVariant();
}

bool ParticleTableModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	static int rows;
	int row = index.row();
	int col = index.column();
	if (row < rowCount() - 1)
	{
		QStringList experimentstobeUpdated;
		if (node->experimentName() != "All experiments")
			experimentstobeUpdated << node->experimentName();
		else
			experimentstobeUpdated = node->parent->experimentsList();
		for each (QString experiment in experimentstobeUpdated)
		{
			if (col == 0)
			{
				node->particleInitialCondition(experiment)[row].Particle = value.toString();
				if (!data(createIndex(row, 1), DefaultValuesListRole).toStringList().contains(node->particleInitialCondition(experiment)[row].Model))
					node->particleInitialCondition(experiment)[row].Model = data(createIndex(row, 1), DefaultValuesListRole).toStringList()[0];
			}
			if (col == 1)
				node->particleInitialCondition(experiment)[row].Model = value.toString();
			if (col == 2)
				node->particleInitialCondition(experiment)[row].Value = value.toString();
		}
		return true;
	}
	if (value.toString() == "Add..." || value.toString() == "") return false;
	ParticleInitialConditionItem item;
	item.Particle = value.toString();
	item.Model = "Mobile";// data(value.toString(), DefaultValuesListRole).toStringList()[0];
	item.Value = "0";

//	insertRows(rowCount(), 1);
//	node->particleInitialCondition().append(item);
	insertRows(rowCount(), 1);
	if (node->experimentName() != "All experiments")
	{
		node->particleInitialCondition().append(item);
		gWidget->log(QString("Particle initial condition (%1, %2, %3) added to block: %4 for experiment %5.").arg(item.Particle).arg(item.Model).arg(item.Value).arg(node->Name()).arg(node->parent->experimentName()));
		return true;
	}
	for each(QString experiment in node->parent->experimentsList())
		node->particleInitialCondition(experiment).append(item);
	gWidget->log(QString("Particle initial condition (%1, %2, %3) added to block: %4 for all experiments.").arg(item.Particle).arg(item.Model).arg(item.Value).arg(node->Name()));
	return true;
}
bool ParticleTableModel::insertRows(int row, int count, const QModelIndex & parent)
{
	beginInsertRows(parent, row , row + count-1);
	endInsertRows();
	return true;
}
bool ParticleTableModel::removeRows(int firstRow, int lastRow, const QModelIndex & parent)
{
	beginRemoveRows(parent, firstRow, lastRow);
	if (node->experimentName() == "All experiments")
		for each(QString experiment in node->parent->experimentsList())
			node->particleInitialCondition(experiment).removeAt(firstRow);
	else
		node->particleInitialCondition().removeAt(firstRow);
	endRemoveRows();
	return true;
}
void ParticleTableModel::removeCurrentItem()
{
	if (rowCount() == 1) return;
	int row = qTable->currentIndex().row();
	if (row == rowCount() - 1) return;
	removeRows(row, row);
}

int ParticleTableModel::rowCount(const QModelIndex & index) const
{
	return node->particleInitialCondition().count() + 1;
}
