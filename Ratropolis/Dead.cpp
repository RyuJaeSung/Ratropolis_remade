#include "stdafx.h"
#include "Dead.h"
#include "TimeMgr.h"
#include "TextureMgr.h"
#include "ScrollMgr.h"
#include "Graphic_Device.h"

CDead::CDead()
	:m_iColor(255), m_fTime(0.f), m_fPosY(0.f)
{
	m_pDevice = nullptr;
}


CDead::~CDead()
{
	Release();
}

HRESULT CDead::Initialize()
{
	m_pDevice = CGraphic_Device::Get_Instance();
	m_tFrame.fMaxFrame = 1.f;
	m_tFrame.fStartFrame = 0.f;
	return S_OK;
}

int CDead::Update()
{
	m_tFrame.fStartFrame += CTimeMgr::Get_Instance()->Get_DeltaTime();

		m_fTime = 0.f;
		m_fPosY -= 5.f;

		if (m_iColor <= 0)
			m_iColor = 0;
		m_iColor -= 5;

	if (m_tFrame.fStartFrame >= m_tFrame.fMaxFrame)
		return OBJ::DEAD;

	return OBJ::LIVE;
}

void CDead::Late_Update()
{
}

void CDead::Render()
{
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(__T("InGame"), __T("UI"), 18);
	if (!pTexInfo)
		return;
	D3DXMATRIX	matScale, matTrans;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
	float fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + fScrollX, m_tInfo.vPos.y + m_fPosY, 0.f);

	m_tInfo.matWorld = matScale * matTrans;

	m_pDevice->Get_Sprite()->SetTransform(&m_tInfo.matWorld);
	m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(m_iColor, 255, 255, 255));

}

void CDead::Release()
{
}
