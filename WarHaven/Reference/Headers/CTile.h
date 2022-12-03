#pragma once
#include "Engine_Defines.h"

#define		MAX_ADJ_TILES	8

BEGIN(Engine)


class CTileLayer;


class CTile final
{
	friend class CTileLayer;

public:
	enum eTileFlags
	{
		/* Ÿ���� �ƴ� �� ġ�� ���� */
		eTileFlags_None = 0,
		eTileFlags_Blocked = (1 << 0),

		/* ���� Ÿ���߿� �ٸ� ���̾��� Ÿ���� �ִ� ���. */
		eTileFlags_Stair = (1 << 1),
	};

private:
	CTile();
	~CTile();

public:
	void	Set_TileFlag(_uint eFlag);
	void	Add_TileFlag(eTileFlags eFlag);
	void	Remove_TileFlag(eTileFlags eFlag);

public:
	/* ��ȿ�� Ÿ���� �� (�� �� �ִ� Ÿ������)*/
	_bool	Is_VaildTile();

private:
	/* ���� Ÿ�� */
	CTile* m_arrAdjTiles[MAX_ADJ_TILES] = {};

	/* Ÿ���� �߾� ������ */
	_float4			m_vCenterPos;
	
	/* Ÿ�� �÷��� */
	_uint		m_eTileFlag = eTileFlags_None;


private:
	/* �� ������ layer */
	CTileLayer* m_pOwnerLayer = nullptr;
	

};

END