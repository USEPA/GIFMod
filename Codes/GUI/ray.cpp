#include "Ray.h"
#include "node.h"
#include <QPainter>
#include <math.h>
#include <qdebug.h>

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;

//Ray::Ray(Node *sourceNode, Node *destNode)
Ray::Ray()
: arrowSize(10)
{
    setAcceptedMouseButtons(0);
//    source = sourceNode;
//    dest = destNode;
//    source->addRay(this);
//    dest->addRay(this);
//    adjust();
	itemType = Object_Types::RayLine;
	GUI = "Ray";

//	sourceID = source->ID;
//	destID = dest->ID;
}

void Ray::adjust(Node *_Node, QPointF *_Point)
{
    if (!_Node || !_Point)
        return;
	QLineF line(mapFromItem(_Node, _Node->Width() / 2, _Node->Height() / 2), *_Point);
//	QLineF line(mapFromItem(_Node, _Node->Width() / 2, _Node->Height() / 2), mapToScene(*_Point));
	qreal length = line.length();
	prepareGeometryChange();

	qreal Ox, Oy, Dx, Dy;
		
	if (abs(line.dx()) < 1)
	{
		Ox = 0;
		Dx = 0;
	}
	else
	{
		Ox = line.dx() / abs(line.dx())*min(int(_Node->Width() / 2), int(fabs(_Node->Height() / 2.0 * line.dx() / (line.dy()+0.5))));
		Dx = -line.dx(); // abs(line.dx())*min(0, int(fabs(dest->Height / 2.0 * line.dx() / (line.dy() + 0.5))));
	}
	if (abs(line.dy()) < 1)
	{
		Oy = 0;
		Dy = 0;
	}
	else
	{
		Oy = line.dy() / abs(line.dy())*min(int(_Node->Height() / 2), int(fabs(_Node->Width() / 2.0 * line.dy() / (line.dx()+0.5))));
		Dy = -line.dy(); // abs(line.dy())*min(int(dest->Height / 2), int(fabs(dest->Width / 2.0 * line.dy() / (line.dx() + 0.5))));
	}
	
/*	if ((abs(line.dy()) < max(_Node->Height, dest->Height)) && (abs(line.dx()) < max(_Node->Width, dest->Width))) 
		sourcePoint = destPoint = line.p1(); 
	else
	{*/
		QPointF RayOffsetSource(Ox, Oy);

		QPointF RayOffsetDest(Dx, Dy);

//		if (abs(Ox) + abs(Oy) + abs(Dx) + abs(Dy) > 10000)
//			qDebug() <<"Warning" << line.dx() << line.dy();
//		qDebug() << QString("Ox%1, Oy%2, Dx%3, Dy%4").arg(Ox).arg(Oy).arg(Dx).arg(Dy);
//		qDebug() << QString("Ox%1, Oy%2, Dx%3, Dy%4").arg(RayOffsetSource.x()).arg(RayOffsetSource.y()).arg(RayOffsetDest.x()).arg(RayOffsetDest.y());
//		qDebug() << QString("Px%1, Py%2, adjx%3, adjy%4").arg(source->x()).arg(source->y());
//		qDebug() << QString("Px%1, Py%2, adjx%3, adjy%4").arg(line.p1().x()).arg(line.p1().y()).arg(sourcePoint.x()).arg(sourcePoint.y());

//		sourcePoint = line.p1() + RayOffsetSource;
//		destPoint = line.p2()  + RayOffsetDest;

		sourcePoint = line.p1() + RayOffsetSource;
		destPoint = *_Point;
		update();
		//}
}
void Ray::adjust(Node *source, Node *dest)
{
	QLineF line(mapFromItem(source, source->Width() / 2, source->Height() / 2), mapFromItem(dest, dest->Width() / 2, dest->Height() / 2));
	qreal length = line.length();

	prepareGeometryChange();

	qreal Ox, Oy, Dx, Dy;

	if (abs(line.dx()) < 1)
	{
		Ox = 0;
		Dx = 0;
	}
	else
	{
		Ox = line.dx() / abs(line.dx())*min(int(source->Width() / 2), int(fabs(source->Height() / 2.0 * line.dx() / (line.dy() + 0.5))));
		Dx = -line.dx() / abs(line.dx())*min(int(dest->Width() / 2), int(fabs(dest->Height() / 2.0 * line.dx() / (line.dy() + 0.5))));
	}
	if (abs(line.dy()) < 1)
	{
		Oy = 0;
		Dy = 0;
	}
	else
	{
		Oy = line.dy() / abs(line.dy())*min(int(source->Height() / 2), int(fabs(source->Width() / 2.0 * line.dy() / (line.dx() + 0.5))));
		Dy = -line.dy() / abs(line.dy())*min(int(dest->Height() / 2), int(fabs(dest->Width() / 2.0 * line.dy() / (line.dx() + 0.5))));
	}
	QPointF edgeOffsetSource(Ox, Oy);
	QPointF edgeOffsetDest(Dx, Dy);
	sourcePoint = line.p1() + edgeOffsetSource;
	destPoint = line.p2() + edgeOffsetDest;
}

QRectF Ray::boundingRect() const
{
//    if (!sourcePoint || !destPoint)
//       return QRectF();

    qreal penWidth = 1;
    qreal extra = (penWidth + arrowSize) / 2.0;

    return QRectF(sourcePoint, QSizeF(destPoint.x() - sourcePoint.x(),
                                      destPoint.y() - sourcePoint.y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}
void Ray::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
//    if (!source || !dest)
//        return;

    QLineF line(sourcePoint, destPoint);
//	qDebug() << line.length();
    if (qFuzzyCompare(line.length(), qreal(0.)))
        return;
    // Draw the line itself
	painter->setPen(QPen(valid?Qt::black:Qt::gray, 1, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin));
	painter->drawLine(line);
//! [5]

//! [6]
    // Draw the arrows
    double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0)
        angle = TwoPi - angle;

    QPointF sourceArrowP1 = sourcePoint + QPointF(sin(angle + Pi / 3) * arrowSize,
                                                  cos(angle + Pi / 3) * arrowSize);
    QPointF sourceArrowP2 = sourcePoint + QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
                                                  cos(angle + Pi - Pi / 3) * arrowSize);
    QPointF destArrowP1 = destPoint + QPointF(sin(angle - Pi / 3) * arrowSize,
                                              cos(angle - Pi / 3) * arrowSize);
    QPointF destArrowP2 = destPoint + QPointF(sin(angle - Pi + Pi / 3) * arrowSize,
                                              cos(angle - Pi + Pi / 3) * arrowSize);

	painter->setBrush(valid ? Qt::black : Qt::gray);
	painter->drawPolygon(QPolygonF() << line.p1() << sourceArrowP1 << sourceArrowP2);
    painter->drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2);
}
void Ray::setValidation(bool _valid)
{
	valid = _valid;
}
