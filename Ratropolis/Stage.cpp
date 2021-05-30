#include "stdafx.h"
//객체
#include "Stage.h"
#include "Ui.h"
#include "Castle.h"
#include "Clock.h"
#include "MyFont.h"
#include "Terrain.h"
#include "CradList.h"
#include "DefenseWall.h"
#include "Basic.h"
#include "Wave.h"
#include "Reward.h"
#include "AddGold.h"

//Manager
#include "TimeMgr.h"
#include "ObjMgr.h"
#include "TextureMgr.h"
#include "ScrollMgr.h"
#include "CradMgr.h"
#include "KeyMgr.h"

//함수들
#include "Collision.h"
#include "Graphic_Device.h"
#include "AbstractFactory.h"


CStage::CStage()
	:m_pTerrain(nullptr), m_fSpeed(10.f), m_fCloudMove(20.f),m_bCloud(false), m_fFPSTime(0.f), m_bStageStart(true), m_fGameTime(0.f)
{
	m_pDevice = nullptr;
	CSoundMgr::Get_Instance()->PlayBGM(__T("MainGame.wav"));
	m_tStage.m_iGold = 2000;
	m_tStage.m_iTex = 6;
	m_tStage.m_iMaxPop = 5;
	m_tStage.m_iPop = 5;
	m_tStage.m_iWave = 1;
	m_tStage.m_iMaxWave = 10;
	m_tStage.m_iTexTime = 0;
	m_tStage.m_iReSetTime = 0;
}


CStage::~CStage()
{
	Release();
}

HRESULT CStage::Initialize()
{
	m_pDevice = CGraphic_Device::Get_Instance();
	CScrollMgr::Get_Instance()->Set_ScrollX(-((TILECX>>1) * 50.f) - 20);
	CCradMgr::Get_Instance()->Setting_Crad();
	if (!m_pTerrain)
	{
		m_pTerrain = new CTerrain;
		m_pTerrain->Initialize();
	}
	Add_Ui();
	
	for (int i = 0; i < 11; ++i)
		CObjMgr::Get_Instance()->Add_Object(OBJ::OBJECT, CAbstractFactroy<CWave>::Create(D3DXVECTOR3(1500.f + (550.f * i), 650.f, 0.f)));
	return S_OK;
}

void CStage::Update()
{

	if (m_bCloud)
	{
		
		StageStart();
		CObjMgr::Get_Instance()->Button_Ckeck();
		CObjMgr::Get_Instance()->Update();
		if (!Game_Progress)
		{
			CCradMgr::Get_Instance()->Update();
			Add_Gold();

			if (m_bCreateWave)
			{
				CObjMgr::Get_Instance()->Add_Object(OBJ::OBJECT,CAbstractFactroy<CWave>::Create());
				m_bCreateWave = false;
			}
		}
	}
}

void CStage::Late_Update()
{
	if (m_bCloud)
	{
		if (!Game_Progress)
		{
			CObjMgr::Get_Instance()->Click_Mouse();
			CObjMgr::Get_Instance()->Late_Update();
			CScrollMgr::Get_Instance()->Map_Move();
		}
	}
}

void CStage::Render()
{
	Render_Background();
	Render_Font();
	m_pTerrain->Render();
	CObjMgr::Get_Instance()->Render();
	Render_Black();
	CCradMgr::Get_Instance()->Render();
	if (!m_bCloud)
	{
		Cloud_Move();
		if (m_bCloud)
		{
			CCradMgr::Get_Instance()->Select_Crad();
			CCradMgr::Get_Instance()->Add_Render();
		}
	}
}

void CStage::Release()
{
	SAFE_DELETE(m_pTerrain);
	CScrollMgr::Get_Instance()->ReSet();
	for(int i = 0; i < OBJ::OBJ_END; ++i)
		CObjMgr::Get_Instance()->Clear_Object((OBJ::ID)i);

	CCradMgr::Get_Instance()->Clear_Crad();
	CSoundMgr::Get_Instance()->StopAll();
}


