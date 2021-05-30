#include "stdafx.h"
#include "House.h"
#include "TextureMgr.h"
#include "Stage.h"
#include "SceneMgr.h"
#include "ScrollMgr.h"
#include "Graphic_Device.h"

CHouse::CHouse()
{
	m_pDevice = nullptr;
}


CHouse::~CHouse()
{
	Release();
}

HRESULT CHouse::Initialize()
{
	m_pDevice = CGraphic_Device::Get_Instance();
	dynamic_cast<CStage*>(CSceneMgr::Get_Instance()->Get_Scene())->Get_Stage().m_iMaxPop += 4;
	dynamic_cast<CStage*>(CSceneMgr::Get_Instance()->Get_Scene())->Get_Stage().m_iPop += 4;
	dynamic_cast<CStage*>(CSceneMgr::Get_Instance()->Get_Scene())->Get_Stage().m_iTex += 1;
	m_tInfo.vPos.y -= 5.f;
	return S_OK;
}

int CHouse::Update()
{
	return OBJ::LIVE;
}

void CHouse::Late_Update()
{
}

void CHouse::Render()
{
	float fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	if (m_tInfo.vPos.x < (-fScrollX + (WINCX + 100.f)) && m_tInfo.vPos.x  >(-fScrollX - 100.f))
	{
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
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + fScrollX, m_tInfo.vPos.y + pTexInfo->tImageInfo.Height + 20.f, 0.f);

		m_tInfo.matWorld = matScale * matTrans;

		m_pDevice->Get_Sprite()->SetTransform(&m_tInfo.matWorld);
		m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(120, 255, 255, 255));

	}
}

void CHouse::Release()
{
}
