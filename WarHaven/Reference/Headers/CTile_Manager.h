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
	/* ���̾�� ����. 
	1���� : X �� Ÿ�� ���� 
	2���� : z �� Ÿ�� ���� 
	3���� : Ÿ���� �� ���� ���� ( ��� Ÿ���� ����, ���簢�� )
	4���� : ���̾� ���� */
	HRESULT		Create_Layers(_uint iNumPointsX, _uint iNumPointsZ, _float fTileSize, _uint iNumLayers);
	
private:
	/* ���̾� ������ ��� ����. */
	vector<CTileLayer*>	m_vecLayers;
	_uint				m_iNumLayers = 0;



};

END