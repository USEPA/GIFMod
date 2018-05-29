#include <QtWidgets>
#if defined(QT_PRINTSUPPORT_LIB)
//#include <QtPrintSupport/qtprintsupportglobal.h>
//#if QT_CONFIG(printdialog)
//#include <QPrintDialog>
//#endif
#endif

#include "imageviewer.h"
#include "labelimage.h"

ImageViewer::ImageViewer(bool _menu)
    : imageLabel(new LabelImage)
	, scrollArea(new QScrollArea)
	, scaleFactor(1)
{
	menu = _menu;
	imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    //imageLabel->setScaledContents(true);
    imageLabel->setAlignment(Qt::AlignCenter);

	scrollArea->setBackgroundRole(QPalette::Dark);
	scrollArea->setWidget(imageLabel);
	scrollArea->setVisible(false);
	setCentralWidget(scrollArea);
	
	if (menu) createActions();

	resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);
}


bool ImageViewer::loadFile(const QString &fileName, bool keepOriginalSize)
{
	QImageReader reader(fileName);
	reader.setAutoTransform(true);
	const QImage newImage = reader.read();
	if (newImage.isNull()) {
		QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
			tr("Cannot load %1: %2")
			.arg(QDir::toNativeSeparators(fileName), reader.errorString()));
		return false;
	}

    setImage(newImage, keepOriginalSize);

	setWindowFilePath(fileName);

	if (menu)
	{	
		const QString message = tr("Opened \"%1\", %2x%3, Depth: %4")
			.arg(QDir::toNativeSeparators(fileName)).arg(image.width()).arg(image.height()).arg(image.depth());
		statusBar()->showMessage(message);
	}
	return true;
}

void ImageViewer::setImage(const QImage &newImage, bool keepOriginalSize)
{
	image = newImage;

    ((LabelImage *)imageLabel)->setPixmap(QPixmap::fromImage(image), keepOriginalSize);
    //imageLabel->setPixmap(QPixmap::fromImage(image));

    scaleFactor = 1.0;

	scrollArea->setVisible(true);
	if (menu)
	{
		printAct->setEnabled(true);
		fitToWindowAct->setEnabled(true);
		updateActions();
	}

	if (!menu)
		//imageLabel->adjustSize();
		//imageLabel->showMaximized();
		fitToWindow();
	else if (!fitToWindowAct->isChecked())
		imageLabel->adjustSize();
}


bool ImageViewer::saveFile(const QString &fileName)
{
	QImageWriter writer(fileName);

	if (!writer.write(image)) {
		QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
			tr("Cannot write %1: %2")
			.arg(QDir::toNativeSeparators(fileName)), writer.errorString());
		return false;
	}
	const QString message = tr("Wrote \"%1\"").arg(QDir::toNativeSeparators(fileName));
	statusBar()->showMessage(message);
	return true;
}


static void initializeImageFileDialog(QFileDialog &dialog, QFileDialog::AcceptMode acceptMode)
{
	static bool firstDialog = true;

	if (firstDialog) {
		firstDialog = false;
		const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
		dialog.setDirectory(picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.last());
	}

	QStringList mimeTypeFilters;
	const QByteArrayList supportedMimeTypes = acceptMode == QFileDialog::AcceptOpen
		? QImageReader::supportedMimeTypes() : QImageWriter::supportedMimeTypes();
	foreach(const QByteArray &mimeTypeName, supportedMimeTypes)
		mimeTypeFilters.append(mimeTypeName);
	mimeTypeFilters.sort();
	dialog.setMimeTypeFilters(mimeTypeFilters);
	dialog.selectMimeTypeFilter("image/jpeg");
	if (acceptMode == QFileDialog::AcceptSave)
		dialog.setDefaultSuffix("jpg");
}

void ImageViewer::open()
{
	QFileDialog dialog(this, tr("Open File"));
	initializeImageFileDialog(dialog, QFileDialog::AcceptOpen);

	while (dialog.exec() == QDialog::Accepted && !loadFile(dialog.selectedFiles().first())) {}
}

void ImageViewer::saveAs()
{
	QFileDialog dialog(this, tr("Save File As"));
	initializeImageFileDialog(dialog, QFileDialog::AcceptSave);

	while (dialog.exec() == QDialog::Accepted && !saveFile(dialog.selectedFiles().first())) {}
}

void ImageViewer::print()
{
	Q_ASSERT(imageLabel->pixmap());
/*#if QT_CONFIG(printdialog)
	QPrintDialog dialog(&printer, this);
	if (dialog.exec()) {
		QPainter painter(&printer);
		QRect rect = painter.viewport();
		QSize size = imageLabel->pixmap()->size();
		size.scale(rect.size(), Qt::KeepAspectRatio);
		painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
		painter.setWindow(imageLabel->pixmap()->rect());
		painter.drawPixmap(0, 0, *imageLabel->pixmap());
	}
#endif*/
}

void ImageViewer::copy()
{
#ifndef QT_NO_CLIPBOARD
	QGuiApplication::clipboard()->setImage(image);
#endif // !QT_NO_CLIPBOARD
}

#ifndef QT_NO_CLIPBOARD
static QImage clipboardImage()
{
	if (const QMimeData *mimeData = QGuiApplication::clipboard()->mimeData()) {
		if (mimeData->hasImage()) {
			const QImage image = qvariant_cast<QImage>(mimeData->imageData());
			if (!image.isNull())
				return image;
		}
	}
	return QImage();
}
#endif // !QT_NO_CLIPBOARD

