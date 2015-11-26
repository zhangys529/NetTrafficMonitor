#include "stdafx.h"
#include "NetTraffic.h"
#include "PerfInterface.h"

CNetTraffic* CNetTraffic::m_pInstance = NULL;

CNetTraffic::CNetTraffic()
{
	m_listInterfaces.RemoveAll();
	m_mapBandwidths.clear();
	m_mapIncrementalIncomingTraffic.clear();
	m_mapIncrementalOutgoingTraffic.clear();
	m_mapTotalIncomingTraffics.clear();
	m_mapTotalOutgoingTraffics.clear();
}


CNetTraffic::~CNetTraffic()
{
}

BOOL CNetTraffic::RefreshInterfacesTraffic()
{
	try
	{
		PERF_DATA_BLOCK* pDataBlock = GetDataBlock(_T("510"));			// 得到数据块
		PERF_OBJECT_TYPE* pObjectType = FirstObject(pDataBlock);	// 得到第一个对象
		DWORD a = 0;
		for (; a < pDataBlock->NumObjectTypes; ++a)
		{
			if (pObjectType->ObjectNameTitleIndex == 510)			// 判断是否是网络对象(索引是510)
			{
				break;
			}
			pObjectType = NextObject(pObjectType);					// 下一个对象
		}
		if (a == pDataBlock->NumObjectTypes)						// 没有网络对象
		{
			delete[] pDataBlock;
			return FALSE;
		}
		PERF_INSTANCE_DEFINITION* pInstance = FirstInstance(pObjectType);
		char szNameBuffer[255];
		for (LONG b = 0; b < pObjectType->NumInstances; ++b)
		{
			wchar_t* pSrcName = (wchar_t*)((BYTE*)pInstance + pInstance->NameOffset);
			CString strName = WideToMulti(pSrcName, szNameBuffer, sizeof(szNameBuffer));
			PERF_COUNTER_BLOCK* pCounterBlock = GetCounterBlock(pInstance);
			// 带宽
			DWORD dwBandwithOffset = GetCounterOffset(pObjectType, ETrafficType::Bandwidth);
			DWORD dwBandwith = *((DWORD*)((BYTE*)pCounterBlock + dwBandwithOffset));
			// 输入流量
			DWORD dwIncomingTrafficOffset = GetCounterOffset(pObjectType, ETrafficType::IncomingTraffic);
			DWORD dwIncomingTraffic = *((DWORD*)((BYTE*)pCounterBlock + dwIncomingTrafficOffset));
			// 输出流量
			DWORD dwOutgoingTrafficOffset = GetCounterOffset(pObjectType, ETrafficType::OutgoingTraffic);
			DWORD dwOutgoingTraffic = *((DWORD*)((BYTE*)pCounterBlock + dwOutgoingTrafficOffset));
			// 增量流量
			DWORD dwIncrementalIncomingTraffic = 0;
			DWORD dwIncrementalOutgoingTraffic = 0;

			// 通过接口名找缓存数据, 如果找不到则新增, 能找到则计算其增量数据
			POSITION pos = m_listInterfaces.Find(strName);
			if (NULL == pos)
			{
				m_listInterfaces.AddTail(strName);
			}
			else
			{
				dwIncrementalIncomingTraffic = dwIncomingTraffic - m_mapTotalIncomingTraffics[strName];
				dwIncrementalOutgoingTraffic = dwOutgoingTraffic - m_mapTotalOutgoingTraffics[strName];
			}
			m_mapBandwidths[strName] = dwBandwith;
			m_mapIncrementalIncomingTraffic[strName] = dwIncrementalIncomingTraffic;
			m_mapIncrementalOutgoingTraffic[strName] = dwIncrementalOutgoingTraffic;
			m_mapTotalIncomingTraffics[strName] = dwIncomingTraffic;
			m_mapTotalOutgoingTraffics[strName] = dwOutgoingTraffic;

			pInstance = NextInstance(pInstance);
		}
		delete[] pDataBlock;
		return TRUE;
	}
	catch (...)
	{
		return FALSE;
	}
}
int CNetTraffic::GetNetworkInterfacesCount()
{
	return m_listInterfaces.GetCount();
}
CString CNetTraffic::GetNetworkInterfaceName(int iIndex)
{
	POSITION pos = m_listInterfaces.FindIndex(iIndex);
	if (NULL == pos)
	{
		return _T("");
	}
	return m_listInterfaces.GetAt(pos);
}
DWORD CNetTraffic::GetBandwidth(int iIndex)
{
	CString strName = GetNetworkInterfaceName(iIndex);
	if ("" == strName)
	{
		return 0;
	}
	return m_mapBandwidths[strName];
}
DWORD CNetTraffic::GetIncrementalIncomingTraffic(int iIndex)
{
	CString strName = GetNetworkInterfaceName(iIndex);
	if ("" == strName)
	{
		return 0;
	}
	return m_mapIncrementalIncomingTraffic[strName];
}
DWORD CNetTraffic::GetIncrementalOutgoingTraffic(int iIndex)
{
	CString strName = GetNetworkInterfaceName(iIndex);
	if ("" == strName)
	{
		return 0;
	}
	return m_mapIncrementalOutgoingTraffic[strName];
}
DWORD CNetTraffic::GetTotalIncomingTraffic(int iIndex)
{
	CString strName = GetNetworkInterfaceName(iIndex);
	if ("" == strName)
	{
		return 0;
	}
	return m_mapTotalIncomingTraffics[strName];
}
DWORD CNetTraffic::GetTotalOutgoingTraffic(int iIndex)
{
	CString strName = GetNetworkInterfaceName(iIndex);
	if ("" == strName)
	{
		return 0;
	}
	return m_mapTotalOutgoingTraffics[strName];
}

CNetTraffic* CNetTraffic::create_instance()
{
	if (NULL == m_pInstance)
	{
		m_pInstance = new CNetTraffic();
	}
	return m_pInstance;
}