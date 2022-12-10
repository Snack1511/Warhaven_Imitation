#pragma once
#include "CImGui_Window.h"

BEGIN(Engine)
class CGameObject;
class CMesh_Terrain;
END

BEGIN(Client)

class CDebugObject;
class CPath;

class CWindow_Path final
	: public CImGui_Window
{
private:
	CWindow_Path();
	virtual ~CWindow_Path();

public:
	static CWindow_Path* Create();

public:
	// CImGui_Window을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void Tick() override;
	virtual HRESULT Render() override;

private:

	struct PATH_DESC
	{
		CPath*	pPath = nullptr;
		vector<CDebugObject*>	vecDebugBoxes;
		vector<CDebugObject*>	vecLineBox;
		_bool	bSelected = false;
		_uint	iCurSelectedPositionIdx = 99999;

	};

	vector<PATH_DESC>	m_vecPathDesc;
	_uint						m_iCurSelectedIndex = 99999;
	_bool						m_bPicking = false;

	_bool						m_bCreateMode = false;

	CMesh_Terrain* m_pMeshTerrain = nullptr;

private:
	void		Find_Terrain();
	void		Update_CreateMode();
	void		Update_SelectMode();

private:
	void	Create_DebugObject(_float4 vPos);
	void	Create_NewPath();

	void	ReMake_Lines(_uint iCurPathIndex);
};
END
