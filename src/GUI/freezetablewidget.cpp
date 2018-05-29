#ifdef GIFMOD
#include "freezetablewidget.h"
#include <QScrollBar>
#include <QHeaderView>


FreezeTableWidget::FreezeTableWidget(QAbstractItemModel * model)
{
      setModel(model);
      frozenTableView = new QTableView(this);

      init();

      //connect the headers and scrollbars of both tableviews together
      connect(horizontalHeader(),SIGNAL(sectionResized(int,int,int)), this,
              SLOT(updateSectionWidth(int,int,int)));
      connect(verticalHeader(),SIGNAL(sectionResized(int,int,int)), this,
              SLOT(updateSectionHeight(int,int,int)));

      connect(frozenTableView->verticalScrollBar(), SIGNAL(valueChanged(int)),
              verticalScrollBar(), SLOT(setValue(int)));
      connect(verticalScrollBar(), SIGNAL(valueChanged(int)),
              frozenTableView->verticalScrollBar(), SLOT(setValue(int)));


      connect(model, SIGNAL(rowsRemoved(QModelIndex,int,int)), SLOT(onRowsRemoved(QModelIndex,int,int)));
	  connect(model, SIGNAL(wrongSymbolDetected(QModelIndex, QString)), SLOT(onWrongSymbolDetected(QModelIndex, QString)));
}


FreezeTableWidget::~FreezeTableWidget()
{
      delete frozenTableView;
}

void FreezeTableWidget::onRowsRemoved(const QModelIndex &parent, int first, int last)
{
    if (model()->rowCount() == 0) {

    }
}

void FreezeTableWidget::onWrongSymbolDetected(const QModelIndex& index, const QString& str)
{
	int i = 0;
}


QTableView* FreezeTableWidget::frozenView() {return frozenTableView;}


void FreezeTableWidget::init()
{
    connect(horizontalHeader(),SIGNAL(sectionResized(int,int,int)), this,
            SLOT(updateSectionWidthSecond(int,int,int)));

      frozenTableView->setModel(model());
      frozenTableView->setFocusPolicy(Qt::NoFocus);
      frozenTableView->verticalHeader()->hide();
      frozenTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);

      viewport()->stackUnder(frozenTableView);

      frozenTableView->setStyleSheet("QTableView { border: none;}"); //for demo purposes
      frozenTableView->setSelectionModel(selectionModel());
      for (int col = 2; col < model()->columnCount(); ++col)
            frozenTableView->setColumnHidden(col, true);

      frozenTableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
      frozenTableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
      frozenTableView->show();

      updateFrozenTableGeometry();

      setHorizontalScrollMode(ScrollPerPixel);
      setVerticalScrollMode(ScrollPerPixel);
      frozenTableView->setVerticalScrollMode(ScrollPerPixel);

      updateFrozenTableGeometry();
}


void FreezeTableWidget::updateSectionWidth(int logicalIndex, int /* oldSize */, int newSize)
{
      if (logicalIndex == 0 || logicalIndex == 1){
            frozenTableView->setColumnWidth(logicalIndex, newSize);
            updateFrozenTableGeometry();
      }
}


void FreezeTableWidget::updateSectionWidthSecond(int logicalIndex, int /* oldSize */, int newSize)
{
      if (logicalIndex == 0){
            setColumnWidth(0, newSize);
            updateFrozenTableGeometry();
      }
      else if (logicalIndex == 1) {
          setColumnWidth(1, newSize);
          updateFrozenTableGeometry();
      }
}


void FreezeTableWidget::updateSectionHeight(int logicalIndex, int /* oldSize */, int newSize)
{
      frozenTableView->setRowHeight(logicalIndex, newSize);
}


void FreezeTableWidget::resizeEvent(QResizeEvent * event)
{
      QTableView::resizeEvent(event);
      updateFrozenTableGeometry();
 }


QModelIndex FreezeTableWidget::moveCursor(CursorAction cursorAction,
                                          Qt::KeyboardModifiers modifiers)
{
      QModelIndex current = QTableView::moveCursor(cursorAction, modifiers);

      if (cursorAction == MoveLeft && current.column() > 0
              && visualRect(current).topLeft().x() < frozenTableView->columnWidth(0) ){
            const int newValue = horizontalScrollBar()->value() + visualRect(current).topLeft().x()
                                 - frozenTableView->columnWidth(0) - frozenTableView->columnWidth(1);
            horizontalScrollBar()->setValue(newValue);
      }
      return current;
}


void FreezeTableWidget::scrollTo (const QModelIndex & index, ScrollHint hint){
    if (index.column() > 1)
        QTableView::scrollTo(index, hint);
}


void FreezeTableWidget::updateFrozenTableGeometry()
{
    frozenTableView->setColumnWidth(0, columnWidth(0));
    frozenTableView->setColumnWidth(1, columnWidth(1));
    frozenTableView->setGeometry(verticalHeader()->width() + frameWidth(),
                                 frameWidth(), columnWidth(0) + columnWidth(1),
                                 viewport()->height() + horizontalHeader()->height());
}



#endif