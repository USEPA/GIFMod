#ifndef EXPDELEGATE_H
#define EXPDELEGATE_H

#include <QItemDelegate>

class StatusViewer;

class expDelegate : public QItemDelegate
{
public:
    expDelegate(QStringList words, StatusViewer* statusbar, QObject * p = 0);

    QWidget* createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const;
    void setEditorData(QWidget * editor, const QModelIndex & index) const;
    void setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const;

private:
   
    StatusViewer* statusbar;
	QStringList words;
};

#endif // EXPDELEGATE_H
