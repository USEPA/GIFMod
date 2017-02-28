#include "treeitem.h"
#include "treemodel.h"
#include <QStringList>
#include "GWidget.h"
#include "node.h"
#include "edge.h"
#include "entity.h"
#include "qdebug.h" //remove it

TreeModel::TreeModel(GraphWidget *parent) : QAbstractItemModel(parent)
{
		Parent = parent;
#ifdef GIFMOD
		if (parent->applicationShortName == "GIFMod")
		{
			rootItem = new TreeItem("Root", parent, TreeItem::Type::Root);// , 0);
			settings = new TreeItem("Settings", parent, TreeItem::Type::SettingsBranch);//, rootItem);
			projectSettings = new TreeItem("Project settings", parent, TreeItem::Type::Item);//, settings);
			climateSettings = new TreeItem("Climate settings", parent, TreeItem::Type::Item);//, settings);
			solverSettings = new TreeItem("Solver settings", parent, TreeItem::Type::Item);//, settings);
			blocks = new TreeItem("Blocks", parent, TreeItem::Type::NodesBranch);//, rootItem);
			connectors = new TreeItem("Connectors", parent, TreeItem::Type::EdgesBranch);//, rootItem);
			waterQuality = new TreeItem("Water quality", parent, TreeItem::Type::WaterQualityBranch);//, rootItem);
			particle = new TreeItem("Particles", parent, TreeItem::Type::Branch);//, waterQuality);
			constituent = new TreeItem("Constituents", parent, TreeItem::Type::Branch);//, waterQuality);
			evapotranspiration = new TreeItem("Evapotranspirations", parent, TreeItem::Type::Branch);//, waterQuality);
			buildUp = new TreeItem("Build-ups", parent, TreeItem::Type::Branch);//, waterQuality);
			extrenalFlux = new TreeItem("External fluxes", parent, TreeItem::Type::Branch);//, waterQuality);
			reactions = new TreeItem("Reactions", parent, TreeItem::Type::ReactionsBranch);//, waterQuality);
			reactionParameter = new TreeItem("Reaction parameters", parent, TreeItem::Type::Branch);//, reactions);
			//				reaction = new TreeItem("Reaction", parent, TreeItem::Type::Branch, reactions);
			reactionNetwork = new TreeItem("Reaction network", parent, TreeItem::Type::ReactionNetworkItem);//, reactions);
			inverseModeling = new TreeItem("Inverse modeling", parent, TreeItem::Type::InverseModelingBranch);//, rootItem);
			GA = new TreeItem("Genetic algorithm", parent, TreeItem::Type::Item);//, inverseModeling);
			MCMC = new TreeItem("Markov chain Monte Carlo", parent, TreeItem::Type::Item);//, inverseModeling);
			parameter = new TreeItem("Parameters", parent, TreeItem::Type::Branch);//, inverseModeling);
			observed = new TreeItem("Observations", parent, TreeItem::Type::Branch);//, inverseModeling);
			control = new TreeItem("Control", parent, TreeItem::Type::ControlBranch);//, rootItem);
			sensor = new TreeItem("Sensors", parent, TreeItem::Type::Branch);
			objectiveFunction = new TreeItem("Objective functions", parent, TreeItem::Type::Branch);
			controller = new TreeItem("Controllers", parent, TreeItem::Type::Branch);

			QList<TreeItem*> rootNodes, settingsNodes, waterQualityNodes, reactionsNodes, inverseModelingNodes, controlNodes;
			rootNodes << settings << blocks << connectors << evapotranspiration << waterQuality << inverseModeling << control;
			settingsNodes << projectSettings << climateSettings << solverSettings;
			waterQualityNodes << particle << constituent << buildUp << extrenalFlux << reactions;
			reactionsNodes << reactionParameter << reactionNetwork;
			inverseModelingNodes << GA << MCMC << parameter << observed;
			controlNodes << objectiveFunction << sensor << controller;
			rootItem->addChild(rootNodes);
			settings->addChild(settingsNodes);
			waterQuality->addChild(waterQualityNodes);
			reactions->addChild(reactionsNodes);
			inverseModeling->addChild(inverseModelingNodes);
			control->addChild(controlNodes);

		}
#endif
#ifdef GWA
		if (parent->applicationShortName == "GWA")
		{
			rootItem = new TreeItem("Root", parent, TreeItem::Type::Root);// , 0);
			settings = new TreeItem("Settings", parent, TreeItem::Type::SettingsBranch);//, rootItem);
			globalSettings = new TreeItem("Global Settings", parent, TreeItem::Type::Item);//, settings);
		//	solver = new TreeItem("Solver", parent, TreeItem::Type::Item);//, settings);
			wells = new TreeItem("Wells", parent, TreeItem::Type::WellsBranch);//, rootItem);
			tracers = new TreeItem("Tracers", parent, TreeItem::Type::TracersBranch);//, rootItem);
/*			waterQuality = new TreeItem("Water Quality", parent, TreeItem::Type::WaterQualityBranch, rootItem);
			particle = new TreeItem("Particles", parent, TreeItem::Type::Branch, waterQuality);
			constituent = new TreeItem("Constituents", parent, TreeItem::Type::Branch, waterQuality);
			buildUp = new TreeItem("Build-ups", parent, TreeItem::Type::Branch, waterQuality);
			extrenalFlux = new TreeItem("External fluxes", parent, TreeItem::Type::Branch, waterQuality);
			reactions = new TreeItem("Reactions", parent, TreeItem::Type::ReactionsBranch, waterQuality);
			reactionParameter = new TreeItem("Reaction parameters", parent, TreeItem::Type::Branch, reactions);
			//				reaction = new TreeItem("Reaction", parent, TreeItem::Type::Branch, reactions);
			reactionNetwork = new TreeItem("Reaction Network", parent, TreeItem::Type::ReactionNetworkItem, reactions);
			*/			inverseModeling = new TreeItem("Inverse Modeling", parent, TreeItem::Type::InverseModelingBranch);//, rootItem);
			GA = new TreeItem("Genetic Algorithm", parent, TreeItem::Type::Item);//, inverseModeling);
			MCMC = new TreeItem("Markov Chain Monte Carlo", parent, TreeItem::Type::Item);//, inverseModeling);
			parameter = new TreeItem("Parameters", parent, TreeItem::Type::Branch);//, inverseModeling);
			observed = new TreeItem("Observations", parent, TreeItem::Type::Branch);//, inverseModeling);

			QList<TreeItem*> rootNodes, settingsNodes, inverseModelingNodes;
			rootNodes << settings << wells << tracers << inverseModeling;
			settingsNodes << globalSettings;// << solver;
			inverseModelingNodes << GA << MCMC << parameter << observed;
			rootItem->addChild(rootNodes);
			settings->addChild(settingsNodes);
			inverseModeling->addChild(inverseModelingNodes);

		}
#endif

		Parent->log("Tree model created.");
}

void TreeModel::refresh(QString part)
{/*
	//Parent = parent;
	//rootItem = new TreeItem("Anaconda", parent, TreeItem::Type::Root, 0);
	//settings = new TreeItem("Settings", parent, TreeItem::Type::SettingsBranch, rootItem);
	//globalSettings = new TreeItem("Global Settings", parent, TreeItem::Type::Item, settings);
	//solver = new TreeItem("Solver", parent, TreeItem::Type::Item, settings);
	//blocks = new TreeItem("Blocks", parent, TreeItem::Type::NodesBranch, rootItem);
	//connectors = new TreeItem("Connectors", parent, TreeItem::Type::EdgesBranch, rootItem);
	//waterQuality = new TreeItem("Water Quality", parent, TreeItem::Type::WaterQualityBranch, rootItem);
	//reactions = new TreeItem("Reactions", parent, TreeItem::Type::ReactionsBranch, waterQuality);
	//reactionNetwork = new TreeItem("Reaction Network", parent, TreeItem::Type::ReactionNetworkItem, reactions);
	//inverseModeling = new TreeItem("Inverse Modeling", parent, TreeItem::Type::InverseModelingBranch, rootItem);
	//GA = new TreeItem("Genetic Algorithm", parent, TreeItem::Type::Item, inverseModeling);
	//MCMC = new TreeItem("Markov Chain Monte Carlo", parent, TreeItem::Type::Item, inverseModeling);

	Parent->log("Tree Model refresh.");
	TreeItem* item;
	QString name;
	item = particle; name = "Particle";
	if (part.toLower() == "all" || part.toLower() == name.toLower())
	{
		qDeleteAll(item->childItems);
		item->childItems.clear();
		for each (QString e in Parent->EntityNames(name))
			new TreeItem(Parent->entity(e), item);
	}

	item = constituent; name = "Constituent";
	if (part.toLower() == "all" || part.toLower() == name.toLower())
	{
		qDeleteAll(item->childItems);
		item->childItems.clear();
		for each (QString e in Parent->EntityNames(name))
			new TreeItem(Parent->entity(e), item);
	}

	item = buildUp; name = "Build-Up";
	if (part.toLower() == "all" || part.toLower() == name.toLower())
	{
		qDeleteAll(item->children);
		item->children.clear();
		for each (QString e in Parent->EntityNames(name))
			new TreeItem(Parent->entity(e), item);
	}

	item = extrenalFlux; name = "External flux";
	if (part.toLower() == "all" || part.toLower() == name.toLower())
	{
		qDeleteAll(item->children);
		item->children.clear();
		for each (QString e in Parent->EntityNames(name))
			new TreeItem(Parent->entity(e), item);
	}

	item = reactionParameter; name = "Reaction parameter";
	if (part.toLower() == "all" || part.toLower() == name.toLower())
	{
		qDeleteAll(item->children);
		item->children.clear();
		for each (QString e in Parent->EntityNames(name))
			new TreeItem(Parent->entity(e), item);
	}

	item = parameter; name = "Parameter";
	if (part.toLower() == "all" || part.toLower() == name.toLower())
	{
		qDeleteAll(item->children);
		item->children.clear();
		for each (QString e in Parent->EntityNames(name))
			new TreeItem(Parent->entity(e), item);
	}

	item = observed; name = "Observed";
	if (part.toLower() == "all" || part.toLower() == name.toLower())
	{
		qDeleteAll(item->children);
		item->children.clear();
		for each (QString e in Parent->EntityNames(name))
			new TreeItem(Parent->entity(e), item);
	}

	update();*/
}

TreeModel::~TreeModel()
{
	delete rootItem;
}

void TreeModel::addChildFromMenu(const QString name, QModelIndex *parentIndex)
{
	TreeItem *parent = 0;
	if (name == "Controller")
		parent = this->controller;
	if (name == "Sensor")
		parent = this->sensor;
	if (name == "Objective function")
		parent = this->objectiveFunction;
	if (name == "Constituent")
		parent = this->constituent;
	if (name == "Particle")
		parent = this->particle;
	if (name == "Build-up")
		parent = this->buildUp;
	if (name == "External flux")
		parent = this->extrenalFlux;
	if (name == "Reaction parameter")
		parent = this->reactionParameter;
	if (name == "Parameter")
		parent = this->parameter;
	if (name == "Observation")
		parent = this->observed;
	if (name == "Evapotranspiration")
		parent = this->evapotranspiration;
	if (name == "Wells")
		parent = this->wells;
	if (name == "Tracers")
		parent = this->tracers;
	if (parent == 0) return;
	Entity* newEntity = new Entity(name, "No Name", parent->gWidget);
//	parent->addChild(new TreeItem(newEntity));
	beginInsertRows(*parentIndex, 0,1);
	endInsertRows();
	Parent->log(QString("One %1 created by tree view command.").arg(name));
	//new TreeItem(e->Name(), parentFromMenu->gWidget, TreeItem::Type::EntityItem, parentFromMenu);
}

void TreeModel::prepareToAddFromMenu(const QString  name)
{
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
	else
		return rootItem->columnCount();
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (role == Role::TreeItemType)
	{
		TreeItem *item = itemFromIndex(index);
		return item->type;
	}
	if (role == Role::TreeParentItemType)
	{
		TreeItem *item = itemFromIndex(index);
		return singularform(item->parent()->Name());
	}

	if ((role != Qt::DisplayRole) && (role != Qt::EditRole))
		return QVariant();
	
	TreeItem *item = itemFromIndex(index);
	reset;
	return QString("%1").arg(item->data(index.column()).toString());// .arg(rowCount(index)); // Name();
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return 0;
	return Qt::ItemIsSelectable | Qt::ItemIsEnabled; 
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	return QString("Header %1").arg(section);

	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		return rootItem->data(section);

	return QVariant();
}
QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent)
const
{
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	TreeItem *parentItem;

	if (!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<TreeItem*>(parent.internalPointer());

	TreeItem *childItem = parentItem->child(row);
	if (childItem)
		return createIndex(row, column, childItem);
	else
		return QModelIndex();
}
QModelIndex TreeModel::parent(const QModelIndex &index) const
{
	if (!index.isValid())
		return QModelIndex();

	TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
	TreeItem *parentItem = childItem->parent();

	if (parentItem == rootItem)
		return QModelIndex();

	return createIndex(parentItem->row(), 0, parentItem);
}

TreeItem* TreeModel::itemFromIndex(const QModelIndex &index) const
{
	if (index.isValid())
		return static_cast<TreeItem *>(index.internalPointer());
	else
		return 0;// rootItem;
}

bool TreeModel::hasChildren(const QModelIndex & parent) const
{
	return true;// itemFromIndex(parent)->childCount();
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
	TreeItem *parentItem;
	if (parent.column() > 0)
		return 0;

	if (!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<TreeItem*>(parent.internalPointer());

	return parentItem->childCount();
}

void TreeModel::deleteNode(int row, QModelIndex &parent, QString name)
{
	Parent->log(QString("Deleting %1 block by tree view command.").arg(name));
	beginRemoveRows(parent, row, row);
	blocks->deleteChild(name);
	Parent->deleteNode(name);
//	blocks->Children();
//	connectors->Children();
	endRemoveRows();
}
void TreeModel::deleteNode(Node *node)
{
	Parent->log(QString("Deleting %1 block by tree view command.").arg(node->Name()));
	TreeItem *branch;
#ifdef GIFMOD
	branch = blocks;
#endif
#ifdef GWA
	if (node->objectType.ObjectType == "Well")
		branch = wells;
	else if (node->objectType.ObjectType == "Tracer")
		branch = tracers;
#endif
	QModelIndex parent = createIndex(rootItem->indexOf(branch), 0, rootItem);
	QString name = node->Name();
	int row = branch->indexOf(name);
	QList<Edge*> edges = node->edgeList;
	for each (Edge *edge in edges)
		deleteEdge(edge);
	beginRemoveRows(parent, row, row);
	branch->deleteChild(name);
	Parent->deleteNode(name);
	endRemoveRows();
}
void TreeModel::deleteEdge(int row, QModelIndex &parent, QString name)
{
	Parent->log(QString("Deleting %1 connector by tree view command.").arg(name));
	beginRemoveRows(parent, row, row);
	connectors->deleteChild(name);
	Parent->deleteEdge(name);
//	connectors->Children();
	endRemoveRows();
}
void TreeModel::deleteEdge(QString edgeName)
{
	QString name = edgeName;
	Parent->log(QString("Deleting %1 connector by tree view command.").arg(name));
	QModelIndex parent = createIndex(rootItem->indexOf(connectors), 0, rootItem);
	int row = connectors->indexOf(name);
	beginRemoveRows(parent, row, row);
	connectors->deleteChild(name);
	Parent->deleteEdge(name);
	//	connectors->Children();
	endRemoveRows();
}
void TreeModel::deleteEdge(Edge *edge)
{
	QString name = edge->Name();
	Parent->log(QString("Deleting %1 connector by tree view command.").arg(name));
	QModelIndex parent = createIndex(rootItem->indexOf(connectors), 0, rootItem);
	int row = connectors->indexOf(name);
	beginRemoveRows(parent, row, row);
	connectors->deleteChild(name);
	Parent->deleteEdge(name);
	//	connectors->Children();
	endRemoveRows();
}
void TreeModel::deleteEntity(Entity *entity)
{
	QString name = entity->Name();
	QString type = entity->objectType.ObjectType;

	Parent->log(QString("Deleting %1 %2 by tree view command.").arg(name).arg(type));
	//createIndex(itemFromIndex(index)->parent->Children().indexOf(itemFromIndex(index)), 0, index.internalId());
	TreeItem *branch = entityParentItemfromType(type);
	if (!branch)
	{
		Parent->log(QString("Error in deleting %1 %2 by tree view command.").arg(name).arg(type));
		return;
	}
	QModelIndex parent = createIndex(rootItem->indexOf(branch), 0, branch->parent());
	int row = branch->indexOf(name);
	beginRemoveRows(parent, row, row);
//	TreeItem *branch = entityParentItemfromType(type);
	branch->deleteChild(name);
	Parent->deleteEntity(name);
	//	refresh(type);
	endRemoveRows();
}
void TreeModel::deleteEntity(int row, QModelIndex &parent, QString type, QString name)
{
	Parent->log(QString("Deleting %1 %2 by tree view command.").arg(name).arg(type));
	//createIndex(itemFromIndex(index)->parent->Children().indexOf(itemFromIndex(index)), 0, index.internalId());
	beginRemoveRows(parent, row, row);
	TreeItem *branch = entityParentItemfromType(type);
	branch->deleteChild(name);
	Parent->deleteEntity(name);
//	refresh(type);
	endRemoveRows();
}

QString TreeModel::singularform(QString name) const
{
	if (name.right(2) != "es" && name.right(1) == "s") return name.left(name.length() - 1);
	if (name == "External fluxes") return "External flux";
	if (name == "Observations") return "Observed";
	if (name == "Particles") return "Particle";
	return name;
}

void TreeModel::add(Node *node)
{
#ifdef GIFMOD
	Parent->log("Adding one block to tree view.");
	QModelIndex parent = createIndex(rootItem->indexOf(blocks), 0, rootItem);
	int row = blocks->childCount();
	beginInsertRows(parent, row, row);
	blocks->addChild(new TreeItem(node));
	endInsertRows();
	reset;
	//emit dataChanged(createIndex(row, 0, blocks), createIndex(row + 1, 0, blocks));
#endif
#ifdef GWA
	Parent->log("Adding one well to tree view.");
	QModelIndex parent = createIndex(rootItem->indexOf(wells), 0, rootItem);
	int row = wells->childCount();
	beginInsertRows(parent, row, row);
	wells->addChild(new TreeItem(node));
	endInsertRows();
	reset;
	//emit dataChanged(createIndex(row, 0, blocks), createIndex(row + 1, 0, blocks));
#endif

}
void TreeModel::addWell(Node *node)
{
	Parent->log("Adding one well to tree view.");
	QModelIndex parent = createIndex(rootItem->indexOf(wells), 0, rootItem);
	int row = wells->childCount();
	beginInsertRows(parent, row, row);
	wells->addChild(new TreeItem(node));
	endInsertRows();
	reset;
	//emit dataChanged(createIndex(row, 0, blocks), createIndex(row + 1, 0, blocks));
}
void TreeModel::addTracer(Node *node)
{
	Parent->log("Adding one well to tree view.");
	QModelIndex parent = createIndex(rootItem->indexOf(tracers), 0, rootItem);
	int row = tracers->childCount();
	beginInsertRows(parent, row, row);
	tracers->addChild(new TreeItem(node));
	endInsertRows();
	reset;
	//emit dataChanged(createIndex(row, 0, blocks), createIndex(row + 1, 0, blocks));
}

void TreeModel::add(Edge *edge)
{
	connectors->addChild(new TreeItem(edge));
}
TreeItem * TreeModel::entityParentItemfromType(QString type) const
{
	TreeItem *parent = 0;
	if (type == "Sensor")
		parent = this->sensor;
	if (type == "Objective function")
		parent = this->objectiveFunction;
	if (type == "Controller")
		parent = this->controller;
	if (type == "Constituent")
		parent = this->constituent;
	if (type == "Particle")
		parent = this->particle;
	if (type == "Build-up")
		parent = this->buildUp;
	if (type == "External flux")
		parent = this->extrenalFlux;
	if (type == "Reaction parameter")
		parent = this->reactionParameter;
	if (type == "Parameter")
		parent = this->parameter;
	if (type == "Observation")
		parent = this->observed;
	if (type == "Evapotranspiration")
		parent = this->evapotranspiration;
	if (type == "Wells")
		parent = this->wells;
	if (type == "Tracers")
		parent = this->tracers;
	return parent;
}
void TreeModel::add(Entity *entity)
{
	QString type = entity->objectType.ObjectType;
	TreeItem *parent = entityParentItemfromType(type);
	if (parent)parent->addChild(new TreeItem(entity));
}