void CStage::Cloud_Move()
{
	m_fCloudMove += m_fSpeed;
	RECT rClient = { 0, 0, WINCX, WINCY };

	for (int i = 0; i < 3; ++i)
	{
		const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(__T("InGame"), __T("UI"), 12);
		if (!pTexInfo)
		{
			ERR_MSG(__T("Cloud Load Failed"));
			return;
		}

		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
		float fPosY = float((WINCY / 3.f * i) + ((WINCY / 3.f) * 0.5f));
		float fPosX = 0.f;
		D3DXMATRIX matScale, matTrans, matWorld;
		D3DXMatrixScaling(&matScale, 3.f, 3.f, 0.f);
		if (0 == i % 2)
		{
			fPosX = float(WINCX >> 1) + m_fCloudMove;
			D3DXMatrixTranslation(&matTrans, fPosX, fPosY, 0.f);
		}
		else
		{
			fPosX = float(WINCX >> 1) - m_fCloudMove;
			D3DXMatrixTranslation(&matTrans, fPosX, fPosY, 0.f);
		}
		matWorld = matScale * matTrans;

		m_pDevice->Get_Sprite()->SetTransform(&matWorld);
		m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

		RECT rImage = { LONG(fPosX - fCenterX), LONG(fPosY - fCenterY) ,LONG(fPosX + fCenterX) , LONG(fPosY + fCenterY) };
		if (CCollision::ClientRectCkeck(rClient, rImage))
			m_bCloud = true;
		else
			m_bCloud = false;
	}
}

void CStage::StageStart()
{

		float CreatePosX = 0.f;
		int iCreatePos = 0;
		
		if (CKeyMgr::Get_Instance()->KeyDown(KEY_ONE))
		{
			iCreatePos = 1;
			CreatePosX = 1500.f;
			CObjMgr::Get_Instance()->Add_Object(OBJ::MONSTER, CAbstractFactroy<CBasic>::Create(D3DXVECTOR3(CreatePosX, 380.f, 0.f), D3DXVECTOR3(0.5f, 0.5f, 0.f), float(iCreatePos), __T("Monster"), __T("BasicMove"), 0));
			m_bStageStart = true;
		}
		if (CKeyMgr::Get_Instance()->KeyDown(KEY_TWO))
		{
			iCreatePos = 0;
			CreatePosX = 6000.f;
			CObjMgr::Get_Instance()->Add_Object(OBJ::MONSTER, CAbstractFactroy<CBasic>::Create(D3DXVECTOR3(CreatePosX, 380.f, 0.f), D3DXVECTOR3(0.5f, 0.5f, 0.f), float(iCreatePos), __T("Monster"), __T("BasicMove"), 0));
			m_bStageStart = true;
		}
	
	
	if (CObjMgr::Get_Instance()->Get_Object(OBJ::MONSTER).size() == 1)
		m_vRewardPos = CObjMgr::Get_Instance()->Get_Object(OBJ::MONSTER).front()->Get_Pos();

	if (CObjMgr::Get_Instance()->Get_Object(OBJ::MONSTER).empty() && m_bStageStart)
	{
		CObjMgr::Get_Instance()->Add_Object(OBJ::CRAD, CAbstractFactroy<CReward>::Create(m_vRewardPos, D3DXVECTOR3(0.5f, 0.5f, 0.f), __T("InGame"), __T("UI"), 19));
		m_bStageStart = false;
	}
}


