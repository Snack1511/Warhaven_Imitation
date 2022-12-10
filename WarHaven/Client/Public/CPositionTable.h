#pragma once
#include "CTable.h"

BEGIN(Engine)
END

BEGIN(Client)

class CPath;
/* �ʿ��� ��ġ�� �����س��� ���̺� Ŭ���� */

class CPositionTable final
	: public CTable
{
private:
	CPositionTable();
	virtual ~CPositionTable();

	friend class CGameSystem;

public:
	static CPositionTable* Create();

public:
	virtual HRESULT	Initialize() override;
	virtual void	Release() override;

public:
	HRESULT					Load_Position(string strFileKey);
	HRESULT					Load_Path(string strFileKey);

public:
	_float4					Find_Position(string strPositionKey);
	void					Add_Position(string strPositionKey, _float4 vPosition);

	CPath*					Find_Path(string strPathKey);

private:
	map<_hashcode, _float4>	m_mapPosition;
	map<_hashcode, CPath*>	m_mapPathes;
};

END