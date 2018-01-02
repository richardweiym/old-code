#pragma  once 

class IHYTimer
{
public:
 IHYTimer();

 virtual unsigned __int64 Start() = 0;  //start count
 virtual unsigned __int64 Stop() = 0; //stop count

 virtual double GetInterval() const = 0; //return value in milliseconds

protected:
 unsigned __int64 m_nStartTimer;
 unsigned __int64 m_nStopTimer;
 
};

 

// call GetTickCount() API
class CHYTickCountTimer: public IHYTimer
{
public:
 CHYTickCountTimer();

 virtual unsigned __int64 Start();
 virtual unsigned __int64 Stop();
 virtual double GetInterval() const;
};

 

// call timeGetTimer() API
class CHYSysTimer: public IHYTimer
{
public:
 CHYSysTimer();

 virtual unsigned __int64 Start();
 virtual unsigned __int64 Stop();
 virtual double GetInterval() const;
};


//call QueryPerformanceCount() API
class CHYQueryPerformanceTimer: public IHYTimer
{
public:
 CHYQueryPerformanceTimer();

 virtual unsigned __int64 Start();
 virtual unsigned __int64 Stop();
 virtual double GetInterval() const;

protected:
 static unsigned __int64 m_nFrequency;
};


// get by CPU main frequency
class CHYRDTSCTimer: public IHYTimer
{
public:
 CHYRDTSCTimer();

 virtual unsigned __int64 Start();
 virtual unsigned __int64 Stop();
 virtual double GetInterval() const;

protected:
 static int s_nFrequency;
};

class CHYTimeCostCalculator
{
public:
	CHYTimeCostCalculator(LPCTSTR lpszTitle);
	~CHYTimeCostCalculator();

private:
	TCHAR m_szTitle[128];
	CHYQueryPerformanceTimer m_timer;
};