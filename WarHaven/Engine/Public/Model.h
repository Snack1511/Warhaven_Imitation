#pragma once
#include "Component.h"


BEGIN(Engine)

class CShader;
class CHierarchyNode;

class ENGINE_DLL CModel : public CComponent
{
	DECLARE_PROTOTYPE(CModel);

protected:
	CModel(_uint iGroupIdx);
	CModel(const CModel& rhs);
	virtual ~CModel();

public:
	static CModel* Create(_uint iGroupIdx, MODEL_TYPE eType, wstring wstrModelFilePath, _float4x4 TransformMatrix);
	static CModel* Create(_uint iGroupIdx, MODEL_TYPE eType, wstring wstrModelFilePath, wstring wstrInstanceFilePath, _float4x4 TransformMatrix);
	static CModel* Create(_uint iGroupIdx, MODEL_TYPE eType, wstring wstrModelFilePath, _uint iNumInstance, _float4x4 TransformMatrix);
	static CModel* Create(_uint iGroupIdx, MODEL_TYPE eType, wstring wstrModelFilePath, _uint iNumInstance, VTXINSTANCE* pInstanceData, _float4x4 TransformMatrix);
public:
	wstring	Get_ModelFilePath() { return m_wstrModelFilePath; }
	wstring	Get_TextureFilePath(_uint iIndex);
	wstring	Get_TextureFilePathFromParts(_uint iMeshPartType);
	class CTexture* Get_TextureFromParts(_uint iMeshPartType, _uint iMeshPartType2);
	_float4x4 Get_BoneMatrix(const char* pBonename);
	_float4x4 Get_TransformationMatrix() { return m_TransformMatrix; }

	_float4 Get_Color(_uint iMeshPartType);
	void	Set_NewModel(wstring wstrModelFilePath);
	void	Set_NewTransformMatrix(_float4x4 TransformMatrix);
	void	Change_Texture(_uint iModelPart, _uint iTextureIndex, wstring wstrFilePath);
	void	Set_ShaderPass(_uint iModelPart, _uint iShaderPass);
	void	Set_ShaderPassToAll(_uint iShaderPass);
	virtual void Set_Owner(CGameObject* pOwner) override;

	MODEL_TYPE	Get_ModelType() { return m_eMODEL_TYPE; }

public:
	_uint Get_NumMeshContainers() const {
		return m_iNumMeshContainers;
	}

	vector<pair<_uint, class CMeshContainer*>>& Get_MeshContainers() { return m_MeshContainers; }
	vector<pair<_uint, CHierarchyNode*>>& Get_Hierarchynodes() { return m_vecHierarchyNodes; }
	vector<pair<_uint, MODEL_MATERIAL>>& Get_Materials() { return m_Materials; }

public:
	void	No_Culling() { m_bCulling = false; }
	void	Set_ShaderFlag(_float4 vFlag);
	void	Set_ShaderFlag(_uint iMeshPartType, _float4 vFlag);
	void	Set_ShaderColor(_uint iMeshPartType, _float4 vColor);
	void	Set_OutlinePower(_float fOutline);
	void	Set_OutlinePower(_uint iMeshPartType, _float fOutline);

	_bool	Is_Instancing() { return m_bInstancing; }

	void	Switch_MeshContainerBone(_uint iMeshPartType, string strBoneName);
	HRESULT	Add_Model(wstring wstrModelFilePath, _uint iMeshPartType);
	HRESULT	Add_Model(wstring wstrModelFilePath, _uint iMeshPartType, string strBoneName);
	HRESULT Delete_ModelParts(_uint iMeshPartType);
	HRESULT Enable_ModelParts(_uint iMeshPartType, _bool bEnable = true);

public:
	HRESULT Initialize_Prototype() override;
	HRESULT Initialize() override;
	void Tick() override;
	void Late_Tick() override;
	virtual void	Final_Tick() override;


	HRESULT Render(_uint iMeshContainerIndex, class CShader* pShader, _uint iPassIndex = 0, const char* pConstantBoneName = nullptr);
	HRESULT Bake_Shadow(_uint iMeshContainerIndex, class CShader* pShader, _uint iPassIndex = 0);
	void	Release();
	void	Start() override;

public:
	CHierarchyNode* Find_HierarchyNode(const char* pBoneName);
	void			Sort_HierarchyNode();

