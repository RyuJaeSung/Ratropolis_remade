#pragma once
typedef struct tagTexInfo
{
	LPDIRECT3DTEXTURE9	pTexture;
	D3DXIMAGE_INFO		tImageInfo;
}TEXINFO;

typedef struct tagFrame
{
	float fStartFrame;
	float fMaxFrame;
}FRAME;

typedef struct tagTile
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vSize; 
	BYTE byDrawID;
	//0 건설 가능 1 건설 불가능
	BYTE byOption;
}TILE;

typedef struct tagPathInfo
{
	wstring wstrPath;
	wstring wstrObjectKey;
	wstring wstrStateKey;
	DWORD	dwCount;
}PATHINFO;

typedef	struct tagInfo
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vDir;
	D3DXVECTOR3 vSize;
	D3DXMATRIX	matWorld;

	float m_fAngle;
	float m_fSpeed;
}INFO;

typedef struct tagCradInfo
{
	wstring m_wstrName;			//이름(찾을때도 사용됨)
	int		m_iPopulation;		//인구수(건물 지어야할 땅의 수)
	int		m_iAtt;	
	int		m_iHp;
	int		m_iGold;
	int		m_iRange;
	int		m_iImageNumber;		//이미지 숫자
	DWORD	m_dwType;			//카드 종류(0. 경제 1.군사 2.건물 3.기술)
}CRADINFO;

typedef struct tagFont
{
	TCHAR	m_szBuffer[16];
	bool	m_bCheck;
	RECT	m_rSize;
}FONT;

typedef struct tagSatge
{
	int m_iGold;
	int m_iTex;
	int m_iPop;
	int m_iMaxPop;
	int m_iWave;
	int m_iMaxWave;
	int m_iReSetTime;
	int m_iTexTime;
}STAGEINFO;


typedef struct tagSoundInfo
{
	char chunkId[4];
	unsigned long chunkSize;
	char format[4];
	char subChunKID[4];
	unsigned long subChunkSize;
	unsigned short audioFormat;
	unsigned short numChannels;
	unsigned long sampleRate;
	unsigned long bytesPerSecond;
	unsigned short blockAlign;
	unsigned short bitsPerSample;
	char dataChunkID[4];
	unsigned long dataSize;
}SOUNDINFO;