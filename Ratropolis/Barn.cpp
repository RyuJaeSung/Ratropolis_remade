#include "stdafx.h"
#include "Barn.h"
#include "TextureMgr.h"
#include "Stage.h"
#include "SceneMgr.h"
#include "CradMgr.h"
#include "AddGold.h"
#include "ObjMgr.h"
#include "ScrollMgr.h"
#include "Graphic_Device.h"


CBarn::CBarn()
{
	m_pDevice = nullptr;
}


CBarn::~CBarn()
{
}

HRESULT CBarn::Initialize()
{
	m_pDevice = CGraphic_Device::Get_Instance();
	m_tInfo.vPos.y -= 40.f;

	return S_OK;
}

int CBarn::Update()
{
	if (CCradMgr::Get_Instance()->Get_Ckeck())
	{
		m_iCrad_Number = CCradMgr::Get_Instance()->Get_ListHands().size();
		for (int i = 0; i < m_iCrad_Number; ++i)
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::UI);
			CSoundMgr::Get_Instance()->PlaySound(L"Gold.wav", CSoundMgr::UI);
			CObjMgr::Get_Instance()->Add_Object(OBJ::GOLD, CAbstractFactroy<CAddGold>::Create(8.f));
			dynamic_cast<CStage*>(CSceneMgr::Get_Instance()->Get_Scene())->Get_Stage().m_iGold += 8;
		}
		CCradMgr::Get_Instance()->Set_Ckeck(false);
	}

	return OBJ::LIVE;
}

void CBarn::Late_Update()
{
}

void CBarn::Render()
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
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + fScrollX, m_tInfo.vPos.y + fCenterY + 90.f, 0.f);

		m_tInfo.matWorld = matScale * matTrans;

		m_pDevice->Get_Sprite()->SetTransform(&m_tInfo.matWorld);
		m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(120, 255, 255, 255));
	}
}

void CBarn::Release()
{
}
