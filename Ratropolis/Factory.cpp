#include "stdafx.h"
#include "Factory.h"
#include "TextureMgr.h"
#include "Stage.h"
#include "SceneMgr.h"
#include "ScrollMgr.h"
#include "Graphic_Device.h"


CFactory::CFactory()
{
	m_pDevice = nullptr;
}


CFactory::~CFactory()
{
	Release();
}

HRESULT CFactory::Initialize()
{
	m_pDevice = CGraphic_Device::Get_Instance();
	dynamic_cast<CStage*>(CSceneMgr::Get_Instance()->Get_Scene())->Get_Stage().m_iReSetTime += 6;
	m_tInfo.vPos.y -= 100.f;
	return S_OK;
}

int CFactory::Update()
{
	return OBJ::LIVE;
}

void CFactory::Late_Update()
{
}

void CFactory::Render()
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
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + fScrollX, m_tInfo.vPos.y + fCenterY + 60.f, 0.f);

		m_tInfo.matWorld = matScale * matTrans;

		m_pDevice->Get_Sprite()->SetTransform(&m_tInfo.matWorld);
		m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(120, 255, 255, 255));
	}
}

void CFactory::Release()
{
}
