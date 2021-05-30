#pragma once
class CGraphic_Device;
class CScene
{
public:
	explicit CScene();
	virtual ~CScene();
public:
	const PLAYER::TYPE Get_PlayerType() { return m_ePlayerType; }
	STAGEINFO& Get_Stage() { return m_tStage; }

public:
	void Set_PlayerType(const PLAYER::TYPE eType) { m_ePlayerType = eType; }

public:
	virtual HRESULT Initialize() PURE;
	virtual void Update() PURE;
	virtual void Late_Update() PURE;
	virtual void Render() PURE;
	virtual void Release() PURE;
	
protected:
	CGraphic_Device* m_pDevice;
	STAGEINFO		m_tStage;
	PLAYER::TYPE	m_ePlayerType;
};

