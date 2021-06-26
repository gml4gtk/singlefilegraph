#pragma once

#include <QObject>

class CMainWindow;
class CEditorScene;


class SFGGraphLayoutUIController : public QObject
{
	Q_OBJECT

public:
	explicit SFGGraphLayoutUIController(CMainWindow *parent, CEditorScene *scene);

Q_SIGNALS:
	void loadFinished();
	void layoutFinished();


private Q_SLOTS:
	void doSFGLayout()		{ doLayout("sfg", *m_scene); }

private:
	bool doLayout(const QString &engine, CEditorScene &scene);

	CMainWindow *m_parent = nullptr;
	CEditorScene *m_scene = nullptr;

	QString m_defaultEngine;
};

// end.
