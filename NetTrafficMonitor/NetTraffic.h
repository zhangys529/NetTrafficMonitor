#pragma once
class CNetTraffic
{
private:
	CNetTraffic();
	~CNetTraffic();
	enum ETrafficType			// 流量类型
	{
		AllTraffic = 388,		// 总的流量
		IncomingTraffic = 264,	// 输入流量
		OutgoingTraffic = 506,	// 输出流量
		Bandwidth = 520			// 带宽
	};
public:
	BOOL RefreshInterfacesTraffic();						// 刷新所有接口数据
	int GetNetworkInterfacesCount();						// 得到接口的数目
	CString GetNetworkInterfaceName(int iIndex);			// 得到index索引接口的名字
	DWORD GetBandwidth(int iIndex);							// 得到index索引接口的带宽
	DWORD GetIncrementalIncomingTraffic(int iIndex);		// 得到index索引接口的增量输入流量
	DWORD GetIncrementalOutgoingTraffic(int iIndex);		// 得到index索引接口的增量输出流量
	DWORD GetTotalIncomingTraffic(int iIndex);				// 得到index索引接口的总输入流量
	DWORD GetTotalOutgoingTraffic(int iIndex);				// 得到index索引接口的总输出流量

	static CNetTraffic* create_instance();
private:
	CStringList m_listInterfaces;
	CList<DWORD, DWORD&> m_listBandwidths;					// 带宽
	CList<DWORD, DWORD&> m_listIncrementalIncomingTraffic;	// 增量输入流量
	CList<DWORD, DWORD&> m_listIncrementalOutgoingTraffic;	// 增量输出流量
	CList<DWORD, DWORD&> m_listTotalIncomingTraffics;		// 总输入流量
	CList<DWORD, DWORD&> m_listTotalOutgoingTraffics;		// 总输出流量

	static CNetTraffic* m_pInstance;
};

