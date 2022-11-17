#pragma once
#include "GameObject.h"
#include "Client_Defines.h"
BEGIN(Engine)
class CModel;
class CModel_Renderer;
class CCollider;
class CCollider_Sphere;
class CShader;
class CPhysXCollider;
END
BEGIN(Client)
class CStructure :
    public CGameObject
{
	////테스트
	DECLARE_PROTOTYPE(CStructure)//Clone
	//DECLARE_GAMEOBJECT(CStructure)//Create
protected:
    CStructure();
    virtual ~CStructure();

public:
	static CStructure* Create(wstring MeshPath);
	static CStructure* Create(wstring MeshPath, _float4x4 ObjectWorldMatrix);
	static CStructure* Create(wstring MeshPath, _float4 vScale, _float4x4 ObjectWorldMatrix);

public:
	virtual void	Unit_CollisionEnter(CGameObject* pOtherObj, const _uint& eColType);
	virtual void	Unit_CollisionStay(CGameObject* pOtherObj, const _uint& eColType);
	virtual void	Unit_CollisionExit(CGameObject* pOtherObj, const _uint& eColType) {};

public:
	virtual void	Set_ShaderResource(CShader* pShader, const char* pConstantName);
	virtual void	Set_Passes(VTXMODEL_PASS_TYPE ePassType);

public:
	enum class ePhysXEnum {eCONVEX, eTRIANGLE, eBOX, eEND};

	/* iLODLevel 에 맞춰 충돌체 생성, 실패시 걍 안만들어짐 */
	void			Make_PhysXCollider(ePhysXEnum eShapeType, _uint iLODLevel = 3);

	/* 박스 추가함. 해당 컴포넌트 반환.*/
	CPhysXCollider* Make_PhysXCollier_Box();

	/* 박스 위치 옮김*/
	void			RePosition_Box(_uint iIndex, _float4 vOffsetPosition = ZERO_VECTOR);
	/* 박스 크기 바꿈*/
	void			ReScale_Box(_uint iIndex, _float4 vScale);
	/* 박스 회전 (x = right축, y = up축, z = look을 축으로 회전 )*/
	void			Rotate_Box(_uint iIndex, _float4 vAngles);
	
public:
	// CGameObject을(를) 통해 상속됨

	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

protected:
	CModel* m_pModelCom = nullptr;
	CModel* m_pLODModelCom = nullptr;
	wstring m_DebugPath = TEXT("");

protected:
	/* PhysX Collider */
	ePhysXEnum				m_eCurType = ePhysXEnum::eEND;

	vector<CPhysXCollider*>	m_vecPhysXColliders;

protected:
	virtual	HRESULT	SetUp_Model(wstring strMeshPath);
	virtual HRESULT SetUp_World(_float4x4 worldMat);
	virtual HRESULT SetUp_World(_float4 vScale, _float4x4 worldMat);
protected:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;
};
END