void ImageViewer::paste()
{
#ifndef QT_NO_CLIPBOARD
	const QImage newImage = clipboardImage();
	if (newImage.isNull()) {
		statusBar()->showMessage(tr("No image in clipboard"));
	}
	else {
        setImage(newImage, true);
		setWindowFilePath(QString());
		const QString message = tr("Obtained image from clipboard, %1x%2, Depth: %3")
			.arg(newImage.width()).arg(newImage.height()).arg(newImage.depth());
		statusBar()->showMessage(message);
	}
#endif // !QT_NO_CLIPBOARD
}

void ImageViewer::zoomIn()
{
	scaleImage(1.25);
}

void ImageViewer::zoomOut()
{
	scaleImage(0.8);
}

void ImageViewer::normalSize()
{
	imageLabel->adjustSize();
	scaleFactor = 1.0;
}

void ImageViewer::fitToWindow()
{
	bool fitToWindow=true;
	if (menu) fitToWindow = fitToWindowAct->isChecked();
	scrollArea->setWidgetResizable(fitToWindow);
	if (!fitToWindow && menu)
		normalSize();
	if (menu) updateActions();
}

void ImageViewer::about()
{
	QMessageBox::about(this, tr("About Image Viewer"),
		tr("<p>The <b>Image Viewer</b> example shows how to combine QLabel "
			"and QScrollArea to display an image. QLabel is typically used "
			"for displaying a text, but it can also display an image. "
			"QScrollArea provides a scrolling view around another widget. "
			"If the child widget exceeds the size of the frame, QScrollArea "
			"automatically provides scroll bars. </p><p>The example "
			"demonstrates how QLabel's ability to scale its contents "
			"(QLabel::scaledContents), and QScrollArea's ability to "
			"automatically resize its contents "
			"(QScrollArea::widgetResizable), can be used to implement "
			"zooming and scaling features. </p><p>In addition the example "
			"shows how to use QPainter to print an image.</p>"));
}

void ImageViewer::closed()
{
	emit close();
}

void ImageViewer::createActions()
{
	QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

	QAction *openAct = fileMenu->addAction(tr("&Open..."), this, &ImageViewer::open);
	openAct->setShortcut(QKeySequence::Open);

	saveAsAct = fileMenu->addAction(tr("&Save As..."), this, &ImageViewer::saveAs);
	saveAsAct->setEnabled(false);

    printAct = fileMenu->addAction(tr("&Print..."), this, &ImageViewer::print);
    printAct->setShortcut(QKeySequence::Print);
    printAct->setEnabled(false);

	fileMenu->addSeparator();

	QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));

	copyAct = editMenu->addAction(tr("&Copy"), this, &ImageViewer::copy);
	copyAct->setShortcut(QKeySequence::Copy);
	copyAct->setEnabled(false);

	QAction *pasteAct = editMenu->addAction(tr("&Paste"), this, &ImageViewer::paste);
	pasteAct->setShortcut(QKeySequence::Paste);

	QMenu *viewMenu = menuBar()->addMenu(tr("&View"));

	zoomInAct = viewMenu->addAction(tr("Zoom &In (25%)"), this, &ImageViewer::zoomIn);
	zoomInAct->setShortcut(QKeySequence::ZoomIn);
	zoomInAct->setEnabled(false);

	zoomOutAct = viewMenu->addAction(tr("Zoom &Out (25%)"), this, &ImageViewer::zoomOut);
	zoomOutAct->setShortcut(QKeySequence::ZoomOut);
	zoomOutAct->setEnabled(false);

	normalSizeAct = viewMenu->addAction(tr("&Normal Size"), this, &ImageViewer::normalSize);
	normalSizeAct->setShortcut(tr("Ctrl+S"));
	normalSizeAct->setEnabled(false);

	viewMenu->addSeparator();

	fitToWindowAct = viewMenu->addAction(tr("&Fit to Window"), this, &ImageViewer::fitToWindow);
	fitToWindowAct->setEnabled(false);
	fitToWindowAct->setCheckable(true);
	fitToWindowAct->setShortcut(tr("Ctrl+F"));

	QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));

	helpMenu->addAction(tr("&About"), this, &ImageViewer::about);
	helpMenu->addAction(tr("About &Qt"), &QApplication::aboutQt);
}

void ImageViewer::updateActions()
{
	saveAsAct->setEnabled(!image.isNull());
	copyAct->setEnabled(!image.isNull());
	zoomInAct->setEnabled(!fitToWindowAct->isChecked());
	zoomOutAct->setEnabled(!fitToWindowAct->isChecked());
	normalSizeAct->setEnabled(!fitToWindowAct->isChecked());
}

void ImageViewer::scaleImage(double factor)
{
	Q_ASSERT(imageLabel->pixmap());
	scaleFactor *= factor;
	imageLabel->resize(scaleFactor * imageLabel->pixmap()->size());

	adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
	adjustScrollBar(scrollArea->verticalScrollBar(), factor);

	zoomInAct->setEnabled(scaleFactor < 3.0);
	zoomOutAct->setEnabled(scaleFactor > 0.333);
}

void ImageViewer::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
	scrollBar->setValue(int(factor * scrollBar->value()
		+ ((factor - 1) * scrollBar->pageStep() / 2)));
}

void ImageViewer::mousePressEvent(QMouseEvent * event)
{
	emit clicked();
}
