#include "stdafx.h"
#include "LongBow.h"
#include "CradMgr.h"
#include "TextureMgr.h"
#include "ScrollMgr.h"
#include "Stage.h"
#include "SceneMgr.h"
#include "ObjMgr.h"
#include "Dead.h"
#include "DefenseWall.h"
#include "TimeMgr.h"
#include "AbstractFactory.h"
#include "Arrow.h"
#include "Graphic_Device.h"


CLongBow::CLongBow()
	:m_fDetection(1000.f), m_eNewState(PLAYER::MOVE), m_eNowState(PLAYER::STATE_END)
{
	m_pDevice = nullptr;
}


CLongBow::~CLongBow()
{
}

HRESULT CLongBow::Initialize()
{
	m_pDevice = CGraphic_Device::Get_Instance();
	CRADINFO* tUnitInfo = CCradMgr::Get_Instance()->Get_Crad(__T("LongBow"));
	m_tUnitInfo = (*tUnitInfo);
	dynamic_cast<CStage*>(CSceneMgr::Get_Instance()->Get_Scene())->Get_Stage().m_iPop -= m_tUnitInfo.m_iPopulation;
	m_tFrame.fMaxFrame = 20.f;
	m_tFrame.fStartFrame = 0.f;
	m_tInfo.m_fSpeed = 5.f;
	m_fWallPosX = m_tInfo.vPos.x;
	if (m_tInfo.m_fAngle > 2)
	{
		m_tInfo.vSize.x *= -1.f;
		m_bTurn = false;
	}
	else
	{
		m_tInfo.vSize.x *= -1.f;
		m_bTurn = true;
	}
	return S_OK;
}

int CLongBow::Update()
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

void CLongBow::Late_Update()
{
}

void CLongBow::Render()
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
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + fScrollX, m_tInfo.vPos.y + fCenterY + 15.f, 0.f);

		m_tInfo.matWorld = matScale * matTrans;

		m_pDevice->Get_Sprite()->SetTransform(&m_tInfo.matWorld);
		m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(120, 255, 255, 255));
	}
}

void CLongBow::Release()
{
}

void CLongBow::State()
{


	if (m_eNewState != m_eNowState)
	{
		switch (m_eNewState)
		{
		case PLAYER::STAND:
			m_wstrStateKey = __T("LongBowWait");
			break;
		case PLAYER::MOVE:
			m_wstrStateKey = __T("LongBowMove");
			break;
		case PLAYER::ATTACK:
			m_wstrStateKey = __T("LongBowAttack");
			break;
		}
		m_dwCount = 0;
		m_eNowState = m_eNewState;
	}

	Find_Target();

	switch (m_eNowState)
	{
	case PLAYER::STAND:
		Stand();
		break;
	case PLAYER::MOVE:
		Move();
		break;
	case PLAYER::ATTACK:
		Attack();
		break;
	}

}

void CLongBow::Stand()
{
	m_tFrame.fStartFrame += CTimeMgr::Get_Instance()->Get_DeltaTime();

	if (m_tFrame.fStartFrame >= 0.3f)
	{
		++m_fTime;
		m_tFrame.fStartFrame = 0.f;
	}
}

void CLongBow::Move()
{
	if (!m_bTurn)
		LeftMove();
	else
		RightMove();
}

void CLongBow::RightMove()
{
	float fPosY = -1.f;
	m_tFrame.fStartFrame += CTimeMgr::Get_Instance()->Get_DeltaTime();
	if (m_tFrame.fStartFrame >= 0.3f)
	{
		int iRandomPosX = rand() % 10;
		if (m_fWallPosX - 80.f - iRandomPosX >= m_tInfo.vPos.x)
		{
			m_eNewState = PLAYER::STAND;
			m_tInfo.vSize.x *= -1.f;
			return;
		}

		if (m_dwCount == 0)
			m_dwCount = 1;
		else
			m_dwCount = 0;

		++m_fTime;
		m_tFrame.fStartFrame = 0.f;

		m_tInfo.vPos.x += m_tInfo.m_fSpeed * fPosY;

		
	}
}

void CLongBow::LeftMove()
{
	float fPosY = 1.f;
	m_tFrame.fStartFrame += CTimeMgr::Get_Instance()->Get_DeltaTime();
	if (m_tFrame.fStartFrame >= 0.3f)
	{
		int iRandomPosX = rand() % 10;
		if (m_fWallPosX + 80.f + iRandomPosX <= m_tInfo.vPos.x)
		{
			m_eNewState = PLAYER::STAND;
			m_tInfo.vSize.x *= -1.f;
		}

		if (m_dwCount == 0)
			m_dwCount = 1;
		else
			m_dwCount = 0;

		++m_fTime;
		m_tFrame.fStartFrame = 0.f;

		m_tInfo.vPos.x += m_tInfo.m_fSpeed * fPosY;
	}
}

void CLongBow::Attack()
{
	m_tFrame.fStartFrame += CTimeMgr::Get_Instance()->Get_DeltaTime();
	if (m_tFrame.fStartFrame >= 0.3f)
	{
		if (m_dwCount == 4)
			m_dwCount = 0;
		else if (m_dwCount == 2)
		{
			Create_Arrow();
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

void CLongBow::Find_Target()
{
	for (auto& iter : CObjMgr::Get_Instance()->Get_Object(OBJ::MONSTER))
	{
		float x = iter->Get_Pos().x;
		if (x <= m_tInfo.vPos.x + 1000.f && x >= m_tInfo.vPos.x - 1000.f)
		{
			m_eNewState = PLAYER::ATTACK;
			m_pTarget = iter;

			if (m_eNowState == PLAYER::MOVE)
				m_tInfo.vSize.x *= -1.f;
			return;
		}
	}

	if (m_eNowState == PLAYER::ATTACK)
		m_eNewState = PLAYER::STAND;
}

void CLongBow::Create_Arrow()
{
	if (!m_pTarget)
		return;

	CObj* Obj = new CArrow;
	Obj->Set_Pos(m_tInfo.vPos);
	dynamic_cast<CArrow*>(Obj)->Set_Target(m_pTarget);
	dynamic_cast<CArrow*>(Obj)->Set_Damage(m_tUnitInfo.m_iAtt);
	Obj->Initialize();
	CObjMgr::Get_Instance()->Add_Object(OBJ::BULLET, Obj);
}
