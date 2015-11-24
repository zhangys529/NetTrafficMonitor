#pragma once
#include "stdafx.h"
#include <winperf.h>

#define DEFAULT_BUFFER_SIZE 40960L
PERF_DATA_BLOCK* GetDataBlock(LPCSTR lpValueName)
{
	DWORD ret;	// RegQueryValueEx返回的值
	DWORD type;	// 从RegQueryValueEx返回的值:本例中忽略该变量
	unsigned char* data = new unsigned char[DEFAULT_BUFFER_SIZE];	// 为性能数据缓冲
	DWORD size = DEFAULT_BUFFER_SIZE;	// 缓冲的尺寸
	// 从网络对象(索引是510)查询性能数据
	while (ERROR_SUCCESS != (ret = RegQueryValueEx(HKEY_PERFORMANCE_DATA, lpValueName, 0, &type, data, &size)))
	{
		if (ERROR_MORE_DATA == ret)
		{
			size += DEFAULT_BUFFER_SIZE;	// 缓冲尺寸太小，增加内存分配
			delete[] data;
			data = new unsigned char[size];
		}
		else	// 未定义的错误
		{
			return NULL;
		}
	}
	return (PERF_DATA_BLOCK*)data;
}
PERF_OBJECT_TYPE* FirstObject(PERF_DATA_BLOCK* dataBlock)
{
	return (PERF_OBJECT_TYPE*)((BYTE*)dataBlock + dataBlock->HeaderLength);
}
PERF_OBJECT_TYPE* NextObject(PERF_OBJECT_TYPE* act)
{
	return (PERF_OBJECT_TYPE*)((BYTE*)act + act->TotalByteLength);
}
PERF_COUNTER_DEFINITION* FirstCounter(PERF_OBJECT_TYPE* perfObject)
{
	return (PERF_COUNTER_DEFINITION*)((BYTE*)perfObject + perfObject->HeaderLength);
}
PERF_COUNTER_DEFINITION* NextCounter(PERF_COUNTER_DEFINITION* perfCounter)
{
	return (PERF_COUNTER_DEFINITION*)((BYTE*)perfCounter + perfCounter->ByteLength);
}
PERF_COUNTER_BLOCK* GetCounterBlock(PERF_INSTANCE_DEFINITION* pInstance)
{
	return (PERF_COUNTER_BLOCK*)((BYTE*)pInstance + pInstance->ByteLength);
}
PERF_INSTANCE_DEFINITION* FirstInstance(PERF_OBJECT_TYPE* pObject)
{
	return (PERF_INSTANCE_DEFINITION*)((BYTE*)pObject + pObject->DefinitionLength);
}
PERF_INSTANCE_DEFINITION* NextInstance(PERF_INSTANCE_DEFINITION* pInstance)
{
	PERF_COUNTER_BLOCK* pCtrBlk = GetCounterBlock(pInstance);
	return (PERF_INSTANCE_DEFINITION*)((BYTE*)pInstance + pInstance->ByteLength + pCtrBlk->ByteLength);
}
char* WideToMulti(wchar_t* source, char* dest, int size)
{
	WideCharToMultiByte(CP_ACP, 0, source, -1, dest, size, 0, 0);
	return dest;
}
DWORD GetCounterOffset(PERF_OBJECT_TYPE* pObjectType, int type)
{
	PERF_COUNTER_DEFINITION* pCounter = FirstCounter(pObjectType);	// 找到第一个计数器
	for (int b = 0; b < pObjectType->NumCounters; ++b)
	{
		if (pCounter->CounterNameTitleIndex == type)
		{
			return pCounter->CounterOffset;
		}
		pCounter = NextCounter(pCounter);
	}
	return ULONG_MAX;
}
