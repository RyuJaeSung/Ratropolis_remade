#pragma once
#include "Scene.h"
class CTerrain;
class CStage :
	public CScene
{
public:
	explicit CStage();
	virtual ~CStage();

public:
	void Render_Background();
	void Render_Font();
	void Render_Black();

public:
	void Add_Ui();
	void Add_Gold();
public:
	void Cloud_Move();
	void StageStart();

public:
	CTerrain* Get_Terrain() { return m_pTerrain; }
	bool Get_StageStart() { return m_bStageStart; }
public:
	virtual HRESULT Initialize() override;
	virtual void Update() override;
	virtual void Late_Update() override;
	virtual void Render() override;
	virtual void Release() override;

private:
	D3DXVECTOR3		m_vRewardPos;
	CTerrain*		m_pTerrain;
	bool			m_bCloud;
	bool			m_bStageStart;
	bool			m_bCreateWave;
	float			m_fCreateWave;
	float			m_fCloudMove;
	float			m_fSpeed;
	float			m_fFPSTime;
	float			m_fGameTime;
};

