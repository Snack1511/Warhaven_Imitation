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
	_uint		Find_Index(_float4 vPosition);

public:
	/* ���̾�� ����. 
	1���� : X �� Ÿ�� ���� 
	2���� : z �� Ÿ�� ���� 
	3���� : Ÿ���� �� ���� ���� ( ��� Ÿ���� ����, ���簢�� )
	4���� : ���̾� ���� */
	HRESULT		Create_Layers(_uint iNumTilesX, _uint iNumTilesZ, _float fTileSize, _uint iNumLayers);
	
private:
	/* ���̾� ������ ��� ����. */
	vector<CTileLayer*>	m_vecLayers;
	_uint				m_iNumLayers = 0;

	_uint				m_iNumTilesX = 0;
	_uint				m_iNumTilesZ = 0;
	_uint				m_iTotalTileSize = 0;

	_float				m_fTileSize = 0.f;



};

END