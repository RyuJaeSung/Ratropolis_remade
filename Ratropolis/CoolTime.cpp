#include "stdafx.h"
#include "CoolTime.h"
#include "TimeMgr.h"
#include "SceneMgr.h"
#include "TextureMgr.h"
#include "ErrMsg.h"
#include "ObjMgr.h"
#include "AddGold.h"
#include "AbstractFactory.h"
#include "Graphic_Device.h"

CCoolTime::CCoolTime()
	:m_iGold(0), m_iPop(0), m_iTime(0), m_fTurn(false)
{
	m_pDevice = nullptr;
	ZeroMemory(&m_tFrame, sizeof(m_tFrame));
}

CCoolTime::~CCoolTime()
{
	Release();
}

HRESULT CCoolTime::Initialize()
{
	if (CSceneMgr::Get_Instance()->Get_Stage().m_iPop - m_iPop <= 0)
	{
		TCHAR szBuf[32] = {};
		swprintf_s(szBuf, __T("인구수가 모자릅니다."));
		CObjMgr::Get_Instance()->Add_Object(OBJ::CRAD, CAbstractFactroy<CErrMsg>::Create(szBuf));
		return E_FAIL;
	}

	m_pDevice = CGraphic_Device::Get_Instance();
	m_tFrame.fStartFrame = 0.f;
	m_tInfo.m_fAngle = 0.f;
	m_tInfo.vDir = { 1.f, 0.f, 0.f };
	m_fTurnAngle = 360.f / m_tFrame.fMaxFrame;

	CSceneMgr::Get_Instance()->Get_Stage().m_iPop -= m_iPop;
	return S_OK;
}

int CCoolTime::Update()
{

	m_tFrame.fStartFrame += CTimeMgr::Get_Instance()->Get_DeltaTime();
	m_tInfo.m_fAngle += m_fTurnAngle * CTimeMgr::Get_Instance()->Get_DeltaTime();

	if (1.f <= m_tFrame.fStartFrame)
	{
		++m_iTime;
		m_tFrame.fStartFrame = 0.f;
	}

	if (m_iTime >= m_tFrame.fMaxFrame)
	{
		Option_Type();
		CSceneMgr::Get_Instance()->Get_Stage().m_iPop += m_iPop;
		CObjMgr::Get_Instance()->Delete_CoolTimeCount();
		return OBJ::DEAD;
	}

	return OBJ::LIVE;
}

void CCoolTime::Late_Update()
{
	if (m_tInfo.m_fAngle >= 180.f)
		m_fTurn = true;
	else
		m_fTurn = false;
}

void CCoolTime::Render()
{
	Render_CoolTimeBar();
	Render_Type();
}

void CCoolTime::Release()
{
}

void CCoolTime::Option_Type()
{
	switch (m_eType)
	{
	case CCoolTime::GOLD:
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::UI);
		CSoundMgr::Get_Instance()->PlaySound(L"Gold.wav", CSoundMgr::UI);
		CObjMgr::Get_Instance()->Add_Object(OBJ::GOLD, CAbstractFactroy<CAddGold>::Create(60.f));
		CSceneMgr::Get_Instance()->Get_Stage().m_iGold += 60;
		break;
	}
}

void CCoolTime::Render_CoolTimeBar()
{
	const TEXINFO* pTexInfo_Bar = CTextureMgr::Get_Instance()->Get_TexInfo(__T("InGame"), __T("CoolTime"), 0);
	if (!pTexInfo_Bar)
		return;
	const TEXINFO* pTexInfo_Back = CTextureMgr::Get_Instance()->Get_TexInfo(__T("InGame"), __T("CoolTime"), 1);
	if (!pTexInfo_Back)
		return;

	if (!m_fTurn) //맨처음 시작할때
	{
		Render_TopBack(pTexInfo_Back);
		Render_TopBar(pTexInfo_Bar);
		Render_BottomBar(pTexInfo_Bar);
	}
	else //180도가 지나간 시점
	{
		Render_BottomBack(pTexInfo_Back);
		Render_BottomBar(pTexInfo_Bar);
		Render_TopBack(pTexInfo_Back);

	}
}

void CCoolTime::Render_Type()
{

	const TEXINFO* pTexInfo = nullptr;
	switch (m_eType)
	{
	case CCoolTime::GOLD:
		pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(__T("InGame"), __T("CoolTime"), 2);
		break;
	}
	if (!pTexInfo)
		return;

	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMATRIX matScale, matTrains;
	D3DXMatrixScaling(&matScale, 0.5f, 0.5f, 0.f);
	D3DXMatrixTranslation(&matTrains, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	m_tInfo.matWorld = matScale * matTrains;
	m_pDevice->Get_Sprite()->SetTransform(&m_tInfo.matWorld);
	m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CCoolTime::Render_TopBar(const TEXINFO * pTexInfo)
{
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height);

	D3DXMATRIX matScale, matRotZ, matTrains;
	D3DXMatrixScaling(&matScale, 0.7f, 0.7f, 0.f);
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(m_tInfo.m_fAngle));
	D3DXMatrixTranslation(&matTrains, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	m_tInfo.matWorld = matScale * matRotZ * matTrains;

	m_pDevice->Get_Sprite()->SetTransform(&m_tInfo.matWorld);
	m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CCoolTime::Render_TopBack(const TEXINFO * pTexInfo)
{
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height);

	D3DXMATRIX matScale, matRotZ, matTrains;
	D3DXMatrixScaling(&matScale, 0.7f, 0.7f, 0.f);
	D3DXMatrixTranslation(&matTrains, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	m_tInfo.matWorld = matScale * matTrains;
	m_pDevice->Get_Sprite()->SetTransform(&m_tInfo.matWorld);
	m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CCoolTime::Render_BottomBar(const TEXINFO * pTexInfo)
{
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height);

	D3DXMATRIX matScale, matRotZ, matTrains;
	D3DXMatrixScaling(&matScale, 0.7f, -0.7f, 0.f);
	D3DXMatrixTranslation(&matTrains, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	if(!m_fTurn)
		m_tInfo.matWorld = matScale * matTrains;
	else
	{
		D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(m_tInfo.m_fAngle + 180.f));
		m_tInfo.matWorld = matScale * matRotZ * matTrains;
	}
	m_pDevice->Get_Sprite()->SetTransform(&m_tInfo.matWorld);
	m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CCoolTime::Render_BottomBack(const TEXINFO * pTexInfo)
{
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height);

	D3DXMATRIX matScale, matRotZ, matTrains;
	D3DXMatrixScaling(&matScale, 0.7f, -0.7f, 0.f);
	D3DXMatrixTranslation(&matTrains, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	m_tInfo.matWorld = matScale * matTrains;
	m_pDevice->Get_Sprite()->SetTransform(&m_tInfo.matWorld);
	m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}