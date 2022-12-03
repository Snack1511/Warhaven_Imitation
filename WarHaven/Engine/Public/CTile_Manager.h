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
	/* 레이어들 생성. 
	1인자 : X 축 타일 갯수 
	2인자 : z 축 타일 갯수 
	3인자 : 타일의 한 변의 길이 ( 모든 타일이 동일, 정사각형 )
	4인자 : 레이어 갯수 */
	HRESULT		Create_Layers(_uint iNumPointsX, _uint iNumPointsZ, _float fTileSize, _uint iNumLayers);
	
private:
	/* 레이어 단위로 들고 있음. */
	vector<CTileLayer*>	m_vecLayers;
	_uint				m_iNumLayers = 0;



};

END