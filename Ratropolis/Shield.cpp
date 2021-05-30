#include "stdafx.h"
#include "Shield.h"
#include "CradMgr.h"
#include "TextureMgr.h"
#include "ScrollMgr.h"
#include "Stage.h"
#include "SceneMgr.h"
#include "ObjMgr.h"
#include "Dead.h"
#include "DefenseWall.h"
#include "TimeMgr.h"
#include "Graphic_Device.h"

CShield::CShield()
	:m_fDetection(200.f), m_eNewState(PLAYER::MOVE), m_eNowState(PLAYER::STATE_END), m_iDefense(0)
{
	pTarget = nullptr;
	m_pDevice = nullptr;
}


CShield::~CShield()
{
}

HRESULT CShield::Initialize()
{
	m_pDevice = CGraphic_Device::Get_Instance();
	CRADINFO* tUnitInfo = CCradMgr::Get_Instance()->Get_Crad(__T("Shield"));
	m_tUnitInfo = (*tUnitInfo);
	dynamic_cast<CStage*>(CSceneMgr::Get_Instance()->Get_Scene())->Get_Stage().m_iPop -= m_tUnitInfo.m_iPopulation;
	m_tFrame.fMaxFrame = 20.f;
	m_tFrame.fStartFrame = 0.f;
	m_tInfo.m_fSpeed = 5.f;
	m_iSkill = 9999;
	m_fWallPosX = m_tInfo.vPos.x;
	m_iNowHp = m_tUnitInfo.m_iHp;
	return S_OK;
}

int CShield::Update()
{
	if (m_tUnitInfo.m_iHp <= 0)
	{
		CObjMgr::Get_Instance()->Add_Object(OBJ::EFFECT, CAbstractFactroy<CDead>::Create(m_tInfo.vPos, D3DXVECTOR3(0.2f, 0.2f, 0.f)));
		dynamic_cast<CStage*>(CSceneMgr::Get_Instance()->Get_Scene())->Get_Stage().m_iPop += m_tUnitInfo.m_iPopulation;
		return OBJ::DEAD;
	}
	State();

	return OBJ::LIVE;
}

void CShield::Late_Update()
{
}

void CShield::Render()
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

		//물가
		D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, -m_tInfo.vSize.y, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + fScrollX, m_tInfo.vPos.y + 170.f, 0.f);

		m_tInfo.matWorld = matScale * matTrans;

		m_pDevice->Get_Sprite()->SetTransform(&m_tInfo.matWorld);
		m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(120, 255, 255, 255));

		//방패스킬
		if (m_bDefense || m_iSkill <= 10)
		{
			pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(m_wstrObjectKey, __T("ShieldSkill"), 0);
			if (!pTexInfo)
				return;
			fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
			fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

			D3DXMatrixScaling(&matScale, 0.7f, 0.7f, 0.f);
			D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + fScrollX, m_tInfo.vPos.y, 0.f);

			m_tInfo.matWorld = matScale * matTrans;

			m_pDevice->Get_Sprite()->SetTransform(&m_tInfo.matWorld);
			m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

			++m_iSkill;
			m_bDefense = false;
		}
	}
}

void CShield::Release()
{
}

void CShield::State()
{
	Find_Target();

	if (m_eNewState != m_eNowState)
	{
		switch (m_eNewState)
		{
		case PLAYER::STAND:
			m_wstrStateKey = __T("ShieldWait");
			break;
		case PLAYER::MOVE:
			m_wstrStateKey = __T("ShieldMove");
			break;
		case PLAYER::ATTACK:
			m_wstrStateKey = __T("ShieldAttack");
			break;
		}
		m_dwCount = 0;
		m_eNowState = m_eNewState;
	}

	switch (m_eNowState)
	{
	case PLAYER::STAND:
		Stand();
		break;
	case PLAYER::MOVE:
		Move();
		break;
	case PLAYER::ATTACK:
		Defense();
		Attack();
		break;
	}
}

void CShield::Stand()
{
	m_tFrame.fStartFrame += CTimeMgr::Get_Instance()->Get_DeltaTime();

	if (m_tFrame.fStartFrame >= 0.3f)
	{
		++m_fTime;
		m_tFrame.fStartFrame = 0.f;
	}
}

void CShield::Move()
{
	float fPosY = 1.f;
	m_tFrame.fStartFrame += CTimeMgr::Get_Instance()->Get_DeltaTime();
	if (m_tFrame.fStartFrame >= 0.3f)
	{
		int iRandomPosX = rand() % 10;
		if (m_tInfo.m_fAngle < 3 && m_fWallPosX + 80.f + iRandomPosX <= m_tInfo.vPos.x)
		{
			m_eNewState = PLAYER::STAND;
			return;
		}
		else if (m_tInfo.m_fAngle > 2 && m_fWallPosX - 80.f - iRandomPosX >= m_tInfo.vPos.x)
		{
			m_eNewState = PLAYER::STAND;
			return;
		}
		if (m_dwCount == 0)
			m_dwCount = 1;
		else
			m_dwCount = 0;

		++m_fTime;
		m_tFrame.fStartFrame = 0.f;

		if (m_tInfo.m_fAngle >= 3)
			fPosY = -1.f;

		m_tInfo.vPos.x += m_tInfo.m_fSpeed * fPosY;
	}
}

void CShield::Attack()
{
	m_tFrame.fStartFrame += CTimeMgr::Get_Instance()->Get_DeltaTime();
	if (m_tFrame.fStartFrame >= 0.2f)
	{
		if (m_dwCount == 5)
			m_dwCount = 0;
		else if (m_dwCount == 2)
		{
			if(pTarget)
				pTarget->Set_Damge(m_tUnitInfo.m_iAtt);
			++m_dwCount;
		}
		else
			++m_dwCount;

		++m_fTime;
		m_tFrame.fStartFrame = 0.f;

		if (m_tFrame.fMaxFrame <= m_fTime)
			m_fTime = 0.f;
	}
}

void CShield::Find_Target()
{
	for (auto& iter : CObjMgr::Get_Instance()->Get_Object(OBJ::MONSTER))
	{
		float x = iter->Get_Pos().x;
		
		if (x <= m_tInfo.vPos.x + 100.f && x >= m_tInfo.vPos.x - 100.f)
		{
			m_eNewState = PLAYER::ATTACK;
			pTarget = iter;
			return;
		}
		
	}

	if (m_eNowState == PLAYER::ATTACK)
		m_eNewState = PLAYER::STAND;

}

void CShield::Defense()
{
	if (m_iNowHp != m_tUnitInfo.m_iHp)
	{
		if (m_iDefense == 4)
		{
			m_iSkill = 0;
			m_bDefense = true;
			m_iDefense = 0;
			return;
		}
		++m_iDefense;
		m_iNowHp = m_tUnitInfo.m_iHp;
	}

}
