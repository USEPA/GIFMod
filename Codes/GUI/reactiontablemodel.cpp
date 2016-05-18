#include "reactiontablemodel.h"
#include <QRegularExpression>
#include "GWidget.h"
#include "process.h"
#include "qvariant.h"
#include <Qcolor>

struct ReactionTableModelPri{
    ReactionTableModel* This;
    QStringList Words, Constituents, Parameters, Functions, Physicals;
    QList<QStringList> Data;

    QStringList Brackets, BinaryOperators;

    ReactionTableModelPri()
    {
        Brackets <<"(" <<")";
        BinaryOperators <<"^" <<"*" <<"/" <<"+" <<"-";
    }

    QStringList emptyRow()
    {
        QStringList list;
        for (int i = 0; i < Constituents.count() + 2; i++) {
            list.append("");
        }
        return list;
    }
    QString Export(const QString& s)
    {
        QString d;
        int l = 0, r = s.length();
        for (int i = 0; i < s.length();) {
			if (s[i].isSpace()) {
				i++;  continue;
			}
            l = i;
            // the first char must start with a letter or underscore
            if (s[i].isLetter() || s[i] == QChar('_')) {
                while (i < s.length() && (s[i].isLetter() || s[i] == QChar('_') || s[i].isNumber())) i++;
            }
            r = i;
            if (l == r) {
                d.append(s[i]);i++;
            }
            else {
                QString var = s.mid(l, r - l);
				if (Constituents.contains(var))
					d.append(QString("c[%1]").arg(var));
				else if (Parameters.contains(var))
					d.append(QString("p[%1]").arg(var));
				else if (Functions.contains(var))
					d.append(QString("f[%1]").arg(var));
				else if (Physicals.contains(var))
					d.append(QString("f[%1]").arg(var)); // ASK
				else
					d.append(var);
            }
        }
        return d;
    }

    QString Import(const QString& s)
    {
        QString d;
        int i = 0;
        while (i < s.length()) {
            int iStart = s.indexOf('[', i);
            int iEnd = s.indexOf(']', i);            
            if (iStart > 0 && iEnd > 0) {
				d.append(s.mid(i, iStart - i - 1));
                int ind = s.mid(iStart + 1, iEnd - iStart - 1).toInt();
                QChar c = s.at(iStart - 1);
                QString var;
                switch (c.toLatin1()) {
                    case 'c': var = Constituents.at(ind); break;
                    case 'p': var = Parameters.at(ind); break;
                    case 'f': var = Functions.at(ind); break;
					case 'g': var = Physicals.at(ind); break; //ASK
				}
                d.append(var);
				i = iEnd + 1;
            }
            else {
                d.append(s.right(s.length() - i));break;
            }
        }
        return d;
    }
};


ReactionTableModel::ReactionTableModel(GraphWidget *gw, const QStringList &constituents, const QStringList &parameters,
	const QStringList &functions, const QStringList &physicals, const QList<QStringList> &data, QObject *p)
    : QAbstractTableModel(p), d(new ReactionTableModelPri)
{
	mainGraphWidget = gw;
    d->This = this;
	d->Constituents = mainGraphWidget->EntityNames("Constituent");
    d->Parameters = parameters;
    d->Functions = functions;
	d->Physicals = physicals;
	d->Words << constituents << parameters << physicals<< functions ;

	if (mainGraphWidget->Processes.count() == 0) {
		new Process("Process", mainGraphWidget);
    }

/*    foreach (QStringList row, data) {
        QStringList r;
        r.append(row.first());
        for (int i = 1; i < row.length(); i++) {
            r.append(d->Import(row[i]));
        }
        d->Data.append(r);
    }*/
}

ReactionTableModel::~ReactionTableModel() {delete d;}


QStringList ReactionTableModel::constituents() const {return d->Constituents;}
QStringList ReactionTableModel::functions() const {return d->Functions;}
QStringList ReactionTableModel::physicals() const { return d->Physicals; }
QStringList ReactionTableModel::parameters() const { return d->Parameters; }
QStringList ReactionTableModel::operators() const {QStringList opts; opts << d->Brackets << d->BinaryOperators; return opts;}

int ReactionTableModel::columnCount(const QModelIndex &) const {return d->Constituents.count()+2;}

QVariant ReactionTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && (role == Qt::DisplayRole || role == Qt::EditRole)) {
        if (section == 0) {
            return "Process Name";
        }
        else if (section == 1) {
            return "Process Rate";
        }
        else if (section > 1 && section <= d->Constituents.length() + 1) {
            return d->Constituents[section - 2];
        }
        else {
            return QVariant();
        }
    }
    else {
        return QAbstractTableModel::headerData(section, orientation, role);
    }
}


