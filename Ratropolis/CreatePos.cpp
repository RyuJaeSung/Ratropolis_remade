#include "stdafx.h"
#include "CreatePos.h"
#include "TextureMgr.h"
#include "ObjMgr.h"
#include "SceneMgr.h"
#include "Stage.h"
#include "Terrain.h"
#include "DefenseWall.h"
#include "ScrollMgr.h"
#include "TimeMgr.h"
#include "ErrMsg.h"
#include "AbstractFactory.h"
#include "Graphic_Device.h"

//병사
#include "Militia.h"
#include "Shield.h"
#include "LongBow.h"

CCreatePos::CCreatePos()
	:m_bRender(false), m_bCreate(false), m_fNowY(0.f), m_fMaxY(20.f), m_fMinY(-20.f), m_fMove(2.f), m_iWallNum(0)
{
	m_pDevice = nullptr;
}


CCreatePos::~CCreatePos()
{
}

HRESULT CCreatePos::Initialize()
{
	m_pDevice = CGraphic_Device::Get_Instance();
	return S_OK;
}

int CCreatePos::Update()
{
	POINT pMouse = {};
	GetCursorPos(&pMouse);
	ScreenToClient(g_hWnd, &pMouse);
	m_iWallNum = 0;

	for (auto& iter : CObjMgr::Get_Instance()->Get_Object(OBJ::WALL))
	{
		int TileWall = dynamic_cast<CDefenseWall*>(iter)->Get_TileNum();
		int TilePs = dynamic_cast<CStage*>(CSceneMgr::Get_Instance()->Get_Scene())->Get_Terrain()->Get_TileEx(pMouse.x);

		if (TileWall == TilePs && pMouse.y <= 420.f)
		{
			int TypeNum = dynamic_cast<CDefenseWall*>(iter)->Get_WallState();

			if (TypeNum != 0)
			{
				m_bRender = true;

					if (m_bCreate)
					{
						Create_Militory(TileWall);
						return OBJ::DEAD;
					}

			}
			return OBJ::LIVE;
		}
		++m_iWallNum;
	}
	return OBJ::LIVE;
}

void CCreatePos::Late_Update()
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

void CCreatePos::Render()
{
	float fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	int iWall = 0;
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(m_wstrObjectKey, m_wstrStateKey, m_dwCount);
	if (!pTexInfo)
		return;

	for (auto& iter : CObjMgr::Get_Instance()->Get_Object(OBJ::WALL))
	{
		if (dynamic_cast<CDefenseWall*>(iter)->Get_WallState() == 0)
		{
			++iWall;
			continue;
		}
		float fCenterX = 0.f;
		float fCenterY = 0.f;

		m_tInfo.vPos = iter->Get_Pos();

		D3DXMATRIX matScale, matTrans;
		D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
		if (m_bRender)
		{
			if (iWall == m_iWallNum)
			{
				const TEXINFO* pTexInfo_Effect = CTextureMgr::Get_Instance()->Get_TexInfo(m_wstrObjectKey, __T("Wall"), 11);
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

		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + fScrollX, m_tInfo.vPos.y + m_fNowY - 80.f, 0.f);

		m_tInfo.matWorld = matScale * matTrans;

		m_pDevice->Get_Sprite()->SetTransform(&m_tInfo.matWorld);
		m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

		++iWall;
	}
}

void CCreatePos::Release()
{
}

void CCreatePos::Create_Militory(int Tile)
{
	D3DXVECTOR3 vPos = {};
	CObj* pObj = nullptr;
	float num = 0;
	float fDir = 1.f;
	for (auto& iter : CObjMgr::Get_Instance()->Get_Object(OBJ::WALL))
	{
		int TileNum = dynamic_cast<CDefenseWall*>(iter)->Get_TileNum();

		if (TileNum == Tile)
		{
			if (num > 2)
				fDir *= -1.f;
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);
			switch (m_eName)
			{
			case PLAYER::MILITIA:
				CSoundMgr::Get_Instance()->PlaySound(L"MilitiaSelect.wav", CSoundMgr::PLAYER);
				vPos = iter->Get_Pos();
				pObj = CAbstractFactroy<CMilitia>::Create(D3DXVECTOR3(vPos.x, vPos.y + 35.f, 0.f), D3DXVECTOR3(0.5f * fDir, 0.5f, 0.f), num, __T("Military"), m_wstrStage, 0);
				dynamic_cast<CDefenseWall*>(iter)->Add_Militory(m_eName, pObj);
				CObjMgr::Get_Instance()->Add_Object(OBJ::PLATER, pObj);
				return;
			case PLAYER::SHIELD:
				CSoundMgr::Get_Instance()->PlaySound(L"ShieldSelect.wav", CSoundMgr::PLAYER);
				vPos = iter->Get_Pos();
				pObj = CAbstractFactroy<CShield>::Create(D3DXVECTOR3(vPos.x, vPos.y + 40.f, 0.f), D3DXVECTOR3(0.3f * fDir, 0.3f, 0.f), num, __T("Military"), m_wstrStage, 0);
				dynamic_cast<CDefenseWall*>(iter)->Add_Militory(m_eName, pObj);
				CObjMgr::Get_Instance()->Add_Object(OBJ::PLATER, pObj);
				return;
			case PLAYER::LONGBOW:
				CSoundMgr::Get_Instance()->PlaySound(L"LongBowSelect.wav", CSoundMgr::PLAYER);
				D3DXVECTOR3 vPos = iter->Get_Pos();
				if (num > 2)
					vPos.x += 80.f;
				else
					vPos.x -= 80.f;
				CObj* pObj = CAbstractFactroy<CLongBow>::Create(D3DXVECTOR3(vPos.x, vPos.y + 40.f, 0.f), D3DXVECTOR3(0.5f * fDir, 0.5f, 0.f), num, __T("Military"), m_wstrStage, 0);
				dynamic_cast<CDefenseWall*>(iter)->Add_Militory(m_eName, pObj);
				CObjMgr::Get_Instance()->Add_Object(OBJ::PLATER, pObj);
				return;
			}
		}

		++num;
	}
}

bool CCreatePos::NoCreate_Pop()
{
	int iPop = 0;
	switch (m_eName)
	{
	case PLAYER::MILITIA:
		iPop = 1;
		break;
	case PLAYER::SHIELD:
		iPop = 1;
		break;
	case PLAYER::LONGBOW:
		iPop = 2;
		break;
	}


	if (dynamic_cast<CStage*>(CSceneMgr::Get_Instance()->Get_Scene())->Get_Stage().m_iPop - iPop < 0)
	{
		TCHAR szBuf[32] = {};
		swprintf_s(szBuf, __T("인구가 부족합니다."));
		CObjMgr::Get_Instance()->Add_Object(OBJ::CRAD, CAbstractFactroy<CErrMsg>::Create(szBuf));
		return false;
	}
	return true;
}
