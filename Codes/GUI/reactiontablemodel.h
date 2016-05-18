#ifndef EXPRESSIONTABLEMODEL_H
#define EXPRESSIONTABLEMODEL_H

#include <QObject>
#include <QAbstractTableModel>
#include <QList>
class GraphWidget;

struct ReactionTableModelPri;

class ReactionTableModel : public QAbstractTableModel {
    Q_OBJECT
public:
    explicit ReactionTableModel(GraphWidget* mainGraphWidget, const QStringList& constituents, const QStringList& parameters,
		const QStringList& functions, const QStringList& physical, const QList<QStringList>& data = QList<QStringList>(), QObject* p = 0);
    ~ReactionTableModel();

signals:
    void invalidExpressoinDetected(const QString& msg);
	void wrongSymbolDetected(const QModelIndex&, const QString &sym);

public:
    QStringList constituents() const;
    QStringList functions() const;
	QStringList physicals() const;
    QStringList parameters() const;
    QStringList operators() const;
    bool validateExp(const QString& exp) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parentt = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    bool insertRows(int position, int rows, const QModelIndex &parent = QModelIndex());
    bool removeRows(int position, int rows, const QModelIndex &parent = QModelIndex());
	bool removeRow(QString processName, int row);// int position, int rows, const QModelIndex &parent = QModelIndex());

    void addItem(int rowIndex, bool after);
    void removeItem(int currentRow);
	QList<QStringList> exportToExpession() const;
	GraphWidget *mainGraphWidget;

private:
    ReactionTableModelPri* d;
	QString wrongSymbol;
};

#endif // EXPRESSIONTABLEMODEL_H
