#include "mainwindow.h"
#ifdef WQV
#include "ui_mainwindowWQV.h"
#endif
#ifdef GWA
#include "ui_mainwindowGWA.h"
#endif
#include <QtGui>
#include <QDebug>
#include <qgraphicsview.h>
#include <qmessagebox.h>
#include <QPaintEvent>
#include <QPainter>
#include <GWidget.h>
#include <node.h>
#include "edge.h"
#include "entity.h"
#include <enums.h>
#include <qgraphicsitem.h>
#include <mProplist.h>
#include <qtableWidget.h>
#include <delegate.h>
#include <qfiledialog.h>
#include "qlistview.h"
#include "qapplication.h"
#include "QFileSystemModel.h"
#include "treemodel.h"
#include "reactionwindow.h"
#include "helpWindow.h"
#include "qinputdialog.h"

#ifdef WQV
#include "Medium.h"
#include "MediumSet.h"
#endif

#include "plotWindow.h"
#include <QProgressBar>
#include "logwindow.h"
#include "results.h"
//#ifdef GWA
#include "mainwindow.h"
#include "entity.h"



void MainWindow::on_actionAdd_Well_triggered()
{
	static int Well_Blocks = 1;
	new Node(mainGraphWidget, "Well", QString("Well (%1)").arg(Well_Blocks++), -1, 600, 700, 150, 150);
}

void MainWindow::on_actionAdd_Tracer_triggered()
{
	static int Tracers = 1;
	new Node(mainGraphWidget, "Tracer", QString("Tracer (%1)").arg(Tracers++), -1, 1300, 700, 80, 80);
}

MainWindow::MainWindow(QWidget *parent, QString applicationName, QString shortName, QString extension, QString metafilename, QString modelfilename):
QMainWindow(parent),
ui(new Ui::MainWindow)
{
	this->metafilename = metafilename;
	this->applicationName = applicationName; 
	this->fileExtension = extension;
	this->applicationShortName = shortName;
	log = new logWindow(this);
//	QDesktopWidget *d = QApplication::desktop();
/*	if (d->screenCount() > 1)
	{
		int left = d->screen(0)->width() + 35;
		int width = d->screen(1)->width() * 0.4;
		int height = d->screen(1)->height() * 0.9;
		int top = 35;
		log->setFixedHeight(height);
		log->setFixedWidth(width);
		log->move(left, top);
	}
*/
//	QString a = QString("Screen 0 width(%1), height(%2), Screen 1 width(%3), height(%4)").arg(d->screen(0)->width()).arg(d->screen(0)->height()).arg(d->screen(1)->width()).arg(d->screen(1)->height());
//	log->append(a);
	log->show();
	log->append("Program started.");

	mainGraphWidget = new GraphWidget(this, applicationShortName, metafilename, log, this);
	QObject::connect(mainGraphWidget, SIGNAL(Mouse_Pos(int, int, QString)), this, SLOT(Mouse_Pos(int, int, QString)));

	dockWidget = new QDockWidget(this);
	dockWidget->setWindowTitle("Model Space");
	dockWidget->setObjectName(QStringLiteral("dockWidget"));
	QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
	sizePolicy1.setHorizontalStretch(2);

	dockWidget->setSizePolicy(sizePolicy1);
	dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::TopDockWidgetArea);
	dockWidgetContents = new QWidget();
	dockWidgetContents->setObjectName(QStringLiteral("dockWidgetContents"));
	verticalLayout = new QVBoxLayout(dockWidgetContents);
	verticalLayout->setSpacing(6);
	verticalLayout->setContentsMargins(0, 0, 0, 0);
	verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
	// ***
	verticalLayout->addWidget(mainGraphWidget);
	// ***
	dockWidget->setWidget(dockWidgetContents);
	this->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidget); //4
	qDebug() << "1";
	dockProp = new QDockWidget(this);
	dockProp->setWindowTitle("Properties");
	dockProp->setObjectName(QStringLiteral("dockProp"));
	QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	sizePolicy.setHorizontalStretch(1);

	sizePolicy.setHeightForWidth(dockProp->sizePolicy().hasHeightForWidth());
	dockProp->setSizePolicy(sizePolicy);
	//	dockProp->setFeatures(dockProp->features() | QDockWidget::DockWidgetVerticalTitleBar);
	dockProp->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::TopDockWidgetArea);
	dockPropContents = new QWidget();
	dockPropContents->setObjectName(QStringLiteral("dockPropContents"));
	verticalLayout_2 = new QVBoxLayout(dockPropContents);
	verticalLayout_2->setSpacing(6);
	verticalLayout_2->setContentsMargins(0, 0, 0, 0);
	verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
	dockProp->setWidget(dockPropContents);
	dockProp->setMinimumWidth(size().width() / 2.5);
	qDebug() << "1.1";
	dockProjectExplorer = new QDockWidget(this);
	dockProjectExplorer->setWindowTitle("Project Explorer");
	dockProjectExplorer->setObjectName(QStringLiteral("dockProjectExplorer"));
	QSizePolicy sizePolicyPE(QSizePolicy::Expanding, QSizePolicy::Expanding);
	sizePolicyPE.setHorizontalStretch(1);
	qDebug() << "1.2";
	sizePolicyPE.setHeightForWidth(dockProjectExplorer->sizePolicy().hasHeightForWidth());
	dockProjectExplorer->setSizePolicy(sizePolicyPE);
	dockProjectExplorer->setFeatures(dockProjectExplorer->features());// | QDockWidget::DockWidgetVerticalTitleBar);
	dockProjectExplorer->setAllowedAreas(Qt::AllDockWidgetAreas); // LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::TopDockWidgetArea);
	qDebug() << "1.3";
	projectExplorerContents = new QWidget();
	projectExplorerContents->setObjectName(QStringLiteral("projectExplorerContents"));
	verticalLayout_1 = new QVBoxLayout(projectExplorerContents);
	verticalLayout_1->setSpacing(6);
	verticalLayout_1->setContentsMargins(0, 0, 0, 0);
	verticalLayout_1->setObjectName(QStringLiteral("verticalLayout_1"));
	dockProjectExplorer->setWidget(projectExplorerContents);
	qDebug() << "1.4";
	dockProjectExplorer->setMinimumWidth(size().width() / 3.5);
	qDebug() << "1.5";
	projectExplorer = new QTreeView(projectExplorerContents);
	projectExplorer->setGeometry(this->rect());
	projectExplorer->setContextMenuPolicy(Qt::CustomContextMenu);

	verticalLayout_1->addWidget(projectExplorer);
	qDebug() << "1.6";
	dockProjectExplorer->setWidget(projectExplorerContents);
	qDebug() << "1.9";
	this->addDockWidget(Qt::RightDockWidgetArea, dockProjectExplorer);
	qDebug() << "2";
	tableProp = new QTableView(dockProp);
	// ***
	Delegate *mDelegate = new Delegate(this, mainGraphWidget);
	// ***	
	tableProp->setItemDelegateForColumn(1, mDelegate);
	//tableProp->setItemDelegate(mDelegate);
	tableProp->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::AllEditTriggers
		| QAbstractItemView::SelectedClicked);
	tableProp->setSelectionBehavior(QAbstractItemView::SelectItems);
	//tableProp->setMouseTracking(true);

	verticalLayout_2->addWidget(tableProp);
	qDebug() << "3";
	dockProp->setWidget(dockPropContents);
	qDebug() << 3.01;
	this->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dockProp);
	//	propModel = new PropModel();
	//	tableProp->setModel(propModel);

	//	projModel = new ProjModel(mainGraphWidget);

	// ***
	qDebug() << 3.02;
	projModel = new TreeModel(mainGraphWidget);
	// ***
	qDebug() << 3.03;
	projectExplorer->setObjectName(QStringLiteral("projectExplorer"));

	qDebug() << 3.1;
	//	projectExplorer->setItemDelegate(mDelegate);

	Qt::WindowFlags flags = this->windowFlags();
	qDebug() << 3.2; 
	qDebug() << 3.3;
	setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint);

	// ***
	mainGraphWidget->setProjExp(projectExplorer);
	mainGraphWidget->treeModel = projModel;
	qDebug() << 3.4;
	//mainGraphWidget->setpropModel(propModel);
	mainGraphWidget->settableProp(tableProp);
	qDebug() << 3.5;
	mainGraphWidget->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
	// ***
	tableProp->setContextMenuPolicy(Qt::CustomContextMenu);
	qDebug() << 3.6;
	connect(tableProp, SIGNAL(customContextMenuRequested(const QPoint&)),
		this, SLOT(tablePropShowContextMenu(const QPoint&)));
	qDebug() << 3.7;
	connect(mainGraphWidget, SIGNAL(changed()),
		this, SLOT(gwidgetChanged()));
	qDebug() << 4;
	//tableProp->setTextElideMode(Qt::ElideMiddle);
	//tableProp->setWordWrap(true);
	this->show();
	//tableProp->setModel(projModel);
	projectExplorer->setModel(projModel);
	//projectExplorer->.setChildIndicatorPolicy(QTreeWidgetItem.ShowIndicator)
	setWindowTitle(applicationName);
	centerWidget();