void CStage::Render_Background()
{
	for (int i = 0; i < 13; ++i)
	{
		const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(__T("InGame"), __T("Sky"), 0);
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		D3DXMATRIX matScale, matTrans, matWorld;
		D3DXMatrixScaling(&matScale, 0.5f, 0.5f, 0.f);
		D3DXMatrixTranslation(&matTrans, (530.f * i) + CScrollMgr::Get_Instance()->Get_ScrollX(), fCenterY * 0.5f, 0.f);
		matWorld = matScale * matTrans;
		m_pDevice->Get_Sprite()->SetTransform(&matWorld);
		m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(__T("InGame"), __T("Sky"), 4);
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 0.8f, 0.8f, 0.f);
	D3DXMatrixTranslation(&matTrans, 4300.f + CScrollMgr::Get_Instance()->Get_ScrollX(), fCenterY * 0.5f - 100.f, 0.f);
	matWorld = matScale * matTrans;
	m_pDevice->Get_Sprite()->SetTransform(&matWorld);
	m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


	for (int i = 0; i < 8; ++i)
	{
		//원본
		const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(__T("InGame"), __T("Desert_Back"), (i % 5));
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		D3DXMATRIX matScale, matTrans, matWorld;
		D3DXMatrixScaling(&matScale, 0.5f, 0.5f, 0.f);
		if(i == 2 + ((i/5) * 5))
			D3DXMatrixTranslation(&matTrans, (1000.f * i) + CScrollMgr::Get_Instance()->Get_ScrollX(), float(WINCY >> 1) - 115.f, 0.f);
		else
			D3DXMatrixTranslation(&matTrans, (1000.f * i) + CScrollMgr::Get_Instance()->Get_ScrollX(), float(WINCY>>1) - 100.f, 0.f);
		matWorld = matScale * matTrans;
		m_pDevice->Get_Sprite()->SetTransform(&matWorld);
		m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

		//물가
		D3DXMatrixScaling(&matScale, 0.5f, -0.5f, 0.f);
		if (i == 2 + ((i / 5) * 5))
			D3DXMatrixTranslation(&matTrans, (1000.f * i) + CScrollMgr::Get_Instance()->Get_ScrollX(), float(WINCY >> 1) + 315.f, 0.f);
		else
			D3DXMatrixTranslation(&matTrans, (1000.f * i) + CScrollMgr::Get_Instance()->Get_ScrollX(), float(WINCY >> 1) + 300.f, 0.f);
		matWorld = matScale * matTrans;
		m_pDevice->Get_Sprite()->SetTransform(&matWorld);
		m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(120, 255, 255, 255));
	}

	for (int i = 0; i < 14; ++i)
	{
		//원본
		const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(__T("InGame"), __T("Desert"), (i % 5));
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		D3DXMATRIX matScale, matTrans, matWorld;
		D3DXMatrixScaling(&matScale, 0.5f, 0.4f, 0.f);
		D3DXMatrixTranslation(&matTrans, (498.f * i) + CScrollMgr::Get_Instance()->Get_ScrollX(), float(WINCY >> 1), 0.f);
		matWorld = matScale * matTrans;
		m_pDevice->Get_Sprite()->SetTransform(&matWorld);
		m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

		//물가
		D3DXMatrixScaling(&matScale, 0.5f, -0.4f, 0.f);
		D3DXMatrixTranslation(&matTrans, (498.f * i) + CScrollMgr::Get_Instance()->Get_ScrollX(), float(WINCY >> 1) + 300.f, 0.f);
		matWorld = matScale * matTrans;
		m_pDevice->Get_Sprite()->SetTransform(&matWorld);
		m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(120, 255, 255, 255));
	}


}

