#pragma once
#include "CImGui_Window.h"

BEGIN(Engine)
class CGameObject;
class CMesh_Terrain;
END

BEGIN(Client)

class CDebugObject;

class CWindow_PhysX final
	: public CImGui_Window
{
private:
	CWindow_PhysX();
	virtual ~CWindow_PhysX();

public:
	static CWindow_PhysX* Create();

public:
	// CImGui_Window을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void Tick() override;
	virtual HRESULT Render() override;

private:
	char				m_szName[MIN_STR] = {};

	vector<PxRigidStatic*>		m_StaticBoxes;
	vector<CDebugObject*>		m_pDebugBoxes;

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
	PxRigidStatic* Create_StaticBox(PxTransform tTransform, _float4 vScale);
	
	void	Delete_All();
	void	Create_TestObject();
	void	Create_DebugObject(_float4 vPos, _float4 vScale, _float4 vAngle);
};
END
