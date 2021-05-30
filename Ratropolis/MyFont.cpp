#include "stdafx.h"
#include "MyFont.h"
#include "Graphic_Device.h"


CMyFont::CMyFont()
{
	m_pDevice = nullptr;
}


CMyFont::~CMyFont()
{
	Release();
}

HRESULT CMyFont::Initialize()
{
	m_pDevice = CGraphic_Device::Get_Instance();
	return S_OK;
}

int CMyFont::Update()
{
	return OBJ::LIVE;
}

void CMyFont::Late_Update()
{
}

void CMyFont::Render()
{
	D3DXMATRIX matScale, matTrans;
	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	m_tInfo.matWorld = matScale * matTrans;
	m_pDevice->Get_Sprite()->SetTransform(&m_tInfo.matWorld);
	m_pDevice->Get_Font()->DrawTextW(m_pDevice->Get_Sprite(), m_wstrObjectKey.c_str(), m_wstrObjectKey.length(), nullptr, 0, m_uColor);
}

void CMyFont::Release()
{
}