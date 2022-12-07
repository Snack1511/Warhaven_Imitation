#pragma once
#include "CTileLayer.h"

BEGIN(Engine)

/* ��ã��� Ÿ�� �Ŵ��� */

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
	/* ���̾�� ����. 
	1���� : X �� Ÿ�� ���� 
	2���� : z �� Ÿ�� ���� 
	3���� : Ÿ���� �� ���� ���� ( ��� Ÿ���� ����, ���簢�� )
	4���� : ���̾� ���� */
	HRESULT		Create_Layers(_uint iNumTilesX, _uint iNumTilesZ, _float fTileSize, _uint iNumLayers);
public:
	/*
		Ÿ�� Ž�� --> A*

	*/
	_bool Get_Route(_int iCurLayer, _int StartIndex, _int iTargetLayer, _int TargetIndex, list<CTile*>& pOutRoute);


	
private:
	/* ���̾� ������ ��� ����. */
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