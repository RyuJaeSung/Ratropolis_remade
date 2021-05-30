#include "stdafx.h"
#include "KeyMgr.h"

CKeyMgr* CKeyMgr::m_pInstance = nullptr;
CKeyMgr::CKeyMgr()
	:m_dwDown(0), m_dwKey(0), m_dwUp(0)
{
}


CKeyMgr::~CKeyMgr()
{
}

void CKeyMgr::Update()
{
	m_dwKey = 0;
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		m_dwKey |= KEY_LBUTTON;
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		m_dwKey |= KEY_SPACE;
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		m_dwKey |= KEY_RIGHT;
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		m_dwKey |= KEY_LEFT;
	if(GetAsyncKeyState('1') & 0x8000)
		m_dwKey |= KEY_ONE;
	if (GetAsyncKeyState('2') & 0x8000)
		m_dwKey |= KEY_TWO;
}

bool CKeyMgr::KeyUp(const DWORD & dwKey)
{
	if (m_dwKey & dwKey)
	{
		m_dwUp |= dwKey;
		return false;
	}
	else if (m_dwUp & dwKey)
	{
		m_dwUp ^= dwKey;
		return true;
	}
	return false;
}

bool CKeyMgr::KeyDown(const DWORD & dwKey)
{
	if ((m_dwKey & dwKey) && !(m_dwDown & dwKey))
	{
		m_dwDown |= dwKey;
		return true;
	}
	else if (!(m_dwKey & dwKey) && (m_dwDown & dwKey))
	{
		m_dwDown ^= dwKey;
		return false;
	}
	return false;
}

bool CKeyMgr::KeyPressing(const DWORD & dwKey)
{
	if (m_dwKey & dwKey)
		return true;
	return false;
}
