#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include "edgeitem.h"
#include "vertexitem.h"
#include "CORE/config.h"


VertexItem::VertexItem(int id)
{
	_id = id;

	_size = 0;
	_fontSize = 0;

	_text.setParentItem(this);
	_text.setFont(QFont(FONT_FAMILY));

	setZValue(0);

	setPen(QPen(Qt::NoPen));

	setFlag(ItemIsMovable);
#if QT_VERSION >= 0x040600
	setFlag(ItemSendsGeometryChanges);
#endif
}

void VertexItem::addEdge(EdgeItem *edge)
{
	_edgeList.append(edge);
	edge->adjust();
}

void VertexItem::setCoordinates(const QPoint &coordinates)
{
	_coordinates = coordinates;
	setPos(coordinates.x(), coordinates.y());
}

void VertexItem::setAttribute(const QString &attribute)
{
	if (_attributes.contains(attribute)) {
		_text.setText(_attributes[attribute]);
		_attribute = attribute;
	} else {
		_text.setText(QString());
		_attribute.clear();
	}
}

void VertexItem::addAttribute(const QString &attribute, const QString &value)
{
	_attributes.insert(attribute, value);
}

void VertexItem::setSize(qreal size)
{
	qreal diff = (_size - size) / 2;
	_size = size;

	if (!size) {
		setVisible(false);
	} else {
		prepareGeometryChange();
		moveBy(diff, diff);
		setRect(QRectF(0, 0, size, size));
		_text.setPos(size, size / 2);

		if (!isVisible())
			setVisible(true);
	}
}

void VertexItem::setColor(const QColor &color)
{
	_color = color;
	setBrush(QBrush(color));
	_text.setBrush(QBrush(color));
}

void VertexItem::setFontSize(int size)
{
	_fontSize = size;

	if (size <= 0) {
		_text.setVisible(false);
	} else {
		QFont font = _text.font();
		font.setPixelSize(size);
		_text.setFont(font);
		_text.setPos(_size, _size / 2);

		if (!_text.isVisible())
			_text.setVisible(true);
	}

	update();
}

QVariant VertexItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
	if (change == ItemPositionHasChanged) {
		_coordinates = value.toPoint();
		foreach (EdgeItem *edge, _edgeList)
			edge->adjust();
	}

	return QGraphicsItem::itemChange(change, value);
}

void VertexItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	setPen(QPen(brush(), _size / 2));
	QGraphicsItem::mousePressEvent(event);
}

void VertexItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	setPen(QPen(Qt::NoPen));
	QGraphicsItem::mouseReleaseEvent(event);
}
