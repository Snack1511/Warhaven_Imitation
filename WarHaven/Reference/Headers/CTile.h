#pragma once
#include "Engine_Defines.h"

#define		MAX_ADJ_TILES	8

BEGIN(Engine)


class CTileLayer;


class ENGINE_DLL CTile final
{
	friend class CTileLayer;

public:
	enum eTileFlags
	{
		/* Ÿ���� �ƴ� �� ġ�� ���� */
		eTileFlags_None = 0,
		eTileFlags_Blocked = (1 << 0),

		/* ���� Ÿ���߿� �ٸ� ���̾��� Ÿ���� �ִ� ���. */
		eTileFlags_Default = (1 << 1),
		eTileFlags_Stair = (1 << 2),
	};

private:
	CTile();
	~CTile();

public:
	static CTile* Create(_float4 vCenterPos, _uint iIndex, CTileLayer* pLayer);

public:
	void	Set_TileFlag(_uint eFlag);
	void	Add_TileFlag(eTileFlags eFlag);
	void	Remove_TileFlag(eTileFlags eFlag);

public:
	/* ��ȿ�� Ÿ���� �� (�� �� �ִ� Ÿ������)*/
	_bool	Is_ValidTile();

private:
	/* ���� Ÿ�� */
	CTile* m_arrAdjTiles[MAX_ADJ_TILES] = {};

	/* Ÿ���� �߾� ������ */
	_float4			m_vCenterPos = ZERO_VECTOR;
	_uint			m_iIndex = 0;
	
	/* Ÿ�� �÷��� */
	_uint		m_eTileFlag = eTileFlags_None;


private:
	/* �� ������ layer */
	CTileLayer* m_pOwnerLayer = nullptr;
	

};

END