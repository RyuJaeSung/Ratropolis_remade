#include "stdafx.h"
#include "Monastery.h"
#include "TextureMgr.h"
#include "Stage.h"
#include "TimeMgr.h"
#include "SceneMgr.h"
#include "KeyMgr.h"
#include "CradMgr.h"
#include "ScrollMgr.h"
#include "Graphic_Device.h"



CMonastery::CMonastery()
	:m_fTime(0.f), m_fCoolTime(0.f), m_fMaxTime(10.f), m_bCreate_Bell(false), m_bDelete_Crad(false), m_ButtonCkeck(false)
{
	m_pDevice = nullptr;
	ZeroMemory(&m_rBell, sizeof(m_rBell));
}


CMonastery::~CMonastery()
{
}

HRESULT CMonastery::Initialize()
{
	m_pDevice = CGraphic_Device::Get_Instance();
	m_tInfo.vPos.y -= 95.f;
	return S_OK;
}

int CMonastery::Update()
{

	Time();
	Ckeck();
	
	return OBJ::LIVE;
}

void CMonastery::Late_Update()
{
	if (m_bCreate_Bell)
	{
		if (CKeyMgr::Get_Instance()->KeyDown(KEY_LBUTTON))
		{
			POINT ps = {};
			GetCursorPos(&ps);
			ScreenToClient(g_hWnd, &ps);

			if (PtInRect(&m_rBell, ps))
			{
				m_fTime = 0.f;
				m_bCreate_Bell = false;
				m_bDelete_Crad = true;
			}
		}
	}
}

void CMonastery::Render()
{

	float fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	if (m_tInfo.vPos.x < (-fScrollX + (WINCX + 100.f)) && m_tInfo.vPos.x  >(-fScrollX - 100.f))
	{
		Render_Building();
		if (m_bCreate_Bell)
			Render_Bell();
		if (m_bDelete_Crad)
			Render_DeleteCrad();
	}
}

void CMonastery::Release()
{
}

void CMonastery::Render_Building()
{
	float fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(m_wstrObjectKey, m_wstrStateKey, m_dwCount);
	if (!pTexInfo)
		return;
	D3DXMATRIX	matScale, matTrans;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + fScrollX, m_tInfo.vPos.y, 0.f);

	m_tInfo.matWorld = matScale * matTrans;

	m_pDevice->Get_Sprite()->SetTransform(&m_tInfo.matWorld);
	m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	//¹°°¡
	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, -m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + fScrollX, m_tInfo.vPos.y + fCenterY + 60.f, 0.f);

	m_tInfo.matWorld = matScale * matTrans;

	m_pDevice->Get_Sprite()->SetTransform(&m_tInfo.matWorld);
	m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(120, 255, 255, 255));
	
}

void CMonastery::Render_Bell()
{
	float fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(__T("InGame"), __T("UI"), 15);
	if (!pTexInfo)
		return;

	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
	float fRectX = m_tInfo.vPos.x + fScrollX;
	float fRectY = m_tInfo.vPos.y - fCenterY - 20.f;

	D3DXMATRIX	matScale, matTrans;

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, fRectX, fRectY, 0.f);

	SetRect(&m_rBell, int(fRectX - fCenterX), int(fRectY - fCenterY), int(fRectX + fCenterX), int(fRectY + fCenterY));

	m_tInfo.matWorld = matScale * matTrans;

	m_pDevice->Get_Sprite()->SetTransform(&m_tInfo.matWorld);
	m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CMonastery::Render_DeleteCrad()
{
	Render_Window();
	Render_Button();
	Render_Crad();
}

void CMonastery::Render_Window()
{
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(__T("InGame"), __T("Quest"), 0);
	if (!pTexInfo)
		return;

	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMATRIX matSacle, matTrans, matWorld;
	D3DXMatrixScaling(&matSacle, 0.8f, 0.8f, 0.f);
	D3DXMatrixTranslation(&matTrans, float(WINCX >> 1), float(WINCY >> 1), 0.f);

	matWorld = matSacle * matTrans;
	m_pDevice->Get_Sprite()->SetTransform(&matWorld);
	m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CMonastery::Render_Button()
{
	const TEXINFO* pTexInfo = nullptr;
	if (m_ButtonCkeck)
		pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(__T("InGame"), __T("Quest"), 2);
	else
		pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(__T("InGame"), __T("Quest"), 1);

	if (!pTexInfo)
		return;

	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	SetRect(&m_rButtonRect, int(1090.f - 40), int(70.f - 30), int(1090.f + 10), int(70.f + 15));

	D3DXMATRIX matSacle, matTrans, matWorld;
	D3DXMatrixScaling(&matSacle, 0.5f, 0.5f, 0.f);
	D3DXMatrixTranslation(&matTrans, 1090.f, 70.f, 0.f);

	matWorld = matSacle * matTrans;
	m_pDevice->Get_Sprite()->SetTransform(&matWorld);
	m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CMonastery::Render_Crad()
{
	float fX = 0.f;
	float fY = 0.f;
	TCHAR szBuffer[32] = {};
	int iCount = 0;
	for (auto& iter : CCradMgr::Get_Instance()->Get_ListBag())
	{
		switch (iter->m_dwType)
		{
		case 0:
			swprintf_s(szBuffer, __T("Economy"));
			break;
		case 1:
			swprintf_s(szBuffer, __T("Military"));
			break;
		case 2:
			swprintf_s(szBuffer, __T("Building"));
			break;
		case 3:
			swprintf_s(szBuffer, __T("Technology"));
			break;
		}

		const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(__T("Crad"), szBuffer, iter->m_iImageNumber);
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
		if (iCount == 8)
		{
			fX = 0.f;
			fY += 150.f;
			iCount = 0;
		}

		fX = 100.f * iCount;

		D3DXMATRIX matSacle, matTrans, matWorld;
		D3DXMatrixScaling(&matSacle, 0.3f, 0.3f, 0.3f);
		D3DXMatrixTranslation(&matTrans, 295.f, 190.f, 0.f);
		
		matWorld = matSacle * matTrans;
		m_pDevice->Get_Sprite()->SetTransform(&matWorld);
		m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
		++iCount;
	}
}

void CMonastery::Delete_Crad()
{
}

void CMonastery::Time()
{
	m_fTime += CTimeMgr::Get_Instance()->Get_DeltaTime();
	if (1.f >= m_fTime)
	{
		++m_fCoolTime;
		m_fTime;
	}

	if (m_fTime >= m_fMaxTime)
		m_bCreate_Bell = true;
	else
		m_bCreate_Bell = false;
}

void CMonastery::Ckeck()
{
	POINT pMouse = {};
	GetCursorPos(&pMouse);
	ScreenToClient(g_hWnd, &pMouse);

	if (PtInRect(&m_rButtonRect, pMouse))
	{
		m_ButtonCkeck = true;
		if (CKeyMgr::Get_Instance()->KeyDown(KEY_LBUTTON))
			m_bDelete_Crad = false;
	}
	else
		m_ButtonCkeck = false;
}
