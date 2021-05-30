#include "stdafx.h"
#include "CharacterSelect.h"
#include "TextureMgr.h"
#include "SceneMgr.h"
#include "ObjMgr.h"
#include "Ui.h"
#include "KeyMgr.h"
#include "Collision.h"
#include "AbstractFactory.h"
#include "Graphic_Device.h"
#include "SoundMgr.h"


CCharacterSelect::CCharacterSelect()
{
	m_pDevice = nullptr;
	ZeroMemory(&m_tStart, sizeof(FONT));
	ZeroMemory(&m_tBack, sizeof(FONT));
}


CCharacterSelect::~CCharacterSelect()
{
	Release();
}

HRESULT CCharacterSelect::Initialize() 
{
	m_pDevice = CGraphic_Device::Get_Instance();
	swprintf_s(m_tStart.m_szBuffer, __T("Start"));
	swprintf_s(m_tBack.m_szBuffer, __T("Back"));
	m_tStart.m_bCheck = false;
	m_tBack.m_bCheck = false;
	SetRect(&m_tStart.m_rSize, 1090, 630, 1210, 660);
	SetRect(&m_tBack.m_rSize, 80, 630, 180, 660);

	Add_UI();
	return S_OK;
}	

void CCharacterSelect::Update()
{
	POINT pMouse = {};
	GetCursorPos(&pMouse);
	ScreenToClient(g_hWnd, &pMouse);

	if (CCollision::PosRectCkeck(pMouse, m_tStart.m_rSize))
		m_tStart.m_bCheck = true;
	else
		m_tStart.m_bCheck = false;

	if (CCollision::PosRectCkeck(pMouse, m_tBack.m_rSize))
		m_tBack.m_bCheck = true;
	else
		m_tBack.m_bCheck = false;

	Key_Check(pMouse);
}

void CCharacterSelect::Late_Update()
{
}

void CCharacterSelect::Render()
{
	BackGround_Render();
	Check_Render();
	Font_Render();
	Explanation_Render();

	for (auto& iter : CObjMgr::Get_Instance()->Get_Object(OBJ::UI))
		iter->Render();
	for (auto& iter : CObjMgr::Get_Instance()->Get_Object(OBJ::LEADER))
		iter->Render();
}

void CCharacterSelect::Release()
{
	CObjMgr::Get_Instance()->Clear_Object(OBJ::UI);
	CObjMgr::Get_Instance()->Clear_Object(OBJ::EFFECT);
	CObjMgr::Get_Instance()->Clear_Object(OBJ::LEADER);
}


void CCharacterSelect::Key_Check(POINT& pMouse)
{
	for (int i = 0; i < 3; ++i)
	{
		if (CCollision::PosRectCkeck(pMouse, CTextureMgr::Get_Instance()->Get_Rect(D3DXVECTOR3(670.f + (250.f *i), 120.f, 0.f), __T("UI"), __T("CharacterSelect"), i)) && CKeyMgr::Get_Instance()->KeyDown(KEY_LBUTTON))
		{
			m_ePlayerType = (PLAYER::TYPE)i;
			CObjMgr::Get_Instance()->Clear_Object(OBJ::LEADER);
			CObjMgr::Get_Instance()->Add_Object(OBJ::LEADER, CAbstractFactroy<CUi>::Create(D3DXVECTOR3(750.f, 370.f, 0.f), D3DXVECTOR3(0.4f, 0.4f, 0.f), __T("Player"), __T("CharacterPassive"), m_ePlayerType));
			CObjMgr::Get_Instance()->Add_Object(OBJ::LEADER, CAbstractFactroy<CUi>::Create(D3DXVECTOR3(750.f, 520.f, 0.f), D3DXVECTOR3(0.4f, 0.4f, 0.f), __T("Player"), __T("CharacterActive"), m_ePlayerType));
			CObjMgr::Get_Instance()->Add_Object(OBJ::LEADER, CAbstractFactroy<CUi>::Create(D3DXVECTOR3(300.f, 350.f, 0.f), D3DXVECTOR3(0.3f, 0.3f, 0.f), __T("UI"), __T("Leader"), m_ePlayerType));
			return;
		}
	}

	if (m_tStart.m_bCheck &&CKeyMgr::Get_Instance()->KeyDown(KEY_LBUTTON))
	{
		CSoundMgr::Get_Instance()->StopAll();
		CSceneMgr::Get_Instance()->Scene_Change(SCENE::SCENE_STAGE);
		return;
	}

	if (m_tBack.m_bCheck && CKeyMgr::Get_Instance()->KeyDown(KEY_LBUTTON))
	{
		CObjMgr::Get_Instance()->Clear_Object(OBJ::LEADER);
		CSceneMgr::Get_Instance()->Scene_Change(SCENE::SCENE_MENU);
		return;
	}
}

