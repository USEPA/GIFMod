#ifdef GIFMOD
#ifndef FREEZETABLEWIDGET_H
#define FREEZETABLEWIDGET_H

#include <QTableView>


class FreezeTableWidget : public QTableView {
     Q_OBJECT

public:
      FreezeTableWidget(QAbstractItemModel * model);
      ~FreezeTableWidget();

      QTableView* frozenView();

protected:
      virtual void resizeEvent(QResizeEvent *event);
      virtual QModelIndex moveCursor(CursorAction cursorAction, Qt::KeyboardModifiers modifiers);
      void scrollTo (const QModelIndex & index, ScrollHint hint = EnsureVisible);

private:
      QTableView *frozenTableView;
      void init();
      void updateFrozenTableGeometry();


private slots:
      void updateSectionWidth(int logicalIndex, int oldSize, int newSize);
      void updateSectionWidthSecond(int logicalIndex, int oldSize, int newSize);
      void updateSectionHeight(int logicalIndex, int oldSize, int newSize);
	  void onRowsRemoved(const QModelIndex &parent, int first, int last);
	  void onWrongSymbolDetected(const QModelIndex&, const QString&);

};
#endif // FREEZETABLEWIDGET_H
#endif