#include "stdafx.h"
#include "GameMenu.h"
#include "TextureMgr.h"
#include "SceneMgr.h"
#include "KeyMgr.h"
#include "Graphic_Device.h"

CGameMenu::CGameMenu()
	:m_Execution(false)
{
	m_pDevice = nullptr;
}


CGameMenu::~CGameMenu()
{
	Release();
}

void CGameMenu::Initialize()
{
	m_pDevice = CGraphic_Device::Get_Instance();
	m_vButtonPos[0] = { float(WINCX >> 1), (WINCY >> 1) + 50.f, 0.f };
	m_vButtonPos[1] = { float(WINCX >> 1), (WINCY >> 1) , 0.f };
	m_vButtonPos[2] = { float(WINCX >> 1), (WINCY >> 1) - 50.f, 0.f };
	//버튼 크기 118 x 43 /2 59 x 21
	for (int i = 0; i < BUTTONNUMBER; ++i)
	{
		SetRect(&m_rButton[i], int(m_vButtonPos[i].x - 59), int(m_vButtonPos[i].y - 21), int(m_vButtonPos[i].x + 59), int(m_vButtonPos[i].y + 21));
		m_ButtonCkeck[i] = false;
	}
}

void CGameMenu::Update()
{
	POINT pMouse = {};
	GetCursorPos(&pMouse);
	ScreenToClient(g_hWnd, &pMouse);

	if (m_Execution)
	{	
		if (PtInRect(&m_rButton[0], pMouse))
		{
			m_ButtonCkeck[0] = true;
			if (CKeyMgr::Get_Instance()->KeyDown(KEY_LBUTTON))
				CSceneMgr::Get_Instance()->Scene_Change(SCENE::SCENE_QUIT);
			return;
		}
		else
			m_ButtonCkeck[0] = false;

		if (PtInRect(&m_rButton[1], pMouse))
		{
			m_ButtonCkeck[1] = true;
			if (CKeyMgr::Get_Instance()->KeyDown(KEY_LBUTTON))
			{
				Game_Progress = false;
				m_Execution = false;
			}
			return;
		}
		else
			m_ButtonCkeck[1] = false;

		if (PtInRect(&m_rButton[2], pMouse))
		{
			m_ButtonCkeck[2] = true;
			if (CKeyMgr::Get_Instance()->KeyDown(KEY_LBUTTON))
			{
				CSceneMgr::Get_Instance()->Scene_Change(SCENE::SCENE_MENU);
				Release();
			}
			return;
		}
		else
			m_ButtonCkeck[2] = false;
	}
}

void CGameMenu::Render()
{
	if (m_Execution)
	{
		Render_Window();
		Render_Button();
	}
}

void CGameMenu::Release()
{
	Game_Progress = false;
	m_Execution = false;
}

void CGameMenu::Render_Window()
{
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(__T("InGame"), __T("Menu"), 0);
	if (!pTexInfo)
		return;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 0.5f, 0.5f, 0.f);
	D3DXMatrixTranslation(&matTrans, float(WINCX >> 1), float(WINCY >> 1), 0.f);
	matWorld = matScale * matTrans;

	m_pDevice->Get_Sprite()->SetTransform(&matWorld);
	m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255,255,255,255));

}

void CGameMenu::Render_Button()
{
	TCHAR szBuffer[32] = {};
	const TEXINFO* pTexInfo = nullptr;
	for (int i = 0; i < BUTTONNUMBER; ++i)
	{
		if(m_ButtonCkeck[i])
			pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(__T("InGame"), __T("Menu"), 2);
		else
			pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(__T("InGame"), __T("Menu"), 1);

		if (!pTexInfo)
			return;
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		D3DXMATRIX matScale, matTrans, matWorld;
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, m_vButtonPos[i].x, m_vButtonPos[i].y, 0.f);
		matWorld = matScale * matTrans;

		m_pDevice->Get_Sprite()->SetTransform(&matWorld);
		m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

		if (i == 0)
		{
			swprintf_s(szBuffer, __T("종료"));
			D3DXMatrixScaling(&matScale, 0.5f, 0.5f, 0.f);
			D3DXMatrixTranslation(&matTrans, m_vButtonPos[i].x - 20.f, m_vButtonPos[i].y -10.f, 0.f);
			matWorld = matScale * matTrans;
			m_pDevice->Get_Sprite()->SetTransform(&matWorld);
			m_pDevice->Get_Font()->DrawTextW(m_pDevice->Get_Sprite(), szBuffer, lstrlen(szBuffer), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		
		}
		else if (i == 1)
		{
			swprintf_s(szBuffer, __T("닫기"));
			D3DXMatrixScaling(&matScale, 0.5f, 0.5f, 0.f);
			D3DXMatrixTranslation(&matTrans, m_vButtonPos[i].x - 20.f, m_vButtonPos[i].y - 10.f, 0.f);
			matWorld = matScale * matTrans;
			m_pDevice->Get_Sprite()->SetTransform(&matWorld);
			m_pDevice->Get_Font()->DrawTextW(m_pDevice->Get_Sprite(), szBuffer, lstrlen(szBuffer), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		}

		else if (i == 2)
		{
			swprintf_s(szBuffer, __T("메인메뉴"));
			D3DXMatrixScaling(&matScale, 0.5f, 0.5f, 0.f);
			D3DXMatrixTranslation(&matTrans, m_vButtonPos[i].x - 40.f, m_vButtonPos[i].y - 10.f, 0.f);
			matWorld = matScale * matTrans;
			m_pDevice->Get_Sprite()->SetTransform(&matWorld);
			m_pDevice->Get_Font()->DrawTextW(m_pDevice->Get_Sprite(), szBuffer, lstrlen(szBuffer), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}
}
