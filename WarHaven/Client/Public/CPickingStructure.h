#pragma once
#include "CStructure.h"
#include "Client_Defines.h"
BEGIN(Engine)
class CModel;
class CCollider;
class CCollider_Sphere;
class CShader;
END
BEGIN(Client)
class CPickingStructure :
    public CStructure
{
	//�׽�Ʈ
	DECLARE_PROTOTYPE(CPickingStructure)//Clone
	DECLARE_GAMEOBJECT(CPickingStructure)//Create
protected:
	CPickingStructure();
	virtual ~CPickingStructure();
public:
	// CGameObject��(��) ���� ��ӵ�

	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;


protected:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;
};

END