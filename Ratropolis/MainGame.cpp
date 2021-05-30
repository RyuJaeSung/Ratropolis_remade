#include "stdafx.h"
#include "MainGame.h"
#include "Graphic_Device.h"
#include "FrameMgr.h"
#include "TimeMgr.h"
#include "ScrollMgr.h"
#include "CradMgr.h"
#include "KeyMgr.h"
#include "SceneMgr.h"
#include "MouseMgr.h"
#include "TextureMgr.h"


CMainGame::CMainGame()
{
}


CMainGame::~CMainGame()
{
	Release();
}

void CMainGame::Initialize()
{
	CSoundMgr::Get_Instance()->Initialize();
	if (FAILED(CGraphic_Device::Get_Instance()->Initialize(CGraphic_Device::MODE_WIN)))
	{
		ERR_MSG(__T("Graphic_Device Create Failed"));
		return;
	}
	if (FAILED(CTextureMgr::Get_Instance()->ImageLoadFromFile(__T("../Data/PathExtraction.txt"))))
	{
		ERR_MSG(__T("ImageLoadFromFile Failed"));
		return;
	}
	CMouseMgr::Get_Instance()->Initialize();
	CSceneMgr::Get_Instance()->Scene_Change(SCENE::SCENE_MENU);
	CCradMgr::Get_Instance()->Initialize();
}

bool CMainGame::Update()
{
	CKeyMgr::Get_Instance()->Update();
	CTimeMgr::Get_Instance()->Update();
	
	if (CSceneMgr::Get_Instance()->Update())
		return true;

	if (GetAsyncKeyState(VK_LBUTTON))
		CMouseMgr::Get_Instance()->Set_State(1);
	else
		CMouseMgr::Get_Instance()->Set_State(0);
	

	return false;
}

void CMainGame::Late_Update()
{
	CSceneMgr::Get_Instance()->Late_Update();
}

void CMainGame::Render()
{
	CGraphic_Device::Get_Instance()->Render_Begin();
	CSceneMgr::Get_Instance()->Render();
	CFrameMgr::Get_Instance()->RenderFrame();
	CMouseMgr::Get_Instance()->Render();
	CGraphic_Device::Get_Instance()->Render_End();
}

void CMainGame::Release()
{
	CSoundMgr::Destroy_Instance();
	CMouseMgr::Destroy_Instance();
	CCradMgr::Destroy_Instance();
	CKeyMgr::Destroy_Instance();
	CTimeMgr::Destroy_Instance();
	CSceneMgr::Destroy_Instance();
	CFrameMgr::Destroy_Instance();
	CScrollMgr::Destroy_Instance();
	CTextureMgr::Destroy_Instance();
	CGraphic_Device::Destroy_Instance();
}
