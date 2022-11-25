#pragma once
#include "Engine_Defines.h"

BEGIN(Engine)

class CLight_Manager final
{
	DECLARE_SINGLETON(CLight_Manager)
private:
	CLight_Manager();
	virtual ~CLight_Manager();

public:
	const LIGHTDESC* Get_LightDesc(_uint iIndex);
	class CLight* Get_Light(_uint iIndex);
	class CLight* Get_FirstLight();
	class CLight* Get_LastLight();
public:
	HRESULT Add_Light(const LIGHTDESC& LightDesc);
	HRESULT Render_Lights(class CShader* pShader, class CMesh_Rect* pVIBuffer);
	void	Clear_Lights();
	void	Pop_Light();
	void	Remove_Light(_uint iIndex);
	HRESULT	Load_Lights(wstring wstrPath);
	_int Get_LightSize() { return _int(m_Lights.size()); }
public:
	void	Update_Lights();

private:
	list<class CLight*>				m_Lights;
	list<LIGHTDESC>				m_ReservedLights;
	typedef list<class CLight*>		LIGHTS;

public:
	void Release();
};

END