	_uint Find_MaterialIndex(const char* pMatName);

public:
	HRESULT Bind_SRV(class CShader* pShader, const char* pContantName, _uint iMeshContainerIndex, aiTextureType eType);

protected:
	wstring						m_wstrModelFilePath;
	MODEL_TYPE					m_eMODEL_TYPE = TYPE_END;
	_float4x4					m_TransformMatrix;
	_bool						m_bCloned = false;
	_bool						m_bCulling = false;
	_bool						m_bInstancing = false;

protected:
	_bool						m_bLOD = false;

	enum class eLOD_LEVEL {eDefault, eLOD1, eLOD2, eLOD3, eLOD_END};

	eLOD_LEVEL					m_eLOD_Level = eLOD_LEVEL::eLOD_END;

	_float						m_fLODDistance = 25.f;
	_float4						m_vLODCenterPos = ZERO_VECTOR;
	_float						m_fLODMaxRange = 0.f;

protected:
	vector<pair<_uint, class CMeshContainer*>>			m_MeshContainers;
	_uint			m_iNumMeshContainers = 0;

protected:
	vector<pair<_uint, MODEL_MATERIAL>>					m_Materials;
	_uint			m_iNumMaterials = 0;

protected:
	vector<pair<_uint, CHierarchyNode*>>		m_vecHierarchyNodes;


protected:
	_uint		m_iNumInstance = 0;
	VTXINSTANCE* m_pInstancingMatrices = nullptr;

	_uint m_iLODNumInstance[(_uint)eLOD_LEVEL::eLOD_END] = {};
	_float4x4* m_pIntancingMatricesLOD[(_uint)eLOD_LEVEL::eLOD_END] = {};


protected:
	HRESULT	SetUp_Model(MODEL_TYPE eType, wstring wstrModelFilePath, _float4x4 TransformMatrix, _uint iMeshPartType);
	HRESULT	SetUp_InstancingModel(MODEL_TYPE eType, wstring wstrModelFilePath, wstring wstrInstanceFilePath, _float4x4 TransformMatrix, _uint iMeshPartType);
	HRESULT	SetUp_InstancingModel(MODEL_TYPE eType, wstring wstrModelFilePath, _uint iNumInstance, _float4x4 TransformMatrix, _uint iMeshPartType);
	HRESULT	SetUp_InstancingModel(MODEL_TYPE eType, wstring wstrModelFilePath, _uint iNumInstance, VTXINSTANCE* pInstanceData, _float4x4 TransformMatrix, _uint iMeshPartType);
	HRESULT	SetUp_MeshContainersPtr();
	HRESULT	SetUp_Model_LOD();
	HRESULT	SetUp_InstancingModel_LOD();
	HRESULT	Load_LOD(eLOD_LEVEL eLevel);
	HRESULT	Load_InstancingLOD(eLOD_LEVEL eLevel);

	HRESULT Create_ModelData(class CResource* pResource, _uint iResType, _uint iMeshPartType);
	HRESULT Create_Material(class CResource_Material* pResource, _uint iMeshPartType);
	HRESULT Create_MeshContainer(class CResource_Mesh* pResource, _float4x4 TransformMatrix, _uint iMeshPartType);
	HRESULT Create_InstancingMesh(class CResource_Mesh* pResource, wstring wstrInstanceFilePath, _float4x4 TransformMatrix, _uint iMeshPartType);
	HRESULT Create_InstancingMesh(class CResource_Mesh* pResource, _uint iNumInstance, _float4x4 TransformMatrix, _uint iMeshPartType);
	HRESULT Create_InstancingMesh(class CResource_Mesh* pResource, _uint iNumInstance, VTXINSTANCE* pInstanceData, _float4x4 TransformMatrix, _uint iMeshPartType);
	HRESULT Create_HierarchyNode(class CResource_Bone* pResource, CHierarchyNode* pParent, _uint iDepth, _uint iMeshPartType);

private:
		void	Bake_LODFrustumInfo();



};

END