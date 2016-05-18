#pragma once
#include "gwidget.h"
#include "edge.h"
#include "node.h"
#include <math.h>
#include <QKeyEvent>
#include <qdebug.h>
//#include <mainwindow.h>
#include <vector>
#include "ray.h"
#include "entity.h"
#include "qtableview.h"
#include "treemodel.h"

#include "qmenu.h"
#include <QGraphicsSceneContextMenuEvent>
#include "plotWindow.h"
#include "qinputdialog.h"
#include "qtreeview.h"
#include "process.h"
#include "gridWindow.h"
#include "XString.h"
#include "treemodel.h"
#include <QCoreApplication>
#include "results.h"
#include "helpWindow.h"
#include "qcalendarwidget.h"
#include "PropModel.h"


#ifdef WQV
#include "medium.h"
#include "mainwindow.h"
#include "ui_mainwindowWQV.h"
#include "MediumSet.h"
#endif
#ifdef GWA
#include "gwa.h"
#endif


void Node::update(bool fast)
{
	if (!fast)
	{
		TreeModel* tmodel = static_cast<TreeModel *>(parent->projectExplorer->model());
		tmodel->update();
//		if (isSelected())
//		{
			//PropModel<Node>* propModel = static_cast<PropModel<Node>*>(parent->propModel());
			//propModel->update();
	//		model->update();
	//	}
	}
	QGraphicsItem::update();
};
void Edge::update(bool fast)
{
	if (!fast)
	{
		TreeModel* tmodel = static_cast<TreeModel *>(parent->projectExplorer->model());
		tmodel->update();
//		if (isSelected())
//		{
//			PropModel<Edge>* propModel = static_cast<PropModel<Edge>*>(parent->propModel());
//			propModel->update();
//			model->update();
//
//		}
	}
	QGraphicsItem::update();
};
void Entity::update(bool fast)
{
	if (!fast)
	{
		TreeModel* model = static_cast<TreeModel *>(parent->projectExplorer->model());
		model->update();
	}
//	QGraphicsItem::update();
};

GraphWidget::GraphWidget(QWidget *_parent, QString applicationShortName, QString metafilename, logWindow *logwindow, MainWindow* mainWindow)
	: QGraphicsView(_parent), timerId(0)
{
	parent = _parent;
	this->mainWindow = mainWindow;
	this->applicationShortName = applicationShortName;
	logW = logwindow;

#ifdef WQV
	help = new helpWindow(this, "Help", "GIFModHelp.txt");
#endif
#ifdef GWA
	help = new helpWindow(this, "Help", "GWAHelp.txt");
#endif
	MainGraphicsScene = new QGraphicsScene(this);
	MainGraphicsScene->setItemIndexMethod(QGraphicsScene::NoIndex);
	MainGraphicsScene->setSceneRect(0, 0, 2000, 2000);
	setScene(MainGraphicsScene);
	setCacheMode(CacheBackground);
	setViewportUpdateMode(BoundingRectViewportUpdate);
	setRenderHint(QPainter::Antialiasing);
	setTransformationAnchor(AnchorUnderMouse);
	undo_counter = 0;
	//add_to_undo_list();
//	QObject::connect(MainGraphicsScene, SIGNAL(changed(const QList<QRectF>)), this, SLOT(dadd_to_undo_list()));
	QObject::connect(MainGraphicsScene, SIGNAL(changed(const QList<QRectF>)), this, SLOT(sceneChanged()));
	ModelSpace = mProp('*');
	mList = new mPropList;
	switch (mList->getconfigfromfile(metafilename)){
	case mListReadStatus::fileNotValid:
		log(QString("%1 metaFile Not valid.").arg(metafilename));
		break;
	case mListReadStatus::errorInContents:
		log(QString("%1 metaFile contents has error.").arg(metafilename));
		break;
	case mListReadStatus::readSuccessfully:
		log(QString("%1 metaFile %2 records read successfully.").arg(metafilename).arg(mList->size()));
		break;
	}
	functionList.clear();
	functionList << "exp" << "hsd" << "min" << "max" << "lne" << "lnt" << "sgm" << "pos" << "sq1" << "sqr" // functions
		<< "frs" << "fas" << "ply" << "mon" << "sq2" << "abs";
	
	PhysicalCharacteristicsList.clear();
	QStringList tempPhysicalCharacteristicsList;
	tempPhysicalCharacteristicsList<< "a" << "h" << "v" << "s" << "z0" << "se" << "~theta" << "porosity" << "depression" << "vapor_diff" << "ks" << "~thetas" << "~thetar" << "~alphavg" << "vg_n" << "vg_m" <<
		"~lambda" << "sc" << "bulk_density" << "~epsilonstorage" << "storage_n" << "temperature" << "light";
	for each(QString item in tempPhysicalCharacteristicsList)
		PhysicalCharacteristicsList.append(XString(item));
	QList <mProp> QL1 = mList->List;

	ModelSpace.Model = mList->Models()[0];
#ifdef WQV
	new Entity("Solver setting", "Solver setting", this);
	new Entity("Project setting", "Project setting", this);
	new Entity("Climate setting", "Climate setting", this);
	new Entity("Genetic algorithm", "Genetic algorithm", this);
	new Entity("Markov chain Monte Carlo", "Markov chain Monte Carlo", this);
#endif

#ifdef GWA
	new Entity("Global Settings", "Global Settings", this);
	new Entity("Genetic Algorithm", "Genetic Algorithm", this);
	new Entity("Markov Chain Monte Carlo", "Markov Chain Monte Carlo", this);
#endif
	QList <QString> QL = ModelSpace.getList();
	setMode();
}
void GraphWidget::itemMoved()
{
	if (!timerId)
		timerId = startTimer(1000 / 25);
}
void GraphWidget::keyPressEvent(QKeyEvent *event)
{
	switch (event->key()) {
	case Qt::Key_Delete:
		deleteSelected();
		break;
	case Qt::Key_Backspace:
		deleteSelected();
		break;
	case Qt::Key_Left:
		centerNode->moveBy(-20, 0);
		break;
	case Qt::Key_Right:
		centerNode->moveBy(20, 0);
		break;
	case Qt::Key_Plus:
		zoomIn();
		break;
	case Qt::Key_Minus:
		zoomOut();
		break;
	case Qt::Key_Escape:
		deselectAll();
		break;
	case Qt::Key_Space:
	case Qt::Key_Enter:
		shuffle();
		break;
	default: 
		QGraphicsView::keyPressEvent(event);
	}
}

void GraphWidget::deleteSelected()
{
	for each (Node *node in Nodes()) if (node->isSelected()) {
		treeModel->deleteNode(node);
//		for each (Edge *edge in node->edgeList)
//			delete edge;
//		delete node;
	}
	for each (Edge *edge in Edges()) if (edge->isSelected()) delete edge;
	deselectAll();
}

void GraphWidget::deselectAll() const
{
	for each (Node *node in Nodes()) node->setSelected(false);
	for each (Edge *edge in Edges()) edge->setSelected(false);
	for each (Entity *entity in Entities) entity->setSelected(false);
	tableProp->setModel(0);
//	if ((propModel->type() == "Node") || (propModel->type() == "Edge")) propModel->setParentEntity(Entities[0]);
}

void GraphWidget::timerEvent(QTimerEvent *event)
{
/*	Q_UNUSED(event);

	QList<Node *> nodes;
	foreach(QGraphicsItem *item, scene()->items()) {
		if (Node *node = qgraphicsitem_cast<Node *>(item))
			nodes << node;
	}

//	foreach(Node *node, nodes)
//		node->calculateForces();

	bool itemsMoved = false;
	foreach(Node *node, nodes) {
		if (node->advance())
			itemsMoved = true;
	}

	if (!itemsMoved) {
		killTimer(timerId);
		timerId = 0;
	}*/
}
#ifndef QT_NO_WHEELEVENT
void GraphWidget::wheelEvent(QWheelEvent *event)
{
	scaleView(pow((double)2, event->delta() / 360.0));
}
#endif
void GraphWidget::drawBackground(QPainter *painter, const QRectF &rect)
{
	/*
	Q_UNUSED(rect);

	// Shadow
	QRectF sceneRect = this->sceneRect();
	QRectF rightShadow(sceneRect.right(), sceneRect.top() + 5, 5, sceneRect.height());
	QRectF bottomShadow(sceneRect.left() + 5, sceneRect.bottom(), sceneRect.width(), 5);
	if (rightShadow.intersects(rect) || rightShadow.contains(rect))
		painter->fillRect(rightShadow, Qt::darkGray);
	if (bottomShadow.intersects(rect) || bottomShadow.contains(rect))
		painter->fillRect(bottomShadow, Qt::darkGray);

	// Fill
	QLinearGradient gradient(sceneRect.topLeft(), sceneRect.bottomRight());
	gradient.setColorAt(0, Qt::white);
	gradient.setColorAt(1, Qt::lightGray);
	painter->fillRect(rect.intersected(sceneRect), gradient);
	painter->setBrush(Qt::NoBrush);
	painter->drawRect(sceneRect);

	// Text
	QRectF textRect(sceneRect.left() + 4, sceneRect.top() + 4,
		sceneRect.width() - 4, sceneRect.height() - 4);
	QString message(tr("Click and drag the nodes around, and zoom with the mouse "
		"wheel or the '+' and '-' keys"));

	QFont font = painter->font();
	font.setBold(true);
	font.setPointSize(14);
	painter->setFont(font);
	painter->setPen(Qt::lightGray);
	painter->drawText(textRect.translated(2, 2), message);
	painter->setPen(Qt::black);
	painter->drawText(textRect, message);
	*/
	
}
void GraphWidget::scaleView(qreal scaleFactor)
{
	qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
	if (factor < 0.07 || factor > 100)
		return;

	scale(scaleFactor, scaleFactor);
}
void GraphWidget::shuffle()
{
	foreach(QGraphicsItem *item, scene()->items()) {
		if (qgraphicsitem_cast<Node *>(item))
			item->setPos(-150 + qrand() % 300, -150 + qrand() % 300);
	}
}
void GraphWidget::zoomIn()
{
	scaleView(qreal(1.2));
}
void GraphWidget::zoomOut()
{
	scaleView(1 / qreal(1.2));
}

void GraphWidget::mouseMoveEvent(QMouseEvent *event)
{
	_x = mapToScene(event->pos()).x();
	_y = mapToScene(event->pos()).y();
	bool cursorModeNormal = true;
	setToolTip("");
	QGraphicsView::mouseMoveEvent(event);
	QString txt;
	Node *c1 = qgraphicsitem_cast<Node*> (itemAt(event->pos())); //Get the item at the position
	if (c1 && c1->itemType == Object_Types::Block) //GUI == "Block") //itemType == Object_Types::Block)
	{
		txt = QString("%1, %2: %3").arg(c1->ObjectType().GuiObject).arg(c1->ObjectType().ObjectType).arg(c1->Name());
		QString toolTip = QString("Type: %1\nName: %2").arg(c1->ObjectType().ObjectType).arg(c1->Name());
		toolTip.append(QString("\nBottom Elevation: %1").arg(c1->val("z0").toStringUnit()));
		if (c1->errors.count()) toolTip.append(QString("\n%1 Error(s)").arg(c1->errors.count()));
		if (c1->warnings.count()) toolTip.append(QString("\n%1 Warning(s)").arg(c1->warnings.count()));
		setToolTip(toolTip);
	}
	else for each (c1 in Nodes()) c1->setBold(false);
	Edge *c2 = qgraphicsitem_cast<Edge*> (itemAt(event->pos())); //Get the item at the position
	if (Edges().contains(c2))
		if (c2 && c2->itemType == Object_Types::Connector && c2->dist(mapToScene(event->pos())) < 120) //GUI == "Connector"
		{
			c2->setBold(true);
			c2->update();
			//txt = QString("%1, %2: %3").arg(c2->ObjectType().GuiObject).arg(c2->ObjectType().ObjectType).arg(c2->Name());
			QString toolTip = QString("%1, %2: %3").arg(c2->ObjectType().GuiObject).arg(c2->ObjectType().ObjectType).arg(c2->ObjectType().SubType);
			//QString toolTip = QString("Type: %1\nName: %2").arg(c1->ObjectType().ObjectType).arg(c1->Name());
			//toolTip.append(QString("\nBottom Elevation: %1").arg(c1->val("z0").toStringUnit()));
			//if (c1->errors.count()) toolTip.append(QString("\n%1 Error(s)").arg(c1->errors.count()));
			//if (c1->warnings.count()) toolTip.append(QString("\n%1 Warning(s)").arg(c1->warnings.count()));
			setToolTip(toolTip);

		}
	else for each (c2 in Edges()) c2->setBold(false);
	update();
	emit Mouse_Pos(_x, _y, txt);
	if (Operation_Mode == Operation_Modes::Node1_selected)
	{
		Node *child = qgraphicsitem_cast<Node*> (itemAt(event->pos())); //Get the item at the position
		if (child)
			if ((child->itemType == Object_Types::Block) && (Node1->Name() != child->Name()))
			{
				tempRay->setValidation(true);
				tempRay->adjust(Node1, child);
			}
		if (!child)
		{
			tempRay->setValidation(false);
			tempRay->adjust(Node1, &QPointF(mapToScene(event->pos())));
		}
	}
	if (Operation_Mode == Operation_Modes::NormalMode)
	{
		Node *node = qgraphicsitem_cast<Node*> (itemAt(event->pos())); //Get the item at the position
/*		Edge *edge = static_cast<Edge*> (itemAt(event->pos())); //Get the item at the position
		if (edge)
		{
			setCursor(Qt::ForbiddenCursor);
			qDebug() << edge->dist(mapToScene(event->pos()));
		}
*/
		if (node)
			if (node->itemType == Object_Types::Block)
			{
				int xx = mapToScene(event->pos()).x();
				int yy = mapToScene(event->pos()).y();
				if ((node->corner(xx, yy) == topleft) || (node->corner(xx, yy) == bottomright))
				{
					setCursor(Qt::SizeFDiagCursor);
					cursorModeNormal = false;
				}
				if ((node->corner(xx, yy) == topright) || (node->corner(xx, yy) == bottomleft))
				{
					setCursor(Qt::SizeBDiagCursor);
					cursorModeNormal = false;
				}

				if (node->corner(xx, yy) == none)
					if (node->edge(xx, yy) != noside)
					{
						setCursor(Qt::CrossCursor);
						cursorModeNormal = false;
					}
					else 
					{
						setCursor(Qt::ArrowCursor);
						cursorModeNormal = false;
					}

			}
	}
	if ((Operation_Mode == Operation_Modes::resizeNode) && (event->buttons() == Qt::LeftButton))
	{
		int xx = mapToScene(event->pos()).x();
		int yy = mapToScene(event->pos()).y();
		int px = resizenode->x();
		int py = resizenode->y();
		int pw = resizenode->Width();
		int ph = resizenode->Height();
		int minH = resizenode->minH, minW = resizenode->minW;

		if (resizecorner == topleft && (px - xx + pw) > minW && (py - yy + ph) > minH)
		{
			resizenode->setX(xx);
			resizenode->setWidth(px - xx + pw);
			resizenode->setY(yy);
			resizenode->setHeight(py - yy + ph);
		}
		if (resizecorner == bottomleft && (px - xx + pw) > minW && (yy - py) > minH)
		{
			resizenode->setX(xx);
			resizenode->setWidth(px - xx + pw);
			//resizenode->setY(yy);
			resizenode->setHeight(yy - py);
		}
		if (resizecorner == topright && (xx - px) > minW && (py -yy + ph) > minH)
		{
			//resizenode->setX(xx);
			resizenode->setWidth(xx - px);
			resizenode->setY(yy);
			resizenode->setHeight(py - yy + ph);
		}
		if (resizecorner == bottomright && (xx - px) > minW && (yy - py) > minH)
		{
			//resizenode->setX(xx);
			resizenode->setWidth(xx- px);
			//resizenode->setY(yy);
			resizenode->setHeight(yy - py);
		}
		resizenode->update();
		for each(Edge *edge in resizenode->edges())
		edge->adjust();
	}
	if (cursorModeNormal) setCursor(Qt::ArrowCursor);
}
void GraphWidget::update()
{
	for each (Node* n in Nodes()) n->update();
	for each (Edge* e in Edges()) e->update();
	

}
void GraphWidget::mousePressEvent(QMouseEvent *event)
{
	if (Operation_Mode == Operation_Modes::NormalMode)
	{
		setDragMode(QGraphicsView::RubberBandDrag);
		Node *node = static_cast<Node*> (itemAt(event->pos())); //Get the item at the position
		if (node)
			if (node->itemType == Object_Types::Block)
			{
				int xx = mapToScene(event->pos()).x();
				int yy = mapToScene(event->pos()).y();
				if (event->buttons() == Qt::LeftButton)
				{

					if (node->corner(xx, yy))
					{
						setDragMode(QGraphicsView::NoDrag);
						setMode(Operation_Modes::resizeNode);
						resizenode = node;
						resizecorner = node->corner(xx, yy);
						node->setFlag(QGraphicsItem::ItemIsMovable, false);
					}
					else if (node->edge(xx, yy))
					{
						node->setFlag(QGraphicsItem::ItemIsMovable, false);
						Node1 = node;
						tempRay = new Ray();
						MainGraphicsScene->addItem(tempRay);
						setMode(Operation_Modes::Node1_selected);
					}

					else
					{
						for each (Node* n in Nodes()) n->setFlag(QGraphicsItem::ItemIsMovable, false);
						node->setFlag(QGraphicsItem::ItemIsMovable, true);
					}
				}
			}
	}	
	if (event->buttons() == Qt::LeftButton)
	{
		switch (Operation_Mode) {
			/*	case Operation_Modes::NormalMode:
				{
				Node *child = static_cast<Node*> (itemAt(mouseEvent->pos())); //Get the item at the position
				if (!child)
				//setMode(Operation_Modes::Pan);
				//Pan1 = mouseEvent->pos();
				setDragMode(QGraphicsView::ScrollHandDrag);
				}
				
		case Operation_Modes::Pan:
		{
			//		Node *child = static_cast<Node*> (itemAt(mouseEvent->pos())); //Get the item at the position
			//		if (!child)
			//			setMode(Operation_Modes::Pan);
			Pan1 =  mouseEvent->pos();
			setMode(Operation_Modes::Pan_Started);
			break;
		}*/
		case Operation_Modes::Draw_Connector:
		{
			Node *child = static_cast<Node*> (itemAt(event->pos())); //Get the item at the position
			if (!child) break;
			if (child->itemType != Object_Types::Block) break;
			child->setFlag(QGraphicsItem::ItemIsMovable, false);
			Node1 = child;
			tempRay = new Ray();
			MainGraphicsScene->addItem(tempRay);
			setMode(Operation_Modes::Node1_selected);
			break;
		}
/*		case Operation_Modes::NormalMode:
		{
			Node *node = static_cast<Node*> (itemAt(event->pos())); //Get the item at the position
			Edge *edge = static_cast<Edge*> (itemAt(event->pos())); //Get the item at the position

			if (!node && !edge) deselectAll();
			break;
		}*/ 
		default:
		QGraphicsView::mousePressEvent(event); //Call the ancestor
		}
	}
}
void GraphWidget::mouseReleaseEvent(QMouseEvent *event)
{
	switch (Operation_Mode) {
	case Operation_Modes::Node1_selected:
	{
		Node1->setFlag(QGraphicsItem::ItemIsMovable);
		setMode(1);
		MainGraphicsScene->removeItem(tempRay);
		delete tempRay;
		Node *child = static_cast<Node*> (itemAt(event->pos())); //Get the item at the position
		if (!child)	break;
		if (child->itemType != Object_Types::Block) break;
		if (Node1 != child) new Edge(Node1, child, this);
		break;
	}
	case Operation_Modes::NormalMode:
	{
		if (event->button() == Qt::LeftButton)
		{
			Node *node = static_cast<Node*> (itemAt(event->pos())); //Get the item at the position
			if (node)
				if (node->itemType == Object_Types::Block)
					if (node->flags() == QGraphicsItem::ItemIsMovable)
						node->setFlag(QGraphicsItem::ItemIsMovable, false);
					else deselectAll();
			Edge *edge = static_cast<Edge*> (itemAt(event->pos())); //Get the item at the position
			if (edge)
			{
				if (edge->dist(mapToScene(event->pos())) < 120) edge->setSelected(true);
			}
			if (!node && !edge) deselectAll();
		}
		break;
	}
	case Operation_Modes::resizeNode:
	{
		setMode(NormalMode, true);
	}
	//	default:
	}
	static QMap<QString, QMap<QString, QString>> specs;
	bool changed = false;
	for each (Node *n in Nodes())
	{
		if (specs[n->Name()]["x"].toFloat() != n->x() ||
			specs[n->Name()]["y"].toFloat() != n->y() ||
			specs[n->Name()]["w"].toFloat() != n->Width() ||
			specs[n->Name()]["h"].toFloat() != n->Height())
		{
			changed = true;
			specs[n->Name()]["x"] = QString::number(n->x());
			specs[n->Name()]["y"] = QString::number(n->y());
			specs[n->Name()]["w"] = QString::number(n->Width());
			specs[n->Name()]["h"] = QString::number(n->Height());
		}
	}
	if (changed) gwChanged();
		QGraphicsView::mouseReleaseEvent(event); //Call the ancestor
}
void GraphWidget::settableProp(QTableView*_tableProp)
{
	tableProp = _tableProp;
}
//void GraphWidget::setpropModel(PropModel *_propModel)
//{
//	propModel = _propModel;
//}
void GraphWidget::add_to_undo_list()
{
	if (undo_counter < undolist.size() )
		for (int i = undolist.size() ; i > undo_counter; i--)
			undolist.pop_back();
	undolist.append(compact());
	undo_counter = undolist.size(); 
	qDebug() << "Undo Counter (added) :" << undolist.size() << undo_counter;	
}
void GraphWidget::undo()
{
	trackingUndo= false;
	int counter = undo_counter;
	if (undo_counter>1)
	{
		clear();
		unCompact(undolist[--counter - 1]);
	}
	undo_counter = counter;
	//	refresh();
	qDebug() << undolist.size() << undo_counter;
	trackingUndo= true;
}
void GraphWidget::redo()
{
	trackingUndo = false;
	int counter = undo_counter;
	if (undo_counter < undolist.size())
	{
		clear();
		unCompact(undolist[counter++]);
	}
	//	refresh();
	undo_counter = counter;
	qDebug() << undolist.size() << undo_counter;
	trackingUndo = true;

}
void GraphWidget::sceneChanged()
{
	QRectF rect = MainGraphicsScene->sceneRect();
	QRectF newRect = MainGraphicsScene->itemsBoundingRect();
	float width = newRect.width();
	float height = newRect.height();
	float scale = 1.1;
	newRect.setLeft(newRect.left() - (scale-1)/2*width);
	newRect.setTop(newRect.top() - (scale-1)/2*height);
	newRect.setWidth(width * scale);
	newRect.setHeight(height * scale);

	newRect.setLeft(min(rect.left(), newRect.left()));
	newRect.setTop(min(rect.top(), newRect.top()));
	newRect.setWidth(max(rect.width(), newRect.width()));
	newRect.setHeight(max(rect.height(), newRect.height()));

	MainGraphicsScene->setSceneRect(newRect);

}
QList<Node*> GraphWidget::Nodes() const
{
	QList<Node *> nodes;
	foreach(QGraphicsItem *item, scene()->items()) {
		if (Node *node = qgraphicsitem_cast<Node *>(item))
		{
			nodes << node;
		}		
	}
	return(nodes);
}

