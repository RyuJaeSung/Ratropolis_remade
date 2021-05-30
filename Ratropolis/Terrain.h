#pragma once
class CGraphic_Device;
class CTerrain
{
public:
	CTerrain();
	~CTerrain();

public:
	HRESULT Initialize();
	void	Render();
	void	Release();

public:
	void Building_Place(bool Show_Line) { m_bRener_Line = Show_Line; }
	void Change_Option(const D3DXVECTOR3 vPos, int iSize);
	bool Ckeck_Tile(D3DXVECTOR3 vPos);

public:
	vector<TILE*>& Get_Terrain() { return m_vecTile; }
	bool Get_Option(int iTile, int iSize);
	int Get_Tile(int iPosX);
	int Get_TileEx(int iPosX);
	bool Get_Render() { return m_bRender; }
public:
	const void Set_Option(int Tile);
	const void Set_BlueTile(int MaxTile, int MinTile) { m_iMaxTile = MaxTile; m_iMinTile = MinTile; }

public:
	HRESULT LoadTileData(const wstring& wstrFilePath);

private:
	CGraphic_Device* m_pDevice;
	vector<TILE*> m_vecTile;
	bool m_bRener_Line;
	bool m_bRender;
	int m_iMaxTile;
	int m_iMinTile;
};

