#include "stdafx.h"
#include "CradList.h"
#include "TextureMgr.h"
#include "KeyMgr.h"
#include "CradMgr.h"
#include "ObjMgr.h"
#include "Graphic_Device.h"


CCradList::CCradList()
	:m_eType(TYPE::END), m_pDevice(nullptr), m_ButtonCkeck(false),m_Execution(false)
{
}


CCradList::~CCradList()
{
	Release();
}
void CCradList::Initialize()
{	//창의 크기 1208 x 814
	m_pDevice = CGraphic_Device::Get_Instance();
	m_vPos = { float(WINCX >> 1), float(WINCY >> 1), 0.f };
	m_vSize = { 0.8f, 0.8f, 0.8f };
	//버튼의 크기 116 x 106
	m_vButtonPos = { 1090.f,  70.f, 0.f };
	m_vButtonSize = { 0.5f, 0.5f, 0.f };
	SetRect(&m_rButtonRect, int(m_vButtonPos.x - 40), int(m_vButtonPos.y - 30), int(m_vButtonPos.x + 10), int(m_vButtonPos.y + 15));
	//카드의 나열의 크기
	m_vCradPos = { 295.f, 190.f, 0.f };
	m_vCradSize = { 0.3f, 0.3f, 0.3f };
}

void CCradList::Update()
{
	if (m_Execution)
	{
		if (m_eType != TYPE::END)
		{
			switch (m_eType)
			{
			case CCradList::BAG:
				ListCrad = CCradMgr::Get_Instance()->Get_ListBag();
				break;
			case CCradList::CEMETRY:
				ListCrad = CCradMgr::Get_Instance()->Get_ListCemetry();
				break;
			}
			m_eType = TYPE::END;
		}

		//Button  58 / 53
		POINT pMouse = {};
		GetCursorPos(&pMouse);
		ScreenToClient(g_hWnd, &pMouse);

		if (PtInRect(&m_rButtonRect, pMouse))
		{
			m_ButtonCkeck = true;
			if (CKeyMgr::Get_Instance()->KeyDown(KEY_LBUTTON))
				m_Execution = false;
		}
		else
			m_ButtonCkeck = false;
	}
}

void CCradList::Render()
{
	if (m_Execution)
	{
		Render_Window();
		Render_Button();
		Render_Crad();
	}
}

void CCradList::Release()
{
}

void CCradList::Render_Window()
{
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(__T("InGame"), __T("Quest"), 0);
	if (!pTexInfo)
		return;

	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMATRIX matSacle, matTrans, matWorld;
	D3DXMatrixScaling(&matSacle, m_vSize.x, m_vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_vPos.x, m_vPos.y, 0.f);

	matWorld = matSacle * matTrans;
	m_pDevice->Get_Sprite()->SetTransform(&matWorld);
	m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CCradList::Render_Button()
{
	const TEXINFO* pTexInfo = nullptr;
	if (m_ButtonCkeck)
		pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(__T("InGame"), __T("Quest"), 2);
	else
		pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(__T("InGame"), __T("Quest"), 1);

	if (!pTexInfo)
		return;

	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMATRIX matSacle, matTrans, matWorld;
	D3DXMatrixScaling(&matSacle, m_vButtonSize.x, m_vButtonSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_vButtonPos.x, m_vButtonPos.y, 0.f);

	matWorld = matSacle * matTrans;
	m_pDevice->Get_Sprite()->SetTransform(&matWorld);
	m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CCradList::Render_Crad()
{
	float fX = 0.f;
	float fY = 0.f;
	TCHAR szBuffer[32] = {};
	int iCount = 0;
	for (auto& iter : ListCrad)
	{
		switch (iter->m_dwType)
		{
		case 0:
			swprintf_s(szBuffer, __T("Economy"));
			break;
		case 1:
			swprintf_s(szBuffer, __T("Military"));
			break;
		case 2:
			swprintf_s(szBuffer, __T("Building"));
			break;
		case 3:
			swprintf_s(szBuffer, __T("Technology"));
			break;
		}

		const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(__T("Crad"),szBuffer,iter->m_iImageNumber);
		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
		if (iCount  == 8)
		{
			fX = 0.f;
			fY += 150.f;
			iCount = 0;
		}

		fX = 100.f * iCount;
		


		D3DXMATRIX matSacle, matTrans, matWorld;
		D3DXMatrixScaling(&matSacle, m_vCradSize.x, m_vCradSize.y, 0.f);
		D3DXMatrixTranslation(&matTrans, m_vCradPos.x + fX, m_vCradPos.y + fY, 0.f);

		matWorld = matSacle * matTrans;
		m_pDevice->Get_Sprite()->SetTransform(&matWorld);
		m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
		++iCount;
	}
}