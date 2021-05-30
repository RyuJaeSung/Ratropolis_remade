#include "stdafx.h"
#include "Arrow.h"
#include "TimeMgr.h"
#include "TextureMgr.h"	
#include "Graphic_Device.h"
#include "ScrollMgr.h"

CArrow::CArrow()
	:m_eDir(DIR::END), m_bDelete_Target(false)
{
	m_pDevice = nullptr;
}


CArrow::~CArrow()
{
}

HRESULT CArrow::Initialize()
{
	m_pDevice = CGraphic_Device::Get_Instance();
	m_tInfo.vSize = { 0.2f, 0.2f, 0.f };
	m_vStartPos = m_tInfo.vPos;
	m_fMaxHeight = 200.f;
	m_fTime = 0.f;
	m_fMaxTime = 5.5f;
	Parabola();

	if (m_vStartPos.x < m_pTarget->Get_Pos().x)
	{
		m_tInfo.m_fAngle = 45.f;
		m_eDir = DIR::RIGHT;
	}
	else
	{
		m_tInfo.m_fAngle = -45.f;
		m_eDir = DIR::LEFT;
	}
	return S_OK;
}

int CArrow::Update()
{
	Move();
	if (Attack())
	{
		m_pTarget->Set_Damge(m_iDamage);
		return OBJ::DEAD;
	}

	if (m_bDelete_Target)
		return OBJ::DEAD;

	return OBJ::LIVE;
}

void CArrow::Late_Update()
{
}

void CArrow::Render()
{
	float fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(__T("InGame"), __T("UI"), 23);
	if (!pTexInfo)
		return;
	D3DXMATRIX	matScale, matRotZ, matTrans;
	float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(m_tInfo.m_fAngle));
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + fScrollX, m_tInfo.vPos.y, 0.f);

	m_tInfo.matWorld = matScale* matRotZ * matTrans;

	m_pDevice->Get_Sprite()->SetTransform(&m_tInfo.matWorld);
	m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	//¹°°¡
	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, -m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + fScrollX, m_tInfo.vPos.y + fCenterY + 80.f, 0.f);

	m_tInfo.matWorld = matScale * matTrans;

	m_pDevice->Get_Sprite()->SetTransform(&m_tInfo.matWorld);
	m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(120, 255, 255, 255));
	
}

void CArrow::Release()
{
}

void CArrow::Parabola()
{
 	m_fEndHeight = m_vStartPos.y - m_pTarget->Get_Pos().y;
	m_fHeight = m_fMaxHeight - m_pTarget->Get_Pos().y;

	m_fFG = 2 * m_fHeight / (m_fMaxTime * m_fMaxTime);

	m_fSpeedY = sqrt(2 * m_fFG * m_fHeight);

	float a = m_fFG;
	float b = -2 * m_fSpeedY;
	float c = 2 * m_fEndHeight;

	m_fEndTime = fabsf((-b + sqrt(b * b - 4 * a * c)) / (2 * a));
	m_fSpeedX = (m_pTarget->Get_Pos().x - m_vStartPos.x) / m_fEndTime;
}

void CArrow::Move()
{
	float fDir = 0.f;
	m_fTime += 0.3f;



	if (m_fTime < m_fEndTime)
	{
		if (m_eDir == DIR::RIGHT)
		{
			m_tInfo.m_fAngle += 3.f;
			fDir = -1.f;
		}
		else
		{
			m_tInfo.m_fAngle -= 3.f;
			fDir = 1.f;
		}
		m_tInfo.vPos.x = m_vStartPos.x + m_fSpeedX * m_fTime;
		m_tInfo.vPos.y = m_vStartPos.y + (m_fSpeedX * fDir * m_fTime) - (0.5f * m_fFG * m_fTime * m_fTime);
	}
	else
		m_bDelete_Target = true;

}

bool CArrow::Attack() 
{
	float fx = m_pTarget->Get_Pos().x;
	if(m_tInfo.vPos.x - 50.f <= fx && m_tInfo.vPos.x + 50.f >= fx)
		return true;
	return false;
}
