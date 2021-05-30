#include "stdafx.h"
#include "Farm.h"
#include "TextureMgr.h"
#include "Stage.h"
#include "TimeMgr.h"
#include "SceneMgr.h"
#include "ScrollMgr.h"
#include "KeyMgr.h"
#include "Graphic_Device.h"

CFarm::CFarm()
	:m_fTime(0.f), m_fMaxTime(10.f), m_fCoolTime(0.f), m_AddPop(false)
{
	m_pDevice = nullptr;
}


CFarm::~CFarm()
{
}

HRESULT CFarm::Initialize()
{
	m_pDevice = CGraphic_Device::Get_Instance();
	m_tInfo.vPos.y -= 55.f;
	return S_OK;
}

int CFarm::Update()
{
	m_fTime += CTimeMgr::Get_Instance()->Get_DeltaTime();

	if (m_fTime >= 1.f)
	{
		++m_fCoolTime;
		m_fTime;
	}

	if (m_fTime >= m_fMaxTime)
		m_AddPop = true;
	else
		m_AddPop = false;

	return OBJ::LIVE;
}

void CFarm::Late_Update()
{
	if (m_AddPop)
	{
		POINT ps = {};
		GetCursorPos(&ps);
		ScreenToClient(g_hWnd, &ps);


		if (PtInRect(&m_rAddPoP, ps) && CKeyMgr::Get_Instance()->KeyDown(KEY_LBUTTON))
		{
			dynamic_cast<CStage*>(CSceneMgr::Get_Instance()->Get_Scene())->Get_Stage().m_iPop += 2;
			dynamic_cast<CStage*>(CSceneMgr::Get_Instance()->Get_Scene())->Get_Stage().m_iMaxPop += 2;

			m_fTime = 0.f;
			m_AddPop = false;
		}
	}
}

void CFarm::Render()
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
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + fScrollX, m_tInfo.vPos.y + fCenterY + 80.f, 0.f);

		m_tInfo.matWorld = matScale * matTrans;

		m_pDevice->Get_Sprite()->SetTransform(&m_tInfo.matWorld);
		m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(120, 255, 255, 255));

		if (m_AddPop)
		{
			pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(__T("InGame"), __T("UI"), 16);
			float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
			float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
			float fRectX = m_tInfo.vPos.x + fScrollX;
			float fRectY = m_tInfo.vPos.y - fCenterY - 30.f;

			D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
			D3DXMatrixTranslation(&matTrans, fRectX, fRectY, 0.f);

			SetRect(&m_rAddPoP, int(fRectX - fCenterX), int(fRectY - fCenterY), int(fRectX + fCenterX), int(fRectY + fCenterY));

			m_tInfo.matWorld = matScale * matTrans;

			m_pDevice->Get_Sprite()->SetTransform(&m_tInfo.matWorld);
			m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

		}
	}
}

void CFarm::Release()
{
}
