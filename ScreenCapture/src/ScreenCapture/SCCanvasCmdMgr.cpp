#include "SCCanvasCmdMgr.h"
#include "SCCanvasWindow.h"
#include "SCInstanceAPI.h"
#include "SCCanvasData.h"

using namespace std;

CSCGraphAddCmd::CSCGraphAddCmd(CRefPtr<CSCGraphObject> pGraph)
:m_pGraph(pGraph)
{
	_ASSERTE(!pGraph.IsNull());
}

VOID CSCGraphAddCmd::ExecuateCmd()
{
	CRefPtr<CSCCanvasWindow> pCanvas = GetCanvasWindow();
	pCanvas->OnAddGraph(m_pGraph);
}

VOID CSCGraphAddCmd::UnexecuteCmd()
{
	CRefPtr<CSCCanvasWindow> pCanvas = GetCanvasWindow();
	pCanvas->OnEraseGraph(m_pGraph);
}


CSCGraphEraseCmd::CSCGraphEraseCmd(CRefPtr<CSCGraphObject> pGraph)
:m_pGraph(pGraph)
{
	_ASSERTE(!pGraph.IsNull());
}


VOID CSCGraphEraseCmd::ExecuateCmd()
{
	CRefPtr<CSCCanvasWindow> pCanvas = GetCanvasWindow();
	pCanvas->OnEraseGraph(m_pGraph);
}

VOID CSCGraphEraseCmd::UnexecuteCmd()
{
	CRefPtr<CSCCanvasWindow> pCanvas = GetCanvasWindow();
	pCanvas->OnAddGraph(m_pGraph);
}


CSCSelectRectChangeCmd::CSCSelectRectChangeCmd(CRect& rtOld, CRect& rtNew)
:m_rtNew(rtNew), m_rtOld(rtOld)
{

}

VOID CSCSelectRectChangeCmd::ExecuateCmd()
{
	CRefPtr<CSCCanvasData> pData = GetCanvasData();
	pData->SetSelectRect(m_rtNew);

	CRefPtr<CSCCanvasWindow> pCanvas = GetCanvasWindow();
	pCanvas->RedrawCanvas(NULL);
}

VOID CSCSelectRectChangeCmd::UnexecuteCmd()
{
	CRefPtr<CSCCanvasData> pData = GetCanvasData();
	pData->SetSelectRect(m_rtOld);

	CRefPtr<CSCCanvasWindow> pCanvas = GetCanvasWindow();
	pCanvas->RedrawCanvas(NULL);
}



BOOL CSCCanvasCmdMgr::IsRedoStackEmtpy()
{
	return m_arRedoCmd.empty() == true;
}

BOOL CSCCanvasCmdMgr::IsUndoStackEmpty()
{
	return m_arUndoCmd.empty() == true;
}

BOOL CSCCanvasCmdMgr::Undo()
{
	if(!IsUndoStackEmpty())
	{
		CRefPtr<CSCCmdBase> pCmd = m_arUndoCmd.back();
		_ASSERTE(!pCmd.IsNull());
		m_arRedoCmd.push_back(pCmd);
		m_arUndoCmd.pop_back();

		pCmd->UnexecuteCmd();
		return TRUE;
	}

	return FALSE;
}

BOOL CSCCanvasCmdMgr::Redo()
{
	if(!IsRedoStackEmtpy())
	{
		CRefPtr<CSCCmdBase> pCmd = m_arRedoCmd.back();
		_ASSERTE(!pCmd.IsNull());
		m_arUndoCmd.push_back(pCmd);
		m_arRedoCmd.pop_back();

		pCmd->ExecuateCmd();

		return TRUE;
	}

	return FALSE;	
}

VOID CSCCanvasCmdMgr::OnAddGraphDone(CRefPtr<CSCGraphObject> pGraph)
{
	CRefPtr<CSCCmdBase> pCmd = new CSCGraphAddCmd(pGraph);
	m_arUndoCmd.push_back(pCmd);
}

VOID CSCCanvasCmdMgr::OnEraseGraphDone(CRefPtr<CSCGraphObject> pGraph)
{
	CRefPtr<CSCCmdBase> pCmd = new CSCGraphEraseCmd(pGraph);
	m_arUndoCmd.push_back(pCmd);
}

VOID CSCCanvasCmdMgr::OnSelectRectChangeDone(CRect& rtOld, CRect& rtNew)
{
	CRefPtr<CSCCmdBase> pCmd = new CSCSelectRectChangeCmd(rtOld, rtNew);
	m_arUndoCmd.push_back(pCmd);
}