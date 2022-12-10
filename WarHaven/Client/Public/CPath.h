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
	/* 플레이어 위치 던져주면 인덱스 갱신 함 */
	void	Update_CurrentIndex(_float4 vCurrentPos);
	void	Release();

public:
	_float4	Get_CurDir();
	_float4	Find_NearestPosition();

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