//	if (modelfilename != "")
//		loadModel(modelfilename);
	qDebug() << 5;
	ui->setupUi(this);
	mainGraphWidget->experiments = new QComboBox(ui->experimentsToolbar);
	mainGraphWidget->experiments->addItem("Global");
	mainGraphWidget->experiments->addItem("experiment1");
//	mainGraphWidget->experiments->addItem("experiment2");
	connect(mainGraphWidget->experiments, SIGNAL(currentIndexChanged(const QString&)), mainGraphWidget, SLOT(experimentSelect(const QString&)));
	if (ui->experimentsToolbar->height() < mainGraphWidget->experiments->height())
		ui->experimentsToolbar->setMinimumHeight(mainGraphWidget->experiments->height());
	ui->experimentsToolbar->setMinimumWidth(250);
	QRect rect = mainGraphWidget->experiments->geometry();
	rect.setLeft(rect.left() + 95);
	rect.setWidth(140);
	int i = ui->experimentsToolbar->height();
	int j = mainGraphWidget->experiments->height();
	int k = (ui->experimentsToolbar->height() - mainGraphWidget->experiments->height()) / 2;
	rect.setTop((ui->experimentsToolbar->height() - mainGraphWidget->experiments->height()) / 2 + 8);
	
	mainGraphWidget->experiments->setGeometry(rect);
	mainGraphWidget->experiments->show();
