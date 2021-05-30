#include "stdafx.h"
#include "Terrain.h"
#include "TextureMgr.h"
#include "TimeMgr.h"
#include "ScrollMgr.h"
#include "Graphic_Device.h"

CTerrain::CTerrain()
	:m_bRener_Line(false), m_pDevice(nullptr), m_iMinTile(0), m_iMaxTile(0), m_bRender(false)
{
}


CTerrain::~CTerrain()
{
	Release();
}

HRESULT CTerrain::Initialize()
{
	if (FAILED(LoadTileData(__T("../Data/Map.dat"))))
	{
		ERR_MSG(__T("Map Load Failed"));
		return E_FAIL;
	}

	m_pDevice = CGraphic_Device::Get_Instance();

	return S_OK;
}

void CTerrain::Render()
{
	D3DXMATRIX matScale, matTrans, matWorld;
	int ivecSize = m_vecTile.size();
	float fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();

	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_TexInfo(__T("Terrain"), __T("Tile"), 0);
	if (!pTexInfo)
	{
		ERR_MSG(__T("Terrain Render Failed"));
		return;
	}

	const TEXINFO* pTexLine = CTextureMgr::Get_Instance()->Get_TexInfo(__T("InGame"), __T("Wall"), 9);
	if (!pTexLine)
		return;

	for (int i = 0; i < ivecSize; ++i)
	{

		if (m_vecTile[i]->vPos.x < (-fScrollX + (WINCX + 100.f)) && m_vecTile[i]->vPos.x  > (-fScrollX - 100.f))
		{
			int num = m_vecTile[i]->byOption;
			float fCenterX = float(pTexInfo->tImageInfo.Width >> 1);
			float fCenterY = float(pTexInfo->tImageInfo.Height >> 1);
			 
			if (m_iMinTile < i && i < m_iMaxTile)
			{
				m_bRender = true;
				if (m_bRener_Line && m_vecTile[i]->byOption == 0)
				{
					float fCenterLineX = float(pTexLine->tImageInfo.Width >> 1);
					float fCenterLineY = float(pTexLine->tImageInfo.Height >> 1);

					D3DXMatrixScaling(&matScale, m_vecTile[i]->vSize.x, m_vecTile[i]->vSize.y, 0.f);
					D3DXMatrixTranslation(&matTrans, m_vecTile[i]->vPos.x + fScrollX, m_vecTile[i]->vPos.y - fCenterY, 0.f);
					matWorld = matScale * matTrans;

					m_pDevice->Get_Sprite()->SetTransform(&matWorld);
					m_pDevice->Get_Sprite()->Draw(pTexLine->pTexture, nullptr,
						&D3DXVECTOR3(fCenterLineX, 0.f, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
				}
			}
			else
				m_bRender = false;
			D3DXMatrixScaling(&matScale, m_vecTile[i]->vSize.x, m_vecTile[i]->vSize.y, 0.f);
			D3DXMatrixTranslation(&matTrans, m_vecTile[i]->vPos.x + fScrollX, m_vecTile[i]->vPos.y, 0.f);
			matWorld = matScale * matTrans;

			m_pDevice->Get_Sprite()->SetTransform(&matWorld);
			m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr, 
				&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


			float fYTurn = 60.f+ m_vecTile[i]->vPos.y;
			D3DXMatrixScaling(&matScale, m_vecTile[i]->vSize.x, -m_vecTile[i]->vSize.y, 0.f);
			D3DXMatrixTranslation(&matTrans, m_vecTile[i]->vPos.x + fScrollX, fYTurn, 0.f);
			matWorld = matScale * matTrans;

			m_pDevice->Get_Sprite()->SetTransform(&matWorld);
			m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, nullptr,
				&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(120, 255, 255, 255));


			//TCHAR szBuf[16] = {};
			//swprintf_s(szBuf, __T("%d"), i + 1);
			//m_pDevice->Get_Font()->DrawTextW(m_pDevice->Get_Sprite(), szBuf, lstrlen(szBuf), nullptr, 0, D3DCOLOR_ARGB(255, 255, 0, 0));
		}
	}
}


void CTerrain::Release()
{
	for_each(m_vecTile.begin(), m_vecTile.end(), Safe_Delete<TILE*>);
	m_vecTile.clear();
	m_vecTile.swap(vector<TILE*>());
}

void CTerrain::Change_Option(const D3DXVECTOR3 vPos, int iSize)
{
	float fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	int iTileSize = m_vecTile.size();

	for (int i = 0; i < iTileSize; ++i)
	{	
		if (m_vecTile[i]->vPos.x - (TILECX >> 1) * m_vecTile[i]->vSize.x <= vPos.x && m_vecTile[i]->vPos.x + (TILECX >> 1) * m_vecTile[i]->vSize.x >= vPos.x)
		{
			m_vecTile[i]->byOption = 1;
			if (iSize == 1)
				return;
			else if (iSize % 2 != 0)
			{
				for (int j = 1; j <= iSize * 0.5; ++j)
				{
					m_vecTile[i + j]->byOption = 1;
					m_vecTile[i - j]->byOption = 1;
				}
			}
			else
				m_vecTile[i + 1]->byOption = 1;
		}
	}
}

bool CTerrain::Ckeck_Tile(D3DXVECTOR3 vPos)
{
	float fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	int iTileSize = m_vecTile.size();

	for (int i = 0; i < iTileSize; ++i)
	{
		if (m_vecTile[i]->vPos.x + fScrollX > (fScrollX + (WINCX >> 1)) || m_vecTile[i]->vPos.x + fScrollX < (fScrollX - (WINCX >> 1)))
		{

			if (m_vecTile[i]->vPos.x  - (TILECX >> 1) * m_vecTile[i]->vSize.x <= vPos.x && m_vecTile[i]->vPos.x + (TILECX >> 1) * m_vecTile[i]->vSize.x >= vPos.x)
			{
				if (m_vecTile[i]->byOption == 0)
					return true;
				else
					return false;
			}
		}
	}
	return false;
}

int CTerrain::Get_Tile(int iPosX)
{
	float fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	int ivecSize = m_vecTile.size();

	for (int i = 0; i < ivecSize; ++i)
	{
		if (m_vecTile[i]->vPos.x + fScrollX - (TILECX >>1) <= iPosX && m_vecTile[i]->vPos.x + fScrollX + (TILECX>>1) >= iPosX)
			return i;
	}
	return 0;
}

int CTerrain::Get_TileEx(int iPosX)
{
	float fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	int ivecSize = m_vecTile.size();

	for (int i = 0; i < ivecSize; ++i)
	{
		if (m_vecTile[i]->vPos.x < (-fScrollX + (WINCX + 100.f)) && m_vecTile[i]->vPos.x  >(-fScrollX - 100.f))
		{
			if (m_vecTile[i]->vPos.x + fScrollX - (TILECX >> 1) <= iPosX && m_vecTile[i]->vPos.x + fScrollX + (TILECX >> 1) >= iPosX)
				return i;
		}
	}
	return 0;
}

bool CTerrain::Get_Option(int iTile, int iSize)
{
	int ipOption, imOption;

	if (m_vecTile[iTile]->byOption == 1)
		return false;

	for (int i = 1; i <= iSize * 0.5f; ++i)
	{
		ipOption = m_vecTile[iTile + i]->byOption;
		imOption = m_vecTile[iTile - i]->byOption;

		if (ipOption == 1 || imOption == 1)
			return false;
	}
	return true;
}

const void CTerrain::Set_Option(int Tile)
{
	if (m_vecTile[Tile]->byOption == 0)
		m_vecTile[Tile]->byOption = 1;
	else
		m_vecTile[Tile]->byOption = 0;
}

HRESULT CTerrain::LoadTileData(const wstring & wstrFilePath)
{
	HANDLE hFile = CreateFile(wstrFilePath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		return E_FAIL;
	}
	DWORD dwByte = 0;
	TILE* pTile = nullptr;

	while (1)
	{
		pTile = new TILE;
		ReadFile(hFile, pTile, sizeof(TILE), &dwByte, nullptr);
		if (dwByte == 0)
		{
			SAFE_DELETE(pTile);
			break;
		}
		m_vecTile.push_back(pTile);
	}
	CloseHandle(hFile);
	return S_OK;
}