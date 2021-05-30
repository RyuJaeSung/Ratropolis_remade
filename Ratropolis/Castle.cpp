#include "stdafx.h"
#include "Castle.h"
#include "Stage.h"
#include "SceneMgr.h"
#include "Terrain.h"
#include "Graphic_Device.h"
#include "TextureMgr.h"
#include "ScrollMgr.h"


CCastle::CCastle()
{
	m_pDevice = nullptr;
}


CCastle::~CCastle()
{
}

HRESULT CCastle::Initialize()
{
	m_pDevice = CGraphic_Device::Get_Instance();
	dynamic_cast<CStage*>(CSceneMgr::Get_Instance()->Get_Scene())->Get_Terrain()->Change_Option(m_tInfo.vPos, 7);

	return S_OK;
}

int CCastle::Update()
{
	return 0;
}

void CCastle::Late_Update()
{
}

void CCastle::Render()
{
	float fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	if (m_tInfo.vPos.x < (-fScrollX + (WINCX + 500.f)) && m_tInfo.vPos.x  >(-fScrollX - 500.f))
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
		//¹Ý´ë

		D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, -m_tInfo.vSize.y, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + fScrollX, m_tInfo.vPos.y + fCenterY + 120.f, 0.f);

		m_tInfo.matWorld = matScale * matTrans;

		m_pDevice->Get_Sprite()->SetTransform(&m_tInfo.matWorld);
		m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(120, 255, 255, 255));

	}
}

void CCastle::Release()
{
}
