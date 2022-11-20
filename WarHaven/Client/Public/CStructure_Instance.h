#pragma once
#include "CStructure.h"
BEGIN(Engine)
class CModel;
class CModel_Renderer;
class CCollider;
class CCollider_Sphere;
class CShader;
END
BEGIN(Client)
class CStructure_Instance :
    public CStructure
{
	////테스트
	DECLARE_PROTOTYPE(CStructure_Instance)//Clone
	//DECLARE_GAMEOBJECT(CStructure)//Create
protected:
	CStructure_Instance();
	virtual ~CStructure_Instance();

public:
	static CStructure_Instance* Create(wstring MeshPath, wstring strInstancePath);
	static CStructure_Instance* Create(wstring MeshPath, _int iNumInstance, VTXINSTANCE* pInstanceVTX);

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;

protected:
	virtual	HRESULT	SetUp_InstanceModel(wstring strMeshPath,wstring strInstnacePath);
	virtual	HRESULT	SetUp_InstanceModel(wstring strMeshPath, _int iNumInstance, VTXINSTANCE* pInstanceVTX);

protected:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;
};
END