//	ui->experimantsToolbar->insertWidget()
	mainGraphWidget->add_to_undo_list();
	mainGraphWidget->trackingUndo = true;
}
void MainWindow::centerWidget()
{
	if (isFullScreen())
		return;

	QSize size;
	//if (useSizeHint)
	//	size = w->sizeHint();
	//else
	size = this->size();

	QDesktopWidget *d = QApplication::desktop();
	int ws = d->screen(0)->width();   // returns screen width
	int h = d->screen(0)->height();  // returns screen height
	int mw = size.width();
	int mh = size.height();
	int cw = (ws / 2) - (mw / 2);
	int ch = (h / 2) - (mh / 2);
	move(cw, ch);
}
void MainWindow::on_action_New_triggered()
{
    //new
//	mainGraphWidget->expandNode(projModel->index(-1,-1), false);
	if (mainGraphWidget->hasChanged())
		if (QMessageBox::question(this, tr("Clear Model"),
			"Your model has not been saved.\nAll changes in the model will be lost.\nAre you sure?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::No)
			return;

	mainGraphWidget->trackingUndo = false;
	mainGraphWidget->clear();
	setModelFileName("");
//	setModelFileName("Untitled." + fileExtension);

#ifdef WQV
	new Entity("Solver setting", "Solver setting", mainGraphWidget);
	new Entity("Climate setting", "Climate setting", mainGraphWidget);
	new Entity("Project setting", "Project setting", mainGraphWidget);

#endif
#ifdef GWA
	new Entity("Global settings", "Global settings", mainGraphWidget);
#endif
	new Entity("Genetic algorithm", "Genetic algorithm", mainGraphWidget);
	new Entity("Markov chain Monte Carlo", "Markov chain Monte Carlo", mainGraphWidget);

	
	mainGraphWidget->tableProp = tableProp;
	mainGraphWidget->setMode();
	mainGraphWidget->add_to_undo_list();
	mainGraphWidget->trackingUndo = true;

	log->show();
	log->append("New Model Created.");
	mainGraphWidget->deleteSolution();
	mainGraphWidget->changedState = false;
}

void MainWindow::on_action_Open_triggered()
{
    //open
	if (mainGraphWidget->hasChanged())
		if (QMessageBox::question(this, tr("Open Model"),
			"Your model has not been saved.\nAll changes in the model will be lost.\nAre you sure?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::No)
			return;
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("Open ").append(applicationName), mainGraphWidget->modelPathname(),
		tr("Model (*.").append(fileExtension).append(");;All Files (*)"));
	loadModel(fileName);
	on_actionZoom_All_triggered();
}
bool MainWindow::loadModel(QString modelfilename)
{
	if (modelfilename.isEmpty())
		return false;
	else {

		QFile file(modelfilename);

		if (!file.open(QIODevice::ReadOnly)) {
			QMessageBox::information(this, tr("Unable to open file"),
				file.errorString());
			return false;
		}

		QDataStream in(&file);
		in.setVersion(QDataStream::Qt_4_5);
		QString fileHeader;
		in >> fileHeader;
//		if ((fileExtension == "WQV" && fileHeader != "GIM Data File version 1.0" && fileHeader != "WQV Data File version 1.0" && fileHeader != "WQV Data File version 1.1") ||
//			(fileExtension == "GWA" && fileHeader != "GWA Data File version 1.0"))
//		{
//			QMessageBox::information(this, tr("Unable to open file"),
//	/			"Wrong Data file or the file is corrupted.");
//			return false;
//		}
		QList<QMap<QString, QVariant>> data;
		in >> data;
		mainGraphWidget->trackingUndo = false;
		mainGraphWidget->clear();
		if (fileHeader.contains("1.0"))
			mainGraphWidget->unCompact10(data);
		else if (fileHeader.contains("1.1"))
			mainGraphWidget->unCompact(data);
		else if (fileHeader.contains("1.2"))
			mainGraphWidget->unCompact(data);
		else
		{
			log->append("Bad file, could not openned.");
			return false;
		}
		setModelFileName(modelfilename);
		mainGraphWidget->add_to_undo_list();
//		if (mainGraphWidget->entity("Project setting")->getValue("Working path") == ".")
//			mainGraphWidget->entity("Project setting")->setValue("Working path", OnlyPath(modelfilename));


		//		expandNode(projModel->index(-1, -1), true);
		//		else {
		//			QMap<QString, QString>::iterator i = contacts.begin();
		//			nameLine->setText(i.key());
		//			addressText->setText(i.value());
		//		}
	}

	//	updateInterface(NavigationMode);
	mainGraphWidget->changedState = false;
	mainGraphWidget->trackingUndo = true;
	return true;
}
void MainWindow::on_action_Save_triggered()
{
	QString fileName = (!mainGraphWidget->modelFilename.isEmpty()) ? mainGraphWidget->modelFilename : QFileDialog::getSaveFileName(this,
		tr("Save ").append(applicationName), mainGraphWidget->modelPathname(),
		tr("Model (*.").append(fileExtension).append(");;All Files (*)"));
	saveModel(fileName);
	setModelFileName(fileName);
}
void MainWindow::on_actionSave_As_triggered()
{
	QString fileName = QFileDialog::getSaveFileName(this,
		tr("Save ").append(applicationName), mainGraphWidget->modelPathname(),
		tr("Model (*.").append(fileExtension).append(");;All Files (*)"));
	saveModel(fileName);
	setModelFileName(fileName);
	}


void MainWindow::on_actionZoom_In_triggered()
{
	//Zoom In
	//mainGraphWidget->zoomIn();

	mainGraphWidget->scaleView(1.25);
}

void MainWindow::on_actionZoom_Out_triggered()
{
    //Zoom Out
	//mainGraphWidget->zoomOut();

	mainGraphWidget->scaleView(1 / 1.25);
}

void MainWindow::on_actionZoom_All_triggered()
{
//	mainGraphWidget->MainGraphicsScene->setSceneRect(mainGraphWidget->minX(), mainGraphWidget->minY(), mainGraphWidget->maxX(), mainGraphWidget->maxY());
//	mainGraphWidget->MainGraphicsScene->setSceneRect(mainGraphWidget->MainGraphicsScene->itemsBoundingRect());
//	mainGraphWidget->setSceneRect(mainGraphWidget->MainGraphicsScene->itemsBoundingRect());
//	mainGraphWidget->ensureVisible(mainGraphWidget->MainGraphicsScene->itemsBoundingRect());
	QRectF newRect = mainGraphWidget->MainGraphicsScene->itemsBoundingRect();
	float width = newRect.width();
	float height = newRect.height();
	float scale = 1.1;
	newRect.setLeft(newRect.left() - (scale - 1) / 2 * width);
	newRect.setTop(newRect.top() - (scale - 1) / 2 * height);
	newRect.setWidth(width * scale);
	newRect.setHeight(height * scale);

	mainGraphWidget->fitInView(newRect,Qt::KeepAspectRatio);
}


void MainWindow::on_actionE_xit_triggered()
{
	this->close();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	if (mainGraphWidget->hasChanged())
		if (QMessageBox::question(this, tr("Exit Program"),
			"Your model has not been saved.\nAll changes in the model will be lost.\nAre you sure?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::No)
			event->ignore();
/*	if (mainGraphWidget->model)
		delete mainGraphWidget->model;
/*	if (mainGraphWidget->results)
		delete mainGraphWidget->results;
	if (mainGraphWidget->logW)
		delete mainGraphWidget->logW;

	if (mainGraphWidget)
		delete mainGraphWidget;
*/}

void MainWindow::on_action_Undo_triggered()
{
	mainGraphWidget->undo();
}

void MainWindow::on_action_Redo_triggered()
{
	mainGraphWidget->redo();
}

void MainWindow::on_action_Select_triggered(bool checked)
{
	if (checked)
	{
		ui->actionAdd_Connector->setChecked(false);
		ui->action_Pan->setChecked(false);
		mainGraphWidget->setMode();
	}
	else
//		updateToolbarToggles(mainGraphWidget->setMode(1));
		ui->action_Select->setChecked(true);

}

void MainWindow::on_action_Pan_triggered(bool checked)
{
	if (checked)
	{
		ui->action_Select->setChecked(false);
		ui->actionAdd_Connector->setChecked(false);
		mainGraphWidget->setMode(Operation_Modes::Pan);
	}
	else
		updateToolbarToggles(mainGraphWidget->setMode(1));

}

void MainWindow::on_actionProperty_Box_triggered(bool checked)
{
/*
QString fileName =
		QFileDialog::getOpenFileName(this, tr("Open Bookmark File"),
		QDir::currentPath(),
		tr("Time Series Input Files (*.txt *.csv)"));
	if (fileName.isEmpty())
		return;

	qDebug() << fileName;
*/
//	mainGraphWidget->add_nodes();
/*	QList <mProp> QL;
	mPropList mPL;
	mPL = mainGraphWidget->mList->filter(mainGraphWidget->ModelSpace);
	for (int i = 0; i < mPL.List.size(); i++)
	{
		QTableWidgetItem *item0 = new QTableWidgetItem(mPL.List[i].VariableName);
		QTableWidgetItem *item1 = new QTableWidgetItem(mPL.List[i].Value);
		item0->setToolTip(mPL.List[i].Description);
		item1->setToolTip(mPL.List[i].Description);

		tableProp->setItem(i, 0, item0);
		tableProp->setItem(i, 1, item1);
	}
	tableProp->resizeColumnsToContents();
	tableProp->resize(500, 300);

	tableProp->show();
*/
}

void MainWindow::Mouse_Pos(int x, int y, QString str)
{
	//this->statusBar()->showMessage(QString("X = %1 , Y = %2 ").arg(mainGraphWidget->_x).arg(mainGraphWidget->_y).append(str));
	this->statusBar()->showMessage(QString("X = %1 , Y = %2 ").arg(x).arg(y).append(str));
	
	//qDebug() << ui->graphicsView->mx << ui->graphicsView->my;
	//emit Mouse_Pos();
}
void MainWindow::updateToolbarToggles(Operation_Modes OMode)
{
	ui->action_Select->setChecked((OMode == Operation_Modes::NormalMode)? true : false);
	ui->actionAdd_Connector->setChecked((OMode == Operation_Modes::Draw_Connector) ? true : false);
	ui->action_Pan->setChecked((OMode == Operation_Modes::Pan) ? true : false);

}

void MainWindow::on_dockWidget_dockLocationChanged(const Qt::DockWidgetArea &area)
{
	qDebug() << area;
}

void MainWindow::on_projectExplorer_clicked(const QModelIndex &index)
{
	if (index.data(Role::TreeItemType) == TreeItem::Type::NodeItem ||
		index.data(Role::TreeItemType) == TreeItem::Type::EdgeItem ||
		index.data(Role::TreeItemType) == TreeItem::Type::EntityItem ||
		index.data(Role::TreeItemType) == TreeItem::Type::Item)
	{
		QString type = index.data(Role::TreeParentItemType).toString();
		mainGraphWidget->select(index.data().toString(), type);
	}
}

void MainWindow::on_projectExplorer_customContextMenuRequested(const QPoint &pos)
{
	if (projectExplorer->indexAt(pos).data(Role::TreeItemType) == TreeItem::Type::Branch)
	{
		QMenu *menu = new QMenu;
		TreeModel *model = projModel;
		TreeItem *item = model->itemFromIndex(projectExplorer->indexAt(pos));
		QString type = projModel->singularform(projectExplorer->indexAt(pos).data().toString());
		QModelIndex index = projectExplorer->indexAt(pos);
		addProjectExplorerTreeItem(model, type, index);
		if (type == "Parameter" && mainGraphWidget->results)
		{
			if (mainGraphWidget->results->percentiles.size())
			{
				plotAllPercentileData(mainGraphWidget->results->percentiles, "Percentiles data");
				menu->addAction(QString("Plot percentiles data"), this, SLOT(plotAllPercentileData()));
				menu->addSeparator();
			}
		}
		menu->addAction(QString("Add %1").arg(type) , this, SLOT(addProjectExplorerTreeItem()));
		menu->exec(projectExplorer->mapToGlobal(pos));
	}
	if (projectExplorer->indexAt(pos).data(Role::TreeItemType) == TreeItem::Type::NodeItem)
	{
		QMenu *menu = new QMenu;
		TreeModel *model = projModel;
		TreeItem *item = model->itemFromIndex(projectExplorer->indexAt(pos));
		QString name = projectExplorer->indexAt(pos).data().toString();
		QModelIndex index = projectExplorer->indexAt(pos);
		removeProjectExplorerNodeItem(name, index); // model, type, index);
		menu->addAction(QString("Delete %1").arg(name), this, SLOT(removeProjectExplorerNodeItem()));
		menu->exec(projectExplorer->mapToGlobal(pos));
	}
	if (projectExplorer->indexAt(pos).data(Role::TreeItemType) == TreeItem::Type::EdgeItem)
	{
		QMenu *menu = new QMenu;
		TreeModel *model = projModel;
		TreeItem *item = model->itemFromIndex(projectExplorer->indexAt(pos));
		QString name = projectExplorer->indexAt(pos).data().toString();
		QModelIndex index = projectExplorer->indexAt(pos);
		removeProjectExplorerEdgeItem(name, index); // model, type, index);
		menu->addAction(QString("Delete %1").arg(name), this, SLOT(removeProjectExplorerEdgeItem()));
		menu->exec(projectExplorer->mapToGlobal(pos));
	}
	if (projectExplorer->indexAt(pos).data(Role::TreeItemType) == TreeItem::Type::Item)
	{
		QMenu *menu = new QMenu;
		TreeModel *model = projModel;
		TreeItem *item = model->itemFromIndex(projectExplorer->indexAt(pos));
		if (item->Name() == "Markov chain Monte Carlo")
		{
			if (mainGraphWidget->results->globalSensitivityMatrix.getnumrows())
				menu->addAction(QString("Show global sensitivity matrix"), this, SLOT(showGlobalSensitivityMatrix()));
			if (mainGraphWidget->results->localSensitivityMatrix.getnumrows())
				menu->addAction(QString("Show local sensitivity matrix"), this, SLOT(showLocalSensitivityMatrix()));
			if (mainGraphWidget->results->correlationMatrix.getnumrows())
				menu->addAction(QString("Show correlation matrix"), this, SLOT(showCorrelationMatrix()));
			if (mainGraphWidget->results->localSensitivityMatrix.getnumrows() || mainGraphWidget->results->globalSensitivityMatrix.getnumrows() || mainGraphWidget->results->correlationMatrix.getnumrows())
				menu->exec(projectExplorer->mapToGlobal(pos));
		}
	}
	if (projectExplorer->indexAt(pos).data(Role::TreeItemType) == TreeItem::Type::EntityItem)
	{
		QMenu *menu = new QMenu;
		TreeModel *model = projModel;
		TreeItem *item = model->itemFromIndex(projectExplorer->indexAt(pos));
		QString name = projectExplorer->indexAt(pos).data().toString();
		

		if (item->parent()->Name() == "Observations")
		{
			bool addSeparator = false;
			Entity *obs = mainGraphWidget->entity(name, "Observation");
			QString file = obs->getValue("Observed data").toQString();
			CBTCSet data = CBTCSet(file.replace("./", modelPathname().append('/')).toStdString(), 1);
			if (data.nvars)
			{
				data[0].name = name.toStdString();
				plotObservationData(data[0], QString::fromStdString(data[0].name));
				menu->addAction(QString("Plot observation data").arg(name), this, SLOT(plotObservationData()));
				addSeparator = true;
			}
			if (mainGraphWidget->model != nullptr)
#ifdef WQV
				if (mainGraphWidget->model->ANS_obs.nvars)
			{
				int index = -1;
				for (int i = 0; i<mainGraphWidget->model->ANS_obs.nvars;i++)
					if (mainGraphWidget->model->ANS_obs[i].name == name.toStdString())
					{
						plotModeledData(mainGraphWidget->model->ANS_obs[i], data[0], QString("%1").arg(name));
						menu->addAction(QString("Plot modeled data").arg(name), this, SLOT(plotModeledData()));

						plotAgreementPlotData(data[0], mainGraphWidget->model->ANS_obs[i].interpol(data[0]), QString("Agreement plot %1").arg(name));
						menu->addAction(QString("Plot agreement plot").arg(name), this, SLOT(plotAgreementPlotData()));
						addSeparator = true;
					}
			}
#endif
#ifdef GWA
			if (mainGraphWidget->results->ANS_obs.nvars)
			{
				int index = -1;
				for (int i = 0; i<mainGraphWidget->results->ANS_obs.nvars; i++)
					if (mainGraphWidget->results->ANS_obs[i].name == name.toStdString())
					{
						plotModeledDataDot(mainGraphWidget->results->ANS_obs[i], data[0], QString("%1").arg(name));
						menu->addAction(QString("Plot Modeled Data").arg(name), this, SLOT(plotModeledDataDot()));

						QString dataName = obs->val("tracer") + '@' + obs->val("well");
						plotModeledData(mainGraphWidget->results->projected[dataName], data[0], QString("%1").arg(dataName));
						menu->addAction(QString("Plot Projected Data").arg(name), this, SLOT(plotModeledData()));


						plotAgreementPlotData(data[0], mainGraphWidget->results->ANS_obs[i].interpol(data[0]), QString("Agreement Plot %1").arg(name));
						menu->addAction(QString("Plot Agreement Plot").arg(name), this, SLOT(plotAgreementPlotData()));
						addSeparator = true;
					}
			}
#endif

			if (addSeparator) menu->addSeparator();
			if (mainGraphWidget->results)
			{
//				mainGraphWidget->log(obs->name);
				if (mainGraphWidget->results->hasRealization(obs->name))
				{
					plotRealization(mainGraphWidget->results->realization(obs->name), QString("Realization %1").arg(obs->name));
					menu->addAction(QString("Plot realization data").arg(obs->name), this, SLOT(plotRealization()));
					addSeparator = true;
				}
				if (mainGraphWidget->results->hasRealizationPercentile(obs->name))
				{
					plotRealizationPercentile(mainGraphWidget->results->realizationPercentile(obs->name), QString("Realization percentile %1").arg(obs->name));
					menu->addAction(QString("Plot prediction 95 percentile").arg(obs->name), this, SLOT(plotRealizationPercentile()));
					addSeparator = true;
				}
				if (mainGraphWidget->results->hasNoiseRealization(obs->name))
				{
					plotNoiseRealization(mainGraphWidget->results->noiseRealization(obs->name), QString("Realization (Noise) %1").arg(obs->name));
					menu->addAction(QString("Plot realization (Noise)").arg(obs->name), this, SLOT(plotNoiseRealization()));
					addSeparator = true;
				}
				if (mainGraphWidget->results->hasNoiseRealizationPercentile(obs->name))
				{
					plotNoiseRealizationPercentile(mainGraphWidget->results->noiseRealizationPercentile(obs->name), QString("Realization percentile (Noise) %1").arg(obs->name));
					menu->addAction(QString("Plot prediction 95 percentile (Noise)").arg(obs->name), this, SLOT(plotNoiseRealizationPercentile()));
					addSeparator = true;
				}

			}
			if (addSeparator) menu->addSeparator();
		}


		if (item->parent()->Name() == "Parameters" && mainGraphWidget->results)
		{
			bool addSeparator = false;
			Entity *par = mainGraphWidget->entity(name, "Parameter");
			if (mainGraphWidget->results->hasPercentile(par->name))
			{
				plotPercentileData(mainGraphWidget->results->percentile(par->name), par->name);
				menu->addAction(QString("Plot percentile data").arg(name), this, SLOT(plotPercentileData()));
				addSeparator = true;
			}
			if (mainGraphWidget->results->hasPrior(par->name))
			{
				plotPriorHistogram(mainGraphWidget->results->prior(par->name), QString ("Propr distribution of %1").arg(par->name));
				menu->addAction(QString("Plot prior distribution histogram").arg(name), this, SLOT(plotPriorHistogram()));
				addSeparator = true;
			}
			if (mainGraphWidget->results->hasPosterior(par->name))
			{
				plotPosteriorHistogram(mainGraphWidget->results->posterior(par->name), QString("Posterior distribution of %1").arg(par->name));
				menu->addAction(QString("Plot posterior distribution histogram").arg(name), this, SLOT(plotPosteriorHistogram()));
				addSeparator = true;
			}
			if (addSeparator) menu->addSeparator();
		}
		QModelIndex index = projectExplorer->indexAt(pos);
		removeProjectExplorerEntityItem(name, index); // model, type, index);
		menu->addAction(QString("Delete %1").arg(name), this, SLOT(removeProjectExplorerEntityItem()));
		menu->exec(projectExplorer->mapToGlobal(pos));
	}
	if (projectExplorer->indexAt(pos).data(Role::TreeItemType) == TreeItem::Type::ReactionNetworkItem)
	{
		QMenu *menu = new QMenu;
		menu->addAction(QString("Open reaction network window"), this, SLOT(openRXNWindow()));
		menu->exec(projectExplorer->mapToGlobal(pos));
	}
}
void MainWindow::tablePropShowContextMenu(const QPoint&pos)
{
	QModelIndex i1 = tableProp->indexAt(pos);
	int row = i1.row();
	QModelIndex i2 = i1.sibling(row, 1);
	
	if (i1.column() == 0)
	{
		QMenu *menu = new QMenu;
		int code = i1.data(DescriptionCodeRole).toInt();
		QString variableName = i1.data(VariableNameRole).toString();
		showHelp(code, variableName);
		menu->addAction("Help", this, SLOT(showHelp()));
		menu->addSeparator();
		if (i2.data(VariableTypeRole).toString().toLower().contains("getnumber"))
		{
			menu->addAction("Input number", this, SLOT(getNumber()));
			addParameterIndex(i1); // tableProp->indexAt(pos));
			menu->addSeparator();
		}
		QMenu *estimatesMenu = new QMenu("Parameters");
		menu->addMenu(estimatesMenu);
		estimatesMenu->setEnabled(false);
		if (i2.data(VariableTypeRole).toString().toLower().contains("estimate"))
		{
			for each (QString item in mainGraphWidget->EntityNames("Parameter"))
				estimatesMenu->addAction(QString("%1").arg(item));// , this, SLOT(addParameter()));
			addParameterIndex(i1); // tableProp->indexAt(pos));
			connect(estimatesMenu, SIGNAL(triggered(QAction*)), this, SLOT(addParameter(QAction*)));
			estimatesMenu->setEnabled(true);
		}
		menu->exec(tableProp->mapToGlobal(pos));
	}
	if (i1.column() == 1 && i1.data(TypeRole).toString().toLower().contains("time series"))
	{
		QStringList graphNames;
		QString file = i2.data().toString();
		QString fullfile = i2.data(Qt::ToolTipRole).toString();
		fullfile = fullFilename(fullfile, mainGraphWidget->modelPathname());
		//QString fullfile = fullFilename(QString("./%1").arg(file), modelPathname());
		CBTCSet data;
#ifdef WQV
		if (i1.data(TypeRole).toString().toLower().contains("precipitation"))
			data = CPrecipitation(fullfile.toStdString()).getflow(1);
		else
#endif
			data = CBTCSet(fullfile.toStdString(), 1);

		for each (string name in data.names)
		{
			if (!graphNames.contains(QString::fromStdString(name)))
				graphNames.append(QString::fromStdString(name));
			else mainGraphWidget->warning(QString("Duplicate header %1 in time series, file %1").arg(QString::fromStdString(name)).arg(file));
		}
		if (graphNames.count())
		{
			QMenu *menu = new QMenu;
			bool convertXtoTime = true;
			if (i1.data(TypeRole).toString().toLower().contains("age"))
				convertXtoTime = false;
			plotTimeSeries(0, CBTC(), "", true, true);
			for each (QString subTitle in graphNames)
			{
				QAction* action = menu->addAction(subTitle);
				plotTimeSeries(action, data[subTitle.toStdString()], subTitle, convertXtoTime);
			}
			connect(menu, SIGNAL(triggered(QAction*)), this, SLOT(plotTimeSeries(QAction*)));
			menu->exec(tableProp->mapToGlobal(pos));
		}
	}
}
void MainWindow::showHelp(int code, QString variableName)
{
	static int helpCode;
	static QString helpVariableName;

	if (code != -1)
	{
		helpCode = code;
		helpVariableName = variableName;
		return;
	}

	mainGraphWidget->help->showHelp(helpCode, helpVariableName.toStdString());
}
QModelIndex MainWindow::addParameterIndex(const QModelIndex &index)
{
	static QModelIndex parameterIndex;
	if (index != QModelIndex()) parameterIndex = index;
	return parameterIndex;
}
void MainWindow::addParameter(QAction* item)
{
	QString parameter = item->text();
	tableProp->model()->setData(addParameterIndex(), parameter, setParamRole);
}

void MainWindow::getNumber()
{
	double value = QInputDialog::getDouble(this, "Input Dialog Box", "Enter number:", 0, 0, 2147483647, 4);// item->text();

	tableProp->model()->setData(addParameterIndex(), value);
}


void MainWindow::plotTimeSeries(QAction* action, CBTC data, QString name, bool convertXtoTime, bool reset)
{
	struct graph
	{
		QAction* action;
		CBTC data;
		QString name;
		bool convertXtoTime;
	};
	static vector<graph>graphs;
	
	if (reset){
		graphs.clear();
		return;
	}
	
	if (data.n)
	{
		graph g;
		g.action = action;
		g.data = data;
		g.name = name;
		g.convertXtoTime = convertXtoTime;
		graphs.push_back(g);
	}
	else
	{
		for (int i = 0; i < graphs.size(); i++)
			if (action == graphs[i].action)
			{
				plotWindow *plot = new plotWindow(mainGraphWidget);
				plot->addScatterPlot(graphs[i].data, graphs[i].name, graphs[i].convertXtoTime, plotformat());
				plot->show();
				return;
			}
	}
}


/*void MainWindow::on_tableProp_customContextMenuRequested(const QPoint &pos)
{
	int i = 0;
	/*
	if (projectExplorer->indexAt(pos).data(Role::TreeItemType) == TreeItem::Type::Branch)
	{
		QMenu *menu = new QMenu;
		TreeModel *model = projModel;
		TreeItem *item = model->itemFromIndex(projectExplorer->indexAt(pos));
		QString type = projectExplorer->indexAt(pos).data().toString();
		QModelIndex index = projectExplorer->indexAt(pos);
		addProjectExplorerTreeItem(model, type, index);
		menu->addAction(QString("Add %1").arg(type), this, SLOT(addProjectExplorerTreeItem()));
		menu->exec(projectExplorer->mapToGlobal(pos));
	}
	if (projectExplorer->indexAt(pos).data(Role::TreeItemType) == TreeItem::Type::ReactionNetworkItem)
	{
		QMenu *menu = new QMenu;
		menu->addAction(QString("Open Reaction Network Window"), this, SLOT(openRXNWindow()));
		menu->exec(projectExplorer->mapToGlobal(pos));
	}
	
}*/

void MainWindow::addProjectExplorerTreeItem(TreeModel *model, const QString type,const QModelIndex index)
{
	static TreeModel *_model;
	static QString _type;
	static QModelIndex _index;
	if (model != 0)
	{
		_model = model;
		_type = type;
		_index = index;
		return;
	}
	else
	{
		_model->addChildFromMenu(_type, &_index);
		projectExplorer->setExpanded(_index, true);
	}
}

void MainWindow::removeProjectExplorerNodeItem(QString name, const QModelIndex index)
{
	static QString _name;
	static QModelIndex _index;

	if (!name.isEmpty())
	{
		_name = name;
		_index = index;
		return;
	}
	else
	{
		QModelIndex i = projectExplorer->indexAbove(_index.sibling(0, 0));
		projectExplorer->collapse(i);
		QModelIndex j = projectExplorer->indexBelow(i);
		bool connectors_expanded = projectExplorer->isExpanded(j);
		projectExplorer->collapse(j);
		projModel->deleteNode(_index.row(), i, _name);
		if (projModel->itemFromIndex(i)->childCount())
			projectExplorer->expand(i);

		if (connectors_expanded && projModel->connectors->childCount())
			projectExplorer->expand(j);
	}
}
void MainWindow::removeProjectExplorerEdgeItem(QString name, const QModelIndex index)
{
	static QString _name;
	static QModelIndex _index;

	if (!name.isEmpty())
	{
		_name = name;
		_index = index;
		return;
	}
	else
	{
		QModelIndex i = projectExplorer->indexAbove(_index.sibling(0, 0));
		projectExplorer->collapse(i);
		projModel->deleteEdge(_index.row(), i, _name);
		if (projModel->itemFromIndex(i)->childCount())
			projectExplorer->expand(i);
	}
}
void MainWindow::removeProjectExplorerEntityItem(QString name, const QModelIndex index)
{
	static QString _name;
	static QModelIndex _index;

	if (!name.isEmpty())
	{
		_name = name;
		_index = index;
		return;
	}
	else
	{
		QModelIndex i = projectExplorer->indexAbove(_index.sibling(0, 0));
		projectExplorer->collapse(i);
		QString type = mainGraphWidget->entity(_name)->objectType.ObjectType;
		projModel->deleteEntity(_index.row(), i, type, _name);
		if (projModel->itemFromIndex(i)->childCount())
			projectExplorer->expand(i);
	}
}
void MainWindow::plotPriorHistogram(CBTC histogram, QString name)
{
	static CBTC data;
	static QString _name;
	if (histogram.n)
	{
		data = histogram;
		_name = name;
		return;
	}
	else
	{
		plotWindow *plot = new plotWindow(mainGraphWidget);
		plot->addHistogramPlot(_name, data.t, data.C);
		plot->show();
	}
}
void MainWindow::plotPosteriorHistogram(CBTC histogram, QString name)
{
	static CBTC data;
	static QString _name;
	if (histogram.n)
	{
		data = histogram;
		_name = name;
		return;
	}
	else
	{
		plotWindow *plot = new plotWindow(mainGraphWidget);
		plot->addHistogramPlot(_name, data.t, data.C);
		plot->show();
	}
}
void MainWindow::plotAllPercentileData(vector<percentileData> data, QString name)
{
	static vector<percentileData> _data;
	static QString _name;
	if (data.size())
	{
		_data = data;
		_name = name;
		return;
	}
	else
	{
		plotWindow *plot = new plotWindow(mainGraphWidget);
		for (int i = 0; i < _data.size(); i++)
		{
			QMap<QString, double> mapData;
			mapData["p25"] = _data[i].p25;
			mapData["p50"] = _data[i].p50;
			mapData["p975"] = _data[i].p975;
			mapData["mean"] = _data[i].mean;

			plot->addPercentilePlot(QString::fromStdString(_data[i].parameter), mapData);
		}
		plot->show();
	}
}

void MainWindow::plotPercentileData(percentileData data, QString name)
{
	static percentileData _data;
	static QString _name;
	if (data.parameter !="")
	{
		_data = data;
		_name = name;
		return;
	}
	else
	{
		QMap<QString, double> mapData;
		mapData["p25"] = _data.p25;
		mapData["p50"] = _data.p50;
		mapData["p975"] = _data.p975;
		mapData["mean"] = _data.mean;

		plotWindow *plot = new plotWindow(mainGraphWidget);
		plot->addPercentilePlot(_name, mapData);
		plot->show();
	}
}

void MainWindow::plotObservationData(CBTC data, QString name)
{
	static CBTC _data;
	static QString _name;
	if (data.n)
	{
		_data = data;
		_name = name;
		return;
	}
	else
	{
		plotWindow *plot = new plotWindow(mainGraphWidget);
		bool convertTime = true;
#ifdef GWA
		convertTime = false;
#endif
		plot->addScatterPlot(_data, _name + "(Observation)", convertTime);
		plot->show();
	}
}
void MainWindow::plotModeledData(CBTC modeled, CBTC observed, QString _name)
{
	static CBTC model, obs;
	static QString name;
	if (modeled.n)
	{
		model = modeled;
		obs = observed;
		name = _name;
		return;
	}
	else
	{
		plotWindow *plot = new plotWindow(mainGraphWidget);
		plotformat format, format2;
		format.penStyle = Qt::SolidLine;
		format.lineStyle = QCPGraph::LineStyle::lsNone;
		format.color = Qt::blue;
		format.scatterStyle = QCPScatterStyle::ssPlusCircle;
		bool convertTime = true;
#ifdef GWA
		convertTime = false;
#endif
		plot->addScatterPlot(obs, name + "(Observed)", convertTime, format);
		
		format2.penStyle = Qt::SolidLine;
		format2.scatterStyle = QCPScatterStyle::ssNone;
		plot->addScatterPlot(model, name, convertTime, format2);
		plot->show();
	}
}
void MainWindow::plotModeledDataDot(CBTC modeled, CBTC observed, QString _name)
{
	static CBTC model, obs;
	static QString name;
	if (modeled.n)
	{
		model = modeled;
		obs = observed;
		name = _name;
		return;
	}
	else
	{
		plotWindow *plot = new plotWindow(mainGraphWidget);
		plotformat format, format2;
		format.penStyle = Qt::SolidLine;
		format.lineStyle = QCPGraph::LineStyle::lsNone;
		format.color = Qt::blue;
		format.scatterStyle = QCPScatterStyle::ssPlusCircle;
		bool convertTime = true;
#ifdef GWA
		convertTime = false;
#endif
		plot->addScatterPlot(obs, name + "(Observation)", convertTime, format);
	
		format2.penStyle = Qt::SolidLine;
		format2.scatterStyle = QCPScatterStyle::ssPlusCircle;
		plot->addScatterPlot(model, name + "(Modeled)", convertTime, format2);
		plot->show();
	}
}
void MainWindow::plotAgreementPlotData(CBTC observation, CBTC modeled, QString name)
{
	static CBTC _obs;
	static CBTC _mod;
	static QString _name;
	if (modeled.n)
	{
		_obs = observation;
		_mod = modeled;
		_name = name;
		return;
	}
	else
	{
		plotWindow *plot = new plotWindow(mainGraphWidget);
		map<string, double> reg = regression(_obs.C, _mod.C);
		if (reg["error"] == 0)
		{
			plotformat format, format2;
			format.lineStyle = QCPGraph::LineStyle::lsNone;
			format.scatterStyle = QCPScatterStyle::ssCross; 
			format.color = Qt::blue;
			format.xAxisLabel = "Observation";
			format.yAxisLabel = "Modeled";

			plot->addDotPlot(_obs.C, _mod.C, "Agreement Plot", format);
			CBTC regLine(reg["a"], reg["b"], _obs.C);
			format2.scatterStyle = QCPScatterStyle::ssNone;
			plot->addScatterPlot(regLine, "Agreement regression", false, format2);
			plot->show();
		}
	}
}

void MainWindow::plotRealization(CBTCSet data, QString name)
{
	qDebug() << "called";
	static CBTCSet _data;
	static QString _name;
	qDebug() << data.nvars;
	if (data.nvars)
	{
		_data = data;
		_name = name;
		return;
	}
	else
	{
		plotWindow *plot = new plotWindow(mainGraphWidget);
		plotformat format;
		format.legend = false;
		format.penWidth = 1;
		bool convertTime = true;
#ifdef GWA
		convertTime = false;
#endif
		for (int i = 0; i < _data.nvars; i++)
		{
			plot->addScatterPlot(_data[i], QString::fromStdString(_data[i].name), convertTime, format);
		}
		plot->show();
	}
}

void MainWindow::plotNoiseRealization(CBTCSet data, QString name)
{
	static CBTCSet _data;
	static QString _name;
	if (data.nvars)
	{
		_data = data;
		_name = name;
		return;
	}
	else
	{
		plotWindow *plot = new plotWindow(mainGraphWidget);
		bool convertTime = true;
#ifdef GWA
		convertTime = false;
#endif
		for (int i = 0; i < _data.nvars; i++)
		{
			plot->addScatterPlot(_data[i], QString::fromStdString(_data[i].name), convertTime);
		}
		plot->show();
	}
}
void MainWindow::plotRealizationPercentile(CBTCSet data, QString name)
{
	static CBTCSet _data;
	static QString _name;
	if (data.nvars)
	{
		_data = data;
		_name = name;
		return;
	}
	else
	{
		plotWindow *plot = new plotWindow(mainGraphWidget);
		QCPGraph * g1, * g2;
		bool convertTime = true;
#ifdef GWA
		convertTime = false;
#endif

		for (int i = 0; i < _data.nvars; i++)
		{
			plotformat format;
			format.penWidth = 1;
			format.color = Qt::GlobalColor(i);
			
			if (i == _data.nvars - 1)
				format.fillGraph = g1;
			g2 = plot->addScatterPlot(_data[i], QString::fromStdString(_data[i].name), convertTime, format);
			if (i == 1)
				g1 = g2;
		}
		plot->show();
	}
}
void MainWindow::plotNoiseRealizationPercentile(CBTCSet data, QString name)
{
	static CBTCSet _data;
	static QString _name;
	if (data.nvars)
	{
		_data = data;
		_name = name;
		return;
	}
	else
	{
		plotWindow *plot = new plotWindow(mainGraphWidget);
//		for (int i = 0; i < _data.nvars; i++)
//		{
//			plot->addScatterPlot(_data[i], QString::fromStdString(_data[i].name), true);
//		}
		bool convertTime = true;
#ifdef GWA
		convertTime = false;
#endif

		plot->addScatterPlot(_data, convertTime);
		plot->show();
	}
}
void MainWindow::showLocalSensitivityMatrix()
{
	vector<string> rowHeaders, colHeaders;
	for (int i = 0; i < mainGraphWidget->modelSet->measured_quan.size(); i++)
		colHeaders.push_back(mainGraphWidget->modelSet->measured_quan[i].name);
	for (int i = 0; i < mainGraphWidget->modelSet->parameters.size(); i++)
		rowHeaders.push_back(mainGraphWidget->modelSet->parameters[i].name);
	
//	logWindow *matrixW = new logWindow(this, "Local Sensitivity Matrix", "Matrix (*.csv)");
//	matrixW->append(mainGraphWidget->results->localSensitivityMatrix.toString("", colHeaders, rowHeaders));
//	matrixW->append(mainGraphWidget->results->localSensitivityMatrix.toHtml("", colHeaders, rowHeaders));
//	matrixW->show();
	logWindow *matrixW2 = new logWindow(this, "Local Sensitivity Matrix", "Matrix (*.csv)");
	matrixW2->setHtml(mainGraphWidget->results->localSensitivityMatrix.toHtml("", colHeaders, rowHeaders));
	matrixW2->show();
}
void MainWindow::showGlobalSensitivityMatrix()
{
	vector<string> rowHeaders, colHeaders;
	for (int i = 0; i < mainGraphWidget->modelSet->measured_quan.size(); i++)
		colHeaders.push_back(mainGraphWidget->modelSet->measured_quan[i].name);
	for (int i = 0; i < mainGraphWidget->modelSet->parameters.size(); i++)
		rowHeaders.push_back(mainGraphWidget->modelSet->parameters[i].name);

//	logWindow *matrixW = new logWindow(this, "Global Sensitivity Matrix", "Matrix (*.csv)");
//	matrixW->append(mainGraphWidget->results->globalSensitivityMatrix.toString("", colHeaders, rowHeaders));
//	matrixW->append(mainGraphWidget->results->globalSensitivityMatrix.toHtml("", colHeaders, rowHeaders));
//	matrixW->append(mainGraphWidget->results->globalSensitivityMatrix.toHtml("", colHeaders, rowHeaders));
//	matrixW->show();
	logWindow *matrixW2 = new logWindow(this, "Global Sensitivity Matrix", "Matrix (*.csv)");
	matrixW2->setHtml(mainGraphWidget->results->globalSensitivityMatrix.toHtml("", colHeaders, rowHeaders));
	matrixW2->show();
}
void MainWindow::showCorrelationMatrix()
{
	vector<string> headers;
	for (int i = 0; i < mainGraphWidget->modelSet->parameters.size(); i++)
		headers.push_back(mainGraphWidget->modelSet->parameters[i].name);

//	logWindow *matrixW = new logWindow(this, "Correlation Matrix", "Matrix (*.csv)");
//	matrixW->append(mainGraphWidget->results->correlationMatrix.toString("", headers, headers));
//	matrixW->show();
	logWindow *matrixW2 = new logWindow(this, "Correlation Matrix", "Matrix (*.csv)");
	matrixW2->setHtml(mainGraphWidget->results->correlationMatrix.toHtml("", headers, headers));
	matrixW2->show();
}


void MainWindow::setModelFileName(const QString &fileName) 
{
	mainGraphWidget->modelFilename = fileName;
	setWindowTitle(QString("%1 - %2").arg(OnlyFilename(fileName)).arg(applicationName));
}

QString MainWindow::modelPathname()
{
	return mainGraphWidget->modelPathname();
}
void MainWindow::on_actionlogWindow_triggered()
{
	log->show();
}

MainWindow::~MainWindow()
{
	delete mainGraphWidget->model;
	delete mainGraphWidget->results;
	delete	mainGraphWidget;
//	delete projModel;
	std::exit(0);
	delete ui;

}

void MainWindow::paintEvent(QPaintEvent *e)
{
	//QPainter painter(mainGraphWidget);
	//painter.drawLine(0, 0, 100, 100);
}

void MainWindow::on_actionAdd_Pond_triggered()
{
	//add Pond
	static int Pond_Blocks = 1;
	new Node(mainGraphWidget, "Pond", QString("Pond (%1)").arg(Pond_Blocks++), -1, newBlockX(), newBlockY());
}

void MainWindow::on_actionAdd_Soil_triggered()
{
	//add Soil
	static int Soil_Blocks = 1;

	new Node(mainGraphWidget, "Soil", QString("Soil (%1)").arg(Soil_Blocks++), -1, newBlockX(), newBlockY());


}

void MainWindow::on_actionAdd_Catchment_Area_triggered()
{
	static int Catchment_Blocks = 1;

	new Node(mainGraphWidget, "Catchment", QString("Catchment (%1)").arg(Catchment_Blocks++), -1, newBlockX(), newBlockY());


}

void MainWindow::on_actionAdd_Darcy_Block_triggered()
{
	static int Darcy_Blocks = 1;

	new Node(mainGraphWidget, "Darcy", QString("Darcy (%1)").arg(Darcy_Blocks++), -1, newBlockX(), newBlockY());

}

void MainWindow::on_actionAdd_Stora_ge_triggered()
{
	static int Storage_Blocks = 1;

	new Node(mainGraphWidget, "Storage", QString("Storage (%1)").arg(Storage_Blocks++), -1, newBlockX(), newBlockY());

}

void MainWindow::on_actionAdd_St_ream_triggered()
{
	static int Stream_Blocks = 1;

	new Node(mainGraphWidget, "Stream", QString("Stream (%1)").arg(Stream_Blocks++), -1, newBlockX(), newBlockY());

}

void MainWindow::on_actionAdd_Connector_triggered(bool checked)
{
	//Add Connector
	if (checked)
	{
		ui->action_Select->setChecked(false);
		ui->action_Pan->setChecked(false);
		mainGraphWidget->setMode(Operation_Modes::Draw_Connector);
	}
	else
		updateToolbarToggles(mainGraphWidget->setMode(1));
}

bool MainWindow::saveModel(QString &fileName)
{
	if (fileName.isEmpty())
		return false;
	else {
		QFile file(fileName);
		if (!file.open(QIODevice::WriteOnly)) {
			QMessageBox::information(this, tr("Unable to open file"),
				file.errorString());
			return false;
		}
		QDataStream out(&file);
		out.setVersion(QDataStream::Qt_4_5);
#ifdef WQV
		out << QString("GIFMod Data File version 1.2"); //1.1 with defaultUnit // 1.2 Multi experiment
#endif
#ifdef GWA
		out << QString("GWA Data File version 1.2"); // with defaultUnit
#endif
		//		modelConfig mC(mainGraphWidget);
		out << mainGraphWidget->compact();
		setModelFileName(fileName);
		mainGraphWidget->changedState = false;
		return true;
	}
}


void MainWindow::on_actionClear_Reaction_Network_triggered()
{
	if (QMessageBox::question(this, tr("Clear Reaction Network"),
		"All Constituents, Reaction parameters, and Reaction Network will be deleted.\nAre you sure?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::No)
		return;

	mainGraphWidget->clearRXN();
}

void MainWindow::on_actionLoad_Reaction_Network_triggered()
{
	//open
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("Load Reaction Network"), mainGraphWidget->modelPathname(),
		tr("Reaction Network (*.RXN);;All Files (*)"));
	if (fileName.isEmpty())
		return;
	else {

		QFile file(fileName);

		if (!file.open(QIODevice::ReadOnly)) {
			QMessageBox::information(this, tr("Unable to open file"),
				file.errorString());
			return;
		}

		QDataStream in(&file);
		in.setVersion(QDataStream::Qt_4_5);
		QString fileHeader;
		in >> fileHeader;
		if (fileHeader != "RXN Data File version 1.0")
		{
			QMessageBox::information(this, tr("Unable to open file"),
				"Wrong Data file of the file is corrupted.");
			return;
		}
		QList<QMap<QString, QVariant>> data;
		in >> data;
		//		mainGraphWidget->clear();
		mainGraphWidget->unCompact(data);
		//		expandNode(projModel->index(-1, -1), true);
		//		else {
		//			QMap<QString, QString>::iterator i = contacts.begin();
		//			nameLine->setText(i.key());
		//			addressText->setText(i.value());
		//		}
	}

	//	updateInterface(NavigationMode);

}
void MainWindow::on_actionSave_Reaction_Network_triggered()
{
//	if (!mainGraphWidget->allowRun)
//		return;

	QString fileName = QFileDialog::getSaveFileName(this,
		tr("Save Reaction Network"), mainGraphWidget->modelPathname(),
		tr("Reaction Network (*.RXN);;All Files (*)"));
	if (fileName.isEmpty())
		return;
	else {
		QFile file(fileName);
		if (!file.open(QIODevice::WriteOnly)) {
			QMessageBox::information(this, tr("Unable to open file"),
				file.errorString());
			return;
		}
		QDataStream out(&file);
		out.setVersion(QDataStream::Qt_4_5);
		out << QString("RXN Data File version 1.0");
		//		modelConfig mC(mainGraphWidget);
		out << mainGraphWidget->compactRXN();

	}


}

void MainWindow::on_actionNewExperiment_triggered()
{
	//QStringList globalEntitiesList;
	//globalEntitiesList << "Observarion" << "Parameter" << "External flux" << ""
	addExperiment();
}
void MainWindow::on_actionCopyFromCurrentExperiment_triggered()
{
	addExperiment(mainGraphWidget->experimentName());
}
void MainWindow::addExperiment(QString sourceExperiment)
{
	int counter = mainGraphWidget->experimentsList().count() + 1;
	QString name = QString("experiment%1").arg(counter);
	mainGraphWidget->experiments->addItem(name);
	mainGraphWidget->log(QString("%1 added to the model").arg(name));
	if (sourceExperiment.size())
	{
		mainGraphWidget->copyProps(sourceExperiment, name);
		mainGraphWidget->log(QString("All properties and settings copied from %1 to %2").arg(sourceExperiment).arg(name));
	}
	else
		for each (Entity *e in mainGraphWidget->Entities)
			e->copyProps(mainGraphWidget->experimentsList()[0], name);

	mainGraphWidget->experiments->setCurrentText(name);
}


void MainWindow::openRXNWindow()
{
#ifdef WQV
	QMainWindow *rxnW = new ReactionWindow(mainGraphWidget, this);
	rxnW->show();
#endif
}

void MainWindow::on_actionRun_Model_triggered()
{
	if (!mainGraphWidget->allowRun)
	{
		statusBar()->showMessage("Unable to run Model.");
		mainGraphWidget->log(QString("Unable to run Model, the value of [%1] in property dialog has not been confirmed.").arg(mainGraphWidget->allowRunVariableName));
		mainGraphWidget->allowRun = true;
		return;
	}
	if (!mainGraphWidget->Nodes().size())
	{
		statusBar()->showMessage("Unable to run Model.");
		mainGraphWidget->log("The model should have at least one block to run.");
		return;
	}
#ifdef WQV
	QString fileName = mainGraphWidget->modelFilename;
	mainGraphWidget->modelFilename.replace(QString(".").append(fileExtension), ".temp");
	on_action_Save_triggered();
	if (fileName != "") setModelFileName(fileName);
	QString statusBarText = statusBar()->currentMessage();
	mainGraphWidget->log("Checking for Errors.");
	statusBar()->showMessage("Checking for Errors.");
	//QProgressBar *progress = new QProgressBar(statusBar());
	//progress->setAlignment(Qt::AlignRight);
	//progress->setMaximumSize(180, 19);
	//statusBar()->addPermanentWidget(progress);
	setCursor(Qt::WaitCursor);
	mainGraphWidget->logW->writetotempfile();

	QStringList result = mainGraphWidget->variableValuesHasError();
	QString logMsg;
	mainGraphWidget->log("-------------------------------------------");
	logMsg.append(QString("====   %1 Errors, %2 Warnings   ====").arg(result[0]).arg(result[1]));
	mainGraphWidget->log(logMsg);
	if (result[0].toInt()>0)
	{
		setCursor(Qt::ArrowCursor);
		statusBar()->showMessage("Unable to run Model.");
		mainGraphWidget->log("Faild to run the Model.");
		mainGraphWidget->deselectAll();
		return;
	}
	mainGraphWidget->log("Assembling model configuration.");
	statusBar()->showMessage("Assembling model configuration.");
	mainGraphWidget->logW->writetotempfile();
	QCoreApplication::processEvents();
	runtimeWindow *rtw = new runtimeWindow(mainGraphWidget);
	mainGraphWidget->deleteSolution();
	mainGraphWidget->modelSet = new CMediumSet(mainGraphWidget, rtw);
	rtw->show();
	mainGraphWidget->log("Running Simulation.");
	statusBar()->showMessage("Running Simulation.");
	forwardRun(mainGraphWidget->modelSet, rtw);
	//delete progress;
	setCursor(Qt::ArrowCursor);
//	mainGraphWidget->log(QString::fromStdString(mainGraphWidget->model->fail_reason));
	statusBar()->showMessage("Done.");
	//	QFileDialog::getOpenFileName(this, "Output Files", QString::fromStdString(mainGraphWidget->model->pathname));
#endif
#ifdef GWA
	QString fileName = mainGraphWidget->modelFilename;
	mainGraphWidget->modelFilename.replace(QString(".").append(fileExtension), ".temp");
	on_action_Save_triggered();
	if (fileName != "") setModelFileName(fileName);
	QString statusBarText = statusBar()->currentMessage();
	mainGraphWidget->log("Checking for Errors.");
	statusBar()->showMessage("Checking for Errors.");
	setCursor(Qt::WaitCursor);
	mainGraphWidget->logW->writetotempfile();

	QStringList result = mainGraphWidget->variableValuesHasError();
	QString logMsg;
	mainGraphWidget->log("-------------------------------------------");
	logMsg.append(QString("====   %1 Errors, %2 Warnings   ====").arg(result[0]).arg(result[1]));
	mainGraphWidget->log(logMsg);
	if (result[0].toInt()>0)
	{
		setCursor(Qt::ArrowCursor);
		statusBar()->showMessage("Unable to run Model.");
		mainGraphWidget->log("Faild to run the Model.");
		mainGraphWidget->deselectAll();
		return;
	}
	mainGraphWidget->log("Assembling model configuration.");
	statusBar()->showMessage("Assembling model configuration.");
	QCoreApplication::processEvents();
	runtimeWindow *rtw = new runtimeWindow(mainGraphWidget);
	mainGraphWidget->deleteSolution();
	mainGraphWidget->model = new CGWA(mainGraphWidget, rtw);
	mainGraphWidget->results = new Results;
	//rtw->show();
	mainGraphWidget->log("Running Simulation.");
	statusBar()->showMessage("Running Simulation.");
	forwardRun(mainGraphWidget->model, rtw);
	//delete progress;
	setCursor(Qt::ArrowCursor);
	mainGraphWidget->log("Simulation ended.");
	statusBar()->showMessage("Done.");
	//	QFileDialog::getOpenFileName(this, "Output Files", QString::fromStdString(mainGraphWidget->model->pathname));
#endif
}

void MainWindow::on_actionRun_Model_from_Script_triggered()
{
	if (!mainGraphWidget->allowRun)
	{
		statusBar()->showMessage("Unable to run Model.");
		mainGraphWidget->log(QString("Unable to run Model, the value of [%1] in property dialog has not been confirmed.").arg(mainGraphWidget->allowRunVariableName));
		return;
	}

#ifdef WQV
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("Open Green InfraStructure Script Text"), "",
		tr("Script Text (*.txt);;All Files (*)"));
	if (fileName == "") return;
	mainGraphWidget->modelSet = new CMediumSet(fileName.toStdString());
	//CMedium *model2 = new CMedium(mainGraphWidget);

	forwardRun(mainGraphWidget->modelSet, new runtimeWindow);
#endif
}
void MainWindow::on_actionRun_Inverse_Model_triggered()
{
	if (!mainGraphWidget->allowRun)
	{
		statusBar()->showMessage("Unable to run Model.");
		mainGraphWidget->log(QString("Unable to run Model, the value of [%1] in property dialog has not been confirmed.").arg(mainGraphWidget->allowRunVariableName));
		mainGraphWidget->allowRun = true;
		return;
	}

#ifdef WQV
	QString fileName = mainGraphWidget->modelFilename;
	mainGraphWidget->modelFilename.replace(QString(".").append(fileExtension), ".temp");
	on_action_Save_triggered();
	if (fileName != "") setModelFileName(fileName);
	QString statusBarText = statusBar()->currentMessage();
	if (!mainGraphWidget->EntityNames("Parameter").size())
	{
		mainGraphWidget->log("There is no Parameter defined, at least one parameter should be defined to inverse run the model.");
		setCursor(Qt::ArrowCursor);
		statusBar()->showMessage("Unable to run Model.");
		mainGraphWidget->log("Faild to run the Model.");
		mainGraphWidget->deselectAll();
		return;
	}
	mainGraphWidget->log("Checking for Errors.");
	statusBar()->showMessage("Checking for Errors.");
	//QProgressBar *progress = new QProgressBar(statusBar());
	//progress->setAlignment(Qt::AlignRight);
	//progress->setMaximumSize(180, 19);
	//statusBar()->addPermanentWidget(progress);
	setCursor(Qt::WaitCursor);
	mainGraphWidget->logW->writetotempfile();

	QStringList result = mainGraphWidget->variableValuesHasError();
	QString logMsg;
	mainGraphWidget->log("-------------------------------------------");
	logMsg.append(QString("====   %1 Errors, %2 Warnings   ====").arg(result[0]).arg(result[1]));
	mainGraphWidget->log(logMsg);
	if (result[0].toInt()>0)
	{
		setCursor(Qt::ArrowCursor);
		statusBar()->showMessage("Unable to run Model.");
		mainGraphWidget->log("Faild to run the Model.");
		mainGraphWidget->deselectAll();
		return;
	}
	mainGraphWidget->log("Assembling model configuration.");
	statusBar()->showMessage("Assembling model configuration.");
	mainGraphWidget->logW->writetotempfile();
	QCoreApplication::processEvents();
	runtimeWindow *rtw = new runtimeWindow(mainGraphWidget, "inverse");
	mainGraphWidget->deleteSolution();
	mainGraphWidget->modelSet = new CMediumSet(mainGraphWidget, rtw);
	rtw->show();
	mainGraphWidget->log("Running Simulation.");
	statusBar()->showMessage("Running Simulation.");
	mainGraphWidget->logW->writetotempfile();
	inverseRun(mainGraphWidget->modelSet, rtw);
	//delete progress;
	setCursor(Qt::ArrowCursor);
	mainGraphWidget->log("Simulation ended.");
	statusBar()->showMessage("Done.");
	//	QFileDialog::getOpenFileName(this, "Output Files", QString::fromStdString(mainGraphWidget->model->pathname));
#endif
#ifdef GWA
	QString fileName = mainGraphWidget->modelFilename;
	mainGraphWidget->modelFilename.replace(QString(".").append(fileExtension), ".temp");
	on_action_Save_triggered();
	if (fileName != "") setModelFileName(fileName);
	QString statusBarText = statusBar()->currentMessage();
	mainGraphWidget->log("Checking for Errors.");
	statusBar()->showMessage("Checking for Errors.");
	setCursor(Qt::WaitCursor);
	mainGraphWidget->logW->writetotempfile();

	QStringList result = mainGraphWidget->variableValuesHasError();
	QString logMsg;
	mainGraphWidget->log("-------------------------------------------");
	logMsg.append(QString("====   %1 Errors, %2 Warnings   ====").arg(result[0]).arg(result[1]));
	mainGraphWidget->log(logMsg);
	mainGraphWidget->logW->writetotempfile();
	if (result[0].toInt()>0)
	{
		setCursor(Qt::ArrowCursor);
		statusBar()->showMessage("Unable to run Model.");
		mainGraphWidget->log("Faild to run the Model.");
		mainGraphWidget->deselectAll();
		return;
	}
	mainGraphWidget->log("Assembling model configuration.");
	statusBar()->showMessage("Assembling model configuration.");
	mainGraphWidget->logW->writetotempfile();
	QCoreApplication::processEvents();
	runtimeWindow *rtw = new runtimeWindow(mainGraphWidget, "inverse");
	mainGraphWidget->deleteSolution();
	mainGraphWidget->model = new CGWA(mainGraphWidget, rtw);
	rtw->show();
	mainGraphWidget->log("Running Simulation.");
	statusBar()->showMessage("Running Simulation.");
	mainGraphWidget->logW->writetotempfile();
	inverseRun(mainGraphWidget->model, rtw);
	//delete progress;
	setCursor(Qt::ArrowCursor);
	mainGraphWidget->log("Simulation ended.");
	statusBar()->showMessage("Done.");
	mainGraphWidget->logW->writetotempfile();
	//	QFileDialog::getOpenFileName(this, "Output Files", QString::fromStdString(mainGraphWidget->model->pathname));

#endif
}
void MainWindow::on_action_Hydraulic_Outputs_triggered()
{
	plotWindow *plot = new plotWindow(mainGraphWidget);
	// ************** plot->addScatterPlot(mainGraphWidget->model->ANS, 0);// model->ANS.names[0], model->ANS.BTC[0].t, model->ANS.BTC[0].C);
	//model->ANS;
	plot->show();
}

void MainWindow::on_actionProjectSetting_triggered()
{
	Entity * e = mainGraphWidget->entityByName("Project setting");
	if (e)
		e->setSelected(true);
}

void MainWindow::on_actionClimateSetting_triggered()
{
	Entity * e = mainGraphWidget->entityByName("Climate setting");
	if (e)
		e->setSelected(true);
}

void MainWindow::on_actionSolverSetting_triggered()
{
	Entity * e = mainGraphWidget->entityByName("Solver setting");
	if (e)
		e->setSelected(true);
}


void MainWindow::on_actionExport_to_Script_Language_triggered()
{
	mainGraphWidget->log("Generating Script from Model");
	mainGraphWidget->log("Checking for Errors.");
	statusBar()->showMessage("Checking for Errors.");
	setCursor(Qt::WaitCursor);
	mainGraphWidget->logW->writetotempfile();
	QStringList result = mainGraphWidget->variableValuesHasError();
	QString logMsg;
	mainGraphWidget->log("-------------------------------------------");
	logMsg.append(QString("====   %1 Errors, %2 Warnings   ====").arg(result[0]).arg(result[1]));
	mainGraphWidget->log(logMsg);

	logWindow *scriptW = new logWindow(this, "Generated Script", "Script (*.txt)");
	scriptW->append(mainGraphWidget->script());
	scriptW->show();
	mainGraphWidget->log("Script Generated.");
	mainGraphWidget->log(logMsg);
	setCursor(Qt::ArrowCursor);

}
void MainWindow::on_actionAbout_triggered()
{
	QString ver;
#ifdef WQV
	ver = "Version 0.0.76";
#endif
#ifdef GWA
	ver = "Version 0.0.5";
#endif
	QMessageBox::information(this, "About " + applicationName,
		ver);
}
void MainWindow::gwidgetChanged()
{
	if (windowTitle().right(1) != "*")
		setWindowTitle(windowTitle() + "*");
	//if (mainGraphWidget->undo_counter < 1) 
}
