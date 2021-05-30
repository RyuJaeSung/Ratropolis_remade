#include "stdafx.h"
#include "SelectCrad.h"
#include "CradMgr.h"
#include "KeyMgr.h"
#include "TextureMgr.h"
#include "Graphic_Device.h"


CSelectCrad::CSelectCrad()
	:m_iCkcekPos(5)
{
	m_pDevice = nullptr;
}


CSelectCrad::~CSelectCrad()
{
}

HRESULT CSelectCrad::Initialize()
{
	m_pDevice = CGraphic_Device::Get_Instance();
	ZeroMemory(&m_tCradInfo, sizeof(m_tCradInfo));
	RandomCrad();
	return S_OK;
}

int CSelectCrad::Update()
{
	if (KeyCkeck())
		return OBJ::DEAD;
	return OBJ::LIVE;
}

void CSelectCrad::Late_Update()
{
}

void CSelectCrad::Render()
{
	Render_Window();
	Render_Crad();
}

void CSelectCrad::Release()
{
}

bool CSelectCrad::KeyCkeck()
{
	POINT ps;
	GetCursorPos(&ps);
	ScreenToClient(g_hWnd, &ps);

	for (int i = 0; i < 3; ++i)
	{
		if (PtInRect(&m_rCrad[i], ps))
		{
			m_iCkcekPos = i;
			if (CKeyMgr::Get_Instance()->KeyDown(KEY_LBUTTON))
			{
				CCradMgr::Get_Instance()->Get_ListBag().emplace_back(m_tCradInfo[i]);
				return true;
			}
			return false;
		}
		else
			m_iCkcekPos = 5;
	}

	if (PtInRect(&m_rQuit, ps))
	{
		m_iCkcekPos = 4;

		if (CKeyMgr::Get_Instance()->KeyDown(KEY_LBUTTON))
		{
			return true;
		}
		return false;
	}
	else
		m_iCkcekPos = 5;
	return false;
}

void CSelectCrad::RandomCrad()
{
	int iRend = 0;
	for (int i = 0; i < 3; ++i)
	{
		CRAD::TYPE eRandom = CRAD::TYPE(rand() % 2);

		switch (eRandom)
		{
		case CRAD::ECONOMY_CRAD:
			m_wstrKey[i] = __T("Economy");
			iRend = rand() % 3;
			switch (iRend)
			{
			case CRAD::GRAIN:
				m_tCradInfo[i] = CCradMgr::Get_Instance()->Get_Crad(__T("Grain"));
				break;
			case CRAD::CHEESE:
				m_tCradInfo[i] = CCradMgr::Get_Instance()->Get_Crad(__T("Cheese"));
				break;
			case CRAD::POTTER:
				m_tCradInfo[i] = CCradMgr::Get_Instance()->Get_Crad(__T("Potter"));
				break;
			case CRAD::BUILDINGTAX:
				m_tCradInfo[i] = CCradMgr::Get_Instance()->Get_Crad(__T("BuildingTex"));
				break;
			}
			break;
		case CRAD::MILITARY_CRAD:
			m_wstrKey[i] = __T("Military");
			m_tCradInfo[i] = CCradMgr::Get_Instance()->Get_Crad(__T("Militia"));
			break;
		case CRAD::BUILDING_CRAD:
			m_wstrKey[i] = __T("Building");
			iRend = rand() % 6;
			switch (iRend)
			{
			case CRAD::HOUSE:
				m_tCradInfo[i] = CCradMgr::Get_Instance()->Get_Crad(__T("House"));
				break;
			case CRAD::DEFENSEWALL:
				m_tCradInfo[i] = CCradMgr::Get_Instance()->Get_Crad(__T("DefenseWall"));
				break;
			case CRAD::FACTORY:
				m_tCradInfo[i] = CCradMgr::Get_Instance()->Get_Crad(__T("Factory"));
				break;
			case CRAD::MONASTERY:
				m_tCradInfo[i] = CCradMgr::Get_Instance()->Get_Crad(__T("Monastery"));
				break;
			case CRAD::BARN:
				m_tCradInfo[i] = CCradMgr::Get_Instance()->Get_Crad(__T("Barn"));
				break;
			case CRAD::APARTMENTHOUSE:
				m_tCradInfo[i] = CCradMgr::Get_Instance()->Get_Crad(__T("ApartmentHouse"));
				break;
			case CRAD::FARM:
				m_tCradInfo[i] = CCradMgr::Get_Instance()->Get_Crad(__T("Farm"));
				break;
			}
		}
		

	}
}

void CSelectCrad::Render_Crad()
{
	const TEXINFO* pTexInfo = nullptr;
	D3DXMATRIX	matScale, matTrans;
	float fCenterX = 0.f;
	float fCenterY = 0.f;

	for (int i = 0; i < 3; ++i)
	{
		if (i == m_iCkcekPos)
		{
			pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(__T("Effect"), __T("CradSelect"), 0);
			if (!pTexInfo)
				return;
			fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
			fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

			int iX = (WINCX >> 1) - 300 + (300 * i);
			int iY = WINCY >> 1;
			D3DXMatrixScaling(&matScale, 0.5f, 0.5f, 0.f);
			D3DXMatrixTranslation(&matTrans, float(WINCX >> 1) - 300.f + (300.f * i), float(WINCY >> 1), 0.f);

			m_tInfo.matWorld = matScale * matTrans;

			m_pDevice->Get_Sprite()->SetTransform(&m_tInfo.matWorld);
			m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

		}
		

		pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(__T("Crad"), m_wstrKey[i], m_tCradInfo[i]->m_iImageNumber);
		if (!pTexInfo)
			return;
		fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		int iX = (WINCX >> 1) - 300 + (300 * i);
		int iY = WINCY >> 1;
		D3DXMatrixScaling(&matScale, 0.5f, 0.5f, 0.f);
		D3DXMatrixTranslation(&matTrans, float(WINCX >> 1) - 300.f + (300.f * i), float(WINCY >> 1), 0.f);

		m_tInfo.matWorld = matScale * matTrans;

		m_pDevice->Get_Sprite()->SetTransform(&m_tInfo.matWorld);
		m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

		SetRect(&m_rCrad[i], int(iX - 100.f ), int(iY - 160.f ), int(iX + 100.f ), int(iY + 120.f));
	}

}

void CSelectCrad::Render_Window()
{
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(__T("InGame"), __T("Menu"), 0);
	if (!pTexInfo)
		return;
	D3DXMATRIX	matScale, matTrans;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMatrixScaling(&matScale, 0.8f, 0.8f, 0.f);
	D3DXMatrixTranslation(&matTrans, float(WINCX >> 1), float(WINCY >> 1), 0.f);

	m_tInfo.matWorld = matScale * matTrans;

	m_pDevice->Get_Sprite()->SetTransform(&m_tInfo.matWorld);
	m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	//취소버튼
	int num = 0;
	if (m_iCkcekPos == 4)
		num = 2;
	else
		num = 1;

	pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(__T("InGame"), __T("Menu"), num);
	if (!pTexInfo)
		return;
	fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
	int iX = WINCX >> 1;
	int iY = (WINCY>>1) + 300;

	D3DXMatrixScaling(&matScale, 2.f, 2.f, 0.f);
	D3DXMatrixTranslation(&matTrans, float(iX), float(iY), 0.f);

	m_tInfo.matWorld = matScale * matTrans;

	m_pDevice->Get_Sprite()->SetTransform(&m_tInfo.matWorld);
	m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	TCHAR szBuffer[16] = {};
	swprintf_s(szBuffer, __T("취 소"));


	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, float(WINCX >> 1) - 50.f, float(WINCY >> 1) + 280.f, 0.f);

	m_tInfo.matWorld = matScale * matTrans;

	m_pDevice->Get_Sprite()->SetTransform(&m_tInfo.matWorld);
	m_pDevice->Get_Font()->DrawTextW(m_pDevice->Get_Sprite(), szBuffer, lstrlen(szBuffer), nullptr, 0, D3DCOLOR_ARGB(255, 0, 0, 0));

	SetRect(&m_rQuit, int(iX - 100), int(iY - 30), int(iX + 100), int(iY + 30));
}
