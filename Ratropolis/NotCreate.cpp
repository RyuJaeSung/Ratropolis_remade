#include "stdafx.h"
#include "NotCreate.h"
#include "TextureMgr.h"
#include "ObjMgr.h"
#include "SceneMgr.h"
#include "Stage.h"
#include "Terrain.h"
#include "ScrollMgr.h"
#include "ErrMsg.h"
#include "ScrollMgr.h"
#include "AbstractFactory.h"
#include "Graphic_Device.h"

//건물
#include "House.h"
#include "Factory.h"
#include "Monastery.h"
#include "Barn.h"
#include "ApartmentHouse.h"
#include "Farm.h"
#include "DefenseWall.h"


CNotCreate::CNotCreate()
	:m_bRender(false), m_bBuild(false), m_iMax(0), m_iMin(9999)
{
	m_pDevice = nullptr;
}


CNotCreate::~CNotCreate()
{
}

HRESULT CNotCreate::Initialize()
{
	m_pDevice = CGraphic_Device::Get_Instance();
	return S_OK;
}

int CNotCreate::Update()
{
	if (m_bBuild && m_bRender)
	{
		Build();
		return OBJ::DEAD;
	}

	int num = 0;
	POINT ps = {};
	GetCursorPos(&ps);
	ClientToScreen(g_hWnd, &ps);

	if (ps.y <= (TILECY >> 1) * 8 && ps.y >= (TILECY >> 1) * 6)
	{
		num = dynamic_cast<CStage*>(CSceneMgr::Get_Instance()->Get_Scene())->Get_Terrain()->Get_Tile(ps.x);
		if (dynamic_cast<CStage*>(CSceneMgr::Get_Instance()->Get_Scene())->Get_Terrain()->Get_Option(num, m_iBuildSize))
			m_bRender = true;
		else
			m_bRender = false;

		m_tInfo.vPos = { float((TILECX >> 1) * num) , float((TILECY >> 1) * 7), 0.f };
	}
	else
		m_bRender = false;

	num = dynamic_cast<CStage*>(CSceneMgr::Get_Instance()->Get_Scene())->Get_Terrain()->Get_Tile(ps.x);

	if (m_iMax < num || m_iMin > num)
		m_bRender = false;

	return OBJ::LIVE;
}

void CNotCreate::Late_Update()
{

	for (auto& iter : CObjMgr::Get_Instance()->Get_Object(OBJ::WALL))
	{
		if (dynamic_cast<CDefenseWall*>(iter)->Get_WallState() != 0)
		{
			float x = iter->Get_Pos().x + CScrollMgr::Get_Instance()->Get_ScrollX();
			int iTile = dynamic_cast<CStage*>(CSceneMgr::Get_Instance()->Get_Scene())->Get_Terrain()->Get_Tile(int(x));

			if (m_iMax < iTile)
				m_iMax = iTile;
			else if (m_iMin > iTile)
				m_iMin = iTile;
		}
	}
	dynamic_cast<CStage*>(CSceneMgr::Get_Instance()->Get_Scene())->Get_Terrain()->Set_BlueTile(m_iMax, m_iMin);

	Change_Pos();

}

void CNotCreate::Render()
{


	if (m_bRender)
	{
		float fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
		const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(m_wstrObjectKey, m_wstrStateKey, m_dwCount);
		if (!pTexInfo)
			return;
		D3DXMATRIX	matScale, matTrans;
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height>> 1);

		D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + fScrollX, m_tInfo.vPos.y - m_fAdd_PosY, 0.f);

		m_tInfo.matWorld = matScale * matTrans;

		m_pDevice->Get_Sprite()->SetTransform(&m_tInfo.matWorld);
		m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(120, 255, 255, 255));
	}
}

void CNotCreate::Release()
{
}