void CCharacterSelect::BackGround_Render()
{
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(__T("BackGround"), __T("CharacterSelect"), 0);
	if (!pTexInfo)
	{
		ERR_MSG(__T("CharacterSelect Load Failed"));
		return;
	}
	D3DXMATRIX	matScale, matTrans, matWorld;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, float(WINCX >> 1), float(WINCY >> 1), 0.f);

	matWorld = matScale * matTrans;

	m_pDevice->Get_Sprite()->SetTransform(&matWorld);
	m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CCharacterSelect::Font_Render()
{
	D3DXMATRIX matTrans;
	D3DXMatrixTranslation(&matTrans, 1100.f, 670.f, 0.f);

	m_pDevice->Get_Sprite()->SetTransform(&matTrans);
	if (m_tStart.m_bCheck)
		m_pDevice->Get_Font()->DrawTextW(m_pDevice->Get_Sprite(), m_tStart.m_szBuffer, lstrlen(m_tStart.m_szBuffer), nullptr, 0, D3DCOLOR_ARGB(255, 255, 165, 0));
	else
		m_pDevice->Get_Font()->DrawTextW(m_pDevice->Get_Sprite(), m_tStart.m_szBuffer, lstrlen(m_tStart.m_szBuffer), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

	D3DXMatrixTranslation(&matTrans, 80.f, 670.f, 0.f);
	m_pDevice->Get_Sprite()->SetTransform(&matTrans);
	if (m_tBack.m_bCheck)
		m_pDevice->Get_Font()->DrawTextW(m_pDevice->Get_Sprite(), m_tBack.m_szBuffer, lstrlen(m_tBack.m_szBuffer), nullptr, 0, D3DCOLOR_ARGB(255, 255, 165, 0));
	else
		m_pDevice->Get_Font()->DrawTextW(m_pDevice->Get_Sprite(), m_tBack.m_szBuffer, lstrlen(m_tBack.m_szBuffer), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CCharacterSelect::Explanation_Render()
{
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(__T("UI"), __T("CharacterSelect"), 4);

	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMATRIX	matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, 230.f, 500.f, 0.f);

	matWorld = matScale * matTrans;

	m_pDevice->Get_Sprite()->SetTransform(&matWorld);
	m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(125, 255, 255, 255));

	CObjMgr::Get_Instance()->Add_Object(OBJ::LEADER, CAbstractFactroy<CUi>::Create(D3DXVECTOR3(230.f, 500.f, 0.f), D3DXVECTOR3(1.f, 1.f, 0.f), __T("Player"), __T("ExplanationText"), m_ePlayerType));
	CObjMgr::Get_Instance()->Add_Object(OBJ::UI, CAbstractFactroy<CUi>::Create(D3DXVECTOR3(1000.f, 370.f, 0.f), D3DXVECTOR3(0.6f, 0.6f, 0.f), __T("Player"), __T("ExplanationPassive"), m_ePlayerType));
	CObjMgr::Get_Instance()->Add_Object(OBJ::UI, CAbstractFactroy<CUi>::Create(D3DXVECTOR3(1000.f, 520.f, 0.f), D3DXVECTOR3(0.6f, 0.6f, 0.f), __T("Player"), __T("ExplanationActive"), m_ePlayerType));


}

void CCharacterSelect::Check_Render()
{
	int num = 0;
	for (auto& iter : CObjMgr::Get_Instance()->Get_Object(OBJ::EFFECT))
	{
		if (num == m_ePlayerType)
		{
			iter->Render();
			return;
		}
		++num;
	}
}

void CCharacterSelect::Add_UI()
{
	for (int i = 0; i < 3; ++i)
	{//케릭터 선택창
		CObjMgr::Get_Instance()->Add_Object(OBJ::UI, CAbstractFactroy<CUi>::Create(D3DXVECTOR3(670.f + (250.f *i), 120.f, 0.f), D3DXVECTOR3(1.f, 1.f, 0.f), __T("UI"), __T("CharacterSelect"), i));
		CObjMgr::Get_Instance()->Add_Object(OBJ::EFFECT, CAbstractFactroy<CUi>::Create(D3DXVECTOR3(670.f + (250.f *i), 120.f, 0.f), D3DXVECTOR3(1.f, 1.f, 0.f), __T("Effect"), __T("CharacterSelect"), i));
	}
	//설명창(큰거)
	CObjMgr::Get_Instance()->Add_Object(OBJ::UI, CAbstractFactroy<CUi>::Create(D3DXVECTOR3(920.f, 450.f, 0.f), D3DXVECTOR3(0.5f, 0.5f, 0.f), __T("UI"), __T("CharacterSelect"), 3));
	//설명그림창(작은거)
	for (int i = 0; i < 2; ++i)
		CObjMgr::Get_Instance()->Add_Object(OBJ::UI, CAbstractFactroy<CUi>::Create(D3DXVECTOR3(750.f, 370.f + (150.f *i), 0.f), D3DXVECTOR3(0.5f, 0.5f, 0.f), __T("UI"), __T("Window"), 0));

	//지도자, 패시브, 액티브 그림
	CObjMgr::Get_Instance()->Add_Object(OBJ::LEADER, CAbstractFactroy<CUi>::Create(D3DXVECTOR3(750.f, 370.f, 0.f), D3DXVECTOR3(0.4f, 0.4f, 0.f), __T("Player"), __T("CharacterPassive"), m_ePlayerType));
	CObjMgr::Get_Instance()->Add_Object(OBJ::LEADER, CAbstractFactroy<CUi>::Create(D3DXVECTOR3(750.f, 520.f, 0.f), D3DXVECTOR3(0.4f, 0.4f, 0.f), __T("Player"), __T("CharacterActive"), m_ePlayerType));
	CObjMgr::Get_Instance()->Add_Object(OBJ::LEADER, CAbstractFactroy<CUi>::Create(D3DXVECTOR3(300.f, 350.f, 0.f), D3DXVECTOR3(0.3f, 0.3f, 0.f), __T("UI"), __T("Leader"), m_ePlayerType));

}