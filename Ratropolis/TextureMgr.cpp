#include "stdafx.h"
#include "TextureMgr.h"
#include "SingleTex.h"
#include "MultiTex.h"


CTextureMgr* CTextureMgr::m_pInstance = nullptr;
CTextureMgr::CTextureMgr()
{
}


CTextureMgr::~CTextureMgr()
{
	Release();
}

const TEXINFO * CTextureMgr::Get_TexInfo(const wstring & wstrObjectKey, const wstring & wstrStateKey, const int & rCount)
{
	auto& iter_find = m_mapTex.find(wstrObjectKey);

	if (m_mapTex.end() == iter_find)
		return nullptr;

	return iter_find->second->Get_TexInfo(wstrStateKey, rCount);
}

HRESULT CTextureMgr::Insert_Texture(TEX_ID eTexID, const wstring & wstrFilePath, const wstring & wstrObjectKey, const wstring & wstrStateKey, const int & rCount)
{
	auto& iter_find = m_mapTex.find(wstrObjectKey);
	if (m_mapTex.end() == iter_find)
	{
		CTexture* pTexture = nullptr;
		switch (eTexID)
		{
		case SINGLE:
			pTexture = new CSingleTex;
			break;
		case MULTI:
			pTexture = new CMultiTex;
			break;
		default:
			break;
		}
		if (FAILED(pTexture->Insert_Texture(wstrFilePath, wstrStateKey, rCount)))
		{
			wstring wstrErrMsg = wstrObjectKey + L"Insert Texture Failed";
			ERR_MSG(wstrErrMsg.c_str());
			return E_FAIL;
		}
		m_mapTex.emplace(wstrObjectKey, pTexture);
	}
	else if (MULTI == eTexID)
	{
		if (FAILED(m_mapTex[wstrObjectKey]->Insert_Texture(wstrFilePath, wstrStateKey, rCount)))
		{
			return E_FAIL;
		}
	}
	return S_OK;
}

void CTextureMgr::Release()
{
	for (auto& rPair : m_mapTex)
	{
		Safe_Delete(rPair.second);
	}
	m_mapTex.clear();
}

HRESULT CTextureMgr::ImageLoadFromFile(const wstring & wstrFilePath)
{
	wifstream fin;
	fin.open(wstrFilePath.c_str());

	if (!fin.fail())
	{
		TCHAR szPath[MAX_PATH] = L"";
		TCHAR szObjectKey[MAX_PATH] = L"";
		TCHAR szStateKey[MAX_PATH] = L"";
		TCHAR szCount[MAX_PATH] = L"";
		DWORD dwCount = 0;
		while (true)
		{
			fin.getline(szPath, MAX_PATH, '|');
			fin.getline(szObjectKey, MAX_PATH, '|');
			fin.getline(szStateKey, MAX_PATH, '|');
			fin.getline(szCount, MAX_PATH);
			dwCount = _ttoi(szCount);

			if (fin.eof())
				break;

			if (FAILED(Insert_Texture(MULTI, szPath, szObjectKey, szStateKey, dwCount)))
			{
				ERR_MSG(L"FilePath Load Failed");
				fin.close();
				return E_FAIL;
			}
		}
		fin.close();
	}
	return S_OK;
}

RECT CTextureMgr::Get_Rect(const D3DXVECTOR3& vecPos, const wstring & wstrObjectKey, const wstring & wstrStateKey, const int & rCount)
{
	const TEXINFO* pTexInfo = Get_TexInfo(wstrObjectKey, wstrStateKey, rCount);
	float fx = float(pTexInfo->tImageInfo.Width >> 1);
	float fy = float(pTexInfo->tImageInfo.Height >> 1);
	return { LONG(vecPos.x - fx), LONG(vecPos.y - fy), LONG(vecPos.x + fx), LONG(vecPos.y + fy) };
}