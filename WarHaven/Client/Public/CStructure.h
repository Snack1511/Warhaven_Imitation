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
	//static CStructure* Create(wstring MeshName, wstring strMergeDataPath);

public:
	virtual void	Unit_CollisionEnter(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType, _float4 vHitPos);
	virtual void	Unit_CollisionStay(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType);
	virtual void	Unit_CollisionExit(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType) {};

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
	wstring m_MeshName = TEXT("");
protected:
	/* PhysX Collider */
	ePhysXEnum				m_eCurType = ePhysXEnum::eEND;

	vector<CPhysXCollider*>	m_vecPhysXColliders;

protected:
	virtual	HRESULT	SetUp_Model(wstring strMeshPath);
	virtual HRESULT SetUp_World(_float4x4 worldMat);
	virtual HRESULT SetUp_World(_float4 vScale, _float4x4 worldMat);
	virtual HRESULT SetUp_MergeData(wstring strMeshName, wstring strMergePath);
protected:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;
public:
	_uint Get_ColliderType() { return _uint(m_eCurType); }
	_uint Get_LODType() { return m_iCOlliderSourceLOD; }
	_uint Get_BoxCount();
	vector<_float4>& Get_ColliderPoses() { return m_vecColliderPos; }
	vector<_float4>& Get_ColliderScales() { return m_vecColliderScale; }
	vector<_float4>& Get_ColliderAngles() { return m_vecAngles; }


private:
	_uint m_iCOlliderSourceLOD = 3;
	vector<_float4> m_vecColliderPos;
	vector<_float4> m_vecColliderScale;
	vector<_float4> m_vecAngles;
};
END