void CStage::Render_Font()
{

	CObjMgr::Get_Instance()->Clear_Object(OBJ::FONT);
	int num = 0;
	TCHAR szBuff[32] = {};

	swprintf_s(szBuff, __T("%d"), m_tStage.m_iGold);
	CObjMgr::Get_Instance()->Add_Object(OBJ::FONT, CAbstractFactroy<CMyFont>::Create(D3DXVECTOR3(65.f, 35.f, 0.f), D3DXVECTOR3(0.6f, 0.6f, 0.f), szBuff, 0xFFFFFF00));
	swprintf_s(szBuff, __T("+%d"), m_tStage.m_iTex);
	CObjMgr::Get_Instance()->Add_Object(OBJ::FONT, CAbstractFactroy<CMyFont>::Create(D3DXVECTOR3(120.f, 35.f, 0.f), D3DXVECTOR3(0.5f, 0.5f, 0.f), szBuff, 0xFFFFFF00));
	swprintf_s(szBuff, __T("%d / %d"), m_tStage.m_iPop, m_tStage.m_iMaxPop);
	CObjMgr::Get_Instance()->Add_Object(OBJ::FONT, CAbstractFactroy<CMyFont>::Create(D3DXVECTOR3(190.f, 35.f, 0.f), D3DXVECTOR3(0.6f, 0.6f, 0.f), szBuff, 0xff00b000));
	swprintf_s(szBuff, __T("Wave %d / %d"), m_tStage.m_iWave, m_tStage.m_iMaxWave);
	CObjMgr::Get_Instance()->Add_Object(OBJ::FONT, CAbstractFactroy<CMyFont>::Create(D3DXVECTOR3(100.f, 70.f, 0.f), D3DXVECTOR3(0.5f, 0.5f, 0.f), szBuff, 0xffffffff));
	swprintf_s(szBuff, __T("%d"), CCradMgr::Get_Instance()->Get_ListBag().size());
	CObjMgr::Get_Instance()->Add_Object(OBJ::FONT, CAbstractFactroy<CMyFont>::Create(D3DXVECTOR3(70.f, 620.f, 0.f), D3DXVECTOR3(0.5f, 0.5f, 0.f), szBuff, 0xff000000));
	swprintf_s(szBuff, __T("%d"), CCradMgr::Get_Instance()->Get_ListCemetry().size());
	CObjMgr::Get_Instance()->Add_Object(OBJ::FONT, CAbstractFactroy<CMyFont>::Create(D3DXVECTOR3(1190.f, 620.f, 0.f), D3DXVECTOR3(0.5f, 0.5f, 0.f), szBuff, 0xff000000));

}

void CStage::Render_Black()
{
	D3DXMATRIX matScale, matTrans, matWorld;
	const TEXINFO* pTexInfo = nullptr;

	float fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	D3DXVECTOR3 vInfoRight = { 6200.f, float(WINCY>>1), 0.f};

	if (vInfoRight.x < (-fScrollX + (WINCX + 1000.f)))
	{
		pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(__T("InGame"), __T("Water"), 0);
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, vInfoRight.x + fScrollX, vInfoRight.y, 0.f);
		matWorld = matScale * matTrans;

		m_pDevice->Get_Sprite()->SetTransform(&matWorld);
		m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	D3DXVECTOR3 vInfoLeft = { 1300.f, float(WINCY >> 1), 0.f };
	if (vInfoLeft.x > -fScrollX - 500.f )
	{
		pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(__T("InGame"), __T("Water"), 0);
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, vInfoLeft.x + fScrollX, vInfoLeft.y, 0.f);
		matWorld = matScale * matTrans;

		m_pDevice->Get_Sprite()->SetTransform(&matWorld);
		m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	}

}

