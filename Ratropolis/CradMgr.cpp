#include "stdafx.h"
#include "CradMgr.h"
#include "KeyMgr.h"
#include "ObjMgr.h"
#include "Crad.h"
#include "ErrMsg.h"
#include "SceneMgr.h"
#include "ScrollMgr.h"
#include "TextureMgr.h"
#include "Stage.h"
#include "TimeMgr.h"
#include "Graphic_Device.h"
#include "AbstractFactory.h"

CCradMgr* CCradMgr::m_pInstance = nullptr;
CCradMgr::CCradMgr()
	:m_iHandSize(5), m_bMouseCheck(false), m_fReSetTime(15.f), m_iCoolTime(RESETTIME), iReTurnPrice(0), m_bNoMoney(true), m_bFarmMoney(false)
{
	m_pDevice = nullptr;
}


CCradMgr::~CCradMgr()
{
	Release();
}


HRESULT CCradMgr::LoadCradData(const wstring & wstrFilePath)
{
	HANDLE hFile = CreateFile(wstrFilePath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		ERR_MSG(__T("Crad Data Load Failed"));
		return E_FAIL;
	}

	DWORD dwByte = 0;
	DWORD dwStrbyte = 0;
	CRADINFO* pCrad = nullptr;

	while (1)
	{
		TCHAR szKey[MAX_PATH] = __T("");
		pCrad = new CRADINFO;
		ReadFile(hFile, &dwStrbyte, sizeof(DWORD), &dwByte, nullptr);
		ReadFile(hFile, szKey, dwStrbyte, &dwByte, nullptr);
		ReadFile(hFile, &pCrad->m_iAtt, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &pCrad->m_iPopulation, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &pCrad->m_iGold, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &pCrad->m_iHp, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &pCrad->m_iRange, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &pCrad->m_iImageNumber, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &pCrad->m_dwType, sizeof(DWORD), &dwByte, nullptr);
		pCrad->m_wstrName = szKey;

		if (dwByte == 0)
		{
			delete pCrad;
			break;
		}

		m_mapCradInfo.emplace(pCrad->m_wstrName, pCrad);
	}
	CloseHandle(hFile);

	return S_OK;
}

CRADINFO* CCradMgr::Get_Crad(const wstring & wstrObjectKey)
{
	auto iter_find = m_mapCradInfo.find(wstrObjectKey);
	if (iter_find == m_mapCradInfo.end())
		return nullptr;

	return m_mapCradInfo[wstrObjectKey];
}

HRESULT CCradMgr::Initialize()
{
	if (FAILED(LoadCradData(__T("../Data/Crad.dat"))))
	{
		ERR_MSG(__T("CradMgr Load Failed"));
		return E_FAIL;
	}

	m_pDevice = CGraphic_Device::Get_Instance();
	return S_OK;
}

void CCradMgr::Update()
{
	iReTurnPrice = CSceneMgr::Get_Instance()->Get_Stage().m_iWave * PRICE;
	if (CKeyMgr::Get_Instance()->KeyDown(KEY_SPACE) || m_bMouseCheck)
	{
		if (Skill_Time())
		{
			if (m_bNoMoney)
			{
				CSceneMgr::Get_Instance()->Get_Stage().m_iGold -= iReTurnPrice;
				Change_Crad(m_listHands, m_listCemetry);
				Select_Crad();
				Add_Render();
				m_iCoolTime = 0;
				m_bFarmMoney = true;
			}
			else
			{
				TCHAR szBuff[32] = {};
				swprintf_s(szBuff, __T("골드가 모자릅니다."));
				CObjMgr::Get_Instance()->Add_Object(OBJ::CRAD, CAbstractFactroy<CErrMsg>::Create(szBuff));
			}

		}
		else
		{
			TCHAR szBuff[32] = {};
			swprintf_s(szBuff, __T("사용할 수 없습니다."));
 			CObjMgr::Get_Instance()->Add_Object(OBJ::CRAD, CAbstractFactroy<CErrMsg>::Create(szBuff));
		}
		m_bMouseCheck = false;
	}
}

void CCradMgr::Render()
{
	Render_CoolTime();
	Render_Price();
}

void CCradMgr::Release()
{
	for (auto& rPiar : m_mapCradInfo)
		Safe_Delete(rPiar.second);
	m_mapCradInfo.clear();
}
 
void CCradMgr::Select_Crad(int num /*= 0*/)
{
	if (m_listBag.empty())
	{
		Change_Crad(m_listCemetry, m_listBag);
	}

	for (auto& iter = m_listBag.begin(); iter != m_listBag.end();)
	{
		if (num < m_iHandSize)
		{
			if (rand() % 2)
			{
				m_listHands.emplace_back(*iter);
				iter =  m_listBag.erase(iter);
				++num;
			}
			else
				++iter;
		}
		else
			return;
	}

	if (num < m_iHandSize)
		Select_Crad(num);
	else
		return;

}

void CCradMgr::Change_Crad(list<CRADINFO*>& DeleteList, list<CRADINFO*>& AddList)
{
	for (auto& iter = DeleteList.begin(); iter != DeleteList.end();)
	{
		AddList.emplace_back(*iter);
		iter = DeleteList.erase(iter);
	}
	DeleteList.clear();
}

void CCradMgr::Add_Render()
{
	CObjMgr::Get_Instance()->Clear_Object(OBJ::CRAD);

	TCHAR sz_Buffer[32] = {};
	int iNumber = 0;
	
	for (auto& iter : m_listHands)
	{
		switch (iter->m_dwType)
		{
		case CRAD::ECONOMY_CRAD:
			swprintf_s(sz_Buffer, __T("Economy"));
			break;
		case CRAD::MILITARY_CRAD:
			swprintf_s(sz_Buffer, __T("Military"));
			break;
		case CRAD::BUILDING_CRAD:
			swprintf_s(sz_Buffer, __T("Building"));
			break;
		case CRAD::TECHNOLOGY_CRAD:
			swprintf_s(sz_Buffer, __T("Technology"));
			break;
		}

		CObjMgr::Get_Instance()->Add_Object(OBJ::CRAD, CAbstractFactroy<CCrad>::Create(D3DXVECTOR3((400.f + (WINCX>>1) / m_iHandSize * iNumber) , WINCY - 50.f, 0.f), D3DXVECTOR3(0.5f, 0.5f, 0.f), __T("Crad"), sz_Buffer, iter->m_iImageNumber));
		++iNumber;
	}
}

void CCradMgr::Render_CoolTime()
{	
	//쿨타임 회색 객체
	if (!Skill_Time())
	{
		const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(__T("InGame"), __T("UI"), 14);

		float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
		float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);

		D3DXMATRIX matScale, matTrans;
		D3DXMatrixScaling(&matScale, 0.3f, 0.3f / (m_iCoolTime + 1), 0.f);
		D3DXMatrixTranslation(&matTrans, 1230.f, 550.f - (4.f * m_iCoolTime), 0.f);

		m_tInfo.matWorld = matScale * matTrans;
		m_pDevice->Get_Sprite()->SetTransform(&m_tInfo.matWorld);
		m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(150, 255, 255, 255));
		//쿨타임 시간

		TCHAR szBuffer[16] = {};
		int iAdd_Time = dynamic_cast<CStage*>(CSceneMgr::Get_Instance()->Get_Scene())->Get_Stage().m_iMaxPop;
		int iCoolTime = RESETTIME - m_iCoolTime ;
		swprintf_s(szBuffer, __T("%d초"), iCoolTime);
		D3DXMatrixScaling(&matScale, 0.5f, 0.5f, 0.f);
		D3DXMatrixTranslation(&matTrans, 1215.f, 540.f, 0.f);

		m_tInfo.matWorld = matScale * matTrans;

		m_pDevice->Get_Sprite()->SetTransform(&m_tInfo.matWorld);
		m_pDevice->Get_Font()->DrawTextW(m_pDevice->Get_Sprite(), szBuffer, lstrlen(szBuffer), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

	}
}

void CCradMgr::Render_Price()
{
	D3DXMATRIX matScale, matTrans;
	TCHAR szBuffer[16] = {};

	swprintf_s(szBuffer, __T("%d"), iReTurnPrice);
	D3DXMatrixScaling(&matScale, 0.7f, 0.7f, 0.f);
	D3DXMatrixTranslation(&matTrans, 1215.f, 555.f, 0.f);

	m_tInfo.matWorld = matScale * matTrans;

	m_pDevice->Get_Sprite()->SetTransform(&m_tInfo.matWorld);
	if (CSceneMgr::Get_Instance()->Get_Stage().m_iGold > iReTurnPrice)
	{
		m_bNoMoney = true;
		m_pDevice->Get_Font()->DrawTextW(m_pDevice->Get_Sprite(), szBuffer, lstrlen(szBuffer), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 0));
	}
	else
	{
		m_bNoMoney = false;
		m_pDevice->Get_Font()->DrawTextW(m_pDevice->Get_Sprite(), szBuffer, lstrlen(szBuffer), nullptr, 0, D3DCOLOR_ARGB(255, 255, 0, 0));
	}
}

void CCradMgr::Setting_Crad()
{
	for(int i=0; i<3; ++i)
		m_listBag.emplace_back(Get_Crad(__T("Cheese")));
	m_listBag.emplace_back(Get_Crad(__T("House")));
	m_listBag.emplace_back(Get_Crad(__T("DefenseWall")));
	m_listBag.emplace_back(Get_Crad(__T("Factory")));
	m_listBag.emplace_back(Get_Crad(__T("Barn")));
	m_listBag.emplace_back(Get_Crad(__T("Monastery")));
	m_listBag.emplace_back(Get_Crad(__T("ApartmentHouse")));
	m_listBag.emplace_back(Get_Crad(__T("Farm")));
	m_listBag.emplace_back(Get_Crad(__T("Militia")));
	m_listBag.emplace_back(Get_Crad(__T("Shield")));
	m_listBag.emplace_back(Get_Crad(__T("LongBow")));
	m_listBag.emplace_back(Get_Crad(__T("BuildingTax")));
	m_listBag.emplace_back(Get_Crad(__T("Grain")));
}

void CCradMgr::Release_Crad(int iNumber)
{
	int num = 0;
	for (auto& iter = m_listHands.begin(); iter != m_listHands.end();)
	{
		if (num == iNumber)
		{
			m_listCemetry.emplace_back(*iter);
			m_listHands.erase(iter);
			return;
		}
		++num;
		++iter;
	}
}

void CCradMgr::Delete_Crad(int iNumber)
{
	int num = 0;
	for (auto& iter = m_listHands.begin(); iter != m_listHands.end();)
	{
		if (num == iNumber)
		{
			m_listHands.erase(iter);
			return;
		}
		++num;
		++iter;
	}
}

void CCradMgr::Clear_Crad()
{
	m_listBag.clear();
	m_listHands.clear();
	m_listCemetry.clear();
}

bool CCradMgr::Skill_Time()
{
	m_fReSetTime += CTimeMgr::Get_Instance()->Get_DeltaTime();

	if (m_fReSetTime >= 1)
	{
		++m_iCoolTime;
		m_fReSetTime = 0.f;
	}

	if (m_iCoolTime >= RESETTIME)
		return true;
	return false;
}
