#include "stdafx.h"
#include "FrameMgr.h"
#include "TimeMgr.h"
#include "Graphic_Device.h"
#include "ScrollMgr.h"

CFrameMgr* CFrameMgr::m_pInstance = nullptr;
CFrameMgr::CFrameMgr()
	:m_fDeltaTime(0.f), m_fSecondPerFrame(0.f), m_fFPSTime(0.f), m_iFPS(0)
{
	ZeroMemory(&m_CpuTick, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_BeginTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_EndTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_szFPS, sizeof(m_szFPS));
}


CFrameMgr::~CFrameMgr()
{
}

void CFrameMgr::Initialize(float fFPS)
{
	m_fSecondPerFrame = 1.f / fFPS;
	QueryPerformanceCounter(&m_EndTime);
	QueryPerformanceFrequency(&m_CpuTick);
	QueryPerformanceCounter(&m_BeginTime);
}

bool CFrameMgr::LockFrame()
{
	QueryPerformanceCounter(&m_EndTime);
	QueryPerformanceFrequency(&m_CpuTick);
	m_fDeltaTime += float(m_EndTime.QuadPart - m_BeginTime.QuadPart) / m_CpuTick.QuadPart;
	m_BeginTime = m_EndTime;
	if (m_fSecondPerFrame <= m_fDeltaTime)
	{
		m_fDeltaTime = 0.f;
		return true;
	}

	return false;
}

void CFrameMgr::RenderFrame()
{
	POINT ps = {};
	GetCursorPos(&ps);
	ScreenToClient(g_hWnd, &ps);
	int x = int(CScrollMgr::Get_Instance()->Get_ScrollX());
	++m_iFPS;
	m_fFPSTime += CTimeMgr::Get_Instance()->Get_DeltaTime();
	swprintf_s(m_szFPS, L"FPS : %d", x);
	//if (1.f <= m_ffpstime)
	//{
	//	//swprintf_s(m_szfps, l"fps : %d", m_ifps);
	//	m_ifps = 0;
	//	m_ffpstime = 0.f;
	//}
	//d3dxmatrix mattrans;
	//d3dxmatrixtranslation(&mattrans, 500.f, 100.f, 0.f);
	//cgraphic_device::get_instance()->get_sprite()->settransform(&mattrans);
	//cgraphic_device::get_instance()->get_font()->drawtextw(cgraphic_device::get_instance()->get_sprite(), m_szfps, lstrlen(m_szfps), nullptr, 0, d3dcolor_argb(255, 255, 0, 0));
}