void CNotCreate::Build()
{

	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::EFFECT);
	CSoundMgr::Get_Instance()->PlaySound(L"Building.wav", CSoundMgr::EFFECT);
	if (!lstrcmp(__T("House"), m_BuildingName.c_str()))
	{
		CObjMgr::Get_Instance()->Add_Object(OBJ::BUILDING, CAbstractFactroy<CHouse>::Create(m_tInfo.vPos, m_tInfo.vSize, m_wstrObjectKey, m_wstrStateKey, m_dwCount));
		dynamic_cast<CStage*>(CSceneMgr::Get_Instance()->Get_Scene())->Get_Terrain()->Change_Option(m_tInfo.vPos, 1);
	}
	else if (!lstrcmp(__T("Factory"), m_BuildingName.c_str()))
	{
		CObjMgr::Get_Instance()->Add_Object(OBJ::BUILDING, CAbstractFactroy<CFactory>::Create(m_tInfo.vPos, m_tInfo.vSize, m_wstrObjectKey, m_wstrStateKey, m_dwCount));
		dynamic_cast<CStage*>(CSceneMgr::Get_Instance()->Get_Scene())->Get_Terrain()->Change_Option(m_tInfo.vPos, 5);
	}
	else if (!lstrcmp(__T("Monastery"), m_BuildingName.c_str()))
	{
		CObjMgr::Get_Instance()->Add_Object(OBJ::BUILDING, CAbstractFactroy<CMonastery>::Create(m_tInfo.vPos, m_tInfo.vSize, m_wstrObjectKey, m_wstrStateKey, m_dwCount));
		dynamic_cast<CStage*>(CSceneMgr::Get_Instance()->Get_Scene())->Get_Terrain()->Change_Option(m_tInfo.vPos, 3);
	}
	else if (!lstrcmp(__T("Barn"), m_BuildingName.c_str()))
	{
		CObjMgr::Get_Instance()->Add_Object(OBJ::BUILDING, CAbstractFactroy<CBarn>::Create(m_tInfo.vPos, m_tInfo.vSize, m_wstrObjectKey, m_wstrStateKey, m_dwCount));
		dynamic_cast<CStage*>(CSceneMgr::Get_Instance()->Get_Scene())->Get_Terrain()->Change_Option(m_tInfo.vPos, 3);
	}
	else if (!lstrcmp(__T("ApartmentHouse"), m_BuildingName.c_str()))
	{
		CObjMgr::Get_Instance()->Add_Object(OBJ::BUILDING, CAbstractFactroy<CApartmentHouse>::Create(m_tInfo.vPos, m_tInfo.vSize, m_wstrObjectKey, m_wstrStateKey, m_dwCount));
		dynamic_cast<CStage*>(CSceneMgr::Get_Instance()->Get_Scene())->Get_Terrain()->Change_Option(m_tInfo.vPos, 3);
	}
	else if (!lstrcmp(__T("Farm"), m_BuildingName.c_str()))
	{
		CObjMgr::Get_Instance()->Add_Object(OBJ::BUILDING, CAbstractFactroy<CFarm>::Create(m_tInfo.vPos, m_tInfo.vSize, m_wstrObjectKey, m_wstrStateKey, m_dwCount));
		dynamic_cast<CStage*>(CSceneMgr::Get_Instance()->Get_Scene())->Get_Terrain()->Change_Option(m_tInfo.vPos, 3);
	}
}

bool CNotCreate::Ckeck_Build()
{
	if (!dynamic_cast<CStage*>(CSceneMgr::Get_Instance()->Get_Scene())->Get_Terrain()->Ckeck_Tile(m_tInfo.vPos) || !m_bRender)
	{

		TCHAR szBuffer[32] = {};
		swprintf_s(szBuffer, __T("건설할 수 없습니다."));
		CObjMgr::Get_Instance()->Add_Object(OBJ::CRAD, CAbstractFactroy<CErrMsg>::Create(szBuffer));

		return false;
	}
	return true;
}

void CNotCreate::Change_Pos()
{
	if (!lstrcmp(__T("House"), m_BuildingName.c_str()))
		m_fAdd_PosY = 5.f;
	else if (!lstrcmp(__T("Factory"), m_BuildingName.c_str()))
		m_fAdd_PosY = 100.f;
	else if (!lstrcmp(__T("Monastery"), m_BuildingName.c_str()))
		m_fAdd_PosY = 95.f;
	else if (!lstrcmp(__T("Barn"), m_BuildingName.c_str()))
		m_fAdd_PosY = 40.f;
	else if (!lstrcmp(__T("ApartmentHouse"), m_BuildingName.c_str()))
		m_fAdd_PosY = 70.f;
	else if (!lstrcmp(__T("Farm"), m_BuildingName.c_str()))
		m_fAdd_PosY = 55.f;
}
