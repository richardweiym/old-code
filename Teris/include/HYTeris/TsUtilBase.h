#pragma  once 

#include <vector>

/*Period parameters */
#define CMATH_N 624
#define CMATH_M 397
#define CMATH_MATRIX_A 0x9908b0df   /*constant vector a */
#define CMATH_UPPER_MASK 0x80000000 /*most significant w-r bits */
#define CMATH_LOWER_MASK 0x7fffffff /*least significant r bits */

/*Tempering parameters */
#define CMATH_TEMPERING_MASK_B 0x9d2c5680
#define CMATH_TEMPERING_MASK_C 0xefc60000
#define CMATH_TEMPERING_SHIFT_U(y)  (y >> 11)
#define CMATH_TEMPERING_SHIFT_S(y)  (y << 7)
#define CMATH_TEMPERING_SHIFT_T(y)  (y << 15)
#define CMATH_TEMPERING_SHIFT_L(y)  (y >> 18)

class CRandom
{
   //DATA
   unsigned int                    rseed;
   unsigned long mt [CMATH_N ];    /*the array for the state vector */
   int mti;    /*mti==N+1 means mt [N ] is not initialized **/


   //FUNCTIONS
public:
   CRandom();
   ~CRandom(){ NULL; }

   unsigned int     Random(unsigned int n );
   void             SetRandomSeed(unsigned int n);
   unsigned int     GetRandomSeed(void);
   void             Randomize(void);
};


class CPrimeSearch
{
public:
   CPrimeSearch(unsigned int elements);
   CPrimeSearch(unsigned int nStart, unsigned int nEnd);
   int GetNext(bool restart=false);
   bool Done();
   void Restart();
   int GetCount();
   
private:
	std::vector<int> m_arBuffer;
	int m_nCurrentIndex;
};

#ifdef _DEBUG
#define TS_RUN_ONCE_CHECK {static int s_nRunOnce(0); _ASSERTE(++s_nRunOnce == 1);}
#else
#define TS_RUN_ONCE_CHECK \
//  {static int s_nRunOnce(0); \
// if(++s_nRunOnce != 1) ::MessageBox(NULL, _T("Singleton Run Twice"), _T("TS_RUN_ONCE_CHECK"), MB_OK);}
#endif // _DEBUG

template<typename T>
class CSingleton: private T
{
public:
	static T& Instance()
	{
		static CSingleton<T> t;
		return t;
	}

private:
	CSingleton()
	{
#ifdef _LIB
	_ASSERTE(!_T("Don not use it in Lib, it will procuce multi instance for each module!"));
#endif
	}
	CSingleton(const CSingleton&);
};


//T's base class for CRefPtr<T>
class CRefCountBase
{
public:
	CRefCountBase()
	{
		m_nRefCount = 0;
	}

	int GetRefCount() const
	{
		return m_nRefCount;
	}

	int AddRefCount()
	{
		return ++m_nRefCount;
	}

	int SubRefCount()
	{
		return --m_nRefCount;
	}

private:
	int	m_nRefCount;
};

template<typename T>
class CRefPtr
{
public:
	T* operator->() const
	{
		return m_pRawObj;
	}

	T& operator()() const
	{
		return *m_pRawObj;
	}

	T& operator*() const
	{
		return *m_pRawObj;
	}

	T* GetPtr() const
	{
		return m_pRawObj;
	}

	bool IsNull() const
	{
		return m_pRawObj == NULL;
	}

	CRefPtr()
	{
		m_pRawObj = NULL;
	}

	CRefPtr(T* p)
	{
		m_pRawObj = p;
		if(p != NULL)
		{
			p->AddRefCount();
		}
	}

	CRefPtr(const CRefPtr& ref)
	{
		m_pRawObj = ref.m_pRawObj;
		if(m_pRawObj != NULL)
		{
			m_pRawObj->AddRefCount();
		}
	}

	~CRefPtr()
	{
		if(m_pRawObj != NULL && m_pRawObj->SubRefCount() == 0)
		{
			delete m_pRawObj;
		}
	}

	CRefPtr& operator = (const CRefPtr& ref)
	{
		if(this != &ref)
		{
			if(m_pRawObj != NULL
				&& m_pRawObj->SubRefCount() == 0)
			{
				delete m_pRawObj;
			}

			m_pRawObj = ref.m_pRawObj;

			if(m_pRawObj != NULL)
			{
				m_pRawObj->AddRefCount();
			}
		}

		return *this;
	}

	bool operator == (const CRefPtr& ref) const
	{
		return m_pRawObj == ref.m_pRawObj;
	}

private:
	T* m_pRawObj;
};

class CNoCopy
{
public:
	CNoCopy() { NULL; }

private:
	CNoCopy(const CNoCopy& c);
	CNoCopy& operator = (const CNoCopy& c);
};


