#include "stdafx.h"
#include "Clock.h"
#include "TextureMgr.h"
#include "ScrollMgr.h"
#include "Graphic_Device.h"

CClock::CClock()
{
	m_tInfo.m_fAngle = 0.f;
	m_pDevice = nullptr;
}


CClock::~CClock()
{
	Release();
}

HRESULT CClock::Initialize()
{
	m_pDevice = CGraphic_Device::Get_Instance();
	m_tInfo.m_fAngle = 90.f;
	m_tInfo.m_fSpeed = 5.f;
	return S_OK;
}

int CClock::Update()
{
	m_tInfo.m_fAngle += m_tInfo.m_fSpeed;
	return 0;
}

void CClock::Late_Update()
{
}

void CClock::Render()
{
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(__T("InGame"), __T("UI"), 11);
	if (!pTexInfo)
	{
		ERR_MSG(__T("Clock Image Load Failed"));
		return;
	}
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
	float fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();

	D3DXMATRIX matScale, matTrans, matRetZ, matWorld;
	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + fScrollX, m_tInfo.vPos.y, 0.f);
	D3DXMatrixRotationZ(&matRetZ, D3DXToRadian(m_tInfo.m_fAngle));

	matWorld = matScale * matRetZ * matTrans;

	m_pDevice->Get_Sprite()->SetTransform(&matWorld);
	m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CClock::Release()
{
}
