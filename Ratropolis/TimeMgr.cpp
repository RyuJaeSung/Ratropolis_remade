#include "stdafx.h"
#include "TimeMgr.h"

CTimeMgr* CTimeMgr::m_pInstance = nullptr;
CTimeMgr::CTimeMgr()
{
	ZeroMemory(&m_CpuTick, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_BeginTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_EndTime, sizeof(LARGE_INTEGER));

	QueryPerformanceFrequency(&m_CpuTick);
	QueryPerformanceCounter(&m_BeginTime);
	QueryPerformanceCounter(&m_EndTime);
}


CTimeMgr::~CTimeMgr()
{
}

void CTimeMgr::Update()
{
	QueryPerformanceFrequency(&m_CpuTick);
	QueryPerformanceCounter(&m_EndTime);
	m_fDeltaTime = float(m_EndTime.QuadPart - m_BeginTime.QuadPart) / m_CpuTick.QuadPart;

	m_BeginTime = m_EndTime;
}
