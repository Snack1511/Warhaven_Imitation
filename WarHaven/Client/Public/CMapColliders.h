#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
END

BEGIN(Client)

class CDebugObject;

class CMapColliders
	: public CGameObject
{
	DECLARE_PROTOTYPE(CMapColliders);

private:
	CMapColliders();
	virtual ~CMapColliders();

public:
	static CMapColliders* Create(string wstrFileKey);

public:
	virtual HRESULT	Initialize_Prototype() override;
	virtual HRESULT	Initialize() override;
	virtual HRESULT	Start() override;
	virtual void Release() override;

private:
	vector<PxRigidStatic*>		m_StaticBoxes;

private:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;

private:
	HRESULT		SetUp_MapColliders(string wstrFileKey);
	PxRigidStatic* Create_StaticBox(PxTransform tTransform, _float4 vScale);
};
END
