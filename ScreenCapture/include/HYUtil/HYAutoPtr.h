#pragma  once 

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

	VOID ResetRefCount()
	{
		m_nRefCount = 0;
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