void CStage::Add_Ui()
{
	//양쪽 아이콘
	CObjMgr::Get_Instance()->Add_Object(OBJ::BUTTON, CAbstractFactroy<CUi>::Create(D3DXVECTOR3(50.f, 550.f, 0.f), D3DXVECTOR3(0.4f, 0.4f, 0.f), __T("UI"), __T("Window"), 0));
	CObjMgr::Get_Instance()->Add_Object(OBJ::BUTTON, CAbstractFactroy<CUi>::Create(D3DXVECTOR3(50.f, 650.f, 0.f), D3DXVECTOR3(0.4f, 0.4f, 0.f), __T("UI"), __T("Window"), 0));
	CObjMgr::Get_Instance()->Add_Object(OBJ::BUTTON, CAbstractFactroy<CUi>::Create(D3DXVECTOR3(1230.f, 550.f, 0.f), D3DXVECTOR3(0.4f, 0.4f, 0.f), __T("UI"), __T("Window"), 0));
	CObjMgr::Get_Instance()->Add_Object(OBJ::BUTTON, CAbstractFactroy<CUi>::Create(D3DXVECTOR3(1230.f, 650.f, 0.f), D3DXVECTOR3(0.4f, 0.4f, 0.f), __T("UI"), __T("Window"), 0));
	

	//리더 스킬(액티브),가방, 카드체인지, 묘지
	CObjMgr::Get_Instance()->Add_Object(OBJ::UI, CAbstractFactroy<CUi>::Create(D3DXVECTOR3(50.f, 550.f, 0.f), D3DXVECTOR3(0.4f, 0.4f, 0.f), __T("Player"), __T("CharacterActive"), m_ePlayerType));
	CObjMgr::Get_Instance()->Add_Object(OBJ::UI, CAbstractFactroy<CUi>::Create(D3DXVECTOR3(50.f, 650.f, 0.f), D3DXVECTOR3(0.4f, 0.4f, 0.f), __T("InGame"), __T("UI"), 0));
	CObjMgr::Get_Instance()->Add_Object(OBJ::UI, CAbstractFactroy<CUi>::Create(D3DXVECTOR3(1230.f, 550.f, 0.f), D3DXVECTOR3(0.4f, 0.4f, 0.f), __T("InGame"), __T("UI"), 1));
	CObjMgr::Get_Instance()->Add_Object(OBJ::UI, CAbstractFactroy<CUi>::Create(D3DXVECTOR3(1230.f, 650.f, 0.f), D3DXVECTOR3(0.4f, 0.4f, 0.f), __T("InGame"), __T("UI"), 2));

	//우측 상단 아이콘
	for (int i = 0; i < 3; ++i)
	{
		CObjMgr::Get_Instance()->Add_Object(OBJ::BUTTON, CAbstractFactroy<CUi>::Create(D3DXVECTOR3(1230.f - (100 * i), 50.f, 0.f), D3DXVECTOR3(0.4f, 0.4f, 0.f), __T("UI"), __T("Window"), 0));
		//메뉴, 정지&스타트,전문가
		CObjMgr::Get_Instance()->Add_Object(OBJ::UI, CAbstractFactroy<CUi>::Create(D3DXVECTOR3(1230.f - (100 * i), 50.f, 0.f), D3DXVECTOR3(0.4f, 0.4f, 0.f), __T("InGame"), __T("UI"), i+3));
	}
	//좌측 상단 아이콘
	CObjMgr::Get_Instance()->Add_Object(OBJ::UI, CAbstractFactroy<CUi>::Create(D3DXVECTOR3(150.f, 60.f, 0.f), D3DXVECTOR3(0.8f, 0.8f, 0.f), __T("UI"), __T("Window"), 1));

	//골드, 인구수, 웨이브
	TCHAR szBuff[32] = {};
	CObjMgr::Get_Instance()->Add_Object(OBJ::UI, CAbstractFactroy<CUi>::Create(D3DXVECTOR3(50.f, 43.f, 0.f), D3DXVECTOR3(0.2f, 0.2f, 0.f), __T("InGame"), __T("UI"), 7));
	CObjMgr::Get_Instance()->Add_Object(OBJ::UI, CAbstractFactroy<CUi>::Create(D3DXVECTOR3(160.f, 43.f, 0.f), D3DXVECTOR3(0.2f, 0.2f, 0.f), __T("InGame"), __T("UI"), 8));
	CObjMgr::Get_Instance()->Add_Object(OBJ::UI, CAbstractFactroy<CUi>::Create(D3DXVECTOR3(80.f, 80.f, 0.f), D3DXVECTOR3(0.2f, 0.2f, 0.f), __T("InGame"), __T("UI"), 9));

	//캐슬	
	float fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float fCastleX = float(WINCX >> 1) - fScrollX;
	CObjMgr::Get_Instance()->Add_Object(OBJ::CASTLE, CAbstractFactroy<CCastle>::Create(D3DXVECTOR3(fCastleX, (WINCY >> 1) - 160.f, 0.f), D3DXVECTOR3(0.5f, 0.5f, 0.f), __T("InGame"), __T("UI"), 10));
	CObjMgr::Get_Instance()->Add_Object(OBJ::CASTLE, CAbstractFactroy<CCastle>::Create(D3DXVECTOR3(fCastleX, (WINCY>>1) - 110.f, 0.f), D3DXVECTOR3(0.5f, 0.5f, 0.f), __T("UI"), __T("Leader"), 3));
	CObjMgr::Get_Instance()->Add_Object(OBJ::CASTLE, CAbstractFactroy<CClock>::Create(D3DXVECTOR3(fCastleX, (WINCY>>1) - 200.f, 0.f), D3DXVECTOR3(0.8f,0.8f,0.f)));

	//우측
	CObjMgr::Get_Instance()->Add_Object(OBJ::WALL, CAbstractFactroy<CDefenseWall>::Create(D3DXVECTOR3(fCastleX + (TILECX >> 1) * 11.f , (TILECY >> 1) * 7.f, 0.f), D3DXVECTOR3(-0.4f, 0.4f, 0.f), __T("InGame"), __T("Wall"), 1));
	CObjMgr::Get_Instance()->Add_Object(OBJ::WALL, CAbstractFactroy<CDefenseWall>::Create(D3DXVECTOR3(fCastleX + (TILECX >> 1) * 20.f, (TILECY >> 1) * 7.f, 0.f), D3DXVECTOR3(0.4f, 0.4f, 0.f), __T("InGame"), __T("Wall"), 0));
	CObjMgr::Get_Instance()->Add_Object(OBJ::WALL, CAbstractFactroy<CDefenseWall>::Create(D3DXVECTOR3(fCastleX + (TILECX >> 1) * 29.f, (TILECY >> 1) * 7.f, 0.f), D3DXVECTOR3(0.4f, 0.4f, 0.f), __T("InGame"), __T("Wall"), 0));
	//좌측
	CObjMgr::Get_Instance()->Add_Object(OBJ::WALL, CAbstractFactroy<CDefenseWall>::Create(D3DXVECTOR3(fCastleX - (TILECX >> 1) * 11.f, (TILECY >> 1) * 7.f, 0.f), D3DXVECTOR3(0.4f, 0.4f, 0.f), __T("InGame"), __T("Wall"), 1));
	CObjMgr::Get_Instance()->Add_Object(OBJ::WALL, CAbstractFactroy<CDefenseWall>::Create(D3DXVECTOR3(fCastleX - (TILECX >> 1) * 20.f, (TILECY >> 1) * 7.f, 0.f), D3DXVECTOR3(0.4f, 0.4f, 0.f), __T("InGame"), __T("Wall"), 0));
	CObjMgr::Get_Instance()->Add_Object(OBJ::WALL, CAbstractFactroy<CDefenseWall>::Create(D3DXVECTOR3(fCastleX - (TILECX >> 1) * 29.f , (TILECY >> 1) * 7.f, 0.f), D3DXVECTOR3(0.4f, 0.4f, 0.f), __T("InGame"), __T("Wall"), 0));


	//가방, 묘지안의 카드 갯수
	CObjMgr::Get_Instance()->Add_Object(OBJ::UI, CAbstractFactroy<CUi>::Create(D3DXVECTOR3(80.f, 630.f, 0.f), D3DXVECTOR3(1.f, 1.f, 0.f), __T("InGame"), __T("UI"), 13));
	CObjMgr::Get_Instance()->Add_Object(OBJ::UI, CAbstractFactroy<CUi>::Create(D3DXVECTOR3(1200.f, 630.f, 0.f), D3DXVECTOR3(1.f, 1.f, 0.f), __T("InGame"), __T("UI"), 13));
}

void CStage::Add_Gold()
{
	m_fFPSTime += CTimeMgr::Get_Instance()->Get_DeltaTime();
	if (1.f <= m_fFPSTime)
	{
		++m_fGameTime;
		++m_fCreateWave;
		m_tStage.m_iTexTime += 1;
		m_fFPSTime = 0.f;
		if (m_tStage.m_iTexTime >= TEXTIME)
		{
			m_tStage.m_iGold += m_tStage.m_iTex;
			CObjMgr::Get_Instance()->Add_Object(OBJ::GOLD, CAbstractFactroy<CAddGold>::Create((float)m_tStage.m_iTex));
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::UI);
			CSoundMgr::Get_Instance()->PlaySound(L"Gold.wav", CSoundMgr::UI);
			m_tStage.m_iTexTime = 0;
		}

		if (m_fCreateWave >= 3.f)
		{
			m_bCreateWave = true;
			m_fCreateWave = 0.f;
		}

		if (!m_bStageStart)
		{
			if (m_fGameTime >= 2.f)
			{
				m_fGameTime = 0.f;
				//m_bStageStart = true;
			}
		}
	}
}
