#ifndef __WIN_EVENT_H__
#define __WIN_EVENT_H__

class IWinEventSink
{
public:
	virtual VOID OnForegroundChanged(HWND hWnd) = 0;
	virtual VOID OnFocusChanged(HWND hWnd) = 0;
	virtual VOID OnWinShow(HWND hWnd) = 0;
	virtual VOID OnWinHide(HWND hWnd) = 0;
	virtual VOID OnWinCreate(HWND hWnd) = 0;
	virtual VOID OnWinDestroy(HWND hWnd) = 0;
};


class CWinEvent
{
public:
	enum EVENT_INDEX
	{
		EVENT_INDEX_FOCUS = 0,
		EVENT_INDEX_FOREGROUND,
		EVENT_INDEX_CREATE,
		EVENT_INDEX_DESTROY,
		EVENT_INDEX_SHOW,
		EVENT_INDEX_HIDE,

		EVENT_INDEX_COUNT
	};

	static CWinEvent& GetInstance();

	BOOL InstallEventHook(IWinEventSink* pSink);
	VOID UninstallEventHook();

	IWinEventSink* GetEventSink() const;

private:
	CWinEvent();
	~CWinEvent();

	CWinEvent(const CWinEvent&);
	CWinEvent& operator = (const CWinEvent&);

private:
	HWINEVENTHOOK m_hEvent[EVENT_INDEX_COUNT];
	static CWinEvent s_winEvent;
	IWinEventSink* m_pEventSink;
};


#endif //__WIN_EVENT_H__