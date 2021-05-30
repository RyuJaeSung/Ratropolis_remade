#include "stdafx.h"
#include "Reward.h"
#include "Texture.h"
#include "KeyMgr.h"
#include "ObjMgr.h"
#include "SceneMgr.h"
#include "ScrollMgr.h"
#include "CradMgr.h"
#include "SelectCrad.h"
#include "TextureMgr.h"
#include "Graphic_Device.h"

CReward::CReward()
	:m_bClickBox(false), m_iCkcekPos(5)
{
	m_pDevice = nullptr;
}


CReward::~CReward()
{
	Release();
}

HRESULT CReward::Initialize()
{
	m_pDevice = CGraphic_Device::Get_Instance();
	for (int i = 0; i < 3; ++i)
		ClickBar[i] = false;
	return S_OK;
}

int CReward::Update()
{

	if (ClickBox() && !m_bClickBox)
	{
		CObjMgr::Get_Instance()->Set_ClickCrad(true);
		m_bClickBox = true;
	}

	if (m_bClickBox)
	{
		if (KeyCkcek())
		{
			CObjMgr::Get_Instance()->Set_ClickCrad(false);
			return OBJ::DEAD;
		}
	}
	return OBJ::LIVE;
}

void CReward::Late_Update()
{
}

void CReward::Render()
{

	if (!m_bClickBox)
		Render_Box();
	else
		Render_Reward();
}

void CReward::Release()
{
}

void CReward::Render_Box()
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
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + fScrollX, m_tInfo.vPos.y + 50.f, 0.f);

		m_tInfo.matWorld = matScale * matTrans;

		m_pDevice->Get_Sprite()->SetTransform(&m_tInfo.matWorld);
		m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

		//물가
		D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, -m_tInfo.vSize.y, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + fScrollX, m_tInfo.vPos.y + fCenterY + 155.f, 0.f);

		m_tInfo.matWorld = matScale * matTrans;

		m_pDevice->Get_Sprite()->SetTransform(&m_tInfo.matWorld);
		m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(80, 120, 255, 255));
	}
}

void CReward::Render_Reward()
{
	Render_Window();
	Render_Menu();
}

void CReward::Render_Menu()
{
	const TEXINFO* pTexInfo = nullptr;
	D3DXMATRIX	matScale, matTrans;
	float fCenterX = 0.f;
	float fCenterY = 0.f;

	for (int i = 0; i < 3; ++i)
	{
		if (m_iCkcekPos == i)
		{
			pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(__T("Effect"), __T("CharacterSelect"), 3);
			if (!pTexInfo)
				return;
			fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
			fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

			D3DXMatrixScaling(&matScale, 0.8f, 0.8f, 0.f);
			D3DXMatrixTranslation(&matTrans, float(WINCX >> 1) - 150.f + (150.f *i), float(WINCY >> 1), 0.f);

			int iX = (WINCX >> 1) - 150 + (150 * i);
			int iY = WINCY >> 1;

			m_tInfo.matWorld = matScale * matTrans;

			m_pDevice->Get_Sprite()->SetTransform(&m_tInfo.matWorld);
			m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
		}

		//선택창 Bar
		pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(__T("UI"), __T("Window"), 0);
		if (!pTexInfo)
			return;
		fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		D3DXMatrixScaling(&matScale, 0.7f, 0.7f, 0.f);
		D3DXMatrixTranslation(&matTrans, float(WINCX >> 1) - 150.f +(150.f *i), float(WINCY >> 1), 0.f);

		int iX = (WINCX >> 1) - 150 + (150 *i);
		int iY = WINCY >> 1;

		m_tInfo.matWorld = matScale * matTrans;

		m_pDevice->Get_Sprite()->SetTransform(&m_tInfo.matWorld);
		m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
		
		// 선택창 안 이미지
		pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(__T("InGame"), __T("UI"), 20 + i);
		if (!pTexInfo)
			return;

		fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		D3DXMatrixScaling(&matScale, 0.7f, 0.7f, 0.f);
		D3DXMatrixTranslation(&matTrans, float(WINCX >> 1) - 150.f + (150.f *i), float(WINCY >> 1), 0.f);

		m_tInfo.matWorld = matScale * matTrans;

		m_pDevice->Get_Sprite()->SetTransform(&m_tInfo.matWorld);
		m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

		//선택Bar에 대한 rect값
		SetRect(&m_rBar[i], iX -60, iY - 80, iX + 80, iY + 60);

	}
	int num = 0;
	if (m_iCkcekPos == 4)
		num = 2;
	else
		num = 1;

	//취소버튼
	pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(__T("InGame"), __T("Menu"), num);
	if (!pTexInfo)
		return;
	fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
	int iX = WINCX >> 1;
	int iY = (WINCY >> 1) + 180;

	D3DXMatrixScaling(&matScale, 2.f, 2.f, 0.f);
	D3DXMatrixTranslation(&matTrans, float(iX), float(iY), 0.f);

	m_tInfo.matWorld = matScale * matTrans;

	m_pDevice->Get_Sprite()->SetTransform(&m_tInfo.matWorld);
	m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	SetRect(&m_rQuit, int(iX - 100), int(iY - 20), int(iX + 100), int(iY + 30));

	TCHAR szBuffer[16] = {};
	swprintf_s(szBuffer, __T("취 소"));

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, float(WINCX >> 1)- 50.f, float(WINCY >> 1) + 160.f, 0.f);

	m_tInfo.matWorld = matScale * matTrans;

	m_pDevice->Get_Sprite()->SetTransform(&m_tInfo.matWorld);
	m_pDevice->Get_Font()->DrawTextW(m_pDevice->Get_Sprite(), szBuffer, lstrlen(szBuffer), nullptr, 0, D3DCOLOR_ARGB(255,0,0, 0));


}

void CReward::Render_Window()
{
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(__T("InGame"), __T("Menu"), 0);
	if (!pTexInfo)
		return;
	D3DXMATRIX	matScale, matTrans;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMatrixScaling(&matScale, 0.5f, 0.5f, 0.f);
	D3DXMatrixTranslation(&matTrans, float(WINCX>>1), float(WINCY>>1), 0.f);

	m_tInfo.matWorld = matScale * matTrans;

	m_pDevice->Get_Sprite()->SetTransform(&m_tInfo.matWorld);
	m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

bool CReward::ClickBox()
{
	float fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	POINT ps = {};
	GetCursorPos(&ps);
	ScreenToClient(g_hWnd, &ps);

	SetRect(&m_rBox, int(m_tInfo.vPos.x + fScrollX -60.f) , int(m_tInfo.vPos.y + 10.f), int(m_tInfo.vPos.x + fScrollX + 60.f), int(m_tInfo.vPos.y + 110.f));

	if (PtInRect(&m_rBox, ps) && CKeyMgr::Get_Instance()->KeyDown(KEY_LBUTTON))
		return true;
	return false;
}

bool CReward::KeyCkcek()
{
	POINT ps = {};
	GetCursorPos(&ps);
	ScreenToClient(g_hWnd, &ps);
	for (int i = 0; i < 3; ++i)
	{
		if (PtInRect(&m_rBar[i], ps))
		{
			m_iCkcekPos = i;
			if (CKeyMgr::Get_Instance()->KeyDown(KEY_LBUTTON))
			{
				switch (i)
				{
				case 0:
					CObjMgr::Get_Instance()->Add_Object(OBJ::CRAD, CAbstractFactroy<CSelectCrad>::Create());
					break;
				case 1:
					CCradMgr::Get_Instance()->Get_ListBag().emplace_back(CCradMgr::Get_Instance()->Get_Crad(__T("House")));
					break;
				case 2:
					CSceneMgr::Get_Instance()->Get_Stage().m_iGold += (CSceneMgr::Get_Instance()->Get_Stage().m_iWave * 30);
					break;
				}
				return true;
			}
			return false;
		}
	}

	if (PtInRect(&m_rQuit, ps))
	{
		m_iCkcekPos = 4;
		if (CKeyMgr::Get_Instance()->KeyDown(KEY_LBUTTON))
			return true;
		return false;
	}

	m_iCkcekPos = 5;
	return false;
}