int ReactionTableModel::rowCount(const QModelIndex &) const
{
	return mainGraphWidget->Processes.count();
	//return d->Data.length();
}


QVariant ReactionTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }
	QString text;
	if (role == Qt::DisplayRole || role == Qt::EditRole || role == Qt::ToolTipRole) {
		if (index.row() >= 0 && index.row() < mainGraphWidget->Processes.count()) {
			if (index.column() == 0) text = mainGraphWidget->Processes[index.row()]->name;
			if (index.column() == 1) text = mainGraphWidget->Processes[index.row()]->rate;
			if (index.column() > 1 && index.column() < 2 + d->Constituents.count())
			{
				if (!mainGraphWidget->Processes[index.row()]->stoichiometries.keys().contains(d->Constituents[index.column() - 2])) 
					text = "";
				text = mainGraphWidget->Processes[index.row()]->stoichiometries[d->Constituents[index.column() - 2]];
			}
			// return d->Data.at(index.row()).at(index.column());
			return text;
            }
		return "Reaction Network data is corrupted.";
		
        }
	if (role == Qt::ForegroundRole && index.column() >= 1)
		if (!validateExp(data(index, Qt::DisplayRole).toString())) 
			return QColor(Qt::red);

    return QVariant();
}


Qt::ItemFlags ReactionTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) return 0;
    Qt::ItemFlags fgs = QAbstractItemModel::flags(index);
    fgs = fgs | Qt::ItemIsEditable ;
    return fgs;
}


bool ReactionTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (!index.isValid()) {
		return false;
	}
	QVariant oldData = data(index, role);
	if (value == oldData){
		qDebug() << "nothing has changed.";
		return false;
	}
    if (index.column() >= 0) {
//		wrongSymbol == "";
//        if (!validateExp(value.toString())) {
			if (oldData != value) {
//				if (role == Qt::ForegroundRole)
//					return QColor(Qt::red);
				if (index.column() == 0) mainGraphWidget->Processes[index.row()]->name = value.toString();
				if (index.column() == 1) mainGraphWidget->Processes[index.row()]->rate = value.toString();
				if (index.column() > 1 && index.column() < 2 + d->Constituents.count())
					mainGraphWidget->Processes[index.row()]->stoichiometries[d->Constituents[index.column() - 2]] = value.toString();
//				d->Data[index.row()][index.column()] = value.toString();
				emit dataChanged(index, index);
				if (wrongSymbol != "")
					emit wrongSymbolDetected(index, wrongSymbol);
				return true;
			}
			return false;
        }
	else{
		return false;
	}
}

bool ReactionTableModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    beginInsertRows(parent, position, position + rows - 1);
    for (int i = 0; i < rows; i++) {
		new Process("Process", mainGraphWidget);
		//d->Data.insert(i + position, d->emptyRow());
    }
    endInsertRows();
    return true;
}

bool ReactionTableModel::removeRows(int position, int rows, const QModelIndex &index)
{
    beginRemoveRows(index, position, position + rows - 1);
    for (int i = rows; i > 0; i--)
	{
		
		QString name = index.sibling(i + position - 1,0).data().toString();
		mainGraphWidget->deleteProcess(name);
			//d->Data.removeAt(i + position - 1);
    }
    endRemoveRows();
    return true;
}
bool ReactionTableModel::removeRow(QString processName, int row) //int position, int rows, const QModelIndex &index)
{
	beginRemoveRows(QModelIndex(), row, row);
		mainGraphWidget->deleteProcess(processName);
	endRemoveRows();
	return true;
}

void ReactionTableModel::addItem(int rowIndex, bool after)
{
    if (rowIndex < 0) {
        insertRows(rowCount(), 1);
    }
    else {
        if (after) rowIndex++;
        insertRows(rowIndex, 1);
    }
}


void ReactionTableModel::removeItem(int currentRow)
{
    if (currentRow >= 0 && currentRow < rowCount()) {
        removeRows(currentRow, 1);
    }
}


QList<QStringList> ReactionTableModel::exportToExpession() const
{
    QList<QStringList> data;
    for each (Process * e in mainGraphWidget->Processes) {
        QStringList r;
        r.append(e->name);
		r.append(d->Export(e->rate));
		for each (QString c in d->Constituents) {
			if (e->stoichiometries[c] != "") r.append(QString("Product[%1]=%2").arg(c).arg(d->Export(e->stoichiometries[c])));
        }
        data.append(r);
    }
    return data;
}

bool isNumber(const QString& s)
{
    QString ss = s;
	ss = ss.remove('.');
	if (ss.size() - s.size() >= 2) return false;
    if (ss.length() <= 0) return false;
    foreach (QChar c, ss) {
        if (!c.isDigit()) return false;
    }
    return true;
}

bool ReactionTableModel::validateExp(const QString &tt) const
{
	QString t = tt.trimmed();
	if (t.isEmpty()) return true;
    QStringList opts = operators();
    QStringList optWithBackslash;foreach (QString opt, opts)optWithBackslash << ("\\" + opt);
    QRegularExpression re(optWithBackslash.join('|'));
    QRegularExpressionMatchIterator i = re.globalMatch(t);
    QStringList delims;
    while (i.hasNext()) {
        QRegularExpressionMatch match = i.next();
        for (int i = 0; i <= match.lastCapturedIndex(); ++i) {
            delims << match.captured(i);
        }
    }

    QStringList parts;
    int iStart = 0, iEnd;
    foreach (QString delim, delims) {
        iEnd = t.indexOf(delim, iStart);
		QString part = t.mid(iStart, iEnd - iStart).trimmed();
        iStart = iEnd + 1;
        if (!part.isEmpty()) {
            parts << part;
        }
        parts << delim;
    }
	QString part = t.mid(iStart).trimmed();
    if (!part.isEmpty()) {
        parts << part;
    }

    bool validExpression = true;
    QString msg;

    // check braces counting
    QList<int> counters;
    foreach (QString brace, d->Brackets) {
        counters << parts.count(brace);
    }
    for (int i = 0; i < counters.length() - 1; i++) {
        if (counters[i] != counters[i+1]) {
            msg = "bracket mismatch";
            validExpression = false; break;
        }
    }

    if (validExpression) {
        // check binary operators;
        foreach (QString opt, d->BinaryOperators) {
            int ind = 1;
            while (ind >= 1) {
                ind = parts.indexOf(opt, ind);
                if (ind > 0 && ind < parts.length() - 1) {
                    QString left = parts[ind - 1], right = parts[ind + 1];
					if (!isNumber(left) && !d->Constituents.contains(left) && !d->Physicals.contains(left) && !d->Parameters.contains(left) && left != ")" && !d->Functions.contains(left)) {
//						wrongSymbol = left;
                        msg = QString("bad expression with operator (%1)").arg(opt);
                        validExpression = false;break;
                    }
					if (!isNumber(right) && !d->Constituents.contains(right) && !d->Physicals.contains(right) && !d->Parameters.contains(right) && right != "(" && !d->Functions.contains(right)) {
//						wrongSymbol = right;
						msg = QString("bad expression with operator (%1)").arg(opt);
                        validExpression = false;break;
                    }
                }
                else if (ind == 0 || ind == parts.length() - 1) {
                    msg = QString("operator %1 in wrong position!").arg(opt);
                    validExpression = false;break;
                }
                if (ind >= 1) ind++;
            }
            if (!validExpression)break;
        }
    }

    if (validExpression) {
        // for every two operands there must be an operator in between them
        int INVALID = -1;
        int current = INVALID, next = INVALID;
        for (int i = 0; i < parts.length(); i++) {
            QString part = parts[i];
            if (d->Constituents.contains(part)) {
                next = i;
            }
            if (d->Parameters.contains(part)) {
                next = i;
            }
			if (d->Physicals.contains(part)) {
				next = i;
			}
			if (next > INVALID) {
                if (current > INVALID) {
                    // do the checking here
                    bool alright = false;
                    for (int j = current+1; j < next; j++) {
                        if (d->BinaryOperators.contains(parts[j])) {
                            alright = true; break;
                        }
                    }
                    if (!alright) {
                        msg = QString("no operator between operands (%1) (%2)").arg(parts[current], parts[next]);
                        validExpression = false; break;
                    }
                     current = next; next = INVALID;
                }
                else {
                    current = next;
                    next = INVALID;
                }
            }
        }
    }

    if (validExpression) {
        // general checking: check everything is there
        foreach (QString part, parts) {
            if (part.isEmpty()) continue;
            if (d->Words.contains(part)) continue;
            if (opts.contains(part)) continue;
            if (isNumber(part)) continue;
            msg = QString("invalid symbol (%1) in the expression!").arg(part);
//			wrongSymbol = part;
            validExpression = false; break;
        }
    }
 //   if (!validExpression) {
 //       emit invalidExpressoinDetected(msg);
 //   }
    return validExpression;
}
