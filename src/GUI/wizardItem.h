#pragma once
#include <QGraphicsItem>
#include "XString.h"
#include "enums.h"


//QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
//QT_END_NAMESPACE

class wizard;
struct templateIcon;

class wizardItem : public QGraphicsItem
{
	//Q_OBJECT

public:
	wizardItem(wizard *parent = nullptr, const QString &name = "", const QString &iconFilename = "", const QString &description = "", const QPixmap &icon=QPixmap());	
//	wizardItem(wizard *parent, const templateIcon &tIcon);
	wizardItem(const wizardItem &);
	~wizardItem() {

	}
	objectColor color;
//	wizardItem operator=(const wizardItem &);

	enum { Type = UserType + 1 };
    int type() const Q_DECL_OVERRIDE { return Type; }

 //   QRectF boundingRect() const Q_DECL_OVERRIDE;
 //   QPainterPath shape() const Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;
	QString Name() const { return name; };
//	bool setName(const QString &Name);
//
	bool setObjectType(const QString &);
	bool setObjectSubType(const QString &);
	int Width() const { return width; };
	int Height() const { return height; };
	void setWidth(const int &Width) { width = Width; update(); };
	void setHeight(const int &Height) { height = Height; update(); };

//	QVariant getProp(const QString &propName, const int role = Qt::DisplayRole) const;
//	QVariant getProp(const QString &propName, const QList<wizardItem*> wizardItems, const int role = Qt::DisplayRole) const;
//	XString getValue(const QString &propName) const;
//	bool setProp(const QString &propName, const QVariant &Value, const int role = Qt::EditRole) ;
//	bool setValue(const QString &propName, const XString &Value);
//	corners corner(const int x, const int y);
//	edgesides edge(const int x, const int y);
//	void update(bool fast = false);
//	QString newwizardItemName(const QString name, QList<wizardItem*> &wizardItems) const;
//	static wizardItem* unCompact(QMap<QString, QVariant>, GraphWidget *gwidget, bool oldVersion = false);
//	static wizardItem* unCompact10(QMap<QString, QVariant>, GraphWidget *gwidget);
//	QStringList codes() const;
//	QMap<QString, condition> variableNameConditions() const;
//	bool errorDetected() const { return (errors.count()>0) ? true : false; }
//	QStringList variableNames() const;
//	QString variableName(QString code) const;
//	int experimentID();
//	QString experimentName() const;
//	void copyProps(QString sourceExperiment, QString destExperiment);

//	XString val(const QString & code) const;

	//GraphWidget *parent;
	QString GUI;
	Object_Types itemType;
	QMap<QString, QString> warnings, errors;
	int minH = 30, minW = 40;
	bool oldVersionLoad = false;
	QString middleText = "";

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) Q_DECL_OVERRIDE;
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

signals:
	void hoverMoveEvent(QGraphicsSceneHoverEvent * event);

//public slots:
//	void contextMenuClicked(QAction*);

private:
	int width, height;
	QString name;

	QPointF newPos;
//	GraphWidget *graph;
	void changed();
//	QMap<QString, QString> warnings, errors;
	bool bold = false;
	wizard *parent;
	QString iconFilename;
	QString description;
	QPixmap icon;

};
