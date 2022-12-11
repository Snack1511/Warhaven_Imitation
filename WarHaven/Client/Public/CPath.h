#pragma once
#include "Client_Defines.h"

BEGIN(Client)

class CAIController;

class CPath
{
	DECLARE_PROTOTYPE(CPath);

private:
	CPath();
	~CPath();
	friend class CAIController;
	friend class CWindow_Path;
	friend class CGameSystem;

public:
	static CPath* Create(string strKey);

public:
	HRESULT	Initialize();

	/* 플레이어 위치 던져주면 인덱스 갱신 함 */
	void	Update_CurrentIndex(_float4 vCurrentPos);
	void	Release();

public:
	/* CurIndex 의 위치로 향하는 방향 return 해줌. y값 제거 */
	_float4	Get_CurDir(_float4 vCurrentPos);
	_float4	Get_LatestPosition();
	/* 경로 끝에 도달했는지 여부 */
	_bool	Is_Arrived() { return (m_iCurIndex == m_iNumPositions) ? true : false; }

private:
	CAIController* m_pOwnerController = nullptr;

	string		m_strName;

	/* 진행해야 될 경로 */
	vector<_float4>	m_vecPositions;
	
	_uint			m_iNumPositions = 0;

	_uint			m_iCurIndex = 0;

private:
	void	Save_CurPath();


};

END