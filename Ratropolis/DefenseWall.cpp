#include "stdafx.h"
#include "SceneMgr.h"
#include "Stage.h"
#include "Terrain.h"
#include "DefenseWall.h"
#include "ScrollMgr.h"
#include "TextureMgr.h"
#include "CradMgr.h"
#include "Graphic_Device.h"


CDefenseWall::CDefenseWall()
	:m_iReinforce(0), m_iHp(0), m_iTileNum(0)
{
	m_pDevice = nullptr;
	ZeroMemory(&m_tFrame, sizeof(m_tFrame));
}


CDefenseWall::~CDefenseWall()
{
	Release();
}

void CDefenseWall::Wall_Hp()
{
	CRADINFO* tCradinfo;
	switch (m_iReinforce)
	{
	case 0://나무
		break;
	case 1://나무벽
		m_tInfo.vPos.y -= 35.f;
		tCradinfo = CCradMgr::Get_Instance()->Get_Crad(__T("WoodWall"));
		m_tUnitInfo = (*tCradinfo);
		m_iMaxHp = 200;
		break;
	case 2://돌벽
		m_tInfo.vPos.y -= 35.f;
		tCradinfo = CCradMgr::Get_Instance()->Get_Crad(__T("StoneWall"));
		m_tUnitInfo = (*tCradinfo);
		m_iMaxHp = 300;
		break;
	}
}

bool CDefenseWall::Get_SoiderSize()
{
	for (int i = 0; i < PLAYER::SOLDER_END; ++i)
	{
		if (!m_listSoilder[i].empty())
			return true;
	}
	return false;
}

HRESULT CDefenseWall::Initialize()
{
	m_pDevice = CGraphic_Device::Get_Instance();
	dynamic_cast<CStage*>(CSceneMgr::Get_Instance()->Get_Scene())->Get_Terrain()-> Change_Option(m_tInfo.vPos, 3);
	m_iTileNum = dynamic_cast<CStage*>(CSceneMgr::Get_Instance()->Get_Scene())->Get_Terrain()->Get_Tile(int(m_tInfo.vPos.x + CScrollMgr::Get_Instance()->Get_ScrollX()));
	m_iReinforce = m_dwCount;

	m_tFrame.fMaxFrame = 3.f;
	Wall_Hp();
	return S_OK;
}

int CDefenseWall::Update()
{

	for (int i = 0; i < PLAYER::SOLDER_END; ++i)
	{
		for (auto& iter = m_listSoilder[i].begin(); iter != m_listSoilder[i].end();)
		{
			if ((*iter)->Get_iHp() <= 0)
			{
				SAFE_DELETE((*iter));
				iter = m_listSoilder[i].erase(iter);
			}
			else
				++iter;
		}
	}
	return OBJ::LIVE;
}

void CDefenseWall::Late_Update()
{
	if (m_iReinforce != 0 && m_tUnitInfo.m_iHp <= 0)
	{
		m_tInfo.vPos.y += 35.f;
		m_iReinforce = 0;
	}
}

void CDefenseWall::Render()
{
	Render_Wall();
}

void CDefenseWall::Release()
{
}

void CDefenseWall::Render_Wall()
{
	float fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	if (m_tInfo.vPos.x < (-fScrollX + (WINCX + 100.f)) && m_tInfo.vPos.x  >(-fScrollX - 100.f))
	{
		const TEXINFO* pTexInfo = nullptr;
		switch (m_iReinforce)
		{
		case 0://나무
			pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(m_wstrObjectKey, m_wstrStateKey, 0);
			break;
		case 1://나무벽
			if ((m_iMaxHp >> 1) < m_tUnitInfo.m_iHp)
				pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(m_wstrObjectKey, m_wstrStateKey, 1);
			else if ((m_iMaxHp >> 1) >= m_tUnitInfo.m_iHp)
				pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(m_wstrObjectKey, m_wstrStateKey, 2);
			else
				pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(m_wstrObjectKey, m_wstrStateKey, 3);
			break;
		case 2://돌벽
			if ((m_iMaxHp >> 1) < m_tUnitInfo.m_iHp)
				pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(m_wstrObjectKey, m_wstrStateKey, 5);
			else if ((m_iMaxHp >> 1) >= m_tUnitInfo.m_iHp)
				pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(m_wstrObjectKey, m_wstrStateKey, 6);
			else
				pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(m_wstrObjectKey, m_wstrStateKey, 7);
			break;
		}
		if (!pTexInfo)
			return;

		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		D3DXMATRIX matScale, matTrains;

		D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
		D3DXMatrixTranslation(&matTrains, m_tInfo.vPos.x + fScrollX, m_tInfo.vPos.y, 0.f);

		m_tInfo.matWorld = matScale * matTrains;

		m_pDevice->Get_Sprite()->SetTransform(&m_tInfo.matWorld);
		m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
		//물가
		float fYTurn = 0.f;
		if (m_iReinforce == 0)
			fYTurn = fCenterY + 105.f;
		else
			fYTurn = fCenterY + 90.f;

		D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, -m_tInfo.vSize.y, 0.f);
		D3DXMatrixTranslation(&matTrains, m_tInfo.vPos.x + fScrollX, m_tInfo.vPos.y + fYTurn, 0.f);

		m_tInfo.matWorld = matScale * matTrains;

		m_pDevice->Get_Sprite()->SetTransform(&m_tInfo.matWorld);
		m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(120, 255, 255, 255));
	}
}