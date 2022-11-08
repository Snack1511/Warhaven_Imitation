#pragma once
#include "Engine_Defines.h"

BEGIN(Engine)

class CModel;
class CResource_Bone;

class ENGINE_DLL CHierarchyNode final
{

public:
	enum ANIM_BONE_TYPE { ANIM_BONE_TYPE_DEFAULT, ANIM_BONE_TYPE_BODYUPPER, ANIM_BONE_TYPE_BODYLOWER, ANIM_BONE_TYPE_END };


private:
	CHierarchyNode();
	virtual ~CHierarchyNode();

	friend class CModel;

public:
	CHierarchyNode* Clone() { return new CHierarchyNode(*this); }


public:
	static CHierarchyNode* Create(CResource_Bone* pResource, CHierarchyNode* pParent, _uint iDepth, string strBodyUpperRootBone, string strBodyLowerRootBone, _uint iAnimBoneType = 0);

public:
	void	Set_Anim_Bone_Type(_uint iAnimBoneType) {
		if (ANIM_BONE_TYPE_END <= iAnimBoneType)
		{
			m_eAnim_Bone_Type = ANIM_BONE_TYPE_DEFAULT;
			return;
		}

		m_eAnim_Bone_Type = (ANIM_BONE_TYPE)iAnimBoneType;
	} 

	_uint	Get_Anim_Bone_Type() {
		return (_uint)m_eAnim_Bone_Type;
	}

	void	Set_Parent(CHierarchyNode* pNode) { m_pParent = pNode; }

	_uint Get_Depth() const {
		return m_iDepth;
	}

	const char* Get_Name() const {
		return m_szName;
	}

	_matrix Get_OffsetMatrix() const {
		return XMLoadFloat4x4(&m_OffsetMatrix);
	}

	_float4x4 Get_TransformationMatrix() { return m_TransformationMatrix; }

	_matrix Get_CombinedMatrix() const {
		return XMLoadFloat4x4(&m_CombinedTransformationMatrix);
	}

	void Set_OffsetMatrix(_float4x4 OffsetMatrix) {
		m_OffsetMatrix = OffsetMatrix;
	}

	void Set_TransformationMatrix(_float4x4 TransformationMatrix) {
		m_TransformationMatrix = TransformationMatrix;
	}

	void Set_CurKeyFrame(KEYFRAME& tKeyFrame) { m_tCurKeyFrame = tKeyFrame; }
	void Set_PrevKeyFrame(KEYFRAME& tKeyFrame) { m_tPrevKeyFrame = tKeyFrame; }
	KEYFRAME& Get_CurKeyFrame() { return m_tCurKeyFrame; }
	KEYFRAME& Get_PrevKeyFrame() { return m_tPrevKeyFrame; }

	HRESULT	Add_NewChild(CHierarchyNode* pNode);

	void	ReFind_Parent(class CModel* pModel);

public:
	void	Get_AllNodes(vector<CHierarchyNode*>& vecNodes);

public:
	HRESULT Initialize(CResource_Bone* pResource, CHierarchyNode* pParent, _uint iDepth, string strBodyUpperRootBone, string strBodyLowerRootBone, _uint iAnimBoneType);
	void Update_CombinedTransformationMatrix();

	void	Release();

private:
	ANIM_BONE_TYPE m_eAnim_Bone_Type = ANIM_BONE_TYPE_DEFAULT;


	_uint			m_iDepth = 0;
	char			m_szName[MAX_PATH] = "";

	_float4x4		m_OffsetMatrix;
	_float4x4		m_TransformationMatrix;
	_float4x4		m_CombinedTransformationMatrix;

	CHierarchyNode*	m_pParent = nullptr;
	vector<CHierarchyNode*>	m_pChildrenNodes;

	KEYFRAME		m_tPrevKeyFrame;
	KEYFRAME		m_tCurKeyFrame;

};

END