QList<Edge *> GraphWidget::Edges() const
{
	QList<Edge *> edges;
	for each(QGraphicsItem *item in scene()->items()) 
	{
		if (Edge *edge = qgraphicsitem_cast<Edge *>(item))
			edges << edge;
	}
	return(edges);
}

Operation_Modes GraphWidget::setMode(int i)
{
	return(setMode(Operation_Modes::NormalMode, true));
}

Operation_Modes GraphWidget::setModeCursor()
{
	switch (Operation_Mode) {
	case Operation_Modes::Draw_Connector:
		setCursor(Qt::PointingHandCursor);
		break;
	case Operation_Modes::Pan:
		setCursor(Qt::OpenHandCursor);
		break;
	case Operation_Modes::Node1_selected:
		setCursor(Qt::CrossCursor);
		break;
	case Operation_Modes::resizeNode:
		break;
	case Operation_Modes::NormalMode:
		setCursor(Qt::ArrowCursor);
		break;
	}
	return (Operation_Mode);
}
Operation_Modes GraphWidget::setMode(Operation_Modes OMode, bool back)
{
	static QList<Operation_Modes> Modes;
	bool select, move;
	if (!back)
	{
		Operation_Mode = OMode;
		Modes.push_back(OMode);
	}
	else
	{
		if (Modes.size() < 2)
		{
			Operation_Mode = Operation_Modes::NormalMode;
			setModeCursor();
			return(Operation_Mode);
		}
		Modes.pop_back();
		Operation_Mode = Modes[Modes.size()-1];
	}
	switch (Operation_Mode) {
	case Operation_Modes::Draw_Connector:
		setModeCursor();
		setDragMode(QGraphicsView::NoDrag);
		move = false;
		select = false;
		break;
	case Operation_Modes::Pan:
		setModeCursor();
		setDragMode(QGraphicsView::ScrollHandDrag);
		move = true;
		select = false;
		break;
/*	case Operation_Modes::Pan_Started:
		setCursor(Qt::ClosedHandCursor);
		break;*/
	case Operation_Modes::Node1_selected:
		setModeCursor();
		setDragMode(QGraphicsView::NoDrag);
		move = false;
		select = false;
		break;
	case Operation_Modes::resizeNode:
		//setCursor(Qt::CrossCursor);
		move = false;
		select = false;
		break;
	case Operation_Modes::NormalMode:
		setModeCursor();
		setDragMode(QGraphicsView::RubberBandDrag);
		select = true;
		move = false;
		break;
	}	
	foreach(Node *node, Nodes())
	{
		node->setFlag(QGraphicsItem::ItemIsMovable, move);
		node->setFlag(QGraphicsItem::ItemIsSelectable, select);
	}		
	foreach(Edge *edge, Edges())
		edge->setFlag(QGraphicsItem::ItemIsSelectable, select);
	return (Operation_Mode);
}

bool GraphWidget::select(const QString &name, const QString type) const
{
	bool r = false;
	if (type == "Block")
		for each (Node* node in Nodes())
			if (node->Name() == name)
			{
				deselectAll();
				node->setSelected(true);
				r = true;
				return r;
			}

	if (type == "Connector")
		for each (Edge* edge in Edges())
			if (edge->Name() == name)
			{
				deselectAll();
				edge->setSelected(true);
				r = true;
				return r;
			}
	for each (Entity* entity in Entities)
		if (entity->objectType.ObjectType== type && entity->Name() == name)
		{
			deselectAll();
			tableProp->setModel(entity->model);
			entity->setSelected(true);
			r = true;
			return r;
		}
	for each (Entity* entity in Entities)
		if (entity->objectType.ObjectType == name && entity->Name() == name)
		{
			deselectAll();
			tableProp->setModel(entity->model);
			r = true;
			return r;
		}
	if (type == "Well" || type == "Tracer")
		for each (Node* node in Nodes())
			if (node->objectType.ObjectType == type && node->Name() == name)
			{
				deselectAll();
				node->setSelected(true);
				r = true;
				return r;
			}

	return r;
}
QStringList GraphWidget::EntityNames(const QString &type) const
{
	QStringList r;
	for each (Entity *e in Entities)
		if (e->objectType.ObjectType == type) 
			r.append(e->Name());
	return r;
}
Entity* GraphWidget::entityByName(const QString &name) const
{
	for each (Entity *e in Entities)
		if (e->Name() == name)
			return e;
	return 0;
}

QList<Entity *> GraphWidget::entitiesByType(const QString &type) const
{
	QList<Entity *> r;
	for each (Entity *e in Entities)
		if (e->objectType.ObjectType == type)
			r.append(e);
	return r;
}

QList<Node *> GraphWidget::nodesByType(const QString &type) const
{
	QList<Node *> r;
	for each (Node *e in Nodes())
		if (e->objectType.ObjectType == type)
			r.append(e);
	return r;
}

Node* GraphWidget::node(const QString &name) const
{
	for each (Node* i in Nodes())
		if (i->Name() == name) 
			return i;
	return nullptr;
}

Edge* GraphWidget::edge(const QString &name) const
{
	for each (Edge* i in Edges())
		if (i->Name() == name) return i;
	return nullptr;
}

Entity* GraphWidget::entity(const QString &name, const QString &type) const
{
	for each (Entity* i in Entities)
		if (i->Name() == name) 
			if (type == "*" || i->objectType.ObjectType == type)	return i;
	return nullptr;
}

Process* GraphWidget::process(const QString &name) const
{
	for each (Process* i in Processes)
		if (i->name == name) return i;
	return nullptr;
}
QList<QMap<QString, QVariant>> GraphWidget::compact() const
{
	QList<QMap<QString, QVariant>> list;
	QMap<QString, QVariant> r;
	//r["Undo Counter"] = undo_counter;
	r["Model Space"] = ModelSpace.Model;
	r["Inflow Filenames"] = inflowFileNames;
	r["GUI"] = "Graphic Widget";
	r["Experiments"] = experimentsList();

	qDebug() << r["GUI"].toString() << ", " << r["Name"].toString() << " saved.";
	
	list.append(r);
	for each (Node *n in Nodes())
	{
		list.append(n->compact());
		qDebug() << list.last()["GUI"].toString() << ", " << list.last()["Name"].toString() << " saved.";
	}
	for each (Edge *e in Edges())
	{
		list.append(e->compact());
		qDebug() << list.last()["GUI"].toString() << ", " << list.last()["Name"].toString() << " saved.";
	}
	for each (Entity *e in Entities)
	{
		list.append(e->compact());
		qDebug() << list.last()["GUI"].toString() << ", " << list.last()["Name"].toString() << " saved.";
	}
	for each (Process *p in Processes)
	{
		list.append(p->compact());
		qDebug() << list.last()["GUI"].toString() << ", " << list.last()["Name"].toString() << " saved.";
	}

	return list;
}

QList<QMap<QString, QVariant>> GraphWidget::compactRXN() const
{
	QList<QMap<QString, QVariant>> list;
	for each (Entity *e in Entities)
		if (e->objectType.ObjectType == "Reaction parameter" || e->objectType.ObjectType == "Reaction Network" || e->objectType.ObjectType == "Constituent")
			list.append(e->compact());
	for each (Process *p in Processes)
		list.append(p->compact());
	return list;
}

void GraphWidget::clear()
{
//	expandNode(treeModel->index(-1, -1), false);
	for each (Node *n in Nodes())
		treeModel->deleteNode(n);
	for each (Edge *e in Edges())
		treeModel->deleteEdge(e);
	QList<Entity *> EntitiesCopy = Entities;
	for each (Entity *e in EntitiesCopy)
		treeModel->deleteEntity(e);

	Entities.clear();
	qDeleteAll(Entities);
	Entities.clear();
	qDeleteAll(Processes);
	Processes.clear();
	treeModel->refresh();
	tableProp->setModel(0);
	if (results)
		delete results;
	if (resultsSet.size())
		resultsSet.clear();
	if (model)
		delete model;
	if (modelSet) delete modelSet;
	model = 0; modelSet = 0; results = 0;
}

void GraphWidget::clearRXN()
{
	expandNode(treeModel->index(-1, -1), false);
	for each (Entity *e in Entities)
		if (e->objectType.ObjectType == "Reaction parameter" || e->objectType.ObjectType == "Reaction Network" || e->objectType.ObjectType == "Constituent")
		{
			delete e;
			Entities.removeOne(e);
		}
	treeModel->refresh();
	tableProp->setModel(0);
}

GraphWidget* GraphWidget::unCompact(QList<QMap<QString, QVariant>> list) //, QWidget *parent)
{
	for each (QMap<QString, QVariant> r in list)
	{
		if (r["GUI"].toString() == "Graphic Widget")
		{
			qDebug() << r["GUI"].toString() << " Added.";
			undo_counter = 0;// r["Undo Counter"].toInt();
			ModelSpace.Model = r["Model Space"].toString();
			inflowFileNames = r["Inflow Filenames"].toStringList();
			for each (QString experiment in r["Experiments"].toStringList())
				if (!experimentsList().contains(experiment))
					experiments->addItem(experiment);
			QCoreApplication::processEvents();
		}
	}
	for each (QMap<QString, QVariant> r in list)
	{
		if (r["GUI"].toString() == "Block")
		{
			qDebug() << r["GUI"].toString() << ", " << r["Name"].toString() << " Added.";
			Node::unCompact(r, this);
			QCoreApplication::processEvents();
		}
	}
	for each (QMap<QString, QVariant> r in list)
	{
		if (r["GUI"].toString() == "Connector")
		{
			qDebug() << r["GUI"].toString() << ", " << r["Name"].toString() << " Added.";
			Edge::unCompact(r, this);
			QCoreApplication::processEvents();
		}

	}
	for each (QMap<QString, QVariant> r in list)
	{
		if (r["GUI"].toString() == "Entity")
		{
			qDebug() << r["GUI"].toString() << ", " << r["Name"].toString() << " Added.";
			Entity::unCompact(r, this);
			QCoreApplication::processEvents();
		}
	}
	for each (QMap<QString, QVariant> r in list)
	{
		if (r["GUI"].toString() == "Process")
		{
			qDebug() << r["GUI"].toString() << ", " << r["Name"].toString() << " Added.";
			Process::unCompact(r, this);
			QCoreApplication::processEvents();
		}
	}
	
	QStringList missingList;
	missingList << "Markov chain Monte Carlo" << "Solver setting" << "Project setting" << "Climate setting";
	for each (QString missing  in missingList)
	{
		Entity *e = entityByName(missing);
		if (e == nullptr)	
			new Entity(missing, missing, this);
	}

	treeModel->refresh();

	return this;
}
GraphWidget* GraphWidget::unCompact10(QList<QMap<QString, QVariant>> list) //, QWidget *parent)
{
	for each (QMap<QString, QVariant> r in list)
	{
		if (r["GUI"].toString() == "Graphic Widget")
		{
			qDebug() << r["GUI"].toString() << " Added.";
			undo_counter = 0;// r["Undo Counter"].toInt();
			ModelSpace.Model = r["Model Space"].toString();
			inflowFileNames = r["Inflow Filenames"].toStringList();
			QCoreApplication::processEvents();
		}
	}
	for each (QMap<QString, QVariant> r in list)
	{
		if (r["GUI"].toString() == "Block")
		{
			qDebug() << r["GUI"].toString() << ", " << r["Name"].toString() << " Added.";
			Node::unCompact10(r, this);
			QCoreApplication::processEvents();
		}
	}
	for each (QMap<QString, QVariant> r in list)
	{
		if (r["GUI"].toString() == "Connector")
		{
			qDebug() << r["GUI"].toString() << ", " << r["Name"].toString() << " Added.";
			Edge::unCompact10(r, this);
			QCoreApplication::processEvents();
		}
	}
	for each (QMap<QString, QVariant> r in list)
	{
		if (r["GUI"].toString() == "Entity")
		{
			qDebug() << r["GUI"].toString() << ", " << r["Name"].toString() << " Added.";
			Entity::unCompact10(r, this);
			QCoreApplication::processEvents();
		}
	}
	for each (QMap<QString, QVariant> r in list)
	{
		if (r["GUI"].toString() == "Process")
		{
			qDebug() << r["GUI"].toString() << ", " << r["Name"].toString() << " Added.";
			Process::unCompact(r, this);
			QCoreApplication::processEvents();
		}
	}
	Entity *e = entityByName("Markov Chain Monte Carlo");
	if (e == nullptr)	new Entity("Markov Chain Monte Carlo", "Markov Chain Monte Carlo", this);

	treeModel->refresh();

	return this;
}
void GraphWidget::expandNode(const QModelIndex &parentIndex, bool expand)
{
	projectExplorer->setExpanded(parentIndex, expand);
	for (qint32 rowNum = 0; rowNum < treeModel->rowCount(parentIndex); ++rowNum) {
		QModelIndex childIndex = treeModel->index(rowNum, 0, parentIndex);
		projectExplorer->setExpanded(childIndex, expand);
		expandNode(childIndex, expand);
	}
}

/*CMedium::CMedium(GraphWidget* gw)
{

	getconfigfromfile(filename);   //reads input file and creates lid_config
	set_default();
	f_get_environmental_params(); // load environmental parameters
	f_get_params(); //load charachterictics of unknown parameters *
	f_get_observed(); // load properties of observed data *
	f_get_model_configuration(); // load model structure (blocks, connectors)
	f_get_particle_types(); // load particle types *
	f_get_constituents(); // load constituent properties *
	f_set_default_connector_expressions();
	f_set_default_block_expressions();
	f_get_reactions(); // load reaction network
	f_get_buildup(); // load buildup properties *
	f_get_external_flux(); // load External fluxes *


	log_file_name = "log.txt";

	for (int j = 0; j<Precipitation_filename.size(); j++)
		Precipitation.push_back(CPrecipitation(pathname + Precipitation_filename[j]));
	for (int j = 0; j<Evaporation_filename.size(); j++)
		Evaporation.push_back(CBTC(pathname + Evaporation_filename[j]));

	for (int i = 0; i<Blocks.size(); i++)
	{
		for (int j = 0; j<Blocks[i].inflow_filename.size(); j++)
			Blocks[i].inflow.push_back(CBTCSet(pathname + Blocks[i].inflow_filename[j], 1));


		if ((Blocks[i].indicator == Pond) || (Blocks[i].indicator == Catchment) || (Blocks[i].indicator == Stream))
		{
			if (Blocks[i].precipitation_swch == true)
				for (int j = 0; j<Precipitation_filename.size(); j++)
					Blocks[i].inflow.push_back(Precipitation[j].getflow(Blocks[i].A));
		}
	}
	char buffer[33];
	vector<int> stds;
	for (int i = 0; i<measured_quan.size(); i++)
	{
		if (lookup(stds, measured_quan[i].std_no) == -1)
		{
			stds.push_back(measured_quan[i].std_no);
			measured_quan[i].std_to_param = int(parameters.size());
			range P;

			P.fixed = false;
			P.log = true;
			P.applytoall = true;
			P.tempcorr = 1;
			P.name = "std_" + string(_itoa(i, buffer, 10));
			P.low = exp(-4); P.high = exp(4);
			parameters.push_back(P);

		}
	}
	std.resize(stds.size());
	get_funcs();

	for (int i = 0; i<Blocks.size(); i++)
	{
		for (int j = 0; j<Blocks[i].inflow.size(); j++)
			Blocks[i].inflow[j] = Blocks[i].inflow[j].make_uniform(dt);
	}
	for (int i = 0; i<Evaporation.size(); i++)
		Evaporation[i] = Evaporation[i].make_uniform(dt);

	set_default_params();
}
*/

QStringList GraphWidget::nodeNames() const
{
	QStringList r;
	for each (Node *n in Nodes())
		r.append(n->Name());
	return r;
}

QStringList GraphWidget::nodeNames(const QString &type) const
{
	QStringList r;
	for each (Node *n in Nodes())
		if (n->objectType.ObjectType == type) r.append(n->Name());
	return r;
}

QStringList GraphWidget::edgeNames() const
{
	QStringList r;
	for each (Edge *e in Edges())
		r.append(e->Name());
	return r;
}

void GraphWidget::deleteNode(Node *node)
{
	
	if (static_cast<PropModel<Node>*>(tableProp->model()) == node->model) tableProp->setModel(0);
	for each (Edge * e in node->edgeList)
		treeModel->deleteEdge(e);
	//treeModel->deleteNode(node);
	delete node;
}

void GraphWidget::deleteEdge(Edge *edge)
{
	if (static_cast<PropModel<Edge>*>(tableProp->model()) == edge->model) tableProp->setModel(0);
	edge->sourceNode()->edgeList.removeOne(edge);
	edge->destNode()->edgeList.removeOne(edge);
	//treeModel->deleteEdge(edge);
	delete edge;
}

void GraphWidget::deleteEntity(Entity *entity)
{
	if (static_cast<PropModel<Entity>*>(tableProp->model()) == entity->model) tableProp->setModel(0);
	Entities.removeOne(entity);
	//treeModel->deleteEntity(entity);
	delete entity;
}

void GraphWidget::deleteProcess(Process *process)
{
	Processes.removeOne(process);
	delete process;
}

void GraphWidget::nodeContextMenuRequested(Node* n, QPointF pos)
{

	QMenu menu;
	menu.addAction("Delete");
	menu.addAction("Select");
#ifdef WQV
	menu.addAction("Make array of blocks");
#endif
	QMap < QAction *, QStringList> menuKey;
	if (n->getValue("Inflow time series") != ""&& n->getValue("Inflow time series") != ".")
	{
		QMap<QString, QString> inflowGraphNames;
		QStringList inflowFileNamesofNode = n->getValue("Inflow time series").toQString().split(';');
		for each(QString file in inflowFileNamesofNode)
		{
			CBTCSet inflows = CBTCSet(file.replace("./", modelPathname().append('/')).toStdString(), 1);
			for each (string name in inflows.names)
			{
				if (!inflowGraphNames.keys().contains(QString::fromStdString(name)))
					inflowGraphNames[QString::fromStdString(name)] = file;
				else warning(QString("Duplicate Headers in Inflow time series, Block %1, Header %2, inFlow file(s) %3").arg(n->Name()).arg(QString::fromStdString(name)).arg(n->getValue("Inflow time series")));
			}
		}
		if (inflowGraphNames.count())
		{
			menu.addSeparator();
			QMenu *inflowSubMenu = menu.addMenu("Plot Inflow Properties");
			for each (QString subMenuTitle in inflowGraphNames.keys())
			{
				QStringList list;
				list.append("Inflow");
				list.append(inflowGraphNames[subMenuTitle]); // inflow file
				menuKey[inflowSubMenu->addAction(subMenuTitle)] = list;
			}
		}
	}
	if (n->getValue("Atmospheric Record") != "" && n->getValue("Atmospheric Record") != ".")
	{
		QString file = n->getValue("Atmospheric Record").toQString();
		CBTC record = CBTC(file.replace("./", modelPathname().append('/')).toStdString());
		if (record.n)
		{
			menu.addSeparator();
			//QMenu *atmosphericSubMenu = menu.addMenu(
			menu.addAction("Plot Atmospheric Concentration Record");
		}
	}
	if (model != 0)
	{
#ifdef WQV
		if (model->ANS.BTC[model->getblocksq(n->Name().toStdString())].n > 0)
		{
			menu.addSeparator();
			QMenu *plotSubMenu = menu.addMenu("Plot Hydraulic Results");
			plotSubMenu->addAction("Plot Storage");
			plotSubMenu->addAction("Plot Head");
			QStringList Porous;
			Porous << "Soil" << "Darcy" << "Storage";// << "" << "";
			if (Porous.contains(n->objectType.ObjectType))
				plotSubMenu->addAction("Moisture Content");
			else
				plotSubMenu->addAction("Water Depth");
			plotSubMenu->addAction("Evapotranspiration Rate");

		}
		if ((model->colloid_transport() && entitiesByType("Particle").count()) ||
			(model->constituent_transport() && entitiesByType("Constituent").count()))
		{
			QMenu *waterQualitySubMenu = menu.addMenu("Plot Water Quality Results");

			if (model->colloid_transport() && entitiesByType("Particle").count())
			{
				for each (Entity *p in entitiesByType("Particle"))
				{
					QMenu *particleSubMenu = waterQualitySubMenu->addMenu(p->Name());
					if (p->getValue("Model").contains("Single"))
					{
						QStringList list;
						list.append(("Particle"));
						int BTCid = model->get_member_no(model->getblocksq(n->Name().toStdString()),
							model->lookup_particle_type(p->Name().toStdString()), 0);
						list.append(QString::number(BTCid));
						menuKey[particleSubMenu->addAction("Mobile")] = list;
					}
					if (p->getValue("Model").contains("Dual"))
					{
						QStringList list;
						list.append(("Particle"));
						int BTCid = model->get_member_no(model->getblocksq(n->Name().toStdString()),
							model->lookup_particle_type(p->Name().toStdString()), 0);
						list.append(QString::number(BTCid));
						menuKey[particleSubMenu->addAction("Mobile")] = list;
						list.clear();
						list.append(("Particle"));
						BTCid = model->get_member_no(model->getblocksq(n->Name().toStdString()),
							model->lookup_particle_type(p->Name().toStdString()), 1);
						list.append(QString::number(BTCid));
						menuKey[particleSubMenu->addAction("Attached")] = list;
					}
					if (p->getValue("Model").contains("Triple"))
					{
						QStringList list;
						list.append(("Particle"));
						int BTCid = model->get_member_no(model->getblocksq(n->Name().toStdString()),
							model->lookup_particle_type(p->Name().toStdString()), 0);
						list.append(QString::number(BTCid));
						menuKey[particleSubMenu->addAction("Mobile")] = list;
						list.clear();
						list.append(("Particle"));
						BTCid = model->get_member_no(model->getblocksq(n->Name().toStdString()),
							model->lookup_particle_type(p->Name().toStdString()), 2);
						list.append(QString::number(BTCid));
						menuKey[particleSubMenu->addAction("Reversible attached")] = list;
						list.clear();
						list.append(("Particle"));
						BTCid = model->get_member_no(model->getblocksq(n->Name().toStdString()),
							model->lookup_particle_type(p->Name().toStdString()), 1);
						list.append(QString::number(BTCid));
						menuKey[particleSubMenu->addAction("Irreversible attached")] = list;
					}
				}
			}
			if (model->constituent_transport() && entitiesByType("Constituent").count())
			{
				if (model->colloid_transport() && entitiesByType("Particle").count())
					menu.addSeparator();
				for each (Entity *e in entitiesByType("Constituent"))
				{
					//QMenu *constituentSubMenu = waterQualitySubMenu->addMenu(e->Name());
					QStringList list;
					list.append("Constituent");
					int BTCid = model->get_member_no(model->getblocksq(n->Name().toStdString()), -2, 0, model->RXN().look_up_constituent_no(e->Name().toStdString()));
					list.append(QString::number(BTCid));
					menuKey[waterQualitySubMenu->addAction(e->Name())] = list;
				}
				QMenu *sorbedSubMenu = waterQualitySubMenu->addMenu("Sorbed/Particle associated");
				QMenu *constituentSorbedSubMenu;
				for each (Entity *e in entitiesByType("Constituent"))
				{
					constituentSorbedSubMenu = sorbedSubMenu->addMenu(e->Name());
					QStringList list;
					list.append("Constituent");
					int BTCid = model->get_member_no(model->getblocksq(n->Name().toStdString()), -1, 0, model->RXN().look_up_constituent_no(e->Name().toStdString()));
					list.append(QString::number(BTCid));
					menuKey[constituentSorbedSubMenu->addAction("Soil")] = list;
					for each (Entity *p in entitiesByType("Particle"))
					{
						QMenu *particleSubMenu = constituentSorbedSubMenu->addMenu(p->Name());
						if (p->getValue("Model").contains("Single"))
						{
							QStringList list;
							list.append("Constituent");
							int BTCid = model->get_member_no(model->getblocksq(n->Name().toStdString()),
								model->lookup_particle_type(p->Name().toStdString()), 0, model->RXN().look_up_constituent_no(e->Name().toStdString()));
							list.append(QString::number(BTCid));
							menuKey[particleSubMenu->addAction("Mobile")] = list;
						}
						if (p->getValue("Model").contains("Dual"))
						{
							QStringList list;
							list.append("Constituent");
							int BTCid = model->get_member_no(model->getblocksq(n->Name().toStdString()),
								model->lookup_particle_type(p->Name().toStdString()), 0, model->RXN().look_up_constituent_no(e->Name().toStdString()));
							list.append(QString::number(BTCid));
							menuKey[particleSubMenu->addAction("Mobile")] = list;
							list.clear();
							list.append("Constituent");
							BTCid = model->get_member_no(model->getblocksq(n->Name().toStdString()),
								model->lookup_particle_type(p->Name().toStdString()), 1, model->RXN().look_up_constituent_no(e->Name().toStdString()));
							list.append(QString::number(BTCid));
							menuKey[particleSubMenu->addAction("Attached")] = list;
						}
						if (p->getValue("Model").contains("Triple"))
						{
							QStringList list;
							list.append("Constituent");
							int BTCid = model->get_member_no(model->getblocksq(n->Name().toStdString()),
								model->lookup_particle_type(p->Name().toStdString()), 0, model->RXN().look_up_constituent_no(e->Name().toStdString()));
							list.append(QString::number(BTCid));
							menuKey[particleSubMenu->addAction("Mobile")] = list;
							list.clear();
							list.append("Constituent");
							BTCid = model->get_member_no(model->getblocksq(n->Name().toStdString()),
								model->lookup_particle_type(p->Name().toStdString()), 1, model->RXN().look_up_constituent_no(e->Name().toStdString()));
							list.append(QString::number(BTCid));
							menuKey[particleSubMenu->addAction("Reversible attached")] = list;
							list.clear();
							list.append("Constituent");
							BTCid = model->get_member_no(model->getblocksq(n->Name().toStdString()),
								model->lookup_particle_type(p->Name().toStdString()), 2, model->RXN().look_up_constituent_no(e->Name().toStdString()));
							list.append(QString::number(BTCid));
							menuKey[particleSubMenu->addAction("Irreversible attached")] = list;
						}
					}
				}
			}
		}
#endif
#ifdef GWA
		menu.addSeparator();
		if (n->objectType.ObjectType == "Well")
		{
			menu.addAction("Plot Young Age Distribution");
			menu.addAction("Plot Cumulative Young Age Distribution");
			if (results->youngAgeDistributionRealizations.size())
				menu.addAction("Plot Young Age Distribution (Realizations)");
			if (results->cumulativeYoungAgeDistributionRealizations.size())
				menu.addAction("Plot Cumulative Young Age Distribution (Realizations)");
			if (results->youngAgeDistributionRealizationPercentiles.size())
				menu.addAction("Plot Young Age Distribution (Percentiles)");
			if (results->cumulativeYoungAgeDistributionRealizationPercentiles.size())
				menu.addAction("Plot Cumulative Young Age Distribution (Percentiles)");

			if (model->projected.names.size() > 0)
			{
				QStringList tracersList;
				for (int i = 0; i < model->projected.names.size(); i++)
				{
					QString name = QString::fromStdString(model->projected.names[i]);
					if (name.split('@').last() == n->Name())
						tracersList.append(name.split('@').first());
				}
				if (tracersList.count())
				{
					menu.addSeparator();
					QMenu *plotSubMenu = menu.addMenu("Plot Projected Tracer concentrations");
					for each(QString tracerName in tracersList)
					{
						QAction* ac = plotSubMenu->addAction(tracerName);
						QStringList list;
						list.append("Projected");
						list.append(tracerName);
						menuKey.insert(ac, list);
					}

					if (results->projectedTracerConcentrations.size())
					{
						QMenu *plotSubMenu = menu.addMenu("Plot Projected Tracer concentrations (Realizations)");
						for each(QString tracerName in tracersList)
						{
							QAction* ac = plotSubMenu->addAction(tracerName);
							QStringList list;
							list.append("Realization");
							list.append(tracerName);
							menuKey.insert(ac, list);
						}
						QMenu *plotSubMenu2 = menu.addMenu("Plot Projected Tracer concentrations (Percentiles)");
						for each(QString tracerName in tracersList)
						{
							QAction* ac = plotSubMenu2->addAction(tracerName);
							QStringList list;
							list.append("Percentile");
							list.append(tracerName);
							menuKey.insert(ac, list);
						}
					}
				}
			}
		}
#endif
	}
	QAction *selectedAction = menu.exec(mapToGlobal(mapFromScene(pos.toPoint())));
	if (selectedAction != nullptr)
	{
		if (selectedAction->text() == "Select")
			n->setSelected(true);
		if (selectedAction->text() == "Delete")
			treeModel->deleteNode(n);
#ifdef WQV
		if (selectedAction->text() == "Make array of blocks")
		{
			QString z0variableName = n->variableName("z0");
			XString z0 = n->getValue(n->variableName("z0"));
			QString unit = z0.unit;
			XString height = n->getValue(n->variableName("depth"));
			
			QString gridType = "2D V";
			bool canChangeType = true;
			XString length = 0.0, width = 0.0;
			
			if (n->objectType.ObjectType == "Catchment")
			{
				gridType = "2D H";
				canChangeType = false;
				length = n->getValue(n->variableName("length"));
				width = n->getValue(n->variableName("width"));
			}

			gridWindow *gridwin = new gridWindow(this, z0, gridType, canChangeType, length, width, height);
			gridwin->exec();
			QMap<QString, XString> grid =gridwin->getGridInfo();
			int numberofRows = grid["Number of Rows"].toFloat();
			int numberofColumns = grid["Number of Columns"].toFloat();
			
			
			if (grid.keys().contains("2D Vertical"))
			{
				XString deltaV = grid["delta V"];
				XString deltaH = grid["delta H"];
				XString length = grid["length"];

				if (numberofRows >= 1 && numberofColumns >= 1)// && isnumber(deltaV) && isnumber (deltaH))
				{
					//qDebug() << QString("Creating %1x%2 grid, delta H=%3, delta V=%4.").arg(numberofRows).arg(numberofColumns).arg(deltaH).arg(deltaV);
					Node *n1;
					QList<QList<Node*>> rows;
					for (int rowIndex = 0; rowIndex < numberofRows; rowIndex++)
					{
						QList <Node*> row;
						for (int columnIndex = 0; columnIndex < numberofColumns; columnIndex++)
						{
							if (rowIndex == 0 && columnIndex == 0)
							{
								row.append(n);
							}
							else
							{
								n1 = new Node(*n);
								
								treeModel->add(n1); 
								row.append(n1);
								n1->setName(n->newNodeName(n->Name(), Nodes()));
								n1->edgeList.clear();

								if (columnIndex == 0)
									n1->setX(n->x());
								else
									n1->setX(row[columnIndex - 1]->x() + n->Width() + 50);

								int offsetY;
								if (deltaV <= 0)
									offsetY = n->Height() + 50;
								else
									offsetY = -n->Height() - 50;

								if (rowIndex == 0)
									n1->setY(n->y());
								else
									n1->setY(rows[rowIndex - 1][columnIndex]->y() + offsetY);

								//qDebug() << QString("[%1,%2], x=%3, y=%4").arg(rowIndex).arg(columnIndex).arg(n1->x()).arg(n1->y());
								if (rowIndex != 0)
									z0.setNum(rows[rowIndex - 1][columnIndex]->getValue(z0variableName).toFloat() +
									(1.0 / (numberofRows - 1) * deltaV.toFloat(z0.unit)));
								else
									z0.setNum(row[columnIndex - 1]->getValue(z0variableName).toFloat() +
									(1.0 / (numberofColumns - 1) * deltaH.toFloat(z0.unit)));

								n1->setProp(z0variableName, z0.list(), XStringEditRole);
								MainGraphicsScene->addItem(n1);
								if (rowIndex != 0)
								{
									Edge *e = new Edge(rows[rowIndex - 1][columnIndex], n1, this);
									treeModel->add(e);
									e->setProp(e->variableName("d"), abs(deltaV.toFloat(z0.unit)) / (numberofRows - 1));
									e->copyProps(n, "Vertical");
								}
								if (columnIndex != 0)
								{
									Edge *e = new Edge(row[columnIndex - 1], n1, this);
									treeModel->add(e); 
									if (length > 0) e->setProp(e->variableName("d"), length);
									e->copyProps(n, "Horizontal");
								}

							}
						}
						rows.append(row);
					}
				}
			}
			else //2D Horizontal Grid
			{
				XString deltaHX = grid["delta HX"];
				XString deltaHY = grid["delta HY"];
				XString lengthX = grid["lengthX"];
				XString lengthY = grid["lengthY"];

				if (numberofRows >= 1 && numberofColumns >= 1)// && isnumber(deltaV) && isnumber (deltaH))
				{
					//qDebug() << QString("Creating %1x%2 grid, delta H=%3, delta V=%4.").arg(numberofRows).arg(numberofColumns).arg(deltaH).arg(deltaV);
					Node *n1;
					QList<QList<Node*>> rows;
					for (int rowIndex = 0; rowIndex < numberofRows; rowIndex++)
					{
						QList <Node*> row;
						for (int columnIndex = 0; columnIndex < numberofColumns; columnIndex++)
						{
							if (rowIndex == 0 && columnIndex == 0)
							{
								row.append(n);
							}
							else
							{
								n1 = new Node(*n);
								treeModel->add(n1);
								row.append(n1);
								n1->setName(n->newNodeName(n->Name(), Nodes()));
								n1->edgeList.clear();

								if (columnIndex == 0)
									n1->setX(n->x());
								else
									n1->setX(row[columnIndex - 1]->x() + n->Width() + 50);

								int offsetY;
									offsetY = n->Height() + 50;

								if (rowIndex == 0)
									n1->setY(n->y());
								else
									n1->setY(rows[rowIndex - 1][columnIndex]->y() + offsetY);

								//qDebug() << QString("[%1,%2], x=%3, y=%4").arg(rowIndex).arg(columnIndex).arg(n1->x()).arg(n1->y());
								if (rowIndex != 0)
									z0.setNum(rows[rowIndex - 1][columnIndex]->getValue(z0variableName).toFloat() +
									(1.0 / (numberofRows - 1) * deltaHY.toFloat(z0.unit)));
								else
									z0.setNum(row[columnIndex - 1]->getValue(z0variableName).toFloat() +
									(1.0 / (numberofColumns - 1) * deltaHX.toFloat(z0.unit)));

								n1->setProp(z0variableName, z0.list(), XStringEditRole);
								MainGraphicsScene->addItem(n1);
								if (rowIndex != 0)
								{
									Edge *e = new Edge(rows[rowIndex - 1][columnIndex], n1, this);
									treeModel->add(e); 
									e->copyProps(n, "Horizontal");
									if (lengthY > 0) e->setProp(e->variableName("d"), lengthY);
								}
								if (columnIndex != 0)
								{
									Edge *e = new Edge(row[columnIndex - 1], n1, this);
									treeModel->add(e); 
									e->copyProps(n, "Horizontal");
									if (lengthX > 0) e->setProp(e->variableName("d"), lengthX);
								}

							}
						}
						rows.append(row);
					}
				}
			}
			
		}
		if (selectedAction->text() == "Plot Storage")
		{
			plotWindow *plot = new plotWindow(this);
			plot->addScatterPlot(model->ANS, model->getblocksq(n->Name().toStdString()), QString("%1: %2").arg(n->Name()).arg("Storage"));
			plot->show();
		}
		if (selectedAction->text() == "Plot Head")
		{
			plotWindow *plot = new plotWindow(this);
			plot->addScatterPlot(model->ANS, model->Connector.size() + model->Blocks.size() + model->getblocksq(n->Name().toStdString()), QString("%1: %2").arg(n->Name()).arg("Head"));
			plot->show();
		}
		if (selectedAction->text() == "Moisture Content")
		{
			plotWindow *plot = new plotWindow(this);
			double volume = model->Blocks[model->getblocksq(n->Name().toStdString())].V;
			plot->addScatterPlot(model->ANS, model->getblocksq(n->Name().toStdString()), QString("%1: %2").arg(n->Name()).arg("Moisture Content"), 1.0/volume);
			plot->show();
		}
		if (selectedAction->text() == "Water Depth")
		{
			plotWindow *plot = new plotWindow(this);
			double z0 = model->Blocks[model->getblocksq(n->Name().toStdString())].z0;
			plot->addScatterPlot(model->ANS, model->Connector.size() + model->Blocks.size() + model->getblocksq(n->Name().toStdString()), QString("%1: %2").arg(n->Name()).arg("Water Depth"), 1, -z0);
			plot->show();
		}
		if (selectedAction->text() == "Evapotranspiration Rate")
		{
			plotWindow *plot = new plotWindow(this);
			double z0 = model->Blocks[model->getblocksq(n->Name().toStdString())].z0;
			plot->addScatterPlot(model->ANS, model->Connector.size() + 2*model->Blocks.size() + model->getblocksq(n->Name().toStdString()), QString("%1: %2").arg(n->Name()).arg("Evapotranspiration Rate"), 1, 0);
			plot->show();
		}
#endif
		if (selectedAction->text() == "Plot Atmospheric Concentration Record")
		{
			plotWindow *plot = new plotWindow(this);
			QString file = n->getValue("Atmospheric Record").toQString();
			CBTC record = CBTC(file.replace("./", modelPathname().append('/')).toStdString());
			plot->addScatterPlot(record, n->Name(), false);
			plot->show();
		}
#ifdef GWA
		if (selectedAction->text() == "Plot Young Age Distribution")
		{
			plotWindow *plot = new plotWindow(this);
			
			plot->addScatterPlot(model->wells(n->Name())->young_age_distribution*(1.0 - model->wells(n->Name())->fraction_old), n->Name() + " young age distribution", 1, 0, false);
			plot->show();
		}
		if (selectedAction->text() == "Plot Cumulative Young Age Distribution")
		{
			plotWindow *plot = new plotWindow(this);

			plot->addScatterPlot(model->wells(n->Name())->young_age_distribution.getcummulative()*(1.0-model->wells(n->Name())->fraction_old), n->Name()+" cumulative young age distribution", 1, 0, false);
			plot->show();
		}
		if (selectedAction->text() == "Plot Young Age Distribution (Realizations)")
		{
			plotWindow *plot = new plotWindow(this);

			plot->addScatterPlot(results->youngAgeDistributionRealizations[model->wellIndex(n->Name())], false);// , n->Name() + " cumulative young age distribution", true);
			plot->show();
		}
		if (selectedAction->text() == "Plot Cumulative Young Age Distribution (Realizations)")
		{
			plotWindow *plot = new plotWindow(this);

			plot->addScatterPlot(results->cumulativeYoungAgeDistributionRealizations[model->wellIndex(n->Name())], false);// n->Name() + " cumulative young age distribution", true);
			plot->show();
		}
		if (selectedAction->text() == "Plot Young Age Distribution (Percentiles)")
		{
			plotWindow *plot = new plotWindow(this);

			plot->addScatterPlot(results->youngAgeDistributionRealizationPercentiles[model->wellIndex(n->Name())], false);// n->Name() + " cumulative young age distribution", true);
			plot->show();
		}
		if (selectedAction->text() == "Plot Cumulative Young Age Distribution (Percentiles)")
		{
			plotWindow *plot = new plotWindow(this);

			plot->addScatterPlot(results->cumulativeYoungAgeDistributionRealizationPercentiles[model->wellIndex(n->Name())], false);// n->Name() + " cumulative young age distribution", true);
			plot->show();
		}
#endif
		if (menuKey.keys().contains(selectedAction))
		{
#ifdef WQV
			if (menuKey[selectedAction][0] == "Constituent")
			{
				plotWindow *plot = new plotWindow(this);
				plot->addScatterPlot(model->ANS_constituents, menuKey[selectedAction][1].toInt());
				plot->show();
			}
			if (menuKey[selectedAction][0] == "Particle")
			{
				plotWindow *plot = new plotWindow(this);
				plot->addScatterPlot(model->ANS_colloids, menuKey[selectedAction][1].toInt());
				plot->show();
			}
			if (menuKey[selectedAction][0] == "Inflow")
			{
				CBTCSet inflow = CBTCSet(menuKey[selectedAction][1].toStdString(), 1);
				for (int i = 0; i < inflow.nvars; i++)
					if (selectedAction->text().toStdString() == inflow.names[i])
					{
						plotWindow *plot = new plotWindow(this);
						plot->addScatterDotPlot(inflow, i);
						plot->show();
					}
			}
#endif
#ifdef GWA
			if (menuKey[selectedAction][0] == "Projected")
			{
				QString name = menuKey[selectedAction][1] + '@' + n->Name();
				for (int i = 0; i < model->projected.names.size(); i++)
					if (model->projected.names[i] == name.toStdString())
					{
						plotWindow *plot = new plotWindow(this);
						plot->addScatterDotPlot(model->projected, i, "", false);
						plot->show();
					}
			}
			if (menuKey[selectedAction][0] == "Realization")
			{
				QString name = menuKey[selectedAction][1] + '@' + n->Name();
				for (int i = 0; i < model->projected.names.size(); i++)
					if (model->projected.names[i] == name.toStdString())
					{
						plotWindow *plot = new plotWindow(this, name, name + "Realizations");
						plot->addScatterPlot(results->projectedTracerConcentrations[i], false);
						plot->show();
					}
			}
			if (menuKey[selectedAction][0] == "Percentile")
			{
				QString name = menuKey[selectedAction][1] + '@' + n->Name();
				for (int i = 0; i < model->projected.names.size(); i++)
					if (model->projected.names[i] == name.toStdString())
					{
						plotWindow *plot = new plotWindow(this, name, "Projected Percentiles");
						plot->addScatterPlot(results->projectedTracerConcentrationPercentiles[i], false);
						plot->show();
					}
			}
#endif
		}
	}

}

