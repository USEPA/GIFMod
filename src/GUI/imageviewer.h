#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QMainWindow>
#include <QImage>
#ifndef QT_NO_PRINTER
#include <QPrinter>
#endif

class QAction;
class QLabel;
class QMenu;
class QScrollArea;
class QScrollBar;

class ImageViewer : public QMainWindow
{
	Q_OBJECT

public:
	ImageViewer(bool menu=false);
    bool loadFile(const QString &, bool keepOriginalSize=false);
	bool menu;

	
private slots:
	void open();
	void saveAs();
	void print();
	void copy();
	void paste();
	void zoomIn();
	void zoomOut();
	void normalSize();
	void fitToWindow();
	void about();
	void closed(); 

signals:
	void clicked();
	void close(); 

private:
	void createActions();
	void createMenus();
	void updateActions();
	bool saveFile(const QString &fileName);
    void setImage(const QImage &newImage, bool keepOriginalSize);
	void scaleImage(double factor);
	void adjustScrollBar(QScrollBar *scrollBar, double factor);

	QImage image;
	QLabel *imageLabel;
	QScrollArea *scrollArea;
	double scaleFactor;

#ifndef QT_NO_PRINTER
	QPrinter printer;
#endif

	QAction *saveAsAct;
	QAction *printAct;
	QAction *copyAct;
	QAction *zoomInAct;
	QAction *zoomOutAct;
	QAction *normalSizeAct;
	QAction *fitToWindowAct;

protected:
	void mousePressEvent(QMouseEvent* event);
};

#endif
