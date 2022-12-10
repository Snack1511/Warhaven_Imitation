#pragma once
#include "CImGui_Window.h"

BEGIN(Engine)
class CGameObject;
class CMesh_Terrain;
END

BEGIN(Client)

class CDebugObject;

class CWindow_Position final
	: public CImGui_Window
{
private:
	CWindow_Position();
	virtual ~CWindow_Position();

public:
	static CWindow_Position* Create();

public:
	// CImGui_Window을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void Tick() override;
	virtual HRESULT Render() override;

private:
	char				m_szFileName[MIN_STR] = {};

	vector<pair<string, _float4>>				m_vecPosition;
	vector<CDebugObject*>						m_pDebugBoxes;

	_uint						m_iCurSelectedIndex = 99999;

	_bool						m_bPicking = false;
	_bool						m_bCreateMode = false;

	CMesh_Terrain* m_pMeshTerrain = nullptr;

private:
	void		Find_Terrain();
	void		Update_CreateMode();
	void		Update_SelectMode();

private:
	void		Save_Data();
	void		Load_Data();

private:
	void	Delete_All();
	void	Create_DebugObject(_float4 vPos);
};
END
