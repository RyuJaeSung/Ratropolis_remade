#include "stdafx.h"
#include "Wave.h"
#include "ScrollMgr.h"
#include "TextureMgr.h"
#include "Graphic_Device.h"

CWave::CWave()
{
	m_pDevice = nullptr;
}


CWave::~CWave()
{
}

HRESULT CWave::Initialize()
{	//min900  max6700
	m_pDevice = CGraphic_Device::Get_Instance();
	m_tInfo.vPos = {6700.f, 650.f, 0.f};
	m_tInfo.vSize = { 1.5f, 0.6f, 0.f };
	m_tInfo.m_fSpeed = 3.f;
	return S_OK;
}

int CWave::Update()
{
	if (m_tInfo.vPos.x <= 1000.f)
		return OBJ::DEAD;

	return OBJ::LIVE;
}

void CWave::Late_Update()
{
	m_tInfo.vPos.x -= m_tInfo.m_fSpeed;
}

void CWave::Render()
{
	float fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	if (m_tInfo.vPos.x < (-fScrollX + (WINCX + 250.f)) && m_tInfo.vPos.x  >(-fScrollX - 250.f))
	{
		const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(__T("InGame"), __T("Water"), 1);
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

	}
}

void CWave::Release()
{
}
