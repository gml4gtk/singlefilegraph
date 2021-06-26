#include "SFGGraphLayoutUIController.h"

#include <appbase/CMainWindow.h>

#include <qvgeio/CGraphBase.h>
#include <qvge/CNodeEditorScene.h>
#include <qvge/CNode.h>
#include <qvge/CEdge.h>

#include <QMenuBar>
#include <QMenu>
#include <QProcess>
#include <QTemporaryFile>
#include <QCoreApplication>
#include <QDir>
#include <QMessageBox>
#include <QProgressDialog>
#include <QDebug>
#include <QApplication>

extern "C" {
#include "sfg.h"
#include "sfg.c"
}

SFGGraphLayoutUIController::SFGGraphLayoutUIController(CMainWindow *parent, CEditorScene *scene) :
    m_parent(parent), m_scene(scene),
	m_defaultEngine("sfg")
{
    // add layout menu
    QMenu *layoutMenu = new QMenu(tr("&Sfg"));
    m_parent->menuBar()->insertMenu(m_parent->getWindowMenuAction(), layoutMenu);

    layoutMenu->addAction(tr("Hierarchical Layout"), this, SLOT(doSFGLayout()));
}

//
bool SFGGraphLayoutUIController::doLayout(const QString &engine, CEditorScene &scene)
{
	int nxsize = 0;
	int nysize = 0;
	int nid = 0;
	int i = 0;
	int status = 0;
	int status2 = 0;
	int nnodes = 0;
	int nedges = 0;
	int efid = 0;
	int etid = 0;
	int xpos = 0;
	int ypos = 0;
	double dxpos = 0.0;
	double dypos = 0.0;

	if (0) qDebug() << "running layout with sfg.c version " << sfg_version();

	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

	status = sfg_init ();
	if (status < 0) { }

	auto nodes = scene.getItems<CNode>();

	QMap<CNode*, int> nodeMap;

	// total number of nodes
	if (0) qDebug() << nodes.size() << " number of nodes";

	nid = 0;

	for (auto & node : nodes)
	{
		// give nodes uniq number starting with 1
		// this can be different from internal numbering
		// and it is only used with sfg.c
		nid++;
		// get x size
		nxsize = node->getSize().width();
		if (nxsize <= 0) {
			nxsize = 1;
		}
		// get y size
		nysize = node->getSize().height();
		if (nysize <= 0) {
			nysize = 1;
		}
		if (0) qDebug() << "node id=" << node->getId() << "  nid=" << nid << " size=(" << nxsize << "," << nysize << ")";

		// add node to layoter
		status = sfg_addnode (nid ,nxsize, nysize);
		if (status < 0) { }

		// number of nodes added
		nnodes++;

		nodeMap[node] = nid;
	}

	// if there are no nodes, then no edges
	if (nnodes == 0) {
		status = sfg_deinit ();
		if (status < 0) { }

		Q_EMIT layoutFinished();
		return true;
	}

	auto edges = scene.getItems<CEdge>();

	// add the edges
	for (CEdge* edge: edges)
	{
		// edge with no edge label
		efid = nodeMap[edge->firstNode()];
		etid = nodeMap[edge->lastNode()];

		if ((efid > 0) && (etid > 0)) {
			nedges++;
			status = sfg_addedge (nedges, efid, etid, 0, 0);
			if (status < 0) { }
		} else {
			std::printf("could not add edge %d to %d\n", efid, etid);
		}
	}

	// set some graph spacing
	status = sfg_xspacing(5);
	if (status < 0) { }

	status = sfg_yspacing(15);
	if (status < 0) { }

	// now run the complete layered sugiyama layout
	status = sfg_layout ();

	if (status < 0) {
		// something went wrong
		std:: printf ("error status %d from sfg.c sfg_layout()\n", status);
		Q_EMIT layoutFinished();
		return true;
	}

	if (0) std::printf("%d levels\n", sfg_nlevels());

	// sfg -> qvge
	for (auto it = nodeMap.begin(); it != nodeMap.end(); ++it)
	{
		CNode* node = it.key();
		int nid = it.value();

		xpos = sfg_nodexpos (nid);
		ypos = sfg_nodeypos (nid);

		if (xpos < 0 || ypos < 0) {
			// should not happen
			std::printf ("for node %d unexpected position (%d,%d) skipped\n", nid, xpos, ypos);
		} else {

			dxpos = (1 * xpos);
			dypos = (1 * ypos);

			if (1) {
				node->setPos (dxpos, dypos);
			} else {
				node->setPos (dypos, dxpos);
			}
		}

	}

	status = sfg_deinit ();
	if (status < 0) { }

	// finalize
	scene.setSceneRect(scene.itemsBoundingRect());

	scene.addUndoState();

	QApplication::restoreOverrideCursor();

	Q_EMIT layoutFinished();

	return true;
}

// end.
