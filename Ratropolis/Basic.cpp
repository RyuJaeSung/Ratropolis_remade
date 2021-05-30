#include "stdafx.h"
#include "Basic.h"
#include "ObjMgr.h"
#include "TimeMgr.h"
#include "TextureMgr.h"
#include "DefenseWall.h"
#include "ScrollMgr.h"
#include "CradMgr.h"
#include "Graphic_Device.h"

CBasic::CBasic()
	:m_bStartPos(false), m_eNewState(PLAYER::STATE_END), m_eNowState(PLAYER::STATE_END), m_fDetection(150.f), m_bState(true), m_bAttackTime(true)
{
	m_pDevice = nullptr;
}


CBasic::~CBasic()
{
}

HRESULT CBasic::Initialize()
{
	CRADINFO* crad = CCradMgr::Get_Instance()->Get_Crad(__T("Basic"));
	m_tUnitInfo = (*crad);
	m_tInfo.m_fSpeed = 10.f;
	m_pDevice = CGraphic_Device::Get_Instance();
	m_eNewState = PLAYER::MOVE;

	if (m_tInfo.m_fAngle)
		m_tInfo.vSize.x *= -1.f;

	return S_OK;
}

int CBasic::Update()
{
	if (m_tUnitInfo.m_iHp <= 0)
		return OBJ::DEAD;

	
	Add_Attack();
	State();

	return OBJ::LIVE;
}

void CBasic::Late_Update()
{
}

void CBasic::Render()
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
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + fScrollX, m_tInfo.vPos.y, 0.f);

		m_tInfo.matWorld = matScale * matTrans;

		m_pDevice->Get_Sprite()->SetTransform(&m_tInfo.matWorld);
		m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
		//¹°°¡
		D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, -m_tInfo.vSize.y, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + fScrollX, m_tInfo.vPos.y + fCenterX - 90.f, 0.f);

		m_tInfo.matWorld = matScale * matTrans;

		m_pDevice->Get_Sprite()->SetTransform(&m_tInfo.matWorld);
		m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(120, 255, 255, 255));
	}
}

void CBasic::Release()
{
}

void CBasic::Add_Attack()
{
	float fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float fBest = 9999999;
	CObj* pObj = nullptr;
	CObj* pWall = nullptr;
	float fAttack = 0.f;
	for (auto& iter : CObjMgr::Get_Instance()->Get_Object(OBJ::WALL))
	{

		if (dynamic_cast<CDefenseWall*>(iter)->Get_WallState() == 0)
			continue;

		float fAttack = fabsf(iter->Get_Pos().x - m_tInfo.vPos.x);

		if (fBest > fAttack)
		{
			fBest = fAttack;
			pObj = iter;
			pWall = iter;
		}
	}

	for (int i = 0; i < PLAYER::SOLDER_END; ++i)
	{
		for (auto& Unit : dynamic_cast<CDefenseWall*>(pWall)->Get_SoilderList((PLAYER::SOIDIER)i))
		{
			fAttack = fabsf(Unit->Get_Pos().x - m_tInfo.vPos.x);

 			if (fBest >= fAttack)
			{
				fBest = fAttack;
				pObj = Unit;
			}
		}
	}
	if (m_bAttackTime)
	{
		if (fBest <= 80.f)
		{
			pObj->Set_Damge(m_tUnitInfo.m_iAtt);
			m_eNewState = PLAYER::ATTACK;
			m_bAttackTime = false;
		}
	}

	if (pObj->Get_iHp() <= 0)
	{
		m_eNewState = PLAYER::MOVE;
		m_bAttackTime = true;
	}
}

void CBasic::State()
{
	if (m_eNewState != m_eNowState)
	{
		switch (m_eNewState)
		{
		case PLAYER::MOVE:
			m_wstrStateKey = __T("BasicMove");
			m_dwCount = 0;
			break;
		case PLAYER::ATTACK:
			m_wstrStateKey = __T("BasicAttack");
			m_dwCount = 0;
			break;
		}
		m_eNowState = m_eNewState;
	}


	switch (m_eNowState)
	{
	case PLAYER::MOVE:
		Move();
		break;
	case PLAYER::ATTACK:
		Attack();
		break;
	}
}

void CBasic::Move()
{
	int iDir = 1;
	if (!m_tInfo.m_fAngle)
		iDir = -1;

		m_tFrame.fStartFrame += CTimeMgr::Get_Instance()->Get_DeltaTime();
	if (m_tFrame.fStartFrame >= 0.2f)
	{
		++m_fTime;
		m_tFrame.fStartFrame = 0.f;

		if (m_dwCount == 0)
			m_dwCount = 1;
		else
			m_dwCount = 0;

		m_tInfo.vPos.x += m_tInfo.m_fSpeed * iDir;
	}
}

void CBasic::Attack()
{
	m_tFrame.fStartFrame += CTimeMgr::Get_Instance()->Get_DeltaTime();
	if (m_tFrame.fStartFrame >= 0.2f)
	{
		++m_fTime;
		m_tFrame.fStartFrame = 0.f;

		if (m_dwCount > 2)
			m_dwCount = 0;
		else if (m_dwCount == 2)
		{
			m_bAttackTime = true;
			++m_dwCount;
		}
		else
			++m_dwCount;

	}
}
