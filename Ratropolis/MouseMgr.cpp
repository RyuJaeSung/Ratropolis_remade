#include "stdafx.h"
#include "MouseMgr.h"
#include "Graphic_Device.h"
#include "TextureMgr.h"

CMouseMgr* CMouseMgr::m_pInstance = nullptr;
CMouseMgr::CMouseMgr()
{
	m_pDevice = nullptr;
}


CMouseMgr::~CMouseMgr()
{
	Release();
}

void CMouseMgr::Initialize()
{
	m_pDevice = CGraphic_Device::Get_Instance();
	ShowCursor(false);
	m_iState = 0;
}

void CMouseMgr::Render()
{
	POINT pMouse = {};
	GetCursorPos(&pMouse);
	ScreenToClient(g_hWnd, &pMouse);
	
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(__T("UI"), __T("Mouse"), m_iState);
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 0.4f, 0.4f, 0.f);
	D3DXMatrixTranslation(&matTrans, float(pMouse.x) + 20.f, float(pMouse.y) + 30.f, 0.f);
	matWorld = matScale * matTrans;

	m_pDevice->Get_Sprite()->SetTransform(&matWorld);
	m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CMouseMgr::Release()
{
}
