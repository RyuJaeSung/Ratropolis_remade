#include "stdafx.h"
#include "AddGold.h"
#include "Graphic_Device.h"
#include "TimeMgr.h"
#include "TextureMgr.h"

CAddGold::CAddGold()
	:CreateGold(true), m_fDeleteY(0.f), m_fTime(0.f), m_fDeleteTime(0.f), m_iRColor(255)
{
	m_pDevice = nullptr;
	ZeroMemory(m_AddGold, sizeof(m_AddGold));
}


CAddGold::~CAddGold()
{
}

HRESULT CAddGold::Initialize()
{
	float fSize = 0.5f;
	m_pDevice = CGraphic_Device::Get_Instance();
	m_tInfo.vSize = { fSize, fSize , 0.f };
	POINT ps = {};
	GetCursorPos(&ps);
	ClientToScreen(g_hWnd, &ps);


	m_tInfo.vPos = D3DXVECTOR3(float(ps.x), float(ps.y) - 70.f, 0.f);


	return S_OK;
}

int CAddGold::Update()
{
	if (m_fDeleteTime >= 0.3f)
		return OBJ::DEAD;

	m_fTime += CTimeMgr::Get_Instance()->Get_DeltaTime();

	
	m_fDeleteY -= 1.f;
	m_iRColor -= 1;
	
	if (m_fTime >= 0.5f)
	{
		m_fDeleteTime += 0.2f;
		m_fTime = 0.f;
	}

	return OBJ::LIVE;
}

void CAddGold::Late_Update()
{
	if (CreateGold)
	{
		int Gold = (int)m_tInfo.m_fAngle;
		swprintf_s(m_AddGold, __T("+ %d"), Gold);
		CreateGold = false;
	}
}

void CAddGold::Render()
{
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(__T("InGame"), __T("UI"), 7);
	if (!pTexInfo)
		return;
	
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMATRIX matScale, matTrans;

	D3DXMatrixScaling(&matScale, 0.2f, 0.2f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - 10.f, m_tInfo.vPos.y + m_fDeleteY, 0.f);

	m_tInfo.matWorld = matScale * matTrans;

	m_pDevice->Get_Sprite()->SetTransform(&m_tInfo.matWorld);
	m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iRColor, 255, 255, 255));

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + 10.f, m_tInfo.vPos.y + m_fDeleteY, 0.f);

	m_tInfo.matWorld = matScale * matTrans;

	m_pDevice->Get_Sprite()->SetTransform(&m_tInfo.matWorld);
	m_pDevice->Get_Font()->DrawTextW(m_pDevice->Get_Sprite(), m_AddGold, lstrlen(m_AddGold), nullptr, 0, D3DCOLOR_ARGB(m_iRColor, 255, 255, 0));
}

void CAddGold::Release()
{
}
