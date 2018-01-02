#pragma  once 

#include "../SmsServer/SmsSocketCommunicate.h"

class CMemBuffer
{
public:
	CMemBuffer(UINT nSize, BOOL bFree = TRUE)
	{
		_ASSERT(nSize > 0);
		m_pBuffer = new BYTE[nSize];
		m_nSize = nSize;
		m_bFree = bFree;
		SetMemZero();
	}

	VOID SetMemZero()
	{
		if(m_pBuffer != NULL)
		{
			memset(m_pBuffer, 0, m_nSize);
		}
	}
	
	~CMemBuffer()
	{
		if(m_pBuffer != NULL)
		{
			delete []m_pBuffer;
			m_pBuffer = NULL;
		}
	}

	BYTE* m_pBuffer;
	BOOL m_bFree;
	UINT m_nSize;
};

class CMemPool
{

public:
	CMemPool(UINT nSize, INT nCount)
	{
		::InitializeCriticalSection(&m_cs);
		m_nBufferSize = nSize;
		m_nCount = nCount;
		AllocateAll();
	}
	~CMemPool()
	{
		FreeAll();
		::DeleteCriticalSection(&m_cs); 
	}
	VOID FreeAll()
	{
		CSmsLock lock(m_cs);
		{
			CMemBuffer* pBuf = NULL;
			for(int i=0; i<m_arBuffer.GetSize(); i++)
			{
				pBuf = m_arBuffer[i];
				if(pBuf != NULL)
				{
					delete pBuf;
				}
			}
			m_arBuffer.RemoveAll();
		}
	}

	VOID SetAllMemFree()
	{
		CSmsLock lock(m_cs);
		{
			CMemBuffer* pBuf = NULL;
			for(int i=0; i<m_arBuffer.GetSize(); i++)
			{
				pBuf = m_arBuffer[i];
				if(pBuf != NULL )
				{
					pBuf->m_bFree = TRUE;
				}
			}
		}
	}

	BYTE* GetMemory(UINT& nSize)
	{
		CSmsLock lock(m_cs);
		{
			CMemBuffer* pBuf = NULL;
			for(int i=0; i<m_arBuffer.GetSize(); i++)
			{
				pBuf = m_arBuffer[i];
				if(pBuf != NULL && pBuf->m_bFree)
				{
#ifdef DEBUG
				OutputDebugString("get memory from mempool\n");
#endif
					pBuf->m_bFree = FALSE;
					nSize = m_nBufferSize;
					return pBuf->m_pBuffer;
				}
			}
			return NULL;
		}
		
	}
	VOID FreeMemory(BYTE* pAddress)
	{
		CSmsLock lock(m_cs);
		{
			CMemBuffer* pBuf = NULL;
			for(int i=0; i<m_arBuffer.GetSize(); i++)
			{
#ifdef DEBUG
				OutputDebugString("free memory to mempool\n");
#endif
				pBuf = m_arBuffer[i];
				if(pBuf != NULL && (pBuf->m_pBuffer == pAddress))
				{
					pBuf->m_bFree = TRUE;
					pBuf->SetMemZero();
					break;
				}
			}
		}
	}
private:
	VOID AllocateAll()
	{
		CSmsLock lock(m_cs);
		{
			CMemBuffer* pBuffer = NULL;
			for(int i=0; i<(INT)m_nCount; i++)
			{
				pBuffer = new CMemBuffer(m_nBufferSize, TRUE);
				m_arBuffer.Add((CMemBuffer* &)pBuffer);
			}
		}
	}

private:
	CRITICAL_SECTION m_cs;
	UINT m_nBufferSize;
	UINT m_nCount;
	CSimpleArray<CMemBuffer*> m_arBuffer;

};