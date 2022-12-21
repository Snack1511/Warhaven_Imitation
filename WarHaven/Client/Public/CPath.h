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
	friend class CPlayer;

public:
	static CPath* Create(string strKey);

public:
	HRESULT	Initialize();

	/* 플레이어 위치 던져주면 인덱스 갱신 함 */
	void	Update_CurrentIndex(_float4 vCurrentPos);
	void	Release();

public:
	_float Get_CurY() { return m_vecPositions[m_iCurIndex].y; }

	/* CurIndex 의 위치로 향하는 방향 return 해줌. y값 제거 */
	_float4	Get_CurDir(_float4 vCurrentPos);
	_float4	Get_LatestPosition();
	/* 경로 끝에 도달했는지 여부 */
	_bool	Is_Arrived() { return (m_iCurIndex == (m_iNumPositions-1)) ? true : false; }
	/* 외부에서 경로 끝에 도달했다고 강제로 바꿔주는 구문 ex) 너무 오래 같은 인덱스일 경우..*/
	void	Set_Arrived();

	_uint	Get_CurIndex() { return m_iCurIndex; }
	_uint	Get_PrevIndex() { return m_iPrevIndex; }
	_float4 Get_FrontPosition();
	void	Init_Indices();
	string Get_PathName();

	_float	Get_MoveAcc() { return m_fMoveAcc; }
	void	Init_MoveAcc() { m_fMoveAcc = 0.f; }

	vector<_float4>& Get_vecPositions() { return m_vecPositions; }

private:
	CAIController* m_pOwnerController = nullptr;

	string		m_strName;

	/* 진행해야 될 경로 */
	vector<_float4>	m_vecPositions;
	
	_uint			m_iNumPositions = 0;

	_uint			m_iCurIndex = 0;
	_uint			m_iPrevIndex = 0;

private: /* 이동량 누적용 */
	_float4			m_vPrevPos = ZERO_VECTOR;
	_float			m_fMoveAcc = 0.f;

private:
	void	Save_CurPath();


};

END