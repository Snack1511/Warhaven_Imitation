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

	/* �÷��̾� ��ġ �����ָ� �ε��� ���� �� */
	void	Update_CurrentIndex(_float4 vCurrentPos);
	void	Release();

public:
	/* CurIndex �� ��ġ�� ���ϴ� ���� return ����. y�� ���� */
	_float4	Get_CurDir(_float4 vCurrentPos);
	_float4	Get_LatestPosition();
	/* ��� ���� �����ߴ��� ���� */
	_bool	Is_Arrived() { return (m_iCurIndex == m_iNumPositions) ? true : false; }

private:
	CAIController* m_pOwnerController = nullptr;

	string		m_strName;

	/* �����ؾ� �� ��� */
	vector<_float4>	m_vecPositions;
	
	_uint			m_iNumPositions = 0;

	_uint			m_iCurIndex = 0;

private:
	void	Save_CurPath();


};

END