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

	/* �÷��̾� ��ġ �����ָ� �ε��� ���� �� */
	void	Update_CurrentIndex(_float4 vCurrentPos);
	void	Release();

public:
	_float Get_CurY() { return m_vecPositions[m_iCurIndex].y; }

	/* CurIndex �� ��ġ�� ���ϴ� ���� return ����. y�� ���� */
	_float4	Get_CurDir(_float4 vCurrentPos);
	_float4	Get_LatestPosition();
	/* ��� ���� �����ߴ��� ���� */
	_bool	Is_Arrived() { return (m_iCurIndex == (m_iNumPositions-1)) ? true : false; }
	/* �ܺο��� ��� ���� �����ߴٰ� ������ �ٲ��ִ� ���� ex) �ʹ� ���� ���� �ε����� ���..*/
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

	/* �����ؾ� �� ��� */
	vector<_float4>	m_vecPositions;
	
	_uint			m_iNumPositions = 0;

	_uint			m_iCurIndex = 0;
	_uint			m_iPrevIndex = 0;

private: /* �̵��� ������ */
	_float4			m_vPrevPos = ZERO_VECTOR;
	_float			m_fMoveAcc = 0.f;

private:
	void	Save_CurPath();


};

END