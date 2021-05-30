#include "stdafx.h"
#include "ChangeWall.h"
#include "SceneMgr.h"
#include "Stage.h"
#include "Terrain.h"
#include "DefenseWall.h"
#include "ScrollMgr.h"
#include "ObjMgr.h"
#include "TimeMgr.h"
#include "TextureMgr.h"
#include "Graphic_Device.h"

CChangeWall::CChangeWall()
	:m_fNowY(0.f), m_fMaxY(20.f), m_fMinY(-20.f), m_fMove(2.f), m_iWallNum(0), m_bNot(false)
{
	m_pDevice = nullptr;
}


CChangeWall::~CChangeWall()
{
}

HRESULT CChangeWall::Initialize()
{
	m_pDevice = CGraphic_Device::Get_Instance();
	return S_OK;
}

int CChangeWall::Update()
{
	POINT pMouse = {};
	GetCursorPos(&pMouse);
	ScreenToClient(g_hWnd, &pMouse);
	m_iWallNum = 0;
	for (auto& iter : CObjMgr::Get_Instance()->Get_Object(OBJ::WALL))
	{
		int TileWall = dynamic_cast<CDefenseWall*>(iter)->Get_TileNum();
		int TilePs = dynamic_cast<CStage*>(CSceneMgr::Get_Instance()->Get_Scene())->Get_Terrain()->Get_TileEx(pMouse.x);
		int TypeNum = dynamic_cast<CDefenseWall*>(iter)->Get_WallState();
		
		if(TileWall == TilePs && TypeNum == 0 && pMouse.y <= 420.f)
		{

			m_bCreate = true;	
			if (Before_Wall(m_iWallNum -1))
			{

				if (m_bNot)
				{
					Change_Wall(TileWall);
					return OBJ::DEAD;
				}
			}
			else
				m_bCreate = false;
			return OBJ::LIVE;
		}
		else
			m_bCreate = false;
		++m_iWallNum;
	}
	return OBJ::LIVE;
}

void CChangeWall::Late_Update()
{
	m_fTime += CTimeMgr::Get_Instance()->Get_DeltaTime();
	if (m_fTime >= 0.2f)
	{
		m_fNowY += m_fMove;

		if (m_fNowY >= m_fMaxY || m_fNowY <= m_fMinY)
			m_fMove *= -1.f;

		m_fTime = 0.f;
	}
}

void CChangeWall::Render()
{
	float fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	int iWall = 0;
	bool Not = false;
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(m_wstrObjectKey, m_wstrStateKey, m_dwCount);
	if (!pTexInfo)
		return;

	for (auto& iter : CObjMgr::Get_Instance()->Get_Object(OBJ::WALL))
	{
		if (dynamic_cast<CDefenseWall*>(iter)->Get_WallState() != 0 || !Not)
		{
			Not = true;
			++iWall;
			continue;
		}

		float fCenterX = 0.f;
		float fCenterY = 0.f;

		m_tInfo.vPos = iter->Get_Pos();

		D3DXMATRIX matScale, matTrans;
		D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
		
		
		
		if (m_bCreate)
		{
			if (iWall == m_iWallNum)
			{
				const TEXINFO* pTexInfo_Effect = CTextureMgr::Get_Instance()->Get_TexInfo(m_wstrObjectKey, __T("Wall"), 10);
				if (!pTexInfo_Effect)
					return;

				fCenterX = float(pTexInfo_Effect->tImageInfo.Width >> 1);
				fCenterY = float(pTexInfo_Effect->tImageInfo.Height >> 1);
				D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + fScrollX, m_tInfo.vPos.y, 0.f);

				m_tInfo.matWorld = matScale * matTrans;

				m_pDevice->Get_Sprite()->SetTransform(&m_tInfo.matWorld);
				m_pDevice->Get_Sprite()->Draw(pTexInfo_Effect->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
			}
		}


		fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + fScrollX, m_tInfo.vPos.y + m_fNowY - 50.f, 0.f);

		m_tInfo.matWorld = matScale * matTrans;

		m_pDevice->Get_Sprite()->SetTransform(&m_tInfo.matWorld);
		m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

		++iWall;
		Not = false;
	}
}

void CChangeWall::Release()
{
}

void CChangeWall::Change_Wall(int Tile)
{
	int num = 0;
	for (auto& iter : CObjMgr::Get_Instance()->Get_Object(OBJ::WALL))
	{
		int TileNum = dynamic_cast<CDefenseWall*>(iter)->Get_TileNum();

		if (TileNum == Tile)
		{
			if (num < 3)
				iter->Set_Size(D3DXVECTOR3(-0.4f, 0.4f, 0.f));
			else
				iter->Set_Size(D3DXVECTOR3(0.4f, 0.4f, 0.f));
			dynamic_cast<CDefenseWall*>(iter)->Set_LevelWall(1);
			dynamic_cast<CDefenseWall*>(iter)->Wall_Hp();
			return;
		}
		++num;
	}
}

bool CChangeWall::Before_Wall(int Tile)
{
	int num = 0;
	for (auto& iter : CObjMgr::Get_Instance()->Get_Object(OBJ::WALL))
	{
		if (num == Tile)
		{
			if (dynamic_cast<CDefenseWall*>(iter)->Get_WallState() > 0)
				return true;
			else
				false;
		}
		++num;
	}
	return false;
}
