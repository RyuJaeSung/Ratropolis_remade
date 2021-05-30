#include "stdafx.h"
#include "Crad.h"
#include "TextureMgr.h"
#include "ScrollMgr.h"
#include "Graphic_Device.h"

CCrad::CCrad()
{
	m_pDevice = nullptr;
}


CCrad::~CCrad()
{
	Release();
}

HRESULT CCrad::Initialize()
{
	m_pDevice = CGraphic_Device::Get_Instance();
	return S_OK;
}

int CCrad::Update()
{
	return OBJ::LIVE;
}

void CCrad::Late_Update()
{
}

void CCrad::Render()
{
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(m_wstrObjectKey, m_wstrStateKey, m_dwCount);
	if (!pTexInfo)
		return;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMATRIX matScale, matRotZ, matTrans;
	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(m_tInfo.m_fAngle));
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	m_tInfo.matWorld = matScale *  matTrans;
	m_pDevice->Get_Sprite()->SetTransform(&m_tInfo.matWorld);
	m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CCrad::Release()
{
}
