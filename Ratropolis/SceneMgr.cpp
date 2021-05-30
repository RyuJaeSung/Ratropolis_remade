#include "stdafx.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "MyMenu.h"
#include "CharacterSelect.h"
#include "Stage.h"

CSceneMgr* CSceneMgr::m_pInstance = nullptr;
CSceneMgr::CSceneMgr()
	:m_pScene(nullptr), m_eNewScene(SCENE::SCENE_END), m_eNowScene(SCENE::SCENE_END)
{
	ZeroMemory(&m_eNULL, sizeof(m_eNULL));
}


CSceneMgr::~CSceneMgr()
{
	Release();
}

bool CSceneMgr::Update()
{
	m_pScene->Update();
	if (!m_pScene)
		return true;
	return false;
}

void CSceneMgr::Late_Update()
{
	m_pScene->Late_Update();
}

void CSceneMgr::Render()
{
	m_pScene->Render();
}

void CSceneMgr::Release()
{
	SAFE_DELETE(m_pScene);
	CSoundMgr::Get_Instance()->StopAll();
}

void CSceneMgr::Scene_Change(SCENE::ID eNextScene)
{
	m_eNewScene = eNextScene;
	if (m_eNewScene != m_eNowScene)
	{
		PLAYER::TYPE ePlayerType = PLAYER::TYPE_END;
		if(m_eNowScene == SCENE::SCENE_CHARSELECT)
			ePlayerType = m_pScene->Get_PlayerType();
		SAFE_DELETE(m_pScene);

		switch (m_eNewScene)
		{
		case SCENE::SCENE_MENU:
			m_pScene = new CMyMenu;
			CSoundMgr::Get_Instance()->PlayBGM(L"Menu.wav");
			break;
		case SCENE::SCENE_CHARSELECT:
			m_pScene = new CCharacterSelect;
			break;
		case SCENE::SCENE_STAGE:
			m_pScene = new CStage;
			m_pScene->Set_PlayerType(ePlayerType);
			CSoundMgr::Get_Instance()->PlayBGM(L"Game.wav");
			break;
		case SCENE::SCENE_QUIT:
			DestroyWindow(g_hWnd);
			return;
		}
		m_pScene->Initialize();
		m_eNowScene = m_eNewScene;
	}
}

STAGEINFO & CSceneMgr::Get_Stage()
{
	if (m_eNowScene == SCENE::SCENE_STAGE)
		return m_pScene->Get_Stage();
	return m_eNULL;
}
