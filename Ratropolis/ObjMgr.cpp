#include "stdafx.h"
#include "ObjMgr.h"
#include "CradMgr.h"
#include "KeyMgr.h"
#include "CradList.h"
#include "GameMenu.h"
#include "CoolTime.h"
#include "TimeMgr.h"
#include "Terrain.h"
#include "SceneMgr.h"
#include "ErrMsg.h"
#include "Stage.h"
#include "NotCreate.h"
#include "AbstractFactory.h"
#include "AddGold.h"
#include "Obj.h"
#include "Ui.h"
#include "CreatePos.h"
#include "ChangeWall.h"

CObjMgr* CObjMgr::m_pInstance = nullptr;
CObjMgr::CObjMgr()
	:m_bClickCrad(false), m_iCount(100), m_iCoolTimeCount(0), m_bNotCreate(false)
{
	if (!m_pCradList)
	{
		m_pCradList = new CCradList;
		m_pCradList->Initialize();
	}

	if (!m_pMenu)
	{
		m_pMenu = new CGameMenu;
		m_pMenu->Initialize();
	}
}


CObjMgr::~CObjMgr()
{
	Release();
}

void CObjMgr::Update()
{
	if (!Game_Progress)
	{
		for (int i = 0; i < OBJ::OBJ_END; ++i)
		{
 			for (auto& iter = m_ObjList[i].begin(); iter != m_ObjList[i].end();)
			{
				if ((*iter)->Update())	// dead = true live = false
				{
					if(i != 11)
						SAFE_DELETE(*iter);
					iter = m_ObjList[i].erase(iter);
				}
				else
					++iter;
			}
		}

		m_pCradList->Update();
	}
	else
		m_pMenu->Update();
}

void CObjMgr::Late_Update()
{ 
	for (int i = 0; i < OBJ::OBJ_END; ++i)
	{
		for (auto& iter : m_ObjList[i])
			iter->Late_Update();
	}
}

void CObjMgr::Render()
{
	for (int i = 0; i < OBJ::OBJ_END; ++i)
	{
		for (auto& iter : m_ObjList[i])
 			iter->Render();
	}
	
	m_pCradList->Render();
	m_pMenu->Render();
}

void CObjMgr::Release()
{
	for (int i = 0; i < OBJ::OBJ_END; ++i)
	{
		for_each(m_ObjList[i].begin(), m_ObjList[i].end(), Safe_Delete<CObj*>);
		m_ObjList[i].clear();
	}

	SAFE_DELETE(m_pCradList);
	SAFE_DELETE(m_pMenu);
}

void CObjMgr::Clear_Object(OBJ::ID eID)
{
	for_each(m_ObjList[eID].begin(), m_ObjList[eID].end(), Safe_Delete<CObj*>);
	m_ObjList[eID].clear();
}
//1.지도자 스킬 2.가방 3.리셋 4.묘지 5.메뉴 6.정지&스타트 7.전문가
void CObjMgr::Button_Ckeck()
{
	if (!m_pCradList->Get_Execution())
	{
		int iType = 0;
		POINT ps = {};
		GetCursorPos(&ps);
		ClientToScreen(g_hWnd, &ps);
		list<CObj*>::iterator Ui_iter = m_ObjList[OBJ::UI].begin();
		for (auto& iter : m_ObjList[OBJ::BUTTON])
		{
			RECT rButton = { LONG(iter->Get_Pos().x - 40), LONG(iter->Get_Pos().y + 20) , LONG(iter->Get_Pos().x + 40) ,LONG(iter->Get_Pos().y + 80) };
			if (PtInRect(&rButton, ps))
			{
				if (CKeyMgr::Get_Instance()->KeyDown(KEY_LBUTTON))
				{
					switch (iType)
					{
					case BUTTON::LEADER_SKILL:
						if (Game_Progress)
							return;
						return;
					case BUTTON::BAG:
						if (Game_Progress)
							return;
						m_pCradList->Create(m_pCradList->BAG);
						return;
					case BUTTON::RESET:
						if (Game_Progress)
							return;
						CCradMgr::Get_Instance()->Set_MouseCheck();
						return;
					case BUTTON::CEMETRY:
						if (Game_Progress)
							return;
						m_pCradList->Create(m_pCradList->CEMETRY);
						return;
					case BUTTON::MENU:
						if (Game_Progress)
							return;
						m_pMenu->Create();
						return;
					case BUTTON::STATE_STOP:
						if (Game_Progress)//스타트
						{
							CObj* pobj = CAbstractFactroy<CUi>::Create(D3DXVECTOR3(1130.f, 50.f, 0.f), D3DXVECTOR3(0.4f, 0.4f, 0.f), __T("InGame"), __T("UI"), 4);
							m_ObjList[OBJ::UI].insert(Ui_iter, pobj);
							SAFE_DELETE(*Ui_iter);
							m_ObjList[OBJ::UI].erase(Ui_iter);
							Game_Progress = false;
						}
						else //정지
						{
							CObj* pobj = CAbstractFactroy<CUi>::Create(D3DXVECTOR3(1130.f, 50.f, 0.f), D3DXVECTOR3(0.4f, 0.4f, 0.f), __T("InGame"), __T("UI"), 6);
							m_ObjList[OBJ::UI].insert(Ui_iter, pobj);
							SAFE_DELETE(*Ui_iter);
							m_ObjList[OBJ::UI].erase(Ui_iter);
							Game_Progress = true;
						}
						return;
					case BUTTON::EXPERT:
						if (Game_Progress)
							return;
						return;
					}
				}
			}
			++iType;
			++Ui_iter;
		}
	}
}

void CObjMgr::Click_Mouse()
{
	if (!m_pCradList->Get_Execution())
	{

		int iNumber = 0;
		POINT ps = {};
		GetCursorPos(&ps);
		ClientToScreen(g_hWnd, &ps);

		for (auto& iter = m_ObjList[OBJ::CRAD].begin(); iter != m_ObjList[OBJ::CRAD].end();)
		{
			RECT rc = { LONG((*iter)->Get_Pos().x) - CRADCX, LONG((*iter)->Get_Pos().y) - CRADCY , LONG((*iter)->Get_Pos().x) + CRADCX, LONG((*iter)->Get_Pos().y) + CRADCY };

			if (PtInRect(&rc, ps))
			{
				CRAD::TYPE eType = dynamic_cast<CObj*>(*iter)->Get_Type();
				int iCradNumber = dynamic_cast<CObj*>(*iter)->Get_CradNumber();

				if (!m_bClickCrad && CKeyMgr::Get_Instance()->KeyDown(KEY_LBUTTON))
				{
					if ((*iter)->Get_Type() == CRAD::BUILDING_CRAD || (*iter)->Get_Type() == CRAD::MILITARY_CRAD)
					{
						m_bNotCreate = true;
						dynamic_cast<CStage*>(CSceneMgr::Get_Instance()->Get_Scene())->Get_Terrain()->Building_Place(true);
						Crad_Option(eType, iCradNumber);
					}
					m_iCount = iNumber;
					m_vSavePos = (*iter)->Get_Pos();
					(*iter)->Set_Pos(D3DXVECTOR3(float(ps.x), float(ps.y), 0.f));
					m_bClickCrad = true;
					return;
				}

				else if (m_bClickCrad && CKeyMgr::Get_Instance()->KeyUp(KEY_LBUTTON) && m_iCount == iNumber)
				{
					//건물카드따로
					if ((*iter)->Get_Type() == CRAD::BUILDING_CRAD)
					{
						//벽 강화
						if ((*iter)->Get_CradNumber() == 1)
						{
							if (NoMoney(CCradMgr::Get_Instance()->Get_Crad(Get_Texture(eType, iCradNumber))->m_iGold))
							{
								(*iter)->Set_Pos(m_vSavePos);
								(*iter)->Set_Size(D3DXVECTOR3(0.5f, 0.5f, 0.f));
								SAFE_DELETE(m_ObjList[OBJ::NOCREATE].front());
								m_ObjList[OBJ::NOCREATE].clear();
								m_bClickCrad = false;
								m_iCount = 100;
							}
							else if (!dynamic_cast<CChangeWall*>(m_ObjList[OBJ::NOCREATE].front())->Get_No_Create())
							{
								NoCreate();
								(*iter)->Set_Pos(m_vSavePos);
								(*iter)->Set_Size(D3DXVECTOR3(0.5f, 0.5f, 0.f));
								SAFE_DELETE(m_ObjList[OBJ::NOCREATE].front());
								m_ObjList[OBJ::NOCREATE].clear();
								m_bClickCrad = false;
								m_iCount = 100;
							}
							else
							{
								dynamic_cast<CChangeWall*>(m_ObjList[OBJ::NOCREATE].front())->Set_Not(true);
								SAFE_DELETE((*iter));
								m_ObjList[OBJ::CRAD].erase(iter);
								CCradMgr::Get_Instance()->Delete_Crad(iNumber);
								m_bClickCrad = false;
								m_iCount = 100;
							}
							return;
						}

						dynamic_cast<CStage*>(CSceneMgr::Get_Instance()->Get_Scene())->Get_Terrain()->Building_Place(false);
						if (!dynamic_cast<CNotCreate*>(m_ObjList[OBJ::NOCREATE].front())->Ckeck_Build())
						{
							SAFE_DELETE(m_ObjList[OBJ::NOCREATE].front());
							m_ObjList[OBJ::NOCREATE].clear();
							(*iter)->Set_Pos(m_vSavePos);
							(*iter)->Set_Size(D3DXVECTOR3(0.5f, 0.5f, 0.f));
							m_bClickCrad = false;
							m_iCount = 100;
							return;
						}
						else if (NoMoney(CCradMgr::Get_Instance()->Get_Crad(Get_Texture(eType, iCradNumber))->m_iGold))
						{
							SAFE_DELETE(m_ObjList[OBJ::NOCREATE].front());
							m_ObjList[OBJ::NOCREATE].clear();
							(*iter)->Set_Pos(m_vSavePos);
							(*iter)->Set_Size(D3DXVECTOR3(0.5f, 0.5f, 0.f));
							m_bClickCrad = false;
							m_iCount = 100;
							return;
						}
						else
						{
							dynamic_cast<CNotCreate*>(m_ObjList[OBJ::NOCREATE].front())->Get_Build(true);
							SAFE_DELETE((*iter));
							m_ObjList[OBJ::CRAD].erase(iter);
							CCradMgr::Get_Instance()->Delete_Crad(iNumber);
							m_bClickCrad = false;
							m_iCount = 100;
							return;
						}
					}
					//군사카드 따로
					else if ((*iter)->Get_Type() == CRAD::MILITARY_CRAD)
					{
						if (NoMoney(CCradMgr::Get_Instance()->Get_Crad(Get_Texture(eType, iCradNumber))->m_iGold))
						{
							(*iter)->Set_Pos(m_vSavePos);
							(*iter)->Set_Size(D3DXVECTOR3(0.5f, 0.5f, 0.f));
							SAFE_DELETE(m_ObjList[OBJ::NOCREATE].front());
							m_ObjList[OBJ::NOCREATE].clear();
							m_bClickCrad = false;
							m_iCount = 100;
						}
						else if (!dynamic_cast<CCreatePos*>(m_ObjList[OBJ::NOCREATE].front())->NoCreate_Pop())
						{
							(*iter)->Set_Pos(m_vSavePos);
							(*iter)->Set_Size(D3DXVECTOR3(0.5f, 0.5f, 0.f));
							SAFE_DELETE(m_ObjList[OBJ::NOCREATE].front());
							m_ObjList[OBJ::NOCREATE].clear();
							m_bClickCrad = false;
							m_iCount = 100;
						}
						else if (!dynamic_cast<CCreatePos*>(m_ObjList[OBJ::NOCREATE].front())->Get_Create())
						{
							NoCreate();
							(*iter)->Set_Pos(m_vSavePos);
							(*iter)->Set_Size(D3DXVECTOR3(0.5f, 0.5f, 0.f));
							SAFE_DELETE(m_ObjList[OBJ::NOCREATE].front());
							m_ObjList[OBJ::NOCREATE].clear();
							m_bClickCrad = false;
							m_iCount = 100;
						}
						else
						{
							dynamic_cast<CCreatePos*>(m_ObjList[OBJ::NOCREATE].front())->Set_Create(true);
							SAFE_DELETE((*iter));
							m_ObjList[OBJ::CRAD].erase(iter);
							CCradMgr::Get_Instance()->Release_Crad(iNumber);
							m_bClickCrad = false;
							m_iCount = 100;
						}
						return;
					}
					else if (NoMoney(CCradMgr::Get_Instance()->Get_Crad(Get_Texture(eType, iCradNumber))->m_iGold))
					{
						m_bNotCreate = false;
						(*iter)->Set_Pos(m_vSavePos);
						return;
					}
					else
						Crad_Option(eType, iCradNumber);
				
					SAFE_DELETE((*iter));
					m_ObjList[OBJ::CRAD].erase(iter);
					CCradMgr::Get_Instance()->Release_Crad(iNumber);
					m_bClickCrad = false;
					m_iCount = 100;
					return;
				}
				else if (m_bClickCrad && m_iCount == iNumber)
				{
					(*iter)->Set_Pos(D3DXVECTOR3(float(ps.x), float(ps.y), 0.f));
					if ((*iter)->Get_Type() == CRAD::BUILDING_CRAD || (*iter)->Get_Type() == CRAD::MILITARY_CRAD)
					{
						float fSmall = (ps.y - (60.f * 6)) *0.001f;
						if (fSmall <= 0.1)
							fSmall = 0.1f;
						else if (fSmall >= 0.5)
							fSmall = 0.5f;
						(*iter)->Set_Size(D3DXVECTOR3(fSmall, fSmall, 0.f));
					}
					return;
				}
			}
			else if (m_bClickCrad && m_iCount == iNumber)
			{
				if ((*iter)->Get_Type() == CRAD::BUILDING_CRAD || (*iter)->Get_Type() == CRAD::MILITARY_CRAD)
				{
					dynamic_cast<CStage*>(CSceneMgr::Get_Instance()->Get_Scene())->Get_Terrain()->Building_Place(false);
					SAFE_DELETE(m_ObjList[OBJ::NOCREATE].front());
					m_ObjList[OBJ::NOCREATE].clear();
				}
				m_iCount = 100;
				(*iter)->Set_Pos(m_vSavePos);
				(*iter)->Set_Size(D3DXVECTOR3(0.5f, 0.5f, 0.f));
				m_bClickCrad = false;
			}
			++iNumber;
			++iter;
		}
	}
}

bool CObjMgr::NoMoney(int Money)
{
	if (0 > CSceneMgr::Get_Instance()->Get_Stage().m_iGold - Money)
	{
		TCHAR szBuf[32] = {};
		swprintf_s(szBuf, __T("금액이 부족합니다."));
		CObjMgr::Get_Instance()->Add_Object(OBJ::CRAD, CAbstractFactroy<CErrMsg>::Create(szBuf));
		return true;
	}
	else
	{
		CSceneMgr::Get_Instance()->Get_Stage().m_iGold -= Money;
		return false;
	}
}

void CObjMgr::NoCreate()
{
	TCHAR szBuf[32] = {};
	swprintf_s(szBuf, __T("사용할 수 없습니다."));
	CObjMgr::Get_Instance()->Add_Object(OBJ::CRAD, CAbstractFactroy<CErrMsg>::Create(szBuf));
}

void CObjMgr::Crad_Option(CRAD::TYPE dwType, const int CradNumber)
{
	switch (dwType)
	{
	case CRAD::ECONOMY_CRAD:
		Option_Economy(CRAD::ECONOMY(CradNumber));
		break;
	case CRAD::MILITARY_CRAD:
		Option_Military(CRAD::MILITARY(CradNumber));
		break;
	case CRAD::BUILDING_CRAD:
		Option_Building(CRAD::BUILDING(CradNumber));
		break;
	case CRAD::TECHNOLOGY_CRAD:
		Option_Technology(CRAD::MYTECHNOLOGY(CradNumber));
		break;
	}
}

void CObjMgr::Option_Economy(CRAD::ECONOMY eCrad)
{
	int iNumber = 0;
	CObj* pobj = nullptr;
	switch (eCrad)
	{
	case CRAD::GRAIN:
		pobj = new CCoolTime;
		dynamic_cast<CCoolTime*>(pobj)->Set_Frame(D3DXVECTOR3(350.f + (100.f * m_iCoolTimeCount), 50.f, 0.f), 50.f, 0);
		dynamic_cast<CCoolTime*>(pobj)->Set_Gold(60);
		dynamic_cast<CCoolTime*>(pobj)->Set_Pop(2);
		if (FAILED(pobj->Initialize()))
		{
			SAFE_DELETE(pobj);
			return;
		}
		++m_iCoolTimeCount;
		Add_Object(OBJ::UI, pobj);
		break;
	case CRAD::CHEESE:
		iNumber = 0;
		for (auto& iter : CCradMgr::Get_Instance()->Get_ListHands())
		{
			if (!lstrcmp(iter->m_wstrName.c_str(), __T("Cheese")))
				++iNumber;
		}
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::UI);
		CSoundMgr::Get_Instance()->PlaySound(L"Gold.wav", CSoundMgr::UI);
		CObjMgr::Get_Instance()->Add_Object(OBJ::GOLD, CAbstractFactroy<CAddGold>::Create(iNumber * 30.f));
		CSceneMgr::Get_Instance()->Get_Stage().m_iGold += (iNumber * 30);
		break;
	case CRAD::POTTER:
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::UI);
		CSoundMgr::Get_Instance()->PlaySound(L"Gold.wav", CSoundMgr::UI);
		iNumber = CCradMgr::Get_Instance()->Get_ListHands().size();
		CObjMgr::Get_Instance()->Add_Object(OBJ::GOLD, CAbstractFactroy<CAddGold>::Create(iNumber * 10.f));
		CSceneMgr::Get_Instance()->Get_Stage().m_iGold += (iNumber * 10);
		break;
	case CRAD::BUILDINGTAX:
		if (!m_ObjList[OBJ::BUILDING].empty())
		{
			int iSize = m_ObjList[OBJ::BUILDING].size();
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::UI);
			CSoundMgr::Get_Instance()->PlaySound(L"Gold.wav", CSoundMgr::UI);
			CObjMgr::Get_Instance()->Add_Object(OBJ::GOLD, CAbstractFactroy<CAddGold>::Create(iSize * 5.f));
			CSceneMgr::Get_Instance()->Get_Stage().m_iGold += iSize * 5;
		}
		break;
	case CRAD::MEMBRANELABOR:
		break;
	}
}

void CObjMgr::Option_Military(CRAD::MILITARY eCrad)
{
	switch (eCrad)
	{
	case CRAD::MILITIA:
		dynamic_cast<CStage*>(CSceneMgr::Get_Instance()->Get_Scene())->Get_Terrain()->Building_Place(false);
		Add_Object(OBJ::NOCREATE, CAbstractFactroy<CCreatePos>::Create(m_vSavePos, D3DXVECTOR3(0.5f, 0.5f, 0.f), __T("InGame"), __T("UI"), 17));
		dynamic_cast<CCreatePos*>(m_ObjList[OBJ::NOCREATE].front())->Set_NameState(__T("MilitaWait"));
		dynamic_cast<CCreatePos*>(m_ObjList[OBJ::NOCREATE].front())->Set_Name(PLAYER::MILITIA);
		break;
	case CRAD::SHIELD:
		dynamic_cast<CStage*>(CSceneMgr::Get_Instance()->Get_Scene())->Get_Terrain()->Building_Place(false);
		Add_Object(OBJ::NOCREATE, CAbstractFactroy<CCreatePos>::Create(m_vSavePos, D3DXVECTOR3(0.5f, 0.5f, 0.f), __T("InGame"), __T("UI"), 17));
		dynamic_cast<CCreatePos*>(m_ObjList[OBJ::NOCREATE].front())->Set_NameState(__T("ShieldWait"));
		dynamic_cast<CCreatePos*>(m_ObjList[OBJ::NOCREATE].front())->Set_Name(PLAYER::SHIELD);
		break;
	case CRAD::LongBow:
		dynamic_cast<CStage*>(CSceneMgr::Get_Instance()->Get_Scene())->Get_Terrain()->Building_Place(false);
		Add_Object(OBJ::NOCREATE, CAbstractFactroy<CCreatePos>::Create(m_vSavePos, D3DXVECTOR3(0.5f, 0.5f, 0.f), __T("InGame"), __T("UI"), 17));
		dynamic_cast<CCreatePos*>(m_ObjList[OBJ::NOCREATE].front())->Set_NameState(__T("LongBowWait"));
		dynamic_cast<CCreatePos*>(m_ObjList[OBJ::NOCREATE].front())->Set_Name(PLAYER::LONGBOW);
		break;
	}
}

void CObjMgr::Option_Building(CRAD::BUILDING eCrad)
{
	switch (eCrad)
	{
	case CRAD::HOUSE:
		Add_Object(OBJ::NOCREATE, CAbstractFactroy<CNotCreate>::Create(m_vSavePos, D3DXVECTOR3(0.4f, 0.4f, 0.f), __T("InGame"), __T("Building"), 0));
		dynamic_cast<CNotCreate*>(m_ObjList[OBJ::NOCREATE].front())->Get_Name(__T("House"));
		dynamic_cast<CNotCreate*>(m_ObjList[OBJ::NOCREATE].front())->Get_BuildSize(1);
		break;
	case CRAD::DEFENSEWALL:
		dynamic_cast<CStage*>(CSceneMgr::Get_Instance()->Get_Scene())->Get_Terrain()->Building_Place(false);
		Add_Object(OBJ::NOCREATE, CAbstractFactroy<CChangeWall>::Create(m_vSavePos, D3DXVECTOR3(0.5f, 0.5f, 0.f), __T("InGame"), __T("UI"), 17));
		break;
	case CRAD::FACTORY:
		Add_Object(OBJ::NOCREATE, CAbstractFactroy<CNotCreate>::Create(m_vSavePos, D3DXVECTOR3(0.4f, 0.4f, 0.f), __T("InGame"), __T("Building"), 2));
		dynamic_cast<CNotCreate*>(m_ObjList[OBJ::NOCREATE].front())->Get_Name(__T("Factory"));
		dynamic_cast<CNotCreate*>(m_ObjList[OBJ::NOCREATE].front())->Get_BuildSize(5);
		break;
	case CRAD::MONASTERY:
		Add_Object(OBJ::NOCREATE, CAbstractFactroy<CNotCreate>::Create(m_vSavePos, D3DXVECTOR3(0.4f, 0.4f, 0.f), __T("InGame"), __T("Building"), 3));
		dynamic_cast<CNotCreate*>(m_ObjList[OBJ::NOCREATE].front())->Get_Name(__T("Monastery"));
		dynamic_cast<CNotCreate*>(m_ObjList[OBJ::NOCREATE].front())->Get_BuildSize(3);
		break;
	case CRAD::BARN:
		Add_Object(OBJ::NOCREATE, CAbstractFactroy<CNotCreate>::Create(m_vSavePos, D3DXVECTOR3(0.4f, 0.4f, 0.f), __T("InGame"), __T("Building"), 4));
		dynamic_cast<CNotCreate*>(m_ObjList[OBJ::NOCREATE].front())->Get_Name(__T("Barn"));
		dynamic_cast<CNotCreate*>(m_ObjList[OBJ::NOCREATE].front())->Get_BuildSize(3);
		break;
	case CRAD::APARTMENTHOUSE:
		Add_Object(OBJ::NOCREATE, CAbstractFactroy<CNotCreate>::Create(m_vSavePos, D3DXVECTOR3(0.4f, 0.4f, 0.f), __T("InGame"), __T("Building"), 5));
		dynamic_cast<CNotCreate*>(m_ObjList[OBJ::NOCREATE].front())->Get_Name(__T("ApartmentHouse"));
		dynamic_cast<CNotCreate*>(m_ObjList[OBJ::NOCREATE].front())->Get_BuildSize(3);
		break;
	case CRAD::FARM:
		Add_Object(OBJ::NOCREATE, CAbstractFactroy<CNotCreate>::Create(m_vSavePos, D3DXVECTOR3(0.4f, 0.4f, 0.f), __T("InGame"), __T("Building"), 6));
		dynamic_cast<CNotCreate*>(m_ObjList[OBJ::NOCREATE].front())->Get_Name(__T("Farm"));
		dynamic_cast<CNotCreate*>(m_ObjList[OBJ::NOCREATE].front())->Get_BuildSize(3);
		break;
	}
}

void CObjMgr::Option_Technology(CRAD::MYTECHNOLOGY eCrad)
{
}

const wstring  CObjMgr::Get_Texture(CRAD::TYPE dwType, const int CradNumber)
{
	TCHAR szBuf[32] = {};
	switch (dwType)
	{
	case CRAD::ECONOMY_CRAD:
		switch (CradNumber)
		{
		case 0:
			swprintf_s(szBuf, __T("Grain"));
			return szBuf;
		case 1:
			swprintf_s(szBuf, __T("Cheese"));
			return szBuf;
		case 2:
			swprintf_s(szBuf, __T("Potter"));
			return szBuf;
		case 3:
			swprintf_s(szBuf, __T("BuildingTax"));
			return szBuf;
		case 4:
			swprintf_s(szBuf, __T("MembraneLabor"));
			return szBuf;
		}
		break;
	case CRAD::MILITARY_CRAD:
		switch (CradNumber)
		{
		case 0:
			swprintf_s(szBuf, __T("Militia"));
			return szBuf;
		case 1:
			swprintf_s(szBuf, __T("Shield"));
			return szBuf;
		case 2:
			swprintf_s(szBuf, __T("LongBow"));
			return szBuf;
		}
		break;
	case CRAD::BUILDING_CRAD:
		switch (CradNumber)
		{
		case 0:
			swprintf_s(szBuf, __T("House"));
			return szBuf;
		case 1:
			swprintf_s(szBuf, __T("DefenseWall"));
			return szBuf;
		case 2:
			swprintf_s(szBuf, __T("Factory"));
			return szBuf;
		case 3:
			swprintf_s(szBuf, __T("Monastery"));
			return szBuf;
		case 4:
			swprintf_s(szBuf, __T("Barn"));
			return szBuf;
		case 5:
			swprintf_s(szBuf, __T("ApartmentHouse"));
			return szBuf;
		case 6:
			swprintf_s(szBuf, __T("Farm"));
			return szBuf;
		}
		break;
	case CRAD::TECHNOLOGY_CRAD:
		break;
	}
	return szBuf;
}

