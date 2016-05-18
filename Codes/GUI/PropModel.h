#pragma once
#include "qabstractitemmodel.h"
#include "mproplist.h"
#include "QString.h"

class Node;
class Edge;
class Entity;

template <class T>
class PropModel :
	public QAbstractTableModel
{
public:

	PropModel(){};
	PropModel(Node* parent){ this->parent = parent; };
	PropModel(Edge* parent){ this->parent = parent; };
	PropModel(Entity* parent){	this->parent = parent; };
	QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
	bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
	int rows(const QModelIndex & index = QModelIndex()) const{
	//	qDebug() << "rows" <<  parent->getmList(parent->Filter()).size();
		return parent->getmList(parent->Filter()).size();
	};
	int rowCount(const QModelIndex & index = QModelIndex()) const{
		return parent->getmList(parent->Filter()).size()+8;
	};
	int columnCount(const QModelIndex & parent = QModelIndex()) const {
		return 2; };
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const{
		if (role == Qt::DisplayRole)
		{
			if (orientation == Qt::Horizontal) {
				switch (section)
				{
				case 0:
					return QString("Property");
				case 1:
					return QString("Value");
				}
			}
		}
		return QVariant();
	};
	Qt::ItemFlags flags(const QModelIndex & index) const{
		if (index.row() >= rows()) return 0;
		int col = index.column();
		//	if (index.row() >= list.size()) return 0;
		if (col == 0) return Qt::ItemIsEnabled;
		
		if (col == 1)
		{
			if (data(index, TypeRole) == "ComboBox" && data(index, DefaultValuesListRole).toStringList().count() == 0)
				return 0;
			return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
		}
		return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
	};

	~PropModel(){};
	T *parent;
	GraphWidget *mainGraphWidget() const {
		return parent->parent;
	}
	void update(){
//		if (repaintAll) emit datachanged(index(0, 0), index(rowCount(), columnCount()));
		emit changed();
	}
signals:
	void changed();
public slots:
	void browserCheck(QString)
	{
		QString file;
		if (_fileName.contains("..."))
			file =
			QFileDialog::getOpenFileName(
			0, //qApp->activeWindow(),
			tr("Select the File"),
			QDir::currentPath(),
			tr("Time Series Input Files (*.txt *.csv)"));
		else file = _fileName;
		if (file.isEmpty()) return;

		QString fileName = file;

		if (!mainGraphWidget()->inflowFileNames.contains(fileName)) mainGraphWidget()->inflowFileNames.append(fileName);
		setData(QModelIndex(), fileName, loadIndex);
		//parent->deselectAll();
		return;
	};

private:
	void save(const QModelIndex index) const{
		loadSave(index, "s");
	}
	QModelIndex load() const{
		return loadSave(QModelIndex(), "l");
	}

	QModelIndex loadSave(QModelIndex index, QString ls) const
	{
		static QModelIndex r;
		if (ls == "s") {
			r = index;
			return QModelIndex();
		}
		if (ls == "l")return r;
		return QModelIndex();
	}
};