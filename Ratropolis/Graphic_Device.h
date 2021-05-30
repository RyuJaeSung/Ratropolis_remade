#pragma once
class CGraphic_Device
{
private:
	CGraphic_Device();
	~CGraphic_Device();
public:
	enum WINMODE { MODE_FULL,MODE_WIN};

public:
	inline LPDIRECT3DDEVICE9	Get_Device() { return m_pDevice; }
	inline LPD3DXSPRITE			Get_Sprite() { return m_pSprite; }
	inline LPD3DXFONT			Get_Font()	 { return m_pFont;	 }

public:
	HRESULT	Initialize(WINMODE eWinMode);
	void	Release();
public:
	void	Render_Begin();
	void	Render_End(HWND hWnd = nullptr);

public:
	inline static CGraphic_Device* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CGraphic_Device;
		return m_pInstance;
	}
	inline static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	static CGraphic_Device* m_pInstance;
	LPDIRECT3D9			m_pSDK; 
	LPDIRECT3DDEVICE9	m_pDevice;
	LPD3DXSPRITE		m_pSprite;
	LPD3DXFONT			m_pFont;
};

