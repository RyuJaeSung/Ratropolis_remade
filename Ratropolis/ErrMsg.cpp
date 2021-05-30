#include "stdafx.h"
#include "ErrMsg.h"
#include "TimeMgr.h"
#include "Graphic_Device.h"

CErrMsg::CErrMsg()
	:m_fDeleteTime(0.f), m_iRColor(0)
{
	m_pDevice = nullptr;
	ZeroMemory(&m_wstrTex, sizeof(m_wstrTex));
}


CErrMsg::~CErrMsg()
{
	Release();
}

HRESULT CErrMsg::Initialize()
{
	m_pDevice = CGraphic_Device::Get_Instance();
	m_tInfo.vPos = { float(WINCX >> 1) - 150.f, 300.f, 0.f };
	m_tInfo.vSize = { 1.f,1.f, 0.f };
	m_iRColor = 255;
	return S_OK;
}

int CErrMsg::Update()
{
 	m_fDeleteTime += CTimeMgr::Get_Instance()->Get_DeltaTime();

	if (m_fDeleteTime >= 1.f)
	{
		m_tInfo.vPos.y -= 5.f;
		m_iRColor -= 5;
	}

	if (m_fDeleteTime >= MSGDELETE)
		return OBJ::DEAD;

	return OBJ::LIVE;
}

void CErrMsg::Late_Update()
{
}

void CErrMsg::Render()
{
	D3DXMATRIX matScale, matTrans;

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	
	m_tInfo.matWorld = matScale * matTrans;

	m_pDevice->Get_Sprite()->SetTransform(&m_tInfo.matWorld);
	m_pDevice->Get_Font()->DrawTextW(m_pDevice->Get_Sprite(), m_wstrTex.c_str(), m_wstrTex.length(), nullptr, 0, D3DCOLOR_ARGB(m_iRColor, 255, 0, 0));
}

void CErrMsg::Release()
{
}
