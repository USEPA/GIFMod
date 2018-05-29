#pragma once
#include "GWidget.h"
#include "edge.h"
#include "node.h"
#include <math.h>
#include <QKeyEvent>
#include <qdebug.h>
//#include "utility_funcs.h"

//#include <mainwindow.h>
#include <vector>
#include "ray.h"
#include "entity.h"
#include "qtableview.h"
#include "treemodel.h"
#include "qmessagebox.h"
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
#include "qvariant.h"
#include "colorScheme.h"
//#include "utility_funcs.h"
#include "qfiledialog.h"
#ifdef GIFMOD
#include "Medium.h"
#include "mainwindow.h"
#include "ui_mainwindowGIFMod.h"
#include "MediumSet.h"
#include "BTCSet.h"
#include "reactionwindow.h"
#include "reactiontablemodel.h"
#endif
#ifdef GWA
#include "gwa.h"
#include "utility_funcs.h"
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

#ifdef GIFMOD
	help = new helpWindow(this, "Help", "Help.txt");
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
    for (QString item : tempPhysicalCharacteristicsList)
		PhysicalCharacteristicsList.append(XString(item));
    QList <mProp> QL1 = mList->List;

	ModelSpace.Model = mList->Models()[0];
#ifdef GIFMOD
	new Entity("Solver settings", "Solver settings", this);
	new Entity("Project settings", "Project settings", this);
	new Entity("Climate settings", "Climate settings", this);
	new Entity("Genetic algorithm", "Genetic algorithm", this);
	new Entity("Markov chain Monte Carlo", "Markov chain Monte Carlo", this);
#endif

#ifdef GWA
	new Entity("Project settings", "Project settings", this);
	new Entity("Genetic algorithm", "Genetic algorithm", this);
	new Entity("Markov chain Monte Carlo", "Markov chain Monte Carlo", this);
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
    for (Node *node : Nodes()) if (node->isSelected()) {
		treeModel->deleteNode(node);
//		for (Edge *edge in node->edgeList)
//			delete edge;
//		delete node;
	}
    for (Edge *edge : Edges()) if (edge->isSelected()) delete edge;
	deselectAll();
}

QStringList GraphWidget::selectedItems()const
{
	QStringList names;
	QList<Node *> nodes;
    for (QGraphicsItem *item : scene()->items())
	{
		if (Node *node = qgraphicsitem_cast<Node *>(item))
		{
			nodes << node;
			names << node->Name();
		}
	}

	QList<Edge *> edges;
    for (QGraphicsItem *item : scene()->items())
	{
		if (Edge *edge = qgraphicsitem_cast<Edge *>(item))
		{
			edges << edge;
			names << edge->Name();
		}
	}

	QList<Entity *> entities;
    for (Entity *entity : Entities)
	{
		if (entity->isSelected())
		{
			entities << entity;
			names << entity->Name();
		}
	}

	return names;
}

QString GraphWidget::typeOfSelecetedItems()const
{
	QList<Node *> nodes;
    for (QGraphicsItem *item : scene()->items())
	{
		if (Node *node = qgraphicsitem_cast<Node *>(item))
			nodes << node;
	}

	QList<Edge *> edges;
    for (QGraphicsItem *item : scene()->items())
	{
		if (Edge *edge = qgraphicsitem_cast<Edge *>(item))
			edges << edge;
	}

	QList<Entity *> entities;
    for(Entity *entity : Entities)
	{
		if (entity->isSelected())
			entities << entity;
	}

	if (nodes.size() && !edges.size() && !entities.size())
		return "Nodes";
	if (!nodes.size() && edges.size() && !entities.size())
		return "Edges";
	if (!nodes.size() && !edges.size() && entities.size())
		return "Entities";
	return "None";
}

QList<Node*> GraphWidget::selectedNodes() const
{
	QList<Node *> nodes;
    for(QGraphicsItem *item : scene()->items())
	{
		if (Node *node = qgraphicsitem_cast<Node *>(item))
			if (node->isSelected())
				nodes << node;
	}
	return nodes;
}

QList<Edge*> GraphWidget::selectedEdges() const
{
	QList<Edge *> edges;
    for(QGraphicsItem *item : scene()->items())
	{
		if (Edge *edge = qgraphicsitem_cast<Edge *>(item))
			if (edge->isSelected())
				edges << edge;
	}
	return edges;
}

QList<Entity*> GraphWidget::selectedEntities()const
{
	QList<Entity *> entities;
    for(Entity *entity : Entities)
	{
		if (entity->isSelected() )
			entities << entity;
	}
	return entities;
}

void GraphWidget::deselectAll(QString items) const
{
	if (items.toLower().contains("node"))
        for (Node *node : Nodes()) node->setSelected(false);
	if (items.toLower().contains("edge"))
        for (Edge *edge : Edges()) edge->setSelected(false);
	if (items.toLower().contains("entity"))
        for (Entity *entity : Entities) entity->setSelected(false);
	if (items.toLower().contains("node") ||
		items.toLower().contains("edge") ||
		items.toLower().contains("entity"))
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


void GraphWidget::rubberBandChanged(QRect rubberBandRect, QPointF fromScenePoint, QPointF toScenePoint)
{
	if (fromScenePoint.x() <= toScenePoint.x()) //left to right selection, keep nodes
		deselectAll("Edges");
	else
		deselectAll("Nodes");
}
QList<Node*> GraphWidget::nodes(const QList<QGraphicsItem*> items) const
{
	QList<Node *> nodes;
    for(QGraphicsItem *item : items)
	{
		if (Node *node = qgraphicsitem_cast<Node *>(item))
			nodes << node;
	}
	return nodes;
}
QList<Edge*> GraphWidget::edges(const QList<QGraphicsItem*>items) const
{
	QList<Edge *> edges;
    for(QGraphicsItem *item : items)
	{
		if (Edge *edge = qgraphicsitem_cast<Edge *>(item))
			edges << edge;
	}
	return edges;
}
void GraphWidget::update(bool fast)
{
    for (Node* n : Nodes()) n->update(fast);
    for (Edge* e : Edges()) e->update(fast);
	

}
void GraphWidget::mousePressEvent(QMouseEvent *event)
{
	tableProp->setModel(0);
	Node *node = qgraphicsitem_cast<Node*> (itemAt(event->pos())); //Get the item at the position
	if (node)
	{	//qDebug() << "Name: "<< node->Name()<<" Flag:" << node->flags() << "enabled:" << node->isEnabled() << "active:" << node->isActive();
	}
	Edge *edge = qgraphicsitem_cast<Edge*> (itemAt(event->pos())); //Get the item at the position
	if (edge)
	{	//qDebug() << "Name: " << edge->Name() << " Flag:" << edge->flags() << "enabled:" << edge->isEnabled() << "active:" << edge->isActive();
	}

	if (event->buttons() == Qt::MiddleButton && Operation_Mode == Operation_Modes::NormalMode)
	{
		setMode(Operation_Modes::Pan);
		QMouseEvent *newEvent = new QMouseEvent(event->type(), event->localPos(), Qt::LeftButton, Qt::MouseButtons(1), event->modifiers());
		QGraphicsView::mousePressEvent(newEvent);
		delete newEvent;
		return;
	}
	QGraphicsView::mousePressEvent(event); //Call the ancestor
	if (Operation_Mode == Operation_Modes::NormalMode)
	{
		setDragMode((node || edge) ? QGraphicsView::NoDrag : QGraphicsView::RubberBandDrag);

		if (node)
			if (node->itemType == Object_Types::Block)
			{
				int xx = mapToScene(event->pos()).x();	int yy = mapToScene(event->pos()).y();
				if (event->buttons() == Qt::LeftButton)
				{
					if (event->modifiers() && Qt::ControlModifier) {
//						node->setFlag(QGraphicsItem::ItemIsMovable, false);
//						node->setSelected(true);
					}
					else if (node->corner(xx, yy)) {
						setDragMode(QGraphicsView::NoDrag);
						setMode(Operation_Modes::resizeNode);
						resizenode = node;
						resizecorner = node->corner(xx, yy);
						node->setFlag(QGraphicsItem::ItemIsMovable, false);
					}
					else if (node->edge(xx, yy)) {
						node->setFlag(QGraphicsItem::ItemIsMovable, false);
						Node1 = node;
						tempRay = new Ray();
						MainGraphicsScene->addItem(tempRay);
						setMode(Operation_Modes::Node1_selected);
					}
					else
						node->setFlag(QGraphicsItem::ItemIsMovable, true);
				}
			}
		if (event->buttons() == Qt::LeftButton && Operation_Mode == Operation_Modes::Draw_Connector)
		{
			if (node) {
				if (node->itemType == Object_Types::Block) {
					node->setFlag(QGraphicsItem::ItemIsMovable, false);
					Node1 = node;
					tempRay = new Ray();
					MainGraphicsScene->addItem(tempRay);
					setMode(Operation_Modes::Node1_selected);
				}
			}
		}
	}

	//QGraphicsView::mousePressEvent(event);
}
void GraphWidget::mouseMoveEvent(QMouseEvent *event)
{
	//	//qDebug() << "Mouse MOVE, button: " << event->button() << ", modifier: " << event->modifiers() << ", buttons: " << event->buttons();
	_x = mapToScene(event->pos()).x();
	_y = mapToScene(event->pos()).y();
	int xx = _x;// mapToScene(event->pos()).x();
	int yy = _y;// mapToScene(event->pos()).y();

	if (event->buttons() == Qt::MiddleButton && Operation_Mode == Operation_Modes::Pan)
	{
		QMouseEvent *newEvent = new QMouseEvent(event->type(), event->localPos(), Qt::LeftButton, Qt::MouseButtons(1), event->modifiers());
		//		QGraphicsView::mousePressEvent(newEvent);
		QGraphicsView::mouseMoveEvent(newEvent);
		delete newEvent;
		return;
	}

	if (event->buttons() == Qt::LeftButton && Operation_Mode == Operation_Modes::NormalMode && dragMode() == DragMode::RubberBandDrag)
	{
		QGraphicsView::mouseMoveEvent(event);
		//qDebug() << event->x() << rubberBandRect().x();
		if (event->x() > rubberBandRect().x()) //Dragging to the right
		{
            for (Edge* item : edges(items(rubberBandRect())))
				item->setFlag(QGraphicsItem::ItemIsSelectable, false);
		}
		else
		{
            for (Node* item : nodes(items(rubberBandRect())))
				item->setFlag(QGraphicsItem::ItemIsSelectable, false);

		}
		return;
	}

	bool cursorModeNormal = true;
	setToolTip("");
	QGraphicsView::mouseMoveEvent(event);	
	QString txt;
	Node *n1 = qgraphicsitem_cast<Node*> (itemAt(event->pos())); //Get the item at the position
	if (n1) //itemType == Object_Types::Block)
	{
		txt = QString("%1, %2: %3").arg(n1->ObjectType().GuiObject).arg(n1->ObjectType().ObjectType).arg(n1->Name());
		QString toolTip = QString("Type: %1\nName: %2").arg(n1->ObjectType().ObjectType).arg(n1->Name());
		toolTip.append(QString("\nBottom Elevation: %1").arg(n1->val("z0").toStringUnit()));
		if (n1->errors.count()) toolTip.append(QString("\n%1 Error(s)").arg(n1->errors.count()));
		if (n1->warnings.count()) toolTip.append(QString("\n%1 Warning(s)").arg(n1->warnings.count()));
		setToolTip(toolTip);
	}
    //for (Node* n in Nodes())
	//	if (n!=n1) n->setBold(false);
	Edge *e1 = qgraphicsitem_cast<Edge*> (itemAt(event->pos())); //Get the item at the position
	if (e1)
		//	if (c2 && c2->itemType == Object_Types::Connector && c2->dist(mapToScene(event->pos())) < 120) //GUI == "Connector"
	{
		//		e1->setBold(true);
		//		e1->update();
		//txt = QString("%1, %2: %3").arg(c2->ObjectType().GuiObject).arg(c2->ObjectType().ObjectType).arg(c2->Name());
		QString toolTip = QString("%1, %2: %3").arg(e1->ObjectType().GuiObject).arg(e1->ObjectType().ObjectType).arg(e1->ObjectType().SubType);
		//QString toolTip = QString("Type: %1\nName: %2").arg(c1->ObjectType().ObjectType).arg(c1->Name());
		//toolTip.append(QString("\nBottom Elevation: %1").arg(c1->val("z0").toStringUnit()));
		//if (c1->errors.count()) toolTip.append(QString("\n%1 Error(s)").arg(c1->errors.count()));
		//if (c1->warnings.count()) toolTip.append(QString("\n%1 Warning(s)").arg(c1->warnings.count()));
		setToolTip(toolTip);

	}
    //for (Edge*e in Edges())
	//	if (e!=e1) e->setBold(false);
	//update();
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
            QPointF p = QPointF(mapToScene(event->pos()));
            tempRay->adjust(Node1, &p);
		}
	}
	if (Operation_Mode == Operation_Modes::NormalMode && dragMode()==DragMode::NoDrag)
	{
		Node *node = qgraphicsitem_cast<Node*> (itemAt(event->pos())); //Get the item at the position
																	   /*		Edge *edge = static_cast<Edge*> (itemAt(event->pos())); //Get the item at the position
																	   if (edge)
																	   {
																	   setCursor(Qt::ForbiddenCursor);
																	   //qDebug() << edge->dist(mapToScene(event->pos()));
																	   }
																	   */
		if (node)
			if (node->itemType == Object_Types::Block)
			{
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
		if (resizecorner == topright && (xx - px) > minW && (py - yy + ph) > minH)
		{
			//resizenode->setX(xx);
			resizenode->setWidth(xx - px);
			resizenode->setY(yy);
			resizenode->setHeight(py - yy + ph);
		}
		if (resizecorner == bottomright && (xx - px) > minW && (yy - py) > minH)
		{
			//resizenode->setX(xx);
			resizenode->setWidth(xx - px);
			//resizenode->setY(yy);
			resizenode->setHeight(yy - py);
		}
		resizenode->update();
        for(Edge *edge : resizenode->edges())
			edge->adjust();
	}
	if (cursorModeNormal)
		setCursor(Qt::ArrowCursor);
}
void GraphWidget::mouseReleaseEvent(QMouseEvent *event)
{
//	//qDebug() << "Mouse RELEASE, button: " << event->button() << ", modifier: " << event->modifiers() << ", buttons: " << event->buttons()<<", dragMode: "<< dragMode();
    for (Edge * item : selectedEdges())
		//qDebug() << item->Name();
	if (event->button() == Qt::LeftButton && Operation_Mode == Operation_Modes::NormalMode && dragMode() == DragMode::RubberBandDrag)
	{
        for (Node* item : Nodes())
			item->setFlag(QGraphicsItem::ItemIsSelectable, true);
        for (Edge* item : Edges())
			item->setFlag(QGraphicsItem::ItemIsSelectable, true);
	}
	if (event->button() == Qt::MiddleButton && Operation_Mode == Operation_Modes::Pan)
	{
		setMode(Operation_Modes::NormalMode);
		QMouseEvent *newEvent = new QMouseEvent(event->type(), event->localPos(), Qt::LeftButton, Qt::MouseButtons(1), event->modifiers());
		QGraphicsView::mouseReleaseEvent(newEvent);
		delete newEvent;
		return;

	}
	QGraphicsView::mouseReleaseEvent(event); //Call the ancestor

	switch (Operation_Mode) {
	case Operation_Modes::resizeNode:
	{
		setMode(NormalMode, true);
		break;
	}
	case Operation_Modes::NormalMode:
	{
		if (dragMode() != DragMode::RubberBandDrag)
		{
		
		}
		Node *node = qgraphicsitem_cast<Node*> (itemAt(event->pos())); //Get the item at the position
		Edge *edge = qgraphicsitem_cast<Edge*> (itemAt(event->pos())); //Get the item at the position
		if (event->button() == Qt::LeftButton && dragMode()!=DragMode::RubberBandDrag)
			if (event->modifiers() && Qt::ControlModifier) {
				if (node)
				{
					if (selectedNodes().contains(node))
						node->setSelected(true);
					else
						node->setSelected(false);
				}
				if (edge)
				{
					if (selectedEdges().contains(edge))
						edge->setSelected(false);
					else
						edge->setSelected(true);
				}
			}
			else {
				if (node)
                    for (Node * node : Nodes())
							node->setFlag(QGraphicsItem::ItemIsMovable, false);
				if (edge)
				{
					if (edge->dist(mapToScene(event->pos())) < 120) 
						edge->setSelected(true);
				}
				//if (!node && !edge) deselectAll();
			}
			break;
	}
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
	//	default:
	}
	bool changed = false;
    for (Node *n : Nodes())
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
			n->setProp("x",n->x());
			n->setProp("y", n->y());
			n->setProp("Superficial width", n->Width());
			n->setProp("Superficial height", n->Height());
		}
	}
	if (changed) 
		gwChanged();

}
void GraphWidget::updateNodeCoordinates()
{
    for (Node *n : Nodes())
	{
		if (specs[n->Name()]["x"].toFloat() != n->x() ||
			specs[n->Name()]["y"].toFloat() != n->y() ||
			specs[n->Name()]["w"].toFloat() != n->Width() ||
			specs[n->Name()]["h"].toFloat() != n->Height())
		{
			specs[n->Name()]["x"] = QString::number(n->x());
			specs[n->Name()]["y"] = QString::number(n->y());
			specs[n->Name()]["w"] = QString::number(n->Width());
			specs[n->Name()]["h"] = QString::number(n->Height());
		}
	}
}
#ifdef GIFMOD
void GraphWidget::updateNodesColorCodes(QString propertyName, bool logged, QString colorTheme, vector<double> predifinedMinMax, float time)
{
	if (!hasResults)
	{
		QMessageBox::information(0, "GIFMod", "There is no results available, please run the model first.");
		return;
	}
	//	colorlegend colors;
	if (time == -1)
	{
		vector<CBTC> data;
		vector<float> factors;
		vector<float> shifts;
		QStringList nodeNames = this->nodeNames();
		QStringList removedNodes;
		for (int i = 0; i < nodeNames.size(); i++)
		{
			float factor = 1;
			float shift = 0;
			bool remove = false;
			int index = model->getblocksq(nodeNames[i].toStdString());
			if (propertyName == "Storage") {
                data.push_back(model->Results.ANS[index]);
			}
			if (propertyName == "Head") {
                data.push_back(model->Results.ANS[Edges().count() + Nodes().count() + index]);
			}
			if (propertyName == "Moisture content")
			{
				Node *n = node(nodeNames[i]);
				if (!n->isPorous())
				{
					n->color.color1 = QColor(255, 255, 255, 128);
					removedNodes.append(nodeNames[i]);
					remove = true;
				}
				else
				{
					double volume = n->val("a").convertToDefaultUnit().toDouble() * n->val("depth").convertToDefaultUnit().toDouble();
					factor = 1 / volume;
                    data.push_back(model->Results.ANS[index]);
				}
			}
			if (propertyName == "Water depth")
			{
				Node *n = node(nodeNames[i]);
				double z0 = n->val("z0").convertToDefaultUnit().toDouble();// model->Blocks[model->getblocksq(n->Name().toStdString())].z0;
				shift = -z0;
                data.push_back(model->Results.ANS[Edges().count() + Nodes().count() + index]);
			}
			if (propertyName == "Evapotranspiration rate")
			{
                data.push_back(model->Results.ANS[Edges().count() + 2 * Nodes().count() + index]);
			}
			if (!remove)
			{
				factors.push_back(factor);
				shifts.push_back(shift);
			}
		}
        for (QString rN : removedNodes)
			nodeNames.removeAll(rN);
		colors.data = data;
		colors.factors = factors;
		colors.shifts = shifts;
		colors.nodeNames = nodeNames;
		colors.propertyName = propertyName;
	}

	if (time == -1)
		time = model->Timemin;
	//float t = QInputDialog::getDouble(qApp->activeWindow(), "Input Dialog Box", QString("Enter time between(%1-%2):").arg(model->Timemin).arg(model->Timemax), 0, model->Timemin, model->Timemax, 4);

	colorScheme::colorandLegend(colors, time, "Blue-Red", false, 8);
	applyColorstoNodes();
}

