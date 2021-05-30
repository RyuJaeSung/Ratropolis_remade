#include "stdafx.h"
#include "MyMenu.h"
#include "TextureMgr.h"
#include "Collision.h"
#include "KeyMgr.h"
#include "SceneMgr.h"
#include "Graphic_Device.h"

CMyMenu::CMyMenu()
{
	m_pDevice = nullptr;
	ZeroMemory(&m_tStart, sizeof(m_tStart));
	ZeroMemory(&m_tEnd, sizeof(m_tEnd));
}


CMyMenu::~CMyMenu()
{
	Release();
}


HRESULT CMyMenu::Initialize()
{	
	m_pDevice = CGraphic_Device::Get_Instance();
	swprintf_s(m_tStart.m_szBuffer, __T("Game Start"));
	swprintf_s(m_tEnd.m_szBuffer, __T("Game End"));

	SetRect(&m_tStart.m_rSize, 100, 470, 350, 500);
	SetRect(&m_tEnd.m_rSize, 100, 520, 310, 550);
	m_tStart.m_bCheck = false;
	m_tEnd.m_bCheck = false;
	return S_OK;
}

void CMyMenu::Update()
{
	POINT pMouse = {};
	GetCursorPos(&pMouse);
	ScreenToClient(g_hWnd, &pMouse);

	if (CCollision::PosRectCkeck(pMouse, m_tStart.m_rSize))
		m_tStart.m_bCheck = true;
	else
		m_tStart.m_bCheck = false;

	if (CCollision::PosRectCkeck(pMouse, m_tEnd.m_rSize))
		m_tEnd.m_bCheck = true;
	else
		m_tEnd.m_bCheck = false;

	Key_Check();
}

void CMyMenu::Late_Update()
{
}

void CMyMenu::Render()
{
	for (int i = 0; i < 1; ++i)
	{
		const TEXINFO*	pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(__T("BackGround"), __T("Menu"), i);
		if (!pTexInfo)
		{
			ERR_MSG(__T("MainMenu Load Failed"));
			return;
		}
		D3DXMATRIX	matScale, matTrans, matWorld;
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, float(WINCX >> 1), float(WINCY >> 1), 0.f);

		matWorld = matScale * matTrans;
		m_pDevice->Get_Sprite()->SetTransform(&matWorld);
		m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_RGBA(255, 255, 255, 255));
		Render_Font();
	}
}

void CMyMenu::Render_Font()
{
	D3DXMATRIX matTrans;
	D3DXMatrixTranslation(&matTrans, 100.f, 500.f, 0.f);

	m_pDevice->Get_Sprite()->SetTransform(&matTrans);
	if (m_tStart.m_bCheck)
		m_pDevice->Get_Font()->DrawTextW(m_pDevice->Get_Sprite(), m_tStart.m_szBuffer, lstrlen(m_tStart.m_szBuffer), nullptr, 0, D3DCOLOR_ARGB(255, 255, 165, 0));
	else
		m_pDevice->Get_Font()->DrawTextW(m_pDevice->Get_Sprite(), m_tStart.m_szBuffer, lstrlen(m_tStart.m_szBuffer), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

	D3DXMatrixTranslation(&matTrans, 100.f, 550.f, 0.f);
	m_pDevice->Get_Sprite()->SetTransform(&matTrans);
	if (m_tEnd.m_bCheck)
		m_pDevice->Get_Font()->DrawTextW(m_pDevice->Get_Sprite(), m_tEnd.m_szBuffer, lstrlen(m_tEnd.m_szBuffer), nullptr, 0, D3DCOLOR_ARGB(255, 255, 165, 0));
	else
		m_pDevice->Get_Font()->DrawTextW(m_pDevice->Get_Sprite(), m_tEnd.m_szBuffer, lstrlen(m_tEnd.m_szBuffer), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CMyMenu::Key_Check()
{
	if (m_tStart.m_bCheck &&CKeyMgr::Get_Instance()->KeyDown(KEY_LBUTTON))
	{
		CSceneMgr::Get_Instance()->Scene_Change(SCENE::SCENE_CHARSELECT);
		return;
	}

	if (m_tEnd.m_bCheck && CKeyMgr::Get_Instance()->KeyDown(KEY_LBUTTON))
	{
		CSceneMgr::Get_Instance()->Scene_Change(SCENE::SCENE_QUIT);
		return;
	}
}

void CMyMenu::Release()
{
}
