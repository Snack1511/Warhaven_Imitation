#pragma once
#include "CTileLayer.h"

BEGIN(Engine)

/* 길찾기용 타일 매니저 */

class CTile_Manager final
{
	DECLARE_SINGLETON(CTile_Manager)

private:
	CTile_Manager();
	~CTile_Manager();

public:
	HRESULT		Initialize();
	HRESULT		Render();
	void		Release();

public:
	void		Set_WorldMatrix(_float4x4 matWorld);
	void		Set_WorldPos(_float4 vPos);
	_uint		Find_Index(_float4 vPosition);
	CTile*		Find_Tile(_uint iLayer, _uint iIndex);

	_float4x4	Get_WorldMatrix() { return m_matWorld; }
	_float4		Get_WorldPos() { return m_matWorld.XMLoad().r[3]; }

public:
	/* 레이어들 생성. 
	1인자 : X 축 타일 갯수 
	2인자 : z 축 타일 갯수 
	3인자 : 타일의 한 변의 길이 ( 모든 타일이 동일, 정사각형 )
	4인자 : 레이어 갯수 */
	HRESULT		Create_Layers(_uint iNumTilesX, _uint iNumTilesZ, _float fTileSize, _uint iNumLayers);
public:
	/*
		타일 탐색 --> A*

	*/
	_bool Get_Route(_int iCurLayer, _int StartIndex, _int iTargetLayer, _int TargetIndex, list<CTile*>& pOutRoute);


	
private:
	/* 레이어 단위로 들고 있음. */
	vector<CTileLayer*>	m_vecLayers;
	_uint				m_iNumLayers = 0;

	_uint				m_iNumTilesX = 0;
	_uint				m_iNumTilesZ = 0;
	_uint				m_iTotalTileSize = 0;

	_float				m_fTileSize = 0.f;

	/* worldmat*/
	_float4x4			m_matWorld;
	_float4x4			m_matWorldInv;



};

END