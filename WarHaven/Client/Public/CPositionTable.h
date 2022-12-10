#pragma once
#include "CTable.h"

BEGIN(Engine)
END

BEGIN(Client)

class CPath;
/* 필요한 위치를 저장해놓는 테이블 클래스 */

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

public:
	_float4					Find_Position(string strPositionKey);
	void					Add_Position(string strPositionKey, _float4 vPosition);


private:
	map<_hashcode, _float4>	m_mapPosition;
};

END