void GraphWidget::updateNodesColorCodes_WaterQuality(QStringList property, bool logged, QString colorTheme, vector<double> predifinedMinMax, float time)
{
	if (!hasResults)
	{
		QMessageBox::information(0, "GIFMod", "There is no results available, please run the model first.");
		return;
	}
	//	colorlegend colors;
	if (time == -1)
	{
		vector<CBTC> data;
		vector<float> factors;
		vector<float> shifts;
		QStringList nodeNames = this->nodeNames();
		QStringList removedNodes;
		for (int i = 0; i < nodeNames.size(); i++)
		{
			float factor = 1;
			float shift = 0;
			int index = model->getblocksq(nodeNames[i].toStdString());
			if (property[0] == "Particle")
			{
				int index = model->get_member_no(nodeNames[i], property[1], property[3]);
                data.push_back(model->Results.ANS_colloids[index]);
			}
			else
			{
				int index = model->get_member_no(nodeNames[i], property[1], property[3], property[2]);
                data.push_back(model->Results.ANS_constituents[index]);
			}
			factors.push_back(factor);
			shifts.push_back(shift);
		}
//		for (QString rN in removedNodes)
//			nodeNames.removeAll(rN);
		colors.data = data;
		colors.factors = factors;
		colors.shifts = shifts;
		colors.nodeNames = nodeNames;
		colors.propertyName = QString("%1 %2(%3)").arg(property[1]).arg(property[3]).arg(property[2]);
	}

	if (time == -1)
		time = model->Timemin;
	//float t = QInputDialog::getDouble(qApp->activeWindow(), "Input Dialog Box", QString("Enter time between(%1-%2):").arg(model->Timemin).arg(model->Timemax), 0, model->Timemin, model->Timemax, 4);

	colorScheme::colorandLegend(colors, time, "Blue-Red", false, 8);
	applyColorstoNodes();
}
void GraphWidget::updateEdgesColorCodes(QString propertyName, bool logged, QString colorTheme, vector<double> predifinedMinMax, float time)
{
	if (!hasResults)
	{
		QMessageBox::information(0, "GIFMod", "There is no results available, please run the model first.");
		return;
	}
	//	colorlegend colors;
	if (time == -1)
	{
		vector<CBTC> data;
		vector<float> factors;
		vector<float> shifts;
		QStringList edgeNames = this->edgeNames();
//		QStringList removedEdges;
		for (int i = 0; i < edgeNames.size(); i++)
		{
			float factor = 1;
			float shift = 0;
			int index = model->getconnectorsq(edgeNames[i].toStdString());
			if (propertyName == "Flow") {
                data.push_back(model->Results.ANS[Nodes().count() + index].fabs());
			}
			if (propertyName == "Velocity") {
				CBTC flow, area, velocity;
                flow = model->Results.ANS.BTC[Nodes().count() + index];
                area = model->Results.ANS.BTC[Nodes().count() * 3 + Edges().count() + index];
				velocity = flow % area;
				data.push_back(velocity.fabs());
			}
			if (propertyName == "Area") {
                data.push_back(model->Results.ANS[Nodes().count() * 3 + Edges().size() + index]);
			}
			if (propertyName == "Vapor exchange rate") {
                data.push_back(model->Results.ANS[Nodes().count() * 3 + 2 * Edges().size() + index].fabs());
			}
			factors.push_back(factor);
			shifts.push_back(shift);
		}
//		for (QString rN in removedNodes)
//			nodeNames.removeAll(rN);
		colors.data = data;
		colors.factors = factors;
		colors.shifts = shifts;
		colors.edgeNames = edgeNames;
		colors.propertyName = propertyName;
	}

	if (time == -1)
		time = model->Timemin;
	//float t = QInputDialog::getDouble(qApp->activeWindow(), "Input Dialog Box", QString("Enter time between(%1-%2):").arg(model->Timemin).arg(model->Timemax), 0, model->Timemin, model->Timemax, 4);

	colorScheme::colorandLegend(colors, time, "Blue-Red", false, 8);
	applyColorstoEdges();
}

void GraphWidget::applyColorstoNodes()
{
	for (int i = 0; i < colors.nodeNames.size(); i++)
	{
		Node *n = node(colors.nodeNames[i]);
		n->color.color1 = colors.colors[i];
		n->color.color2 = colors.colors[i];
		n->middleText = colors.middleText[i];
		n->update(true);
	}
	colorCode.nodes = true;
	QString title = QString("%1 at t=%2").arg(colors.propertyName).arg(colors.time);
	colorScheme::showColorandLegend(colors, title, this);
//	update();
	
}

void GraphWidget::applyColorstoEdges()
{
	for (int i = 0; i < colors.edgeNames.size(); i++)
	{
		Edge *e = edge(colors.edgeNames[i]);
		e->color.color1 = colors.colors[i];
		e->update(true);
	}
	colorCode.edges = true;
	QString title = QString("%1 at t=%2").arg(colors.propertyName).arg(colors.time);
	colorScheme::showColorandLegend(colors, title, this);
	//	update();
}
#endif
void GraphWidget::settableProp(QTableView*_tableProp)
{
	tableProp = _tableProp;
}

//void GraphWidget::setpropModel(PropModel *_propModel)
//{
//	propModel = _propModel;
//}
void GraphWidget::add_to_undo_list(QList<QMap<QString, QVariant>> state)
{
	if (undo_counter < undolist.size() )
		for (int i = undolist.size() ; i > undo_counter; i--)
			undolist.pop_back();
	if (state == QList<QMap<QString, QVariant>>())
		undolist.append(compact());
	else
		undolist.append(state);
	undo_counter = undolist.size(); 
	//qDebug() << "Undo Counter (added) :" << undolist.size() << undo_counter;	
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
	//qDebug() << undolist.size() << undo_counter;
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
	//qDebug() << undolist.size() << undo_counter;
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
    for(QGraphicsItem *item : scene()->items())
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
		setCursor(Qt::CrossCursor);
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
        for (Node* node : Nodes())
			if (node->Name() == name)
			{
				deselectAll();
				node->setSelected(true);
				r = true;
				return r;
			}

	if (type == "Connector")
        for (Edge* edge : Edges())
			if (edge->Name() == name)
			{
				deselectAll();
				edge->setSelected(true);
				r = true;
				return r;
			}
    for (Entity* entity : Entities)
		if (entity->objectType.ObjectType== type && entity->Name() == name)
		{
			deselectAll();
			tableProp->setModel(entity->model);
			entity->setSelected(true);
			r = true;
			return r;
		}
    for (Entity* entity : Entities)
		if (entity->objectType.ObjectType == name && entity->Name() == name)
		{
			deselectAll();
			tableProp->setModel(entity->model);
			r = true;
			return r;
		}
	if (type == "Well" || type == "Tracer")
        for (Node* node : Nodes())
			if (node->objectType.ObjectType == type && node->Name() == name)
			{
				deselectAll();
				node->setSelected(true);
				r = true;
				return r;
			}

	return r;
}
/*bool GraphWidget::select(const QStringList &names, const QString type, bool reset) const
{
	bool r = false;
	if (type == "Block")
        for (Node* node in Nodes())
			if (node->Name() == name)
			{
				deselectAll();
				node->setSelected(true);
				r = true;
				return r;
			}

	if (type == "Connector")
        for (Edge* edge in Edges())
			if (edge->Name() == name)
			{
				deselectAll();
				edge->setSelected(true);
				r = true;
				return r;
			}
    for (Entity* entity in Entities)
		if (entity->objectType.ObjectType == type && entity->Name() == name)
		{
			deselectAll();
			tableProp->setModel(entity->model);
			entity->setSelected(true);
			r = true;
			return r;
		}
    for (Entity* entity in Entities)
		if (entity->objectType.ObjectType == name && entity->Name() == name)
		{
			deselectAll();
			tableProp->setModel(entity->model);
			r = true;
			return r;
		}
	if (type == "Well" || type == "Tracer")
        for (Node* node in Nodes())
			if (node->objectType.ObjectType == type && node->Name() == name)
			{
				deselectAll();
				node->setSelected(true);
				r = true;
				return r;
			}

	return r;
}*/
QStringList GraphWidget::EntityNames(const QString &type) const
{
	QStringList r;
    for (Entity *e : Entities)
		if (e->objectType.ObjectType.toLower() == type.toLower()) 
			r.append(e->Name());
	return r;
}
Entity* GraphWidget::entityByName(const QString &name) const
{
    for (Entity *e : Entities)
		if (e->Name().toLower() == name.toLower())
			return e;
	return 0;
}

QList<Entity *> GraphWidget::entitiesByType(const QString &type) const
{
	QList<Entity *> r;
    for (Entity *e : Entities)
		if (e->objectType.ObjectType == type)
			r.append(e);
	return r;
}

QList<Node *> GraphWidget::nodesByType(const QString &type) const
{
	QList<Node *> r;
    for (Node *e : Nodes())
		if (e->objectType.ObjectType == type)
			r.append(e);
	return r;
}

Node* GraphWidget::node(const QString &name) const
{
    for (Node* i : Nodes())
		if (i->Name() == name) 
			return i;
	return nullptr;
}

Edge* GraphWidget::edge(const QString &name) const
{
    for (Edge* i : Edges())
		if (i->Name() == name) return i;
	return nullptr;
}

Entity* GraphWidget::entity(const QString &name, const QString &type) const
{
    for (Entity* i : Entities)
		if (i->Name().toLower() == name.toLower()) 
			if (type == "*" || i->objectType.ObjectType.toLower() == type.toLower())	return i;
	return nullptr;
}

Process* GraphWidget::process(const QString &name) const
{
    for (Process* i : Processes)
		if (i->name == name) return i;
	return nullptr;
}
QList<QMap<QString, QVariant>> GraphWidget::compact() const// QDataStream &out, bool toFile) const
{
	QList<QMap<QString, QVariant>> list;
	QMap<QString, QVariant> r;
	//r["Undo Counter"] = undo_counter;
	r["Model Space"] = ModelSpace.Model;
	r["Inflow Filenames"] = inflowFileNames;
	r["GUI"] = "Graphic Widget";
	r["Experiments"] = experimentsList();
	r["hasResults"] = hasResults;
	

//	//qDebug() << r["GUI"].toString() << ", " << r["Name"].toString() << " saved.";
#ifdef GIFMOD
	if (hasResults){
		getTime();
		for (int i = 0; i < modelSet->Medium.size(); i++)
		{
			QString expName = QString::fromStdString(modelSet->Medium[i].name);
			r[QString("%1 ANS").arg(expName)] = relativePathFilename(QString::fromStdString(modelSet->Medium[i].detoutfilename_hydro), modelPathname());
			//qDebug() << QString("%1 ANS").arg(expName) << " " << getTime();

			r[QString("%1 ANS_colloids").arg(expName)] = relativePathFilename(QString::fromStdString(modelSet->Medium[i].detoutfilename_prtcle), modelPathname());
			//qDebug() << QString("%1 ANS_colloids").arg(expName) << " " << getTime();

			r[QString("%1 ANS_constituents").arg(expName)] = relativePathFilename(QString::fromStdString(modelSet->Medium[i].detoutfilename_wq), modelPathname());
			//qDebug() << QString("%1 ANS_constituents").arg(expName) << " " << getTime();
			if (modelSet->SP.mass_balance_check)
			{
				r[QString("%1 ANS_MB").arg(expName)] = relativePathFilename(QString::fromStdString(modelSet->FI.outputpathname + "output_MB" + modelSet->Medium[i].name + ".txt"), modelPathname());
				//qDebug() << QString("%1 ANS_MB").arg(expName) << " " << getTime();
			}
		}


		r["ANS_obs"] = relativePathFilename(QString::fromStdString(modelSet->FI.detoutfilename_obs), modelPathname());
		//qDebug() << "ANS_obs" << " " << getTime();

//		r["ANS_obs_noise"] = "";
//		out << r2;
//		modelSet->ANS_obs_noise.compact(out);
//		//qDebug() << "ANS_obs_noise" << " " << getTime();
		list.append(r);
	}


	if (hasResults)
	{
		QMap<QString, QVariant> r;
		r["GUI"] = "Block Index";
		for (int i = 0; i < modelSet->Medium[0].Blocks.size(); i++)
			r[QString::fromStdString(modelSet->Medium[0].Blocks[i].ID)] = i;
		//qDebug() << "Block Index" << " " << getTime();

		list.append(r);
		//qDebug() << "append to list" << " " << getTime();
	}
	if (hasResults)
	{
		QMap<QString, QVariant> r;

		r["GUI"] = "Connector Index";
#ifdef GIFMOD
        for (int i = 0; i < modelSet->Medium[0].Connectors.size(); i++)
            r[QString::fromStdString(modelSet->Medium[0].Connectors[i].ID)] = i;
#endif
		//qDebug() << "Connector Index" << " " << getTime();
		list.append(r);
		//qDebug() << "append to list" << " " << getTime();
	}
	



	if (results)
	{
//		QMap<QString, QVariant> r;
//		r["GUI"] = "Inverse Results";
//		r["Inverse Results"] = results->compact();
//		if (toFile)
//			out << r;
//		else
//			list.append(r);
//		//qDebug() << "results + append to list" << " " << getTime();
	}
#endif


    for (Node *n : Nodes())
	{
		list.append(n->compact());
	//	//qDebug() << list.last()["GUI"].toString() << ", " << list.last()["Name"].toString() << " saved.";
	}
	//qDebug() << "nodes append to list" << " " << getTime();


    for (Edge *e : Edges())
	{
		list.append(e->compact());

	//	//qDebug() << list.last()["GUI"].toString() << ", " << list.last()["Name"].toString() << " saved.";
	}
	//qDebug() << "connectors append to list" << " " << getTime();


    for (Entity *e : Entities)
	{
		list.append(e->compact());
		////qDebug() << list.last()["GUI"].toString() << ", " << list.last()["Name"].toString() << " saved.";
	}
	//qDebug() << "entties append to list" << " " << getTime();


    for (Process *p : Processes)
	{
		list.append(p->compact());
		////qDebug() << list.last()["GUI"].toString() << ", " << list.last()["Name"].toString() << " saved.";
	}
	//qDebug() << "processes append to list" << " " << getTime();



	return list;
}

QList<QMap<QString, QVariant>> GraphWidget::compactRXN() const
{
	QList<QMap<QString, QVariant>> list;
    for (Entity *e : Entities)
		if (e->objectType.ObjectType == "Reaction parameter" || e->objectType.ObjectType == "Reaction Network" || e->objectType.ObjectType == "Constituent")
			list.append(e->compact());
    for (Process *p : Processes)
		list.append(p->compact());
	return list;
}

void GraphWidget::clear()
{
//expandNode(treeModel->index(-1, -1), false);
	clearRXN(); 
	expandNode(treeModel->index(-1, -1), false);
    for (Node *n : Nodes())
		treeModel->deleteNode(n);
    for (Edge *e : Edges())
		treeModel->deleteEdge(e);
	QList<Entity *> EntitiesCopy = Entities;
    for (Entity *e : EntitiesCopy)
		treeModel->deleteEntity(e);

	Entities.clear();
//	qDeleteAll(Entities);
//	Entities.clear();
//	qDeleteAll(Processes);
//	Processes.clear();
	treeModel->refresh();
	tableProp->setModel(0);
	if (results)
		delete results;
	if (resultsSet.size())
		resultsSet.clear();
	if (modelSet)
	{
		//qDebug() << "Deleting ModelSet";
		modelSet->clear();
	}
	if (model)
	{
		model->clear(); 
		model = 0;
	}
	//qDebug() << "Model Deleted...";
	model = 0; modelSet = 0; results = 0;
	experimentsComboClear();
	hasResults = false;

}
void GraphWidget::experimentsComboClear(bool addExperiment1)
{
	experiments->clear();
	experiments->addItem("All experiments");
	if (addExperiment1)
		experiments->addItem("experiment1");
}

void GraphWidget::clearRXN()
{
	expandNode(treeModel->index(-1, -1), false);
	QList<Entity*> entitiesCopy = Entities;
    for (Entity *e : entitiesCopy)
		if (e->objectType.ObjectType == "Reaction parameter" || e->objectType.ObjectType == "Reaction Network" || e->objectType.ObjectType == "Constituent")
		{
			treeModel->deleteEntity(e);
			//delete e;
			//Entities.removeOne(e);
		}
	QList<Process*> processesCopy = Processes;
    for (Process *p : processesCopy)
	{
		delete p;
		Processes.removeOne(p);
	}
	

	treeModel->refresh();
	tableProp->setModel(0);
}


GraphWidget* GraphWidget::unCompact(QList<QMap<QString, QVariant>> &list, bool oldVersion) //, QWidget *parent)
{
#ifdef GIFMOD
	modelSet = new CMediumSet;
#endif
#ifdef GWA
	modelSet = new CGWASet;
#endif
	
	for (int i = 0; i<list.size(); i++)
	{
		QString newpath = "";
		if (list[i].value("GUI").toString() == "Graphic Widget")
		{
			//qDebug() << list[i].value("GUI").toString() << " Added.";
			undo_counter = 0;// list[i].value("Undo Counter"].toInt();

			ModelSpace.Model = list[i].value("Model Space").toString();
			inflowFileNames = list[i].value("Inflow Filenames").toStringList();
			hasResults = list[i].value("hasResults").toBool();

			//if (hasResults)			//				{
			experimentsComboClear(false);
			QString path = modelPathname();
            for (QString experiment : list[i].value("Experiments").toStringList())
				if (!experimentsList().contains(experiment))
				{
					experiments->addItem(experiment);
					if (experiment.toLower() != "all experiments")
					{
#ifdef GIFMOD
						CMedium med;
						med.name = experiment.toStdString();

#endif
#ifdef GWA
						CGWA med;
#endif
						
						if (list[i].contains(QString("%1 ANS").arg(experiment)))
						{
							if (newpath == "")
                                med.Results.ANS = CBTCSet(fullFilename(list[i].take(QString("%1 ANS").arg(experiment)).toString(), path).toStdString(), true);
							else
                                med.Results.ANS = CBTCSet(newpath.toStdString() + list[i].take(QString("%1 ANS").arg(experiment)).toString().toStdString(),true);
                            if (!med.Results.ANS.nvars)
							{															
								QMessageBox msgBox;
								msgBox.setText("Output file was not found!");
								msgBox.setInformativeText("Do you want to provide the location for the hydraulics output file?");
								msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
								msgBox.setDefaultButton(QMessageBox::Yes);
								int ret = msgBox.exec();
								if (ret == QMessageBox::Yes)
								{
									QString fileName = QFileDialog::getOpenFileName(this,
										tr("Open ").append(mainWindow->applicationName), modelPathname(),
										tr("*.txt"));

									QFileInfo fileInfo(fileName);
									QString filename_only(fileInfo.fileName());
									newpath = fileInfo.absolutePath() + "/";
                                    med.Results.ANS = CBTCSet((newpath + filename_only).toStdString(), true);
                                    if (!med.Results.ANS.nvars)
										hasResults = false;
								}
								else
									hasResults = false;
							}
						}
#ifdef GIFMOD
						if (hasResults)
						{
							if (list[i].contains(QString("%1 ANS_colloids").arg(experiment)))
							{
								if (newpath == "")
                                    med.Results.ANS_colloids = CBTCSet(fullFilename(list[i].take(QString("%1 ANS_colloids").arg(experiment)).toString(), path).toStdString(), true);
								else
                                    med.Results.ANS_colloids = CBTCSet(newpath.toStdString() + list[i].take(QString("%1 ANS_colloids").arg(experiment)).toString().toStdString(), true);
                                if (med.Results.ANS_colloids.nvars)
									modelSet->SP.colloid_transport = true;
							}
							if (list[i].contains(QString("%1 ANS_constituents").arg(experiment)))
							{
								if (newpath == "")
                                    med.Results.ANS_constituents = CBTCSet(fullFilename(list[i].take(QString("%1 ANS_constituents").arg(experiment)).toString(), path).toStdString(), true);
								else
                                    med.Results.ANS_colloids = CBTCSet(newpath.toStdString() + list[i].take(QString("%1 ANS_constituents").arg(experiment)).toString().toStdString(), true);
                                if (med.Results.ANS_constituents.nvars)
									modelSet->SP.constituent_transport = true;
							}
							if (list[i].contains(QString("%1 ANS_MB").arg(experiment)))
							{
                                med.Results.ANS_MB = CBTCSet(fullFilename(list[i].take(QString("%1 ANS_MB").arg(experiment)).toString(), path).toStdString(), true);
								//if (!med.ANS_MB.nvars)
									//hasResults = false;
							}
						}
						med.parent = modelSet;
#endif
						modelSet->Medium.push_back(med);
					}
				}
			if (list[i].contains("ANS_obs") && hasResults)
#ifdef GIFMOD
			{
				if (newpath == "")
					modelSet->ANS_obs = CBTCSet(fullFilename(list[i].take("ANS_obs").toString(), path).toStdString(), true);
				else
					modelSet->ANS_obs = CBTCSet(newpath.toStdString() + list[i].take("ANS_obs").toString().toStdString(), true);

				//if (!modelSet->ANS_obs.nvars)
					//hasResults = false;
			}
#endif
#ifdef GWA
			modelSet->Medium[0].ANS_obs = CBTCSet(list[i].take("ANS_obs").toString().toStdString(), true);
#endif
			QCoreApplication::processEvents();

		}

		if (list[i].value("GUI").toString() == "Block Index")
		{
			//			r.remove ("GUI");
			//qDebug() << list[i].value("GUI").toString() << " Added.";
            for (QString key : list[i].keys())
				modelSet->blockIndex[key.toStdString()] = list[i].value(key).toInt();
			QCoreApplication::processEvents();
			list[i] = QMap<QString, QVariant>();

		}

		if (list[i].value("GUI").toString() == "Connector Index")
		{
			//			r.remove("GUI");
			//qDebug() << list[i].value("GUI").toString() << " Added.";
            for (QString key : list[i].keys())
				modelSet->connectorIndex[key.toStdString()] = list[i].value(key).toInt();
			QCoreApplication::processEvents();
			list[i] = QMap<QString, QVariant>();

		}

		if (list[i].value("GUI").toString() == "Results")
		{
//			//qDebug() << list[i].value("GUI").toString() << " Added.";
//			results = Results::unCompact(list[i]);
//			QCoreApplication::processEvents();
		}

		if (list[i].value("GUI").toString() == "Block")
		{
			//qDebug() << list[i].value("GUI").toString() << ", " << list[i].value("Name").toString() << " Added.";
			Node::unCompact(list[i], this, oldVersion);
			QCoreApplication::processEvents();
			list[i] = QMap<QString, QVariant>();

		}

		if (list[i].value("GUI").toString() == "Connector")
		{
			//qDebug() << list[i].value("GUI").toString() << ", " << list[i].value("Name").toString() << " Added.";
			Edge::unCompact(list[i], this, oldVersion);
			QCoreApplication::processEvents();
			list[i] = QMap<QString, QVariant>();

		}

		if (list[i].value("GUI").toString() == "Entity")
		{
			//qDebug() << list[i].value("GUI").toString() << ", " << list[i].value("Name").toString() << " Added.";
			Entity::unCompact(list[i], this, oldVersion);
			QCoreApplication::processEvents();
			list[i] = QMap<QString, QVariant>();

		}

		if (list[i].value("GUI").toString() == "Process")
		{
			//qDebug() << list[i].value("GUI").toString() << ", " << list[i].value("Name").toString() << " Added.";
			Process::unCompact(list[i], this);
			QCoreApplication::processEvents();
			list[i] = QMap<QString, QVariant>();

		}
	}

	QStringList missingList;
	missingList << "Markov chain Monte Carlo" << "Project settings";

#ifdef GIFMOD
	missingList << "Solver settings" << "Climate settings";

#endif
    for (QString missing  : missingList)
	{
		Entity *e = entityByName(missing);
		if (e == nullptr)
			new Entity(missing, missing, this);
	}

	treeModel->refresh();
	
	return this;
	
}

/*GraphWidget* GraphWidget::unCompact(QDataStream &in)
{
	QList<QMap<QString, QVariant>> list;
	bool oldVersion = false;
//	in >> list;
//	for (int i = 0; i<list.size(); i++)
	while (!in.atEnd())
	{

		QMap < QString, QVariant > item;// listi;// = list[i];
		in >> item;
		if (item.value("GUI").toString() == "Graphic Widget")
		{
			//qDebug() << item.value("GUI").toString() << " Added.";
			undo_counter = 0;// item.value("Undo Counter"].toInt();
			ModelSpace.Model = item.value("Model Space").toString();
			inflowFileNames = item.value("Inflow Filenames").toStringList();
			hasResults = item.value("hasResults").toBool();
			modelSet = new CMediumSet;
			//if (hasResults)
			//				{
			experimentsComboClear(false);
            for (QString experiment in item.value("Experiments").toStringList())
				if (!experimentsList().contains(experiment))
				{
					experiments->addItem(experiment);
					if (experiment.toLower() != "all experiments")
					{
						CMedium med;
						med.name = experiment.toStdString();
//						med.ANS = CBTCSet::unCompact(item.take(QString("%1 ANS").arg(experiment)).toMap());
//						med.ANS_colloids = CBTCSet::unCompact(item.take(QString("%1 ANS_colloids").arg(experiment)).toMap());
//						med.ANS_constituents = CBTCSet::unCompact(item.take(QString("%1 ANS_constituents").arg(experiment)).toMap());
//						med.ANS_MB = CBTCSet::unCompact(item.take(QString("%1 ANS_MB").arg(experiment)).toMap());
						med.parent = modelSet;
						modelSet->Medium.push_back(med);
					}
				}
//			modelSet->ANS_obs = CBTCSet::unCompact(item.take("ANS_obs").toMap());
//			modelSet->ANS_obs_noise = CBTCSet::unCompact(item.take("ANS_obs_noise").toMap());
			//			}
			//			else
            //				for (QString experiment in item.value("Experiments"].toStringList())
			//					if (!experimentsList().contains(experiment))
			//						experiments->addItem(experiment);
			QCoreApplication::processEvents();
			//			item = QMap<QString, QVariant>();

		}
//		if (item.value("GUI").toString() == "BTC")
		if ((item.value("GUI").toString() == "BTC"))// || item.value("GUI").toString() == "")
			{
			//qDebug() << "trying to add " << item.value("GUI").toString();
			for (int i = 0; i < modelSet->Medium.size(); i++)
				if (modelSet->Medium[i].name != "all experiments")
				{
					QString experiment = QString::fromStdString(modelSet->Medium[i].name);

					if (item.contains(QString("%1 ANS").arg(experiment)))
						modelSet->Medium[i].ANS = CBTCSet::unCompact(in);
					if (item.contains(QString("%1 ANS_colloids").arg(experiment)))
						modelSet->Medium[i].ANS_colloids = CBTCSet::unCompact(in);
					if (item.contains(QString("%1 ANS_constituents").arg(experiment)))
						modelSet->Medium[i].ANS_constituents = CBTCSet::unCompact(in);
					if (item.contains(QString("%1 ANS_MB").arg(experiment)))
						modelSet->Medium[i].ANS_MB = CBTCSet::unCompact(in);
				}

			if (item.contains("ANS_obs"))
				modelSet->ANS_obs = CBTCSet::unCompact(in);
			if (item.contains("ANS_obs_noise"))
				modelSet->ANS_obs_noise = CBTCSet::unCompact(in);
			QCoreApplication::processEvents();
		}
		if (item.value("GUI").toString() == "Block Index")
		{
			//			r.remove ("GUI");
			//qDebug() << item.value("GUI").toString() << " Added.";
            for (QString key in item.keys())
				modelSet->blockIndex[key.toStdString()] = item.value(key).toInt();
			QCoreApplication::processEvents();
			item = QMap<QString, QVariant>();

		}

		if (item.value("GUI").toString() == "Connector Index")
		{
			//			r.remove("GUI");
			//qDebug() << item.value("GUI").toString() << " Added.";
            for (QString key in item.keys())
				modelSet->connectorIndex[key.toStdString()] = item.value(key).toInt();
			QCoreApplication::processEvents();
			item = QMap<QString, QVariant>();

		}

		if (item.value("GUI").toString() == "Results")
		{
			//qDebug() << item.value("GUI").toString() << " Added.";
			results = Results::unCompact(item);
			QCoreApplication::processEvents();
		}

		if (item.value("GUI").toString() == "Block")
		{
			//qDebug() << item.value("GUI").toString() << ", " << item.value("Name").toString() << " Added.";
			Node::unCompact(item, this, oldVersion);
			QCoreApplication::processEvents();
			item = QMap<QString, QVariant>();

		}

		if (item.value("GUI").toString() == "Connector")
		{
			//qDebug() << item.value("GUI").toString() << ", " << item.value("Name").toString() << " Added.";
			Edge::unCompact(item, this, oldVersion);
			QCoreApplication::processEvents();
			item = QMap<QString, QVariant>();

		}

		if (item.value("GUI").toString() == "Entity")
		{
			//qDebug() << item.value("GUI").toString() << ", " << item.value("Name").toString() << " Added.";
			Entity::unCompact(item, this, oldVersion);
			QCoreApplication::processEvents();
			item = QMap<QString, QVariant>();

		}

		if (item.value("GUI").toString() == "Process")
		{
			//qDebug() << item.value("GUI").toString() << ", " << item.value("Name").toString() << " Added.";
			Process::unCompact(item, this);
			QCoreApplication::processEvents();
			item = QMap<QString, QVariant>();
		}
	}

	QStringList missingList;
	missingList << "Markov chain Monte Carlo" << "Solver settings" << "Project settings" << "Climate settings";
    for (QString missing  in missingList)
	{
		Entity *e = entityByName(missing);
		if (e == nullptr)
			new Entity(missing, missing, this);
	}

	treeModel->refresh();

	return this;
}*/
GraphWidget* GraphWidget::unCompact12(QList<QMap<QString, QVariant>> &list, bool oldVersion) //, QWidget *parent)
{
	for (int i = 0; i < list.size(); i++)
	{
		if (list[i].value("GUI").toString() == "Graphic Widget")
		{
			//qDebug() << list[i].value("GUI").toString() << " Added.";
			undo_counter = 0;// list[i].value("Undo Counter"].toInt();
			ModelSpace.Model = list[i].value("Model Space").toString();
			inflowFileNames = list[i].value("Inflow Filenames").toStringList();
			hasResults = list[i].value("hasResults").toBool();
#ifdef GIFMOD
			modelSet = new CMediumSet;
#endif
#ifdef GWA
			modelSet = new CGWASet;
#endif
			//if (hasResults)
			//              {
			experimentsComboClear(false);
            for (QString experiment : list[i].value("Experiments").toStringList())
				if (!experimentsList().contains(experiment))
				{
					experiments->addItem(experiment);
					/*if (experiment.toLower() != "all experiments")
					{
					CMedium med;
					med.name = experiment.toStdString();
					med.ANS = CBTCSet::unCompact(list[i].value(QString("%1 ANS").arg(experiment)).toMap());
					med.ANS_colloids = CBTCSet::unCompact(list[i].value(QString("%1 ANS_colloids").arg(experiment)).toMap());
					med.ANS_constituents = CBTCSet::unCompact(list[i].value(QString("%1 ANS_constituents").arg(experiment)).toMap());
					med.ANS_MB = CBTCSet::unCompact(list[i].value(QString("%1 ANS_MB").arg(experiment)).toMap());
					med.parent = modelSet;
					modelSet->Medium.push_back(med);
					}
					}
					modelSet->ANS_obs = CBTCSet::unCompact(list[i].value("ANS_obs").toMap());
					modelSet->ANS_obs_noise = CBTCSet::unCompact(list[i].value("ANS_obs_noise").toMap());
					//          }
					//          else
                    //              for (QString experiment in list[i].value("Experiments"].toStringList())
					//                  if (!experimentsList().contains(experiment))
					//                      experiments->addItem(experiment);
					QCoreApplication::processEvents();
					list[i] = QMap<QString, QVariant>();
					}
					*/
#ifdef GIFMOD
					if (list[i].value("GUI").toString() == "Block Index")
					{
						//          r.remove ("GUI");
						//qDebug() << list[i].value("GUI").toString() << " Added.";
                        for (QString key : list[i].keys())
							modelSet->blockIndex[key.toStdString()] = list[i].value(key).toInt();
						QCoreApplication::processEvents();
						list[i] = QMap<QString, QVariant>();
					}

					if (list[i].value("GUI").toString() == "Connector Index")
					{
						//          r.remove("GUI");
						//qDebug() << list[i].value("GUI").toString() << " Added.";
                        for (QString key : list[i].keys())
							modelSet->connectorIndex[key.toStdString()] = list[i].value(key).toInt();
						QCoreApplication::processEvents();
						list[i] = QMap<QString, QVariant>();

					}
#endif
					if (list[i].value("GUI").toString() == "Results")
					{
						//qDebug() << list[i].value("GUI").toString() << " Added.";
						results = Results::unCompact(list[i]);
						QCoreApplication::processEvents();
						list[i] = QMap<QString, QVariant>();

					}

					if (list[i].value("GUI").toString() == "Block")
					{
						//qDebug() << list[i].value("GUI").toString() << ", " << list[i].value("Name").toString() << " Added.";
						Node::unCompact(list[i], this, oldVersion);
						QCoreApplication::processEvents();
						list[i] = QMap<QString, QVariant>();

					}

					if (list[i].value("GUI").toString() == "Connector")
					{
						//qDebug() << list[i].value("GUI").toString() << ", " << list[i].value("Name").toString() << " Added.";
						Edge::unCompact(list[i], this, oldVersion);
						QCoreApplication::processEvents();
						list[i] = QMap<QString, QVariant>();

					}

					if (list[i].value("GUI").toString() == "Entity")
					{
						//qDebug() << list[i].value("GUI").toString() << ", " << list[i].value("Name").toString() << " Added.";
						Entity::unCompact(list[i], this, oldVersion);
						QCoreApplication::processEvents();
						list[i] = QMap<QString, QVariant>();

					}

					if (list[i].value("GUI").toString() == "Process")
					{
						//qDebug() << list[i].value("GUI").toString() << ", " << list[i].value("Name").toString() << " Added.";
						Process::unCompact(list[i], this);
						QCoreApplication::processEvents();
						list[i] = QMap<QString, QVariant>();

					}
				}

			QStringList missingList;
			missingList << "Markov chain Monte Carlo" << "Solver settings" << "Project settings" << "Climate settings";
            for (QString missing  : missingList)
			{
				Entity *e = entityByName(missing);
				if (e == nullptr)
					new Entity(missing, missing, this);
			}

			treeModel->refresh();

			return this;
		}
	}
}


GraphWidget* GraphWidget::unCompact10(QList<QMap<QString, QVariant>> list) //, QWidget *parent)
{
    for (QMap<QString, QVariant> r : list)
	{
		if (r["GUI"].toString() == "Graphic Widget")
		{
			//qDebug() << r["GUI"].toString() << " Added.";
			undo_counter = 0;// r["Undo Counter"].toInt();
			ModelSpace.Model = r["Model Space"].toString();
			inflowFileNames = r["Inflow Filenames"].toStringList();
			QCoreApplication::processEvents();
		}
	}
    for (QMap<QString, QVariant> r : list)
	{
		if (r["GUI"].toString() == "Block")
		{
			//qDebug() << r["GUI"].toString() << ", " << r["Name"].toString() << " Added.";
			Node::unCompact10(r, this);
			QCoreApplication::processEvents();
		}
	}
    for (QMap<QString, QVariant> r : list)
	{
		if (r["GUI"].toString() == "Connector")
		{
			//qDebug() << r["GUI"].toString() << ", " << r["Name"].toString() << " Added.";
			Edge::unCompact10(r, this);
			QCoreApplication::processEvents();
		}
	}
    for (QMap<QString, QVariant> r : list)
	{
		if (r["GUI"].toString() == "Entity")
		{
			//qDebug() << r["GUI"].toString() << ", " << r["Name"].toString() << " Added.";
			Entity::unCompact10(r, this);
			QCoreApplication::processEvents();
		}
	}
    for (QMap<QString, QVariant> r : list)
	{
		if (r["GUI"].toString() == "Process")
		{
			//qDebug() << r["GUI"].toString() << ", " << r["Name"].toString() << " Added.";
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
    for (Node *n : Nodes())
		r.append(n->Name());
	return r;
}

QStringList GraphWidget::nodeNames(const QString &type) const
{
	QStringList r;
    for (Node *n : Nodes())
		if (n->objectType.ObjectType.toLower() == type.toLower()) r.append(n->Name());
	return r;
}

QStringList GraphWidget::edgeNames() const
{
	QStringList r;
    for (Edge *e : Edges())
		r.append(e->Name());
	return r;
}

void GraphWidget::deleteNode(Node *node)
{
	
	if (static_cast<PropModel<Node>*>(tableProp->model()) == node->model) tableProp->setModel(0);
    for (Edge * e : node->edgeList)
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

void GraphWidget::nodeContextMenuRequested(Node* n, QPointF pos, QMenu *menu)
{
	bool called_by_clicking_on_graphical_object = false;
	if (!menu) {
		menu = new QMenu;
		menu->addAction("Delete");
		called_by_clicking_on_graphical_object = true; 
	}
	menu->addAction("Select");
#ifdef GIFMOD
	menu->addAction("Make array of blocks");
#endif
	QMap < QAction *, QStringList> menuKey;
	if (n->getValue("Inflow time series") != ""&& n->getValue("Inflow time series") != ".")
	{
		QMap<QString, QString> inflowGraphNames;
		QStringList inflowFileNamesofNode = n->getValue("Inflow time series").toQString().split(';');
        for(QString file : inflowFileNamesofNode)
		{
			CBTCSet inflows = CBTCSet(file.replace("./", modelPathname().append('/')).toStdString(), 1);
            for (string name : inflows.names)
			{
				if (!inflowGraphNames.keys().contains(QString::fromStdString(name)))
					inflowGraphNames[QString::fromStdString(name)] = file;
				else warning(QString("Duplicate Headers in Inflow time series, Block %1, Header %2, inFlow file(s) %3").arg(n->Name()).arg(QString::fromStdString(name)).arg(n->getValue("Inflow time series")));
			}
		}
		if (inflowGraphNames.count())
		{
			menu->addSeparator();
			QMenu *inflowSubMenu = menu->addMenu("Plot Inflow Properties");
            for (QString subMenuTitle : inflowGraphNames.keys())
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
			menu->addSeparator();
			//QMenu *atmosphericSubMenu = menu.addMenu(
			menu->addAction("Plot Atmospheric Concentration Record");
		}
	}
	if (modelSet != NULL)
		model = (experimentID() == 0 || modelSet->Medium.size() == 0) ? 0 : &(modelSet->Medium[experimentID() - 1]);
	else
		model = 0; 


	if (model == 0)
		if (modelSet)
			if (hasResults)
			{
				menu->addSeparator();
				menu->addAction("No expeiments selected to show the results.")->setEnabled(false);
			}
	if (model != 0)
	{
#ifdef GIFMOD
//		if (model->ANS.BTC[model->getblocksq(n->Name().toStdString())].n > 0)
		if (hasResults)
			{
			menu->addSeparator();
			QMenu *plotSubMenu = menu->addMenu("Plot Hydraulic Results");
			plotSubMenu->addAction("Plot Storage");
			plotSubMenu->addAction("Plot Head");
			QStringList Porous;
			Porous << "Soil" << "Darcy" << "Storage" << "Plant" ;// << "" << "";
			if (Porous.contains(n->objectType.ObjectType))
				plotSubMenu->addAction("Moisture Content");
			else
				plotSubMenu->addAction("Water Depth");
			plotSubMenu->addAction("Evapotranspiration Rate");
			if (n->objectType.ObjectType == "Plant")
			{
				plotSubMenu->addAction("Leaf Area Index");
				plotSubMenu->addAction("Bio-volume");
			}

		}
		
		//qDebug() << "Model->constituent_transport: " << model->constituent_transport() << entitiesByType("Constituent");
		if ((model->colloid_transport() && entitiesByType("Particle").count()) ||
			(model->constituent_transport() && entitiesByType("Constituent").count()))
		{
			QMenu *waterQualitySubMenu = menu->addMenu("Plot Constituent Concentrations");
			
			if (model->colloid_transport() && entitiesByType("Particle").count())
			{
                for (Entity *p : entitiesByType("Particle"))
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
			
			QMenu *massesSubMenu = menu->addMenu("Plot Constituent Masses");
			
			if (model->constituent_transport() && entitiesByType("Constituent").count())
			{
				if (model->colloid_transport() && entitiesByType("Particle").count())
					menu->addSeparator();
                for (Entity *e : entitiesByType("Constituent"))
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
                for (Entity *e : entitiesByType("Constituent"))
				{
					constituentSorbedSubMenu = sorbedSubMenu->addMenu(e->Name());
					QStringList list;
					list.append("Constituent");
					int BTCid = model->get_member_no(model->getblocksq(n->Name().toStdString()), -1, 0, model->RXN().look_up_constituent_no(e->Name().toStdString()));
					list.append(QString::number(BTCid));
					menuKey[constituentSorbedSubMenu->addAction("Soil")] = list;
                    for (Entity *p : entitiesByType("Particle"))
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

				// Mass menues:

                for (Entity *e : entitiesByType("Constituent"))
				{
					//QMenu *constituentSubMenu = waterQualitySubMenu->addMenu(e->Name());
					QStringList list;
					list.append("Constituent Mass");
					int BTCid = model->get_member_no(model->getblocksq(n->Name().toStdString()), -2, 0, model->RXN().look_up_constituent_no(e->Name().toStdString()));
					list.append(QString::number(BTCid));
					BTCid = model->getblocksq(n->Name().toStdString());
					list.append(QString::number(BTCid));
					menuKey[massesSubMenu->addAction(e->Name())] = list;
				}
				QMenu *sorbedmassSubMenu = massesSubMenu->addMenu("Sorbed/Particle associated");
				QMenu *constituentSorbedmassSubMenu;
                for (Entity *e : entitiesByType("Constituent"))
				{
					constituentSorbedmassSubMenu = sorbedmassSubMenu->addMenu(e->Name());
					QStringList list;
					list.append("Constituent Mass");
					int BTCid = model->get_member_no(model->getblocksq(n->Name().toStdString()), -1, 0, model->RXN().look_up_constituent_no(e->Name().toStdString()));
					list.append(QString::number(BTCid));
					BTCid = model->getblocksq(n->Name().toStdString());
					list.append(QString::number(BTCid));
					menuKey[constituentSorbedSubMenu->addAction("Soil")] = list;
                    for (Entity *p : entitiesByType("Particle"))
					{
						QMenu *particlemassSubMenu = constituentSorbedmassSubMenu->addMenu(p->Name());
						if (p->getValue("Model").contains("Single"))
						{
							QStringList list;
							list.append("Constituent Mass");
							int BTCid = model->get_member_no(model->getblocksq(n->Name().toStdString()),
								model->lookup_particle_type(p->Name().toStdString()), 0, model->RXN().look_up_constituent_no(e->Name().toStdString()));
							list.append(QString::number(BTCid));
							BTCid = model->getblocksq(n->Name().toStdString());
							list.append(QString::number(BTCid));
							menuKey[particlemassSubMenu->addAction("Mobile")] = list;
						}
						if (p->getValue("Model").contains("Dual"))
						{
							QStringList list;
							list.append("Constituent Mass");
							int BTCid = model->get_member_no(model->getblocksq(n->Name().toStdString()),
								model->lookup_particle_type(p->Name().toStdString()), 0, model->RXN().look_up_constituent_no(e->Name().toStdString()));
							list.append(QString::number(BTCid));
							menuKey[particlemassSubMenu->addAction("Mobile")] = list;
							list.clear();
							list.append("Constituent Mass");
							BTCid = model->get_member_no(model->getblocksq(n->Name().toStdString()),
								model->lookup_particle_type(p->Name().toStdString()), 1, model->RXN().look_up_constituent_no(e->Name().toStdString()));
							list.append(QString::number(BTCid));
							BTCid = model->getblocksq(n->Name().toStdString());
							list.append(QString::number(BTCid));
							menuKey[particlemassSubMenu->addAction("Attached")] = list;
						}
						if (p->getValue("Model").contains("Triple"))
						{
							QStringList list;
							list.append("Constituent Mass");
							int BTCid = model->get_member_no(model->getblocksq(n->Name().toStdString()),
								model->lookup_particle_type(p->Name().toStdString()), 0, model->RXN().look_up_constituent_no(e->Name().toStdString()));
							list.append(QString::number(BTCid));
							menuKey[particlemassSubMenu->addAction("Mobile")] = list;
							list.clear();
							list.append("Constituent Mass");
							BTCid = model->get_member_no(model->getblocksq(n->Name().toStdString()),
								model->lookup_particle_type(p->Name().toStdString()), 1, model->RXN().look_up_constituent_no(e->Name().toStdString()));
							list.append(QString::number(BTCid));
							BTCid = model->getblocksq(n->Name().toStdString());
							list.append(QString::number(BTCid));
							menuKey[particlemassSubMenu->addAction("Reversible attached")] = list;
							list.clear();
							list.append("Constituent Mass");
							BTCid = model->get_member_no(model->getblocksq(n->Name().toStdString()),
								model->lookup_particle_type(p->Name().toStdString()), 2, model->RXN().look_up_constituent_no(e->Name().toStdString()));
							list.append(QString::number(BTCid));
							BTCid = model->getblocksq(n->Name().toStdString());
							list.append(QString::number(BTCid));
							menuKey[particlemassSubMenu->addAction("Irreversible attached")] = list;
						}
					}
				}

			}
		}
#endif
#ifdef GWA
		menu->addSeparator();
		if (n->objectType.ObjectType == "Well")
		{
			menu->addAction("Plot Young Age Distribution");
			menu->addAction("Plot Cumulative Young Age Distribution");
			if (results->youngAgeDistributionRealizations.size())
				menu->addAction("Plot Young Age Distribution (Realizations)");
			if (results->cumulativeYoungAgeDistributionRealizations.size())
				menu->addAction("Plot Cumulative Young Age Distribution (Realizations)");
			if (results->youngAgeDistributionRealizationPercentiles.size())
				menu->addAction("Plot Young Age Distribution (Percentiles)");
			if (results->cumulativeYoungAgeDistributionRealizationPercentiles.size())
				menu->addAction("Plot Cumulative Young Age Distribution (Percentiles)");

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
					menu->addSeparator();
					QMenu *plotSubMenu = menu->addMenu("Plot Projected Tracer concentrations");
                    for(QString tracerName : tracersList)
					{
						QAction* ac = plotSubMenu->addAction(tracerName);
						QStringList list;
						list.append("Projected");
						list.append(tracerName);
						menuKey.insert(ac, list);
					}

					if (results->projectedTracerConcentrations.size())
					{
						QMenu *plotSubMenu = menu->addMenu("Plot Projected Tracer concentrations (Realizations)");
                        for(QString tracerName : tracersList)
						{
							QAction* ac = plotSubMenu->addAction(tracerName);
							QStringList list;
							list.append("Realization");
							list.append(tracerName);
							menuKey.insert(ac, list);
						}
						QMenu *plotSubMenu2 = menu->addMenu("Plot Projected Tracer concentrations (Percentiles)");
                        for(QString tracerName : tracersList)
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
	QAction *selectedAction;
	if (called_by_clicking_on_graphical_object)
		selectedAction = menu->exec(mapToGlobal(mapFromScene(pos.toPoint())));
	else
		selectedAction = menu->exec(pos.toPoint());
	
	if (selectedAction != nullptr)
	{
		if (selectedAction->text() == "Select")
			n->setSelected(true);
		if (selectedAction->text() == "Delete")
			treeModel->deleteNode(n);
#ifdef GIFMOD
		if (selectedAction->text() == "Make array of blocks")
		{
			QString z0variableName = n->variableName("z0");
			XString z0 = n->getValue(n->variableName("z0")).list();
			QString unit = z0.unit;
			XString height = n->getValue(n->variableName("depth"));
			XString width = n->getValue(n->variableName("width"));
			XString length = n->getValue(n->variableName("d"));

			QString gridType = "2D V";
			bool canChangeType = true;
			if (length.isEmpty())
			{
				length = z0;
				length.setNum(0);
			}
			if (width.isEmpty())
			{
				width = z0;
				width.setNum(0);
			}

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
					////qDebug() << QString("Creating %1x%2 grid, delta H=%3, delta V=%4.").arg(numberofRows).arg(numberofColumns).arg(deltaH).arg(deltaV);
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
                                QList<Node*> nodenames = Nodes();
                                n1->setName(n->newNodeName(n->Name(), nodenames));
								n1->edgeList.clear();

								if (columnIndex == 0)
									n1->setX(n->x());
								else
									n1->setX(row[columnIndex - 1]->x() + n->Width() + 50);

								int offsetY = 0;
								if (deltaV.toFloat() < 0)
									offsetY = n->Height() + 50;
								else if(deltaV.toFloat() > 0)
									offsetY = -n->Height() - 50;

								if (rowIndex == 0)
									n1->setY(n->y());
								else
									n1->setY(rows[rowIndex - 1][columnIndex]->y() + offsetY);

								////qDebug() << QString("[%1,%2], x=%3, y=%4").arg(rowIndex).arg(columnIndex).arg(n1->x()).arg(n1->y());
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
									XString connectorLength = z0;
									connectorLength.setNum(abs(deltaV.toFloat(z0.unit)) / (numberofRows - 1));
                                    //bool copyLength = true;
                                    if (connectorLength > 0){
                                        e->setProp(e->variableName("d"), connectorLength.list(), XStringEditRole);
                                        //copyLength=false;
                                    }
									bool copyLength = false;
									e->copyProps(n, "Vertical Array", "Vertical Connector", copyLength);
								}
								if (columnIndex != 0)
								{
									Edge *e = new Edge(row[columnIndex - 1], n1, this);
									treeModel->add(e); 
                                    bool copyLength=true;
                                    if (length > 0){
                                        e->setProp(e->variableName("d"), length.list(), XStringEditRole);
                                        copyLength=false;
                                    }
									e->copyProps(n, "Vertical Array", "Horizontal Connector", copyLength);
								}

							}
						}
						rows.append(row);
					}
				}
			}
			else if (grid.keys().contains("Radially symmetrical array"))
			{
				XString deltaV = grid["delta V"];
				XString deltaH = grid["delta H"];
				XString length = grid["length"];
				XString r0 = grid["r0"];

				if (length.toDouble() <= 0)
				{
					QMessageBox::information(mainWindow, "Radially symmetrical array", "Length should be greater than zero");
					return;
				}

				XString widthAvg = n->getValue(n->variableName("width"));
				XString bottomArea = n->getValue(n->variableName("a"));

				if (widthAvg.toQString() == "" && bottomArea.toQString() == "")
				{
					QMessageBox::information(mainWindow, "Radially symmetrical array",
						"At least one of 'Bottom area' or 'Length' should be defined.");
					return;
				}

				XString w1 = width, w2 = width;
				double r0d = r0.toDouble("m");
				double r1 = r0d;
				double r2 = r1 + length.toDouble("m");
				double theta = width.toDouble("m") / ((r1 + r2) / 2.0);

				if (width.toQString() == "" || width.toDouble()==0)
				{
					theta = 2 * bottomArea.toDouble("m~^2") / (r2*r2 - r1*r1);

					width.setNum(((r1 + r2) / 2.0)*theta);
					width.setUnit("m");

					XString newWidth = width.convertTo(n->getValue(n->variableName("width")).unit);
					n->setProp(n->variableName("width"), newWidth.list(), XStringEditRole);
					width = newWidth;
				}

				w1.setNum(r1*theta);
				w1.setUnit("m");
				w2.setNum(r2*theta);
				w2.setUnit("m");

				//double calculatedWidth = length.toDouble("m") * theta;
/*				if (!isFuzzyEqual(width.toDouble("m"), calculatedWidth)
				{
					QMessageBox::information(mainWindow, "Radially symmetrical array", 
						"Block width is not consistent with the Length should be greater than zero");
					return;
				}
*/
				double calculatedArea = theta / 2.0 *((r2*r2) - (r1*r1));

				if (bottomArea.toQString() == "" || bottomArea.toDouble() ==0)
				{
					bottomArea.setNum(calculatedArea);
					bottomArea.setUnit("m~^2");
					XString area = bottomArea.convertTo(n->getValue(n->variableName("a")).unit);
					n->setProp(n->variableName("a"), area.list(), XStringEditRole);
				}

				double area = n->getValue(n->variableName("a")).toDouble("m~^2");
				if (!isFuzzyEqual(area, calculatedArea))
				{
					QMessageBox::information(mainWindow, "Radially symmetrical array",
						"Block's 'Bottom area', 'Width', and 'length' are not consistent.\nArea = width * length / 2");
					return;
				}

				if (numberofRows >= 1 && numberofColumns >= 1)// && isnumber(deltaV) && isnumber (deltaH))
				{
					////qDebug() << QString("Creating %1x%2 grid, delta H=%3, delta V=%4.").arg(numberofRows).arg(numberofColumns).arg(deltaH).arg(deltaV);
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
                                QList<Node*> nodenames = Nodes();
                                n1->setName(n->newNodeName(n->Name(), nodenames));
								n1->edgeList.clear();

								if (columnIndex == 0)
									n1->setX(n->x());
								else
									n1->setX(row[columnIndex - 1]->x() + n->Width() + 50);

								int offsetY = 0;
								if (deltaV.toFloat() < 0)
									offsetY = n->Height() + 50;
								else if (deltaV.toFloat() > 0)
									offsetY = -n->Height() - 50;

								if (rowIndex == 0)
									n1->setY(n->y());
								else
									n1->setY(rows[rowIndex - 1][columnIndex]->y() + offsetY);

								////qDebug() << QString("[%1,%2], x=%3, y=%4").arg(rowIndex).arg(columnIndex).arg(n1->x()).arg(n1->y());
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
									XString connectorLength = z0;
									connectorLength.setNum(abs(deltaV.toFloat(z0.unit)) / (numberofRows - 1));
									//bool copyLength = true;
									if (connectorLength > 0) {
										e->setProp(e->variableName("d"), connectorLength.list(), XStringEditRole);
										//copyLength=false;
									}
									bool copyLength = false;
									e->copyProps(n1, "Vertical Array", "Vertical Connector", copyLength);
								}
								if (columnIndex != 0)
								{
									double col = columnIndex + 1;
									XString connectorWidth = n->getValue(n->variableName("width"));
									connectorWidth.setNum((r0d + (col-1)*length.toDouble("m"))*theta);
									connectorWidth.setUnit("m");
									connectorWidth.convertTo(n->getValue(n->variableName("width")).unit);

									XString width = n->getValue(n->variableName("width"));
									width.setNum((r0d + (col-1.5)*length.toDouble("m"))*theta);
									width.setUnit("m");
									width.convertTo(n->getValue(n->variableName("width")).unit);
									n1->setProp(n1->variableName("width"), width.list(), XStringEditRole);

									XString area = n->getValue(n->variableName("a"));
									r1 = r0d + (col - 1)*length.toDouble("m");
									r2 = r0d + col*length.toDouble("m");
									area.setNum(theta / 2.0*((r2*r2) - (r1*r1)));
									area.setUnit("m~^2");
									area.convertTo(n->getValue(n->variableName("a")).unit);
									n1->setProp(n->variableName("a"), area.list(), XStringEditRole);
								
									Edge *e = new Edge(row[columnIndex - 1], n1, this);
									treeModel->add(e);
									bool copyLength = true;
									if (length > 0) {
										e->setProp(e->variableName("d"), length.list(), XStringEditRole);
										copyLength = false;
									}
									e->copyProps(n1, "Vertical Array", "Horizontal Connector", copyLength);
									e->setProp(e->variableName("width"), connectorWidth.list(), XStringEditRole);
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
					////qDebug() << QString("Creating %1x%2 grid, delta H=%3, delta V=%4.").arg(numberofRows).arg(numberofColumns).arg(deltaH).arg(deltaV);
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
                                QList<Node*> nodenames = Nodes();
                                n1->setName(n->newNodeName(n->Name(), nodenames));
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

								////qDebug() << QString("[%1,%2], x=%3, y=%4").arg(rowIndex).arg(columnIndex).arg(n1->x()).arg(n1->y());
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
									e->copyProps(n, "Horizontal Array", "Vertical Connector");
									if (lengthY > 0) e->setProp(e->variableName("d"), lengthY.list(), XStringEditRole);
								}
								if (columnIndex != 0)
								{
									Edge *e = new Edge(row[columnIndex - 1], n1, this);
									treeModel->add(e); 
									e->copyProps(n, "Horizontal Array", "Horizontal Connector");
									if (lengthX > 0) e->setProp(e->variableName("d"), lengthX.list(), XStringEditRole);
								}

							}
						}
						rows.append(row);
					}
				}
			}
			
		}
		plotformat format;
		format.xAxisTimeFormat = true;

		if (selectedAction->text() == "Plot Storage")
		{
			format.yAxisLabel.append("Storage (m^3)");
			format.xAxisLabel.append("Time (day)");
			plotWindow *plot = new plotWindow(this, QString("%1: %2").arg(experimentName()).arg(selectedAction->text().remove("Plot ")));
            plot->addScatterPlot(model->Results.ANS, model->getblocksq(n->Name().toStdString()), QString("%1: %2").arg(n->Name()).arg("Storage"), 1, 0, format);
			plot->show();
		}
		if (selectedAction->text() == "Plot Head")
		{
			plotWindow *plot = new plotWindow(this, QString("%1: %2").arg(experimentName()).arg(selectedAction->text().remove("Plot ")));
			format.yAxisLabel.append("Head (m)");
			format.xAxisLabel.append("Time (day)");
            plot->addScatterPlot(model->Results.ANS, Edges().count() + Nodes().count() + model->getblocksq(n->Name().toStdString()), QString("%1: %2").arg(n->Name()).arg("Head"), 1, 0, format);
			plot->show();
		}
		if (selectedAction->text() == "Moisture Content")
		{
			plotWindow *plot = new plotWindow(this, QString("%1: %2").arg(experimentName()).arg(selectedAction->text().remove("Plot ")));
			double volume = n->val("a").convertToDefaultUnit().toDouble() * n->val("depth").convertToDefaultUnit().toDouble(); //model->Blocks[model->getblocksq(n->Name().toStdString())].V; //ask Arash
			format.yAxisLabel.append("Moisture Content");
			format.xAxisLabel.append("Time (day)");
			if (n->objectType.ObjectType=="Plant")
                plot->addScatterPlot(model->Results.ANS, model->getblocksq(n->Name().toStdString()), model->getblocksq(n->Name().toStdString()) + 3 * Edges().count() + 3 * Nodes().count() ,  QString("%1: %2").arg(n->Name()).arg("Moisture Content"), format);
			else
                plot->addScatterPlot(model->Results.ANS, model->getblocksq(n->Name().toStdString()), QString("%1: %2").arg(n->Name()).arg("Moisture Content"), 1.0 / volume, 0, format);
			plot->show();
		}
		if (selectedAction->text() == "Water Depth")
		{
			plotWindow *plot = new plotWindow(this, QString("%1: %2").arg(experimentName()).arg(selectedAction->text().remove("Plot ")));
			format.yAxisLabel.append("Depth (m)");
			format.xAxisLabel.append("Time (day)");
			double z0 = n->val("z0").convertToDefaultUnit().toDouble();// model->Blocks[model->getblocksq(n->Name().toStdString())].z0;
            plot->addScatterPlot(model->Results.ANS, Edges().count() + Nodes().count() + model->getblocksq(n->Name().toStdString()), QString("%1: %2").arg(n->Name()).arg("Water Depth"), 1, -z0, format);
			plot->show();
		}
		if (selectedAction->text() == "Evapotranspiration Rate")
		{
			plotWindow *plot = new plotWindow(this, QString("%1: %2").arg(experimentName()).arg(selectedAction->text().remove("Plot ")));
			format.yAxisLabel.append("Evaporation rate (m^3/day)");
			format.xAxisLabel.append("Time (day)");
			double z0 = n->val("z0").convertToDefaultUnit().toDouble();// model->Blocks[model->getblocksq(n->Name().toStdString())].z0;
            plot->addScatterPlot(model->Results.ANS, Edges().count() + 2 * Nodes().count() + model->getblocksq(n->Name().toStdString()), QString("%1: %2").arg(n->Name()).arg("Evapotranspiration Rate"), 1, 0, format);
			plot->show();
		}
		if (selectedAction->text() == "Leaf Area Index")
		{
			plotWindow *plot = new plotWindow(this, QString("%1: %2").arg(experimentName()).arg(selectedAction->text().remove("Plot ")));
			format.yAxisLabel.append("Leaf Area Index");
			format.xAxisLabel.append("Time (day)");
			double z0 = n->val("z0").convertToDefaultUnit().toDouble();// model->Blocks[model->getblocksq(n->Name().toStdString())].z0;
            plot->addScatterPlot(model->Results.ANS, 3* Edges().count() + 4 * Nodes().count() + model->getblocksq(n->Name().toStdString()), QString("%1: %2").arg(n->Name()).arg("Leaf Area Index"), 1, 0, format);
			plot->show();
		}

		if (selectedAction->text() == "Bio-volume")
		{
			plotWindow *plot = new plotWindow(this, QString("%1: %2").arg(experimentName()).arg(selectedAction->text().remove("Plot ")));
			format.yAxisLabel.append("Bio-volume (m^3)");
			format.xAxisLabel.append("Time (day)");
			double z0 = n->val("z0").convertToDefaultUnit().toDouble();// model->Blocks[model->getblocksq(n->Name().toStdString())].z0;
            plot->addScatterPlot(model->Results.ANS, 3 * Edges().count() + 3 * Nodes().count() + model->getblocksq(n->Name().toStdString()), QString("%1: %2").arg(n->Name()).arg("Bio-volume"), 1, 0, format);
			plot->show();
		}

#endif
		if (selectedAction->text() == "Plot Atmospheric Concentration Record")
		{
			plotWindow *plot = new plotWindow(this, QString("%1: %2").arg(experimentName()).arg(selectedAction->text().remove("Plot ")));
			format.xAxisLabel.append("Time (day)");
			QString file = n->getValue("Atmospheric Record").toQString();
			CBTC record = CBTC(file.replace("./", modelPathname().append('/')).toStdString());
			plot->addScatterPlot(record, n->Name(), plotformat());
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
#ifdef GIFMOD
			if (menuKey[selectedAction][0] == "Constituent")
			{
				plotWindow *plot = new plotWindow(this, QString("%1: %2").arg(experimentName()).arg(selectedAction->text().remove("Plot ")));
                plot->addScatterPlot(model->Results.ANS_constituents, menuKey[selectedAction][1].toInt(), "", 1, 0, format);
				plot->show();
			}

			if (menuKey[selectedAction][0] == "Constituent Mass")
			{
				plotWindow *plot = new plotWindow(this, QString("%1: %2").arg(experimentName()).arg(selectedAction->text().remove("Plot ")));
                plot->addScatterPlot(model->Results.ANS_constituents, menuKey[selectedAction][1].toInt(), model->Results.ANS, menuKey[selectedAction][2].toInt(), "", 1, 0, format);
				plot->show();
			}

			if (menuKey[selectedAction][0] == "Particle")
			{
				plotWindow *plot = new plotWindow(this, QString("%1: %2").arg(experimentName()).arg(selectedAction->text().remove("Plot ")));
                plot->addScatterPlot(model->Results.ANS_colloids, menuKey[selectedAction][1].toInt(), "", 1, 0, format);
				plot->show();
			}
			if (menuKey[selectedAction][0] == "Inflow")
			{
				CBTCSet inflow = CBTCSet(menuKey[selectedAction][1].toStdString(), 1);
				for (int i = 0; i < inflow.nvars; i++)
					if (selectedAction->text().toStdString() == inflow.names[i])
					{
						plotWindow *plot = new plotWindow(this, QString("%1: %2").arg(experimentName()).arg(selectedAction->text().remove("Plot ")));
						if (tolower(inflow.names[i])=="flow")
							format.yAxisLabel.append("Flow (m^3/day)");
						else
							format.yAxisLabel.append("Concentration");
						plot->addScatterDotPlot(inflow, i, "", format);
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
						plotformat format;
						format.xAxisTimeFormat = false;
						plot->addScatterDotPlot(model->projected, i, "", format);
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

void GraphWidget::edgeContextMenuRequested(Edge* e, QPointF pos, QMenu *menu)
{
	QAction *deleteAction;
	bool called_by_clicking_on_graphical_object = false;
	if (!menu) {
		menu = new QMenu();
		deleteAction = menu->addAction("Delete");
		called_by_clicking_on_graphical_object = true; 
	}
	QAction *markAction = menu->addAction("Select");
#ifdef GIFMOD
	model = (experimentID() == 0) ? 0 : &(modelSet->Medium[experimentID() - 1]);

	if (model == 0)
		if (modelSet)
			if (hasResults)
			{
				menu->addSeparator();
				menu->addAction("No expeiments selected to show the results.")->setEnabled(false);
			}
	if (model != 0)
	{
		menu->addSeparator();
		menu->addAction("Plot Flow");
		menu->addAction("Velocity");
		menu->addAction("Area");
		menu->addAction("Vapor exchange rate"); //NEW


	}

	QAction *selectedAction;
	if (called_by_clicking_on_graphical_object)
		selectedAction = menu->exec(mapToGlobal(mapFromScene(pos.toPoint())));
	else
		selectedAction = menu->exec(pos.toPoint());
	if (selectedAction != nullptr)
	{
		if (selectedAction->text() == "Select")
			e->setSelected(true);
		if (selectedAction->text() == "Delete")
			treeModel->deleteEdge(e);
		if (selectedAction->text() == "Plot Flow")
		{
			plotformat format;
			format.xAxisTimeFormat = true;
			plotWindow *plot = new plotWindow(this, QString("%1: %2").arg(experimentName()).arg(selectedAction->text().remove("Plot ")));
			format.yAxisLabel.append(XString::reform(QString("Flow (m~^3/day)")));
			format.xAxisLabel.append("Time (day)");
            plot->addScatterPlot(model->Results.ANS, Nodes().count() + model->getconnectorsq(e->Name().toStdString()), QString("%1: %2").arg(e->Name()).arg("Flow"),1,0,format);
			plot->show();
		}
		if (selectedAction->text() == "Velocity")
		{
			plotformat format;
			format.xAxisTimeFormat = true;
			plotWindow *plot = new plotWindow(this, QString("%1: %2").arg(experimentName()).arg(selectedAction->text().remove("Plot ")));
			CBTC flow, area, velocity;
			format.yAxisLabel.append(XString::reform(QString("Velocity (m/day)")));
			format.xAxisLabel.append("Time (day)");
            flow = model->Results.ANS.BTC[Nodes().count() + model->getconnectorsq(e->Name().toStdString())];
            area = model->Results.ANS.BTC[Nodes().count() * 3 + Edges().count() + model->getconnectorsq(e->Name().toStdString())];
			velocity = flow % area;

			plot->addScatterPlot(velocity, QString("%1: %2").arg(e->Name()).arg("Velocity"), 1, 0, false, format);
			plot->show();
		}
		if (selectedAction->text() == "Area")
		{
			plotformat format;
			format.xAxisTimeFormat = true;
			plotWindow *plot = new plotWindow(this, QString("%1: %2").arg(experimentName()).arg(selectedAction->text().remove("Plot ")));
			format.yAxisLabel.append(XString::reform(QString("Flow (m~^2")));
			format.xAxisLabel.append("Time (day)");
            plot->addScatterPlot(model->Results.ANS, Nodes().count() * 3 + Edges().size() + model->getconnectorsq(e->Name().toStdString()), QString("%1: %2").arg(e->Name()).arg("Area"));
			plot->show();
		}
		if (selectedAction->text() == "Vapor exchange rate")
		{
			plotformat format;
			format.xAxisTimeFormat = true;
			plotWindow *plot = new plotWindow(this, QString("%1: %2").arg(experimentName()).arg(selectedAction->text().remove("Plot ")));
			format.yAxisLabel.append(XString::reform(QString("Flow (m~^3/day)")));
			format.xAxisLabel.append("Time (day)");
            plot->addScatterPlot(model->Results.ANS, Nodes().count() * 3 + 2 * Edges().size() + model->getconnectorsq(e->Name().toStdString()), QString("%1: %2").arg(e->Name()).arg("Vapor exchange rate"));
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
    for (QString inflow : oldInflowFileNames)
	{
		inflowFileNames.append(updateRelativePath(inflow, oldPath, newPath));
	}
    for (Node *n : Nodes())
	{
		QStringList oldInflowFiles = n->getValue("Inflow time series").toQString().split(';');
		QString inflowFiles;
        for (QString file : oldInflowFiles)
			inflowFiles.append(updateRelativePath(file, oldPath, newPath).append(';'));
		inflowFiles = inflowFiles.left(inflowFiles.count() - 1);
		n->setValue("Inflow time series", inflowFiles);
	}
    for (Entity *o : entitiesByType("Observed"))
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
    return QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation).append("/").append(applicationShortName);
    //return QString::fromStdString(getenv("USERPROFILE")).append("/Documents/").append(applicationShortName).append(" Projects");
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



    for (QString experiment : experimentsList())
	{
		experiments->setCurrentText(experiment);
		log("Experiment: " + experiment);
		log("Blocks.");

        for (Node* e : Nodes())
		{
			e->setSelected(true);
			if (n0) n0->setSelected(false);
			e->update(true);
			n0 = e;
			QCoreApplication::processEvents();
			e->warnings.clear();
			e->errors.clear();
			QStringList list = e->variableNames();
			for (int i = 0; i < list.count(); i++)
			{
				QString variableName = list[i];
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
			list = e->variableNameConditions().keys();
			for (int i = 0; i < list.count(); i++)
			{
				QString variableName = list[i];
				if (!EntityNames("Parameter").contains(e->getValue(variableName).toQString()))
				{
					condition c = e->variableNameConditions()[variableName];
					for (int i = 0; i < c.Condition.size(); i++)
					{
						QString condition = c.Condition[i];
                        for(QString code : e->codes())
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
        for (Edge* e : Edges())
		{
			e->setSelected(true);
			if (e0) e0->setSelected(false);
			e->update();
			e0 = e;
			QCoreApplication::processEvents();
			e->warnings.clear();
			e->errors.clear();
			QStringList list = e->variableNames();
			for (int i = 0; i < list.count(); i++)
			{
				QString variableName = list[i];
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
#ifdef GIFMOD
			if (e->val("presc_flow").toBool())
				if (e->val("pre_flow_filename").toQString().isEmpty())
				{
					numberofWarnings++;
					e->warnings["Use Prescribed Flow"] = "Prescribed time series is not defined.";
					e->setProp("Use Prescribed Flow", "No");
					log(QString("Warning: Connector: %1, %2, %3").arg(experiment).arg(e->Name()).arg("Prescribed time series is not defined."));
				}
#endif
			list = e->variableNameConditions().keys();
			for (int i = 0; i < list.count(); i++)
			{
				QString variableName = list[i];
				if (!EntityNames("Parameter").contains(e->getValue(variableName).toQString()))
				{
					condition c = e->variableNameConditions()[variableName];
					for (int i = 0; i < c.Condition.size(); i++)
					{
						QString condition = c.Condition[i];
                        for(QString code : e->codes())
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
        for (Entity* e : Entities)
		{
			e->warnings.clear();
			e->errors.clear();
            for(QString variableName : e->variableNames())
			{
				if (variableName == "Read GA analysis from file")
					int i = 0;
				if (e->getProp(variableName, VariableTypeRole).toString().toLower() == "directory")
					if (!e->getValue(variableName).isEmpty() && !QFile::exists(e->getValue(variableName)))
					{
						numberofErrors++;
						e->errors[variableName] = "Folder not exists.";
						log(QString("Error: %1, %2, Folder (%3) does not exist.").arg(experiment).arg(variableName).arg(e->getValue(variableName)));
					}
				if (e->getProp(variableName, VariableTypeRole).toString().contains("filename"))
					if (e->getProp(variableName, VariableTypeRole).toString().contains("filename"))
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
            for(QString variableName : e->variableNameConditions().keys())
			{
				if (!EntityNames("Parameter").contains(e->getValue(variableName).toQString()))
				{
					condition c = e->variableNameConditions()[variableName];
					for (int i = 0; i < c.Condition.size(); i++)
					{
						QString condition = c.Condition[i];
                        for(QString code : e->codes())
							if (condition.contains(QString("{%1}").arg(code)))
								condition = condition.replace(QString("{%1}").arg(code), QString::number(e->val(code).toDouble()));
						if (e->props.getProp(variableName, experimentName()).isEmpty())
						{
							if (condition.toLower().contains("not empty"))
							{
								if (c.error[i].toLower() == "e")
								{
									numberofErrors++;
									e->errors[variableName] = c.errorDesc[i];// variableName + " should not be empty.";
									log(QString("Error: %1, %2: %3, %4").arg(experiment).arg(e->getValue("Type")).arg(e->Name()).arg(c.errorDesc[i]));// variableName + " should not be empty."));
								}
								if (c.error[i].toLower() == "w")
								{
									numberofWarnings++;
									e->errors[variableName] = c.errorDesc[i];// variableName + " should not be empty.";
									log(QString("Warning: %1, %2: %3, %4").arg(experiment).arg(e->getValue("Type")).arg(e->Name()).arg(c.errorDesc[i]));// variableName + " should not be empty."));
								}
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
#ifdef GIFMOD
		// Check for Observation Locations
        for (Entity* e : entitiesByType("Observation"))
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
		//check for constituents in buildup
        for (Entity* e : entitiesByType("Build-up"))
            for (QString experiment : experimentsList())
				if (!EntityNames("Constituent").contains(e->val("Constituent")))
				{
					e->errors["Constituent"] = QString("%1 was not found in the model").arg(e->val("Constituent"));
					numberofErrors++;
					log(QString("Error: %1, %2: %3, %4").arg(experiment).arg(e->getValue("Type")).arg(e->Name()).arg(e->errors["Constituent"]));
				}
/*		//check for constituents in initial conditions
        for (Entity* e in entitiesByType("Build-up"))
            for (QString experiment in experimentsList())
				if (!EntityNames("Constituent").contains(e->val("Constituent")))
					e->errors["Constituent"] = QString("%1 was not found in the model").arg(e->val("Constituent"));
					*/

		// Check reaction Network
		ReactionWindowPri d;
		for (int i = 0; i < Processes.count(); i++)
			if (!ReactionTableModel::validateNetworkRXNExp(Processes[i]->rate, this, d.Functions, d.Physical))
			{
				numberofErrors++;
				log(QString("Error: Reaction network, Process: %1, has error").arg(Processes[i]->name));
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
    for (Node* e : Nodes())
		n = min(e->x(), n);
	return n;
}
double GraphWidget::minY() const
{
	double n = 100000000;
    for (Node* e : Nodes())
		n = min(e->y(), n);
	return n;
}
double GraphWidget::maxX() const
{
	double n = -1000000000;
    for (Node* e : Nodes())
		n = max(e->x() + e->Width(), n);
	return n;
}
double GraphWidget::maxY() const
{
	double n = -1000000000;
    for (Node* e : Nodes())
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
//	for(QString file in inflowFileNamesofNode)
//	{
	if (file.isEmpty())	return true;
		CBTCSet inflows = CBTCSet(file.toStdString(), 1);
		if (inflows.names.size() == 0) return false;
		return true;
//		for (string name in inflows.names)
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
	int date = julian2xldate(currentCalendar->selectedDate().toJulianDay());
	currentModel->setData(currentIndex, date, Qt::EditRole);

}

#ifdef GIFMOD
void GraphWidget::experimentSelect(const QString & experimentName)
{
	experiments->setCurrentText(experimentName);
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
void GraphWidget::colorSchemeLegend_closed()
{
	colorCode.nodes = false;
	colorCode.edges = false;
	delete legendSliderTime;
	legendSliderTime = 0;
	colors = colorlegend();
	update(true);
}
void GraphWidget::legendSliderChanged_Nodes(int value)
{
	double time = model->Timemin + value * model->dt();
	colorScheme::colorandLegend(colors, time, "Blue-Red", false, 8);
	applyColorstoNodes();
}
void GraphWidget::legendSliderChanged_Edges(int value)
{
	double time = model->Timemin + value * model->dt();
	colorScheme::colorandLegend(colors, time, "Blue-Red", false, 8);
	applyColorstoEdges();
}

#endif
int GraphWidget::experimentID()
{
	return experiments->currentIndex();
}
QString GraphWidget::experimentName()
{
#ifdef GWA
	return "experiment1";
#endif

	QString a = experiments->currentText();
	return experiments->currentText();
}

#ifdef GIFMOD
bool GraphWidget::wizard(QList<command> &commands)
{
	return true;
}

QMap<QString, QString> GraphWidget::find_objects(QString name)
{
	QMap<QString, QString> out;

	if (nodeNames().contains(name))
		out[node(name)->Name()] = node(name)->objectType.ObjectType;


	if (edgeNames().contains(name))
		out[edge(name)->Name()] = edge(name)->objectType.ObjectType;

	QStringList typesList;

	typesList << "Constituent" << "Particle";// << ""

	for (int i = 0; i < typesList.count(); i++)
		if (EntityNames(typesList[i]).contains(name))
		{
			out[entity(name)->Name()] = entity(name)->objectType.ObjectType;
		}
	return out;
}


QList<QVariant> GraphWidget::runCommands(QList<CCommand> &commands)
{
	QList<QVariant> results;
	for (int i = 0; i < commands.size(); i++)
		results.append(runCommand(commands[i]));

	return results; 
}

QVariant GraphWidget::runCommand(CCommand command)
{
	//settings
	bool writeOutput = true, writeDetails = false, orderedOutput = false;
	QString type = "*";
	QString experiment = "All experiments";


	//QString cmd = command.toLower().trimmed();
	//QList<XString> args;
	QStringList settings;
	QString output;
	/*for (int i = 0; i < arguments.count(); i++)
	{
		XString arg = arguments[i];
		QString setting = arg.toQString().toLower().trimmed();

		if (setting.contains('/')) //interprete setting
		{
			setting = setting.remove('/');
			if (setting == "blind" || setting == "b")
				writeOutput = false;
			if (setting == "detail" || setting == "d")
				writeDetails = true;
			if (setting == "sort" || setting == "s" ||
				setting == "order" || setting == "o")
				orderedOutput = true;
			if (setting.contains("t:") || setting.contains("type:"))
				type = setting.right(setting.size() - setting.indexOf(":") - 1).trimmed().toLower();
			if (setting.contains("e:") || setting.contains("exp:") || setting.contains("experiment:"))
				experiment = setting.right(setting.size() - setting.indexOf(":") - 1).trimmed().toLower();

		}
		else //consider argument
			args.append(arg);
	}*/
	if (command.command == "help")
		output.append("Command line syntax is: command [-setting]\nor command [argument] [argument] [-setting]");

	if (command.command == "clear")
		clear();

	if (command.command == "zoom" || command.command == "z")
	{
		if (command.values[0] == "e" || command.values[0] == "extent" || command.values[0] == "a" || command.values[0] == "all")
			mainWindow->zoomAll();
		
	}

	if (command.command == "list")
	{
		QStringList result;
		
		if (command.values[0] == "nodes" || command.values[0] == "")
		{
			result = nodeNames();
		}
		if (command.values[0] == "ponds")
		{
			result = nodeNames("pond");
		}
		if (command.values[0] == "catchments")
		{
			result = nodeNames("catchment");
		}
		if (command.values[0] == "soils")
		{
			result = nodeNames("soil");
		}

		if (orderedOutput)
			result.sort();
		output = result.join('\n');
	}

	if (command.command == "setprop")
	{
		//if (!args.contains("=") || args.indexOf("=") < 2)
		//	output = "Syntax error.";
		//else
		{
			QStringList values = command.values;
			//int equalIndex = args.indexOf("=");
			for (int val_counter=0; val_counter<command.values.count(); val_counter++)
			foreach(QString key, command.parameters.keys())
			{
				QString propName = key;
				XString value = command.parameters.value(key);
				int found = 0;
				Node *n = 0; Edge* ed = 0; Entity *en = 0;
				if (type == "*" || type == "node" || type == "block")
					if (nodeNames().contains(values[val_counter]))
					{
						found++;
						n = node(values[val_counter]);
					}
				if (type == "*" || type == "edge" || type == "connector")
					if (edgeNames().contains(values[val_counter]))
					{
						found++;
						ed = edge(values[val_counter]);
					}
				
				
					if (entityByName(values[val_counter]))
					{
						found++;
						en = entity(values[val_counter]);
					}

				if (!found)
					output = QString("%1 not found in the model.").arg(values[val_counter]);
				else if (found > 1)
					output = QString("More than one %1 found in the model, please specify the type of the object you want to set value.").arg(values[val_counter]);
				else
				{
					if (n) {
						output += setprop(n, XString::reform(propName), value, experiment);
					}
					if (ed) {
						output += setprop(ed, XString::reform(propName), value, experiment);
						}
					if (en) {
						output += setprop(en, XString::reform(propName), value, experiment);
					}
					mainWindow->tableProp->repaint();
				}
			}
		}
	}
	
	if (command.command == "add")
	{
		if (command.values[0] == "")
		{
			output = "Syntax error.";
			return output;
		}
		else
		{
			bool failed = true;
			QString e = command.values[0].toLower();
			QStringList addfromMainWindow;
			addfromMainWindow << "soil" << "pond" << "catchment" << "darcy" << "storage" << "stream" << "plant";
			if (addfromMainWindow.contains(e))
			{
				Node* n = mainWindow->add(e);
				foreach(QString key, command.parameters.keys())
					output += setprop(n, XString::reform(key), command.parameters[key], experiment);

				//qDebug() << int(n->getProp("x").toDouble());
				n->setProp("x", int(n->getProp("x").toDouble()));
				n->setProp("y", int(n->getProp("y").toDouble()));
				n->setProp("Superficial width", int(n->getProp("Superficial width").toDouble()));
				n->setProp("Superficial height", int(n->getProp("Superficial height").toDouble()));
				n->setX(int(n->getProp("x").toDouble()));
				n->setY(int(n->getProp("y").toDouble()));
				n->setWidth(int(n->getProp("Superficial width").toDouble()));
				n->setHeight(int(n->getProp("Superficial height").toDouble()));
				failed = false;
			}
			
			QStringList entityTypes, exactType;

			entityTypes << "sensor" << "objective function" << "controller" << "constituent" << "particle" << "build-up" << "external flux" << "reaction parameter" << "parameter" << "observation" << "evapotranspiration" << "wells" << "tracers";
			exactType << "Sensor" << "Objective function" << "Controller" << "Constituent" << "Particle" << "Build-up" << "External flux" << "Reaction parameter" << "Parameter" << "Observation" << "Evapotranspiration" << "Wells" << "Tracers";
			Entity *en;
			if (entityTypes.contains(e))
			{
				en = new Entity(exactType[entityTypes.indexOf(e)], "No Name", this);

				foreach(QString key, command.parameters.keys())
					output += setprop(en, XString::reform(key), command.parameters[key], experiment);
				failed = false;
			}
			if (e.toLower()=="reaction")
			{ 
				Process *p = new Process(command.get_name(), this);
				p->rate = command.parameters["rate"].toQString();
				foreach(QString X,  command.parameters.keys())
				{
					if (X.split('[')[0].trimmed().toLower() == "stch")
					{
						p->stoichiometries[X.split('[')[1].split(']')[0]] = command.parameters[X];
					}
				}
			
			}
			if (failed)
			{
				output = e + " is not a recognized object!";
				return output;
			}
		}
	}
	if (command.command == "connect")
	{
		if (command.values.count() != 2)
		{
			output = "Two arguments are needed";
			return output;
		}
		else
		{
			Node *n1 = node(command.values[0]);
			Node *n2 = node(command.values[1]);
			if (!n1 || !n2)
			{
				if (!n1)
				{
					output.append(QString("%1 is not accessible").arg(command.values[0]));
					return output;
				}
				if (!n2)
				{
					output.append(QString("%1 is not accessible").arg(command.values[1]));
					return output;
				}
			}
			else
			{
				Edge *e = new Edge(n1, n2, this);
				if (e != NULL)
				{
					if (command.parameters.count("Type")>0) setprop(e, XString::reform("Type"), command.parameters["Type"], experiment);
					foreach(QString key, command.parameters.keys())
						output += setprop(e, XString::reform(key), command.parameters[key], experiment);
				}
			}
		}
	}
		
	if (writeOutput)
	{
		logW->append(output);
		output += command.toQString() + ", Ok";
		return output;
	}


	return QVariant(command.toQString()+", Ok");
}

QString  GraphWidget::setprop(Node *n, QString propname, XString value, QString experiment)
{
	QString output;
	if (!mList->extract_props_for_type(n->objectType.ObjectType).contains(propname.toLower()))
		output = QString(n->Name() + " does not have a property called " + propname);
	else
	{
		QString full_propname = mList->get_proper_property(n->objectType.ObjectType, propname);
		QStringList unitsList = n->getProp(full_propname, UnitsListRole).toStringList();
		QString defaultUnit = n->getProp(full_propname, defaultUnitRole).toString();
		QString unit = "";
		if (value.unit == "") value.unit = defaultUnit;
		for (int i = 0; i < unitsList.count(); i++)
			if (XString::coefficient(XString::reformBack(value.unit)) == XString::coefficient(XString::reformBack(unitsList[i])))
				unit = unitsList[i];
		value.unit = (unit == "") ? defaultUnit : unit;
		value.setUnitsList(unitsList);
		value.defaultUnit = defaultUnit;
		if (full_propname == "Name" && nodeNames().contains(value.toQString()))
		{
			log("Node '" + value.toQString() + "' already exist");
		}
		else
			n->props.setProp(full_propname, value, experiment); n->changed();// update();
		
	}
	return output;
}

QString  GraphWidget::setprop(Edge *ed, QString propname, XString value, QString experiment)
{
	QString output;
	if (!mList->extract_props_for_type(ed->objectType.ObjectType).contains(propname.toLower()))
		output = QString(ed->Name() + " does not have a property called " + propname);
	else
	{
		QString full_propname = mList->get_proper_property(ed->objectType.ObjectType, propname);
		QStringList unitsList = ed->getProp(full_propname, UnitsListRole).toStringList();
		QString defaultUnit = ed->getProp(full_propname, defaultUnitRole).toString();
		if (value.unit == "") value.unit = defaultUnit;
		QString unit = "";
		for (int i = 0; i < unitsList.count(); i++)
		{
			if (XString::reformBack(value.unit) == XString::reformBack(unitsList[i])
				|| XString::coefficient(XString::reformBack(value.unit)) == XString::coefficient(XString::reformBack(unitsList[i])))
			{
				unit = unitsList[i];
				exit;
			}
		}
		value.unit = (unit == "") ? defaultUnit : unit;
		value.setUnitsList(unitsList);
		value.defaultUnit = defaultUnit;
		
		if (full_propname == "Name" && nodeNames().contains(value.toQString()))
		{
			log("Connector '" + value.toQString() + "' already exist");
		}
		else
			ed->props.setProp(full_propname, value, experiment); ed->changed(); // update();
	}
	return output;
}

QString  GraphWidget::setprop(Entity *en, QString propname, XString value, QString experiment)
{
	QString output;
	if (!mList->extract_props_for_type(en->objectType.ObjectType).contains(propname.toLower()))
		output = QString(en->Name() + " does not have a property called " + propname);
	else
	{
		QString full_propname = mList->get_proper_property(en->objectType.ObjectType, propname);
		QStringList unitsList = en->getProp(full_propname, UnitsListRole).toStringList();
		QString defaultUnit = en->getProp(full_propname, defaultUnitRole).toString();
		if (value.unit == "") value.unit = defaultUnit;
		QString unit = "";
		for (int i = 0; i < unitsList.count(); i++)
		{
			if (XString::reformBack(value.unit) == XString::reformBack(unitsList[i])
				|| XString::coefficient(XString::reformBack(value.unit)) == XString::coefficient(XString::reformBack(unitsList[i])))
			{
				unit = unitsList[i];
				exit;
			}
		}
		value.unit = (unit == "") ? defaultUnit : unit;
		value.setUnitsList(unitsList);
		value.defaultUnit = defaultUnit;
		if (full_propname == "Name" && nodeNames().contains(value.toQString()))
		{
			log("Entity '" + value.toQString() + "' already exist");
		}
		else
			en->props.setProp(full_propname, value, experiment);	en->changed();// update();
	}
	return output;
}

#endif

void GraphWidget::copyProps(QString sourceExperiment, QString destExperiment)
{
    for (Node *n : Nodes())
		n->copyProps(sourceExperiment, destExperiment);
    for (Edge *e : Edges())
		e->copyProps(sourceExperiment, destExperiment);
    for (Entity *e : Entities)
		e->copyProps(sourceExperiment, destExperiment);

}

QString getTime(bool reset)
{
	static 	clock_t t0;
	clock_t t1;

	t1 = clock() - t0;
	float run_time = ((float)t1) / CLOCKS_PER_SEC;
	QString st = " sec";
	if (run_time >= 60) { run_time /= 60; st = " min"; }
	if (run_time >= 60) { run_time /= 60; st = " hr"; }

	QString r= QString("%1 %2").arg(run_time).arg(st);
	if (reset)
		t0 = clock();

	return r;
}
bool isFuzzyEqual(double a, double b, double allowableError)
{
	return (fabs(a - b) / ((a + b) / 2) <= allowableError);
}



/*
{
QMenu *waterQualitySubMenu = menu->addMenu("Plot Water Quality Results");

if (model->colloid_transport() && entitiesByType("Particle").count())
{
for (Entity *p in entitiesByType("Particle"))
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
for (Entity *e in entitiesByType("Constituent"))
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
for (Entity *e in entitiesByType("Constituent"))
{
constituentSorbedSubMenu = sorbedSubMenu->addMenu(e->Name());
QStringList list;
list.append("Constituent");
int BTCid = model->get_member_no(model->getblocksq(n->Name().toStdString()), -1, 0, model->RXN().look_up_constituent_no(e->Name().toStdString()));
list.append(QString::number(BTCid));
menuKey[constituentSorbedSubMenu->addAction("Soil")] = list;
for (Entity *p in entitiesByType("Particle"))
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
*/
