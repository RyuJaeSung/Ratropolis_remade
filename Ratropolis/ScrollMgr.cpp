#include "stdafx.h"
#include "KeyMgr.h"
#include "ScrollMgr.h"

CScrollMgr* CScrollMgr::m_pInstance = nullptr;
CScrollMgr::CScrollMgr()
	:m_fScrollX(0.f), m_fSpeed(10.f)
{
	GetCursorPos(&m_tMousePos);
	ScreenToClient(g_hWnd, &m_tMousePos);
}


CScrollMgr::~CScrollMgr()
{
}

void CScrollMgr::Map_Move()
{
	POINT pMouse = {};
	GetCursorPos(&pMouse);
	ScreenToClient(g_hWnd, &pMouse);
	D3DXVECTOR3 vMouse = { float(pMouse.x), float(pMouse.y), 0.f };

	
	if (vMouse.x < 100 && m_fScrollX <= -1000.f && vMouse.y <= SCROLLY)
	{
		m_fScrollX += m_fSpeed;
		m_tMousePos.x += (int)m_fSpeed;
	}
	else if (vMouse.x > WINCX - 100 && m_fScrollX >= -5200.f && vMouse.y <= SCROLLY)
	{
		m_fScrollX -= m_fSpeed;
		m_tMousePos.x -= (int)m_fSpeed;
	}
	else if (CKeyMgr::Get_Instance()->KeyPressing(KEY_RIGHT) && m_fScrollX >= -5200.f)
	{
		m_fScrollX -= m_fSpeed;
		m_tMousePos.x -= (int)m_fSpeed;
	}
	else if (CKeyMgr::Get_Instance()->KeyPressing(KEY_LEFT) && m_fScrollX <= -1000.f)
	{
		m_fScrollX += m_fSpeed;
		m_tMousePos.x += (int)m_fSpeed;
	}
}