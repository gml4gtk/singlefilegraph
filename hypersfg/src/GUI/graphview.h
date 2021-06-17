#ifndef GRAPHVIEW_H_
#define GRAPHVIEW_H_

#include <QGraphicsView>

class QGraphicsScene;
class EdgeItem;
class VertexItem;
class LegendItem;
class BoundingRectItem;


class GraphView : public QGraphicsView
{
	Q_OBJECT

public:
	GraphView(QWidget *parent = 0);
	virtual ~GraphView();

	void setDimensions(const QPoint dimensions);
	QPoint dimensions() const {return _dimensions;}
	void setDirectedGraph(bool state);
	bool directedGraph() const {return _directed;}
	void setLegend(int size);
	int legend() const {return _legend;}

	VertexItem* addVertex();
	EdgeItem* addEdge(VertexItem *src, VertexItem *dst);
	LegendItem* addLegend();

	int vertex_size() const {return _vertexes.size();}
	int edge_size() const {return _edges.size();}
	VertexItem* vertex(int id) const {return _vertexes.at(id);}
	EdgeItem* edge(int id) const {return _edges.at(id);}

	void clear();
	void clearLegend();

	void setVertexLabelAttr(const QString &name);
	void setEdgeLabelAttr(const QString &name);
	void setVertexColor(const QColor &color);
	void setEdgeColor(const QColor &color);
	void setVertexSize(int size);
	void setEdgeSize(int size);
	void setVertexFontSize(int size);
	void setEdgeFontSize(int size);
	void setEdgeZValue(int value);
	void setLegendSize(int size);

	void scale(qreal scaleFactor);
	qreal zoom() {return transform().m11();}

signals:
	void zoomed(qreal z);

private:
	void wheelEvent(QWheelEvent *event);

	QGraphicsScene *_scene;

	BoundingRectItem *_boundingRect;
	QVector<VertexItem*> _vertexes;
	QVector<EdgeItem*> _edges;
	QVector<LegendItem*> _legendItems;

	QPoint _dimensions;
	bool _directed;
	int _legend;
};

#endif /* GRAPHSCENE_H_ */
