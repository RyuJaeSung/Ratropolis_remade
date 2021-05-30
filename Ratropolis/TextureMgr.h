#pragma once

class CTexture;
class CTextureMgr final
{
private:
	CTextureMgr();
	~CTextureMgr();
public:
	enum TEX_ID { SINGLE, MULTI, TEX_END };
	
public:
	const TEXINFO* Get_TexInfo(const wstring& wstrObjectKey, 
		const wstring& wstrStateKey = __T(""),
		const int& rCount = 0); 
	HRESULT Insert_Texture(TEX_ID eTexID, const wstring& wstrFilePath, 
		const wstring& wstrObjectKey, 
		const wstring& wstrStateKey = __T(""),
		const int& rCount = 0);
	void Release();

public:
	HRESULT ImageLoadFromFile(const wstring& wstrFilePath);
	RECT Get_Rect(const D3DXVECTOR3& vecPos, const wstring& wstrObjectKey, const wstring& wstrStateKey, const int& rCount);

public:
	static CTextureMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CTextureMgr;
		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		SAFE_DELETE(m_pInstance);
	}

private:
	static CTextureMgr* m_pInstance;
	map<wstring, CTexture*> m_mapTex;
};

