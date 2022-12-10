#pragma once
#include "Client_Defines.h"

BEGIN(Client)
class CPath
{
	DECLARE_PROTOTYPE(CPath);

private:
	CPath();
	~CPath();
	friend class CPlayer;
	friend class CWindow_Path;
	friend class CPositionTable;

public:
	static CPath* Create(string wstrKey);

public:
	HRESULT	Initialize();
	void	Release();

private:
	string		m_strName;

	/* 진행해야 될 경로 */
	vector<_float4>	m_vecPositions;
	
	_uint			m_iNumPositions = 0;
	_uint			m_iCurIndex = 0;

private:
	void	Save_CurPath();


};

END