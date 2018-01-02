#pragma  once 

#include "SCCanvasGraph.h"
#include <list>

class CSCCmdBase: public CRefCountBase
{
public:
	virtual ~CSCCmdBase() { }

	virtual VOID ExecuateCmd() { }
	virtual VOID UnexecuteCmd() { }
};


class CSCGraphAddCmd: public CSCCmdBase
{
public:
	CSCGraphAddCmd(CRefPtr<CSCGraphObject> pGraph);

	virtual VOID ExecuateCmd();
	virtual VOID UnexecuteCmd();

protected:
	CRefPtr<CSCGraphObject> m_pGraph;
};

class CSCGraphEraseCmd: public CSCCmdBase
{
public:
	CSCGraphEraseCmd(CRefPtr<CSCGraphObject> pGraph);

	virtual VOID ExecuateCmd();
	virtual VOID UnexecuteCmd();

protected:
	CRefPtr<CSCGraphObject> m_pGraph;
};


class CSCSelectRectChangeCmd: public CSCCmdBase
{
public:
	CSCSelectRectChangeCmd(CRect& rtOld, CRect& rtNew);

	virtual VOID ExecuateCmd();
	virtual VOID UnexecuteCmd();

protected:
	CRect m_rtOld;
	CRect m_rtNew;
};

class CSCCanvasCmdMgr: public CRefCountBase
{
public:
	BOOL IsUndoStackEmpty();
	BOOL IsRedoStackEmtpy();

	BOOL Undo();
	BOOL Redo();

	VOID OnAddGraphDone(CRefPtr<CSCGraphObject> pGraph);
	VOID OnEraseGraphDone(CRefPtr<CSCGraphObject> pGraph);

	VOID OnSelectRectChangeDone(CRect& rtOld, CRect& rtNew); 

protected:
	std::list<CRefPtr<CSCCmdBase> >	m_arUndoCmd;
	std::list<CRefPtr<CSCCmdBase> >	m_arRedoCmd;
};