void GraphWidget::edgeContextMenuRequested(Edge* e, QPointF pos)
{
	QMenu menu;
	QAction *deleteAction = menu.addAction("Delete");
	QAction *markAction = menu.addAction("Select");
#ifdef WQV
	if (model != 0)
		if (model->solved())
		{
			menu.addSeparator();
			menu.addAction("Plot Flow");
			menu.addAction("Velocity");
			menu.addAction("Area");
			menu.addAction("Vapor Exchange Rate"); //NEW


		}

	QAction *selectedAction = menu.exec(mapToGlobal(mapFromScene(pos.toPoint())));
	if (selectedAction != nullptr)
	{
		if (selectedAction->text() == "Select")
			e->setSelected(true);
		if (selectedAction->text() == "Delete")
			treeModel->deleteEdge(e);
		if (selectedAction->text() == "Plot Flow")
		{
			plotWindow *plot = new plotWindow(this);
			plot->addScatterPlot(model->ANS, model->Blocks.size() + model->getconnectorsq(e->Name().toStdString()), QString("%1: %2").arg(e->Name()).arg("Flow"));
			plot->show();
		}
		if (selectedAction->text() == "Velocity")
		{
			plotWindow *plot = new plotWindow(this);
			CBTC flow, area, velocity;
			flow = model->ANS.BTC[model->Blocks.size() + model->getconnectorsq(e->Name().toStdString())];
			area = model->ANS.BTC[model->Blocks.size() * 3 + model->Connector.size() + model->getconnectorsq(e->Name().toStdString())];
			velocity = flow % area;

			plot->addScatterPlot(velocity, QString("%1: %2").arg(e->Name()).arg("Velocity"), 1.0, 0.0);
			plot->show();
		}
		if (selectedAction->text() == "Area")
		{
			plotWindow *plot = new plotWindow(this);
			plot->addScatterPlot(model->ANS, model->Blocks.size() * 3 + model->Connector.size() + model->getconnectorsq(e->Name().toStdString()), QString("%1: %2").arg(e->Name()).arg("Area"));
			plot->show();
		}
		if (selectedAction->text() == "Vapor Exchange Rate")
		{
			plotWindow *plot = new plotWindow(this);
			plot->addScatterPlot(model->ANS, model->Blocks.size() * 3 + 2* model->Connector.size() + model->getconnectorsq(e->Name().toStdString()), QString("%1: %2").arg(e->Name()).arg("Vapor Exchange Rate"));
			plot->show();
		}

	}
#endif
}

QString GraphWidget::updateRelativePaths(QString oldPath, QString newPath)
{
	if (newPath == oldPath) return "Paths are same, nothing has changed.";
	QStringList oldInflowFileNames = inflowFileNames;
	inflowFileNames.clear();
	for each (QString inflow in oldInflowFileNames)
	{
		inflowFileNames.append(updateRelativePath(inflow, oldPath, newPath));
	}
	for each (Node *n in Nodes())
	{
		QStringList oldInflowFiles = n->getValue("Inflow time series").toQString().split(';');
		QString inflowFiles;
		for each (QString file in oldInflowFiles)
			inflowFiles.append(updateRelativePath(file, oldPath, newPath).append(';'));
		inflowFiles = inflowFiles.left(inflowFiles.count() - 1);
		n->setValue("Inflow time series", inflowFiles);
	}
	for each (Entity *o in entitiesByType("Observed"))
		o->setValue("Observed Data", updateRelativePath(o->getValue("Observed Data"),oldPath, newPath));
	QString name, var;
	Entity* e;

	name = "Genetic Algorithm";
	var = "initial GA population";
	e = entityByName(name);
	e->setValue(var, updateRelativePath(e->getValue(var), oldPath, newPath));

	name = "Genetic Algorithm";
	e = entityByName(name);
	var = "Evaporation time series";
	e->setValue(var, updateRelativePath(e->getValue(var), oldPath, newPath));

	var = "Precipitation time series";
	e->setValue(var, updateRelativePath(e->getValue(var), oldPath, newPath));
}
QString GraphWidget::defaultDir() const
{ 
	return QString::fromStdString(getenv("USERPROFILE")).append("/Documents/").append(applicationShortName).append(" Projects");
}
QString GraphWidget::modelPathname() const
{
	return (modelFilename == "") ? defaultDir() : OnlyPath(modelFilename);
}

QStringList GraphWidget::variableValuesHasError()
{
	int numberofErrors = 0, numberofWarnings = 0;
	Node *n0 = 0;
	Edge *e0 = 0;
	deselectAll();
	QString savedExperiment = experimentName();
	for each (QString experiment in experimentsList())
	{
		experiments->setCurrentText(experiment);
		log("Experiment: " + experiment);
		log("Blocks.");
		for each (Node* e in Nodes())
		{
			e->setSelected(true);
			if (n0) n0->setSelected(false);
			e->update(true);
			n0 = e;
			QCoreApplication::processEvents();
			e->warnings.clear();
			e->errors.clear();
			for each(QString variableName in e->variableNames())
			{
				if (e->getProp(variableName, VariableTypeRole).toString().toLower() == "directory")
					if (!e->getValue(variableName).isEmpty() && !QFile::exists(e->getValue(variableName)))
					{
						numberofErrors++;
						e->errors[variableName] = "Folder not exists.";
						log(QString("Error: %1, %2, %3, Folder (%4) does not exist.").arg(experiment).arg(e->Name()).arg(variableName).arg(e->getValue(variableName)));
					}
				if (e->getProp(variableName, VariableTypeRole).toString().toLower() == "filename")
					if (!e->getValue(variableName).isEmpty())
					{
						if (e->getProp(variableName, InputMethodRole) == "Select" && !QFile::exists(fullFilename(e->getValue(variableName), modelPathname())))
						{
							numberofErrors++;
							e->errors[variableName] = "File not exists.";
							log(QString("Error: %1, %2, %3, File (%4) does not exist.").arg(experiment).arg(e->Name()).arg(variableName).arg(e->getValue(variableName)));
						}
						else if (e->getProp(variableName, TypeRole).toString().contains("time series"))
						{
							CBTC file = CBTC(fullFilename(e->getValue(variableName), modelPathname()).toStdString());
							if (file.error)
							{
								numberofWarnings++;
								e->warnings[variableName] = "File is invalid.";
								log(QString("Warning: %1, %2, %3, File (%4) has error(s).").arg(experiment).arg(e->Name()).arg(variableName).arg(e->getValue(variableName)));
							}
						}
					}
			}
			for each(QString variableName in e->variableNameConditions().keys())
			{
				if (!EntityNames("Parameter").contains(e->getValue(variableName).toQString()))
				{
					condition c = e->variableNameConditions()[variableName];
					for (int i = 0; i < c.Condition.size(); i++)
					{
						QString condition = c.Condition[i];
						for each(QString code in e->codes())
							if (condition.contains(QString("{%1}").arg(code)))
								condition = condition.replace(QString("{%1}").arg(code), e->val(code));
						if (!conditionConformed(condition, e->props.getProp(variableName, experimentName()), this))
						{
							if (c.error[i].toLower() == "w")
							{
								numberofWarnings++;
								e->warnings[variableName] = c.errorDesc[i];
								log(QString("Warning: Block: %1, %2, %3").arg(experiment).arg(e->Name()).arg(c.errorDesc[i]));
							}
							if (c.error[i].toLower() == "e")
							{
								numberofErrors++;
								e->errors[variableName] = c.errorDesc[i];
								log(QString("Error: Block: %1, %2, %3").arg(experiment).arg(e->Name()).arg(c.errorDesc[i]));
							}
						}
					}
				}
			}
		}
		deselectAll();
		log("Connectors.");
		for each (Edge* e in Edges())
		{
			e->setSelected(true);
			if (e0) e0->setSelected(false);
			e->update();
			e0 = e;
			QCoreApplication::processEvents();
			e->warnings.clear();
			e->errors.clear();
			for each(QString variableName in e->variableNames())
			{
				if (e->getProp(variableName, VariableTypeRole).toString().toLower() == "directory")
					if (!e->getValue(variableName).isEmpty() && !QFile::exists(e->getValue(variableName)))
					{
						numberofErrors++;
						e->errors[variableName] = "Folder not exists.";
						log(QString("Error: %1, %2, Folder (%3) does not exist.").arg(experiment).arg(variableName).arg(e->getValue(variableName)));
					}
				if (e->getProp(variableName, VariableTypeRole).toString().toLower() == "filename")
					if (!e->getValue(variableName).isEmpty())
					{
						if (e->getProp(variableName, InputMethodRole) == "Select" && !QFile::exists(fullFilename(e->getValue(variableName), modelPathname())))
						{
							numberofErrors++;
							e->errors[variableName] = "File not exists.";
							log(QString("Error: %1, %2, File (%3) does not exist.").arg(experiment).arg(variableName).arg(e->getValue(variableName)));
						}
						else if (e->getProp(variableName, TypeRole).toString().contains("time series"))
						{
							CBTC file = CBTC(fullFilename(e->getValue(variableName), modelPathname()).toStdString());
							if (file.error)
							{
								numberofWarnings++;
								e->warnings[variableName] = "File is invalid.";
								log(QString("Warning: %1, %2, %3, File (%4) has error(s).").arg(experiment).arg(e->Name()).arg(variableName).arg(e->getValue(variableName)));
							}
						}
					}
			}
#ifdef WQV
			if (e->val("presc_flow").toBool())
				if (e->val("pre_flow_filename").toQString().isEmpty())
				{
					numberofWarnings++;
					e->warnings["Use Prescribed Flow"] = "Prescribed time series is not defined.";
					e->setProp("Use Prescribed Flow", "No");
					log(QString("Warning: Connector: %1, %2, %3").arg(experiment).arg(e->Name()).arg("Prescribed time series is not defined."));
				}
#endif
			for each(QString variableName in e->variableNameConditions().keys())
			{
				if (!EntityNames("Parameter").contains(e->getValue(variableName).toQString()))
				{
					condition c = e->variableNameConditions()[variableName];
					for (int i = 0; i < c.Condition.size(); i++)
					{
						QString condition = c.Condition[i];
						for each(QString code in e->codes())
							if (condition.contains(QString("{%1}").arg(code)))
								condition = condition.replace(QString("{%1}").arg(code), e->val(code));
						if (!conditionConformed(condition, e->props.getProp(variableName, experimentName()), this))
						{
							if (c.error[i].toLower() == "w")
							{
								numberofWarnings++;
								e->warnings[variableName] = c.errorDesc[i];
								log(QString("Warning: Connector: %1, %2, %3").arg(experiment).arg(e->Name()).arg(c.errorDesc[i]));
							}
							if (c.error[i].toLower() == "e")
							{
								numberofErrors++;
								e->errors[variableName] = c.errorDesc[i];
								log(QString("Error: Connector: %1, %2, %3").arg(experiment).arg(e->Name()).arg(c.errorDesc[i]));
							}
						}
					}
				}
			}
		}
		deselectAll();
		log("Other entities.");
		QCoreApplication::processEvents();
		for each (Entity* e in Entities)
		{
			e->warnings.clear();
			e->errors.clear();
			for each(QString variableName in e->variableNames())
			{
				if (e->getProp(variableName, VariableTypeRole).toString().toLower() == "directory")
					if (!e->getValue(variableName).isEmpty() && !QFile::exists(e->getValue(variableName)))
					{
						numberofErrors++;
						e->errors[variableName] = "Folder not exists.";
						log(QString("Error: %1, %2, Folder (%3) does not exist.").arg(experiment).arg(variableName).arg(e->getValue(variableName)));
					}
				if (e->getProp(variableName, VariableTypeRole).toString().toLower() == "filename")
					if (e->getProp(variableName, VariableTypeRole).toString().toLower() == "filename")
						if (!e->getValue(variableName).isEmpty())
						{
							if (e->getProp(variableName, InputMethodRole) == "Select" && !QFile::exists(fullFilename(e->getValue(variableName), modelPathname())))
							{
								numberofErrors++;
								e->errors[variableName] = "File not exists.";
								log(QString("Error: %1, %2, File (%3) does not exist.").arg(experiment).arg(variableName).arg(e->getValue(variableName)));
							}
							else if (e->getProp(variableName, TypeRole).toString().contains("time series"))
							{
								CBTC file = CBTC(fullFilename(e->getValue(variableName), modelPathname()).toStdString());
								if (file.error)
								{
									numberofWarnings++;
									e->warnings[variableName] = "File is invalid.";
									log(QString("Warning: %1, %2, %3, File (%4) has error(s).").arg(experiment).arg(e->Name()).arg(variableName).arg(e->getValue(variableName)));
								}
							}
						}
			}
			for each(QString variableName in e->variableNameConditions().keys())
			{
				if (!EntityNames("Parameter").contains(e->getValue(variableName).toQString()))
				{
					condition c = e->variableNameConditions()[variableName];
					for (int i = 0; i < c.Condition.size(); i++)
					{
						QString condition = c.Condition[i];
						for each(QString code in e->codes())
							if (condition.contains(QString("{%1}").arg(code)))
								condition = condition.replace(QString("{%1}").arg(code), e->val(code));
						if (e->props.getProp(variableName, experimentName()).isEmpty())
						{
							if (condition.toLower().contains("not empty"))
							{
								numberofErrors++;
								e->errors[variableName] = variableName + " should not be empty.";
								log(QString("Error: %1, %2: %3, %4").arg(experiment).arg(e->getValue("Type")).arg(e->Name()).arg(variableName + " should not be empty."));
							}
						}
						else
							if (!conditionConformed(condition, e->props.getProp(variableName, experimentName()), this))
							{
								if (c.error[i].toLower() == "w")
								{
									numberofWarnings++;
									e->warnings[variableName] = c.errorDesc[i];
									log(QString("Warning: %1, %2: %3, %4").arg(experiment).arg(e->getValue("Type")).arg(e->Name()).arg(c.errorDesc[i]));
								}
								if (c.error[i].toLower() == "e")
								{
									numberofErrors++;
									e->errors[variableName] = c.errorDesc[i];
									log(QString("Error: %1, %2: %3, %4").arg(experiment).arg(e->getValue("Type")).arg(e->Name()).arg(c.errorDesc[i]));
								}
							}
					}

				}
			}
		}
#ifdef WQV
		// Check for Observation Locations
		for each (Entity* e in entitiesByType("Observation"))
		{
			if (e->val("loc_type") == "Block" && !nodeNames().contains(e->val("id")))
			{
				numberofErrors++;
				QString error = QString("Block %1 is not available.").arg(e->val("id"));
				e->errors["Location"] = error;
				log(QString("Error: %1, %2: %3, %4").arg(experiment).arg(e->getValue("Type")).arg(e->Name()).arg(error));
			}
			if (e->val("loc_type") == "Connector" && !edgeNames().contains(e->val("id")))
			{
				numberofErrors++;
				QString error = QString("Experiment %1, connector %2 is not available.").arg(experiment).arg(e->val("id"));
				e->errors["Location"] = error;
				log(QString("Error: %1, %2: %3, %4").arg(experiment).arg(e->getValue("Type")).arg(e->Name()).arg(error));
			}

		}
#endif
	}
	QStringList r;
	r << QString::number(numberofErrors) << QString::number(numberofWarnings);
	experiments->setCurrentText(savedExperiment);
	return r;
}
double GraphWidget::minX() const
{
	double n = 100000000;
	for each (Node* e in Nodes())
		n = min(e->x(), n);
	return n;
}
double GraphWidget::minY() const
{
	double n = 100000000;
	for each (Node* e in Nodes())
		n = min(e->y(), n);
	return n;
}
double GraphWidget::maxX() const
{
	double n = -1000000000;
	for each (Node* e in Nodes())
		n = max(e->x() + e->Width(), n);
	return n;
}
double GraphWidget::maxY() const
{
	double n = -1000000000;
	for each (Node* e in Nodes())
		n = max(e->y() + e->Height(), n);
	return n;
}
void GraphWidget::nodeChanged(Node*)
{
	gwChanged();
}
void GraphWidget::edgeChanged(Edge*)
{
	gwChanged();
}
void GraphWidget::entityChanged(Entity*)
{
	gwChanged();
}
void GraphWidget::gwChanged()
{
	if (trackingUndo)
	{
		add_to_undo_list();
		changedState = true;
		emit changed();
	}
}
bool validInflowFile(QString file)
{
//	QMap<QString, QString> inflowCurveNames;
//	QStringList inflowFileNamesofNode = n->getValue("Inflow time series").toQString().split(';');
//	for each(QString file in inflowFileNamesofNode)
//	{
	if (file.isEmpty())	return true;
		CBTCSet inflows = CBTCSet(file.toStdString(), 1);
		if (inflows.names.size() == 0) return false;
		return true;
//		for each (string name in inflows.names)
//		{
//			if (!inflowCurveNames.keys().contains(QString::fromStdString(name)))
//				inflowCurveNames[QString::fromStdString(name)] = file;
//			else warning(QString("Duplicate Headers in Inflow time series, Block %1, Header %2, inFlow file(s) %3").arg(n->Name()).arg(QString::fromStdString(name)).arg(n->getValue("Inflow time series")));
//		}
//	}
}

void GraphWidget::delegateDatePicked(QCalendarWidget *calendar, QModelIndex index)
{
	static QCalendarWidget *currentCalendar;
	static QModelIndex currentIndex;
	static QAbstractItemModel* currentModel;
	if (calendar != nullptr)
	{
		currentCalendar = calendar;
		currentIndex = index;
		currentModel = propModel();
		return;
	}

	//setModelData(currentEditor, currentModel, currentIndex);
	int date = currentCalendar->selectedDate().toJulianDay() - QDate(1900, 1, 1).toJulianDay();
	currentModel->setData(currentIndex, date, Qt::EditRole);

}

#ifdef WQV
void GraphWidget::experimentSelect(const QString & experimentName)
{
	if (experimentID() && (resultsSet.size() >= experimentID()))
		results = resultsSet[experimentID() - 1];

	QAbstractItemModel * tablePropModel = tableProp->model();
	tableProp->setModel(0);
	tableProp->setModel(tablePropModel);


	if (modelSet)
		if (modelSet->ANS_hyd.size())
			if (experimentID() - 1 < modelSet->Medium.size())
				model = &modelSet->Medium[experimentID() - 1];

	update();
}
#endif
int GraphWidget::experimentID()
{
	return experiments->currentIndex();
}
QString GraphWidget::experimentName()
{
	return experiments->currentText();
}

#ifdef WQV
bool GraphWidget::wizard(QList<command> &commands)
{
	return true;
}
#endif

void GraphWidget::copyProps(QString sourceExperiment, QString destExperiment)
{
	for each (Node *n in Nodes())
		n->copyProps(sourceExperiment, destExperiment);
	for each (Edge *e in Edges())
		e->copyProps(sourceExperiment, destExperiment);
	for each (Entity *e in Entities)
		e->copyProps(sourceExperiment, destExperiment);

}