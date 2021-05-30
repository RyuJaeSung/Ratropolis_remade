#pragma once
class CGraphic_Device;
class CObj
{
public:
	explicit CObj();
	virtual ~CObj();

public:
	void Set_Pos(const D3DXVECTOR3 vecPos) { m_tInfo.vPos = vecPos; }
	void Set_Size(const D3DXVECTOR3 vecSize) { m_tInfo.vSize = vecSize; }
	void Set_Angle(const float fAngle) { m_tInfo.m_fAngle = fAngle; }
	void Set_Path(const wstring& wstrObjectKey, const wstring& wstrStateKey, const DWORD& dwCount) { m_wstrObjectKey = wstrObjectKey; m_wstrStateKey = wstrStateKey; m_dwCount = dwCount; }
	void Set_TileConut(const int& Tile) { m_iTileConut = Tile; }
	void Set_Damge(const int& iDamge) { m_tUnitInfo.m_iHp -= iDamge; }

public:
	const wstring& Get_CradName() { return m_wstrStateKey; }
	const D3DXVECTOR3& Get_Pos() { return m_tInfo.vPos; }
	const D3DXVECTOR3& Get_Size() { return m_tInfo.vSize; }
	const int Get_CradNumber() { return m_dwCount; }
	const CRAD::TYPE Get_Type();
	int Get_TileCount() { return m_iTileConut; }
	int Get_iHp() { return m_tUnitInfo.m_iHp; }
public:
	virtual HRESULT	Initialize() PURE;
	virtual int		Update() PURE;
	virtual void	Late_Update() PURE;
	virtual void	Render() PURE;
	virtual void	Release() PURE;

protected:
	CGraphic_Device* m_pDevice;
	int			m_iTileConut;
	INFO		m_tInfo;
	CRADINFO	m_tUnitInfo;
	wstring		m_wstrObjectKey;
	wstring		m_wstrStateKey;
	DWORD		m_dwCount;
};

