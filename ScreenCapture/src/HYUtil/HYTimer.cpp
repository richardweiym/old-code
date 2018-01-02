#include "HYUtilBase.h"
#include "HYTimer.h"


#pragma  warning(disable: 4035)

#pragma  comment(lib, "Winmm.lib")

IHYTimer::IHYTimer()
{
 m_nStopTimer = m_nStartTimer = 0;
}

//////////  CHYTickCountTimer /////////
CHYTickCountTimer::CHYTickCountTimer()
{
 Start();
}

unsigned __int64  CHYTickCountTimer::Start()
{
 return  (m_nStopTimer = m_nStartTimer = GetTickCount());
}

unsigned __int64 CHYTickCountTimer::Stop()
{
 return (m_nStopTimer = GetTickCount());
}

double CHYTickCountTimer::GetInterval() const
{
 return (double)(unsigned long)(m_nStopTimer - m_nStartTimer);
}
//////////  CHYTickCountTimer /////////

 


//////////  CHYSysTimer /////////
CHYSysTimer::CHYSysTimer()
{
 Start();
}

unsigned __int64  CHYSysTimer::Start()
{
 return  (m_nStopTimer = m_nStartTimer = timeGetTime());
}

unsigned __int64 CHYSysTimer::Stop()
{
 return (m_nStopTimer = timeGetTime());
}

double CHYSysTimer::GetInterval() const
{
 return (double)(unsigned long)(m_nStopTimer - m_nStartTimer);
}
//////////  CHYSysTimer /////////

 

//////////  CHYQueryPerformanceTimer /////////
unsigned __int64 CHYQueryPerformanceTimer::m_nFrequency = 0;
CHYQueryPerformanceTimer::CHYQueryPerformanceTimer()
{
 if(m_nFrequency == 0)
  QueryPerformanceFrequency((LARGE_INTEGER*)&m_nFrequency);
 Start();
}

unsigned __int64  CHYQueryPerformanceTimer::Start()
{
 QueryPerformanceCounter((LARGE_INTEGER*)&m_nStartTimer);
 m_nStopTimer = m_nStartTimer;
 return  m_nStartTimer;
}

unsigned __int64 CHYQueryPerformanceTimer::Stop()
{
 QueryPerformanceCounter((LARGE_INTEGER*)&m_nStopTimer);
 return m_nStopTimer;
}

double CHYQueryPerformanceTimer::GetInterval() const
{
 return 1000 * ((double)(long)(m_nStopTimer - m_nStartTimer)) / (double)(long)m_nFrequency;
}
//////////  CHYQueryPerformanceTimer /////////

 

 

//////////  CHYRDTSCTimer /////////
inline unsigned __int64 GetCycleCount()
{
 __asm _emit 0x0F
 __asm _emit 0x31
}

int CHYRDTSCTimer::s_nFrequency = 0;
CHYRDTSCTimer::CHYRDTSCTimer()
{
 if(s_nFrequency == 0)
 {
  unsigned __int64 nStart = GetCycleCount();
  Sleep(50);
  unsigned __int64   nEnd = GetCycleCount();
  s_nFrequency = (int)( nEnd - nStart ) / 50;
 }

 Start();
}

unsigned __int64  CHYRDTSCTimer::Start()
{
 return (m_nStopTimer = m_nStartTimer = GetCycleCount());
}

unsigned __int64 CHYRDTSCTimer::Stop()
{
 return (m_nStopTimer = GetCycleCount());
}

double CHYRDTSCTimer::GetInterval() const
{
 return  ((double)(long)(m_nStopTimer - m_nStartTimer)) / s_nFrequency;
}
//////////  CHYRDTSCTimer /////////


CHYTimeCostCalculator::CHYTimeCostCalculator(LPCTSTR lpszTitle)
{
	lstrcpyn(m_szTitle, lpszTitle, sizeof(m_szTitle) / sizeof(TCHAR) - 1);
	m_timer.Start();
}


CHYTimeCostCalculator::~CHYTimeCostCalculator()
{
	TCHAR szTrace[256] = {0};
	m_timer.Stop();
	
	_sntprintf(szTrace, sizeof(szTrace) / sizeof(TCHAR) - 1, _T("%s cost time: %fms\n"), m_szTitle, m_timer.GetInterval());
	
	OutputDebugString(szTrace);
}