#pragma once
#include "Engine_Defines.h"

BEGIN(Engine)

class CModel;
class CResource_Bone;
class CGameObject;

class ENGINE_DLL CHierarchyNode final
{
private:
	CHierarchyNode();
	virtual ~CHierarchyNode();

	friend class CModel;

public:
	CHierarchyNode* Clone() { return new CHierarchyNode(*this); }


public:
	static CHierarchyNode* Create(CResource_Bone* pResource, CHierarchyNode* pParent, _uint iDepth, ANIM_DIVIDE eBoneType);

public:
	_bool	Is_CenterNode() { return m_bCenterNode; }
	void	Set_Position(_float4 vPos);
	void	Set_Translation_NoPos(_float4x4 TransformationMatrix);

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

	_float4x4	Get_BoneMatrix();

	void Set_OffsetMatrix(_float4x4 OffsetMatrix) {
		m_OffsetMatrix = OffsetMatrix;
	}

	void Set_PrevMatrix(_float4x4 matPrev) { m_PrevMatrix = matPrev; }

	void Set_TransformationMatrix(_float4x4 TransformationMatrix) {
		m_TransformationMatrix = TransformationMatrix;
	}

	void Set_CurKeyFrame(KEYFRAME& tKeyFrame) { m_tCurKeyFrame = tKeyFrame; }
	void Set_PrevKeyFrame(KEYFRAME& tKeyFrame) { m_tPrevKeyFrame = tKeyFrame; }
	KEYFRAME& Get_CurKeyFrame() { return m_tCurKeyFrame; }
	KEYFRAME& Get_PrevKeyFrame() { return m_tPrevKeyFrame; }
	KEYFRAME& Get_BlendKeyFrame() { return m_tBlendKeyFrame; }

	HRESULT	Add_NewChild(CHierarchyNode* pNode);

	void	ReFind_Parent(class CModel* pModel);

public:
	void	Get_AllNodes(vector<CHierarchyNode*>& vecNodes);
	_bool	Is_Locked() { return m_bLocked; }
	ANIM_DIVIDE Get_BoneType() { return m_eBoneType; }

public:
	HRESULT Initialize(CResource_Bone* pResource, CHierarchyNode* pParent, _uint iDepth, ANIM_DIVIDE eBoneType);
	void Update_CombinedTransformationMatrix();

private:
	//애니메이션 분리용
	_bool		m_bLocked = false;
	ANIM_DIVIDE	m_eBoneType = ANIM_DIVIDE::eDEFAULT;

private:
	CGameObject* m_pOwner = nullptr;

	_bool			m_bMoveNode = false;
	_bool			m_bCenterNode = false;

	_uint			m_iDepth = 0;
	char			m_szName[MAX_PATH] = "";
	
	_float4x4		m_PrevMatrix;
	_float4x4		m_OffsetMatrix;
	_float4x4		m_TransformationMatrix;
	_float4x4		m_CombinedTransformationMatrix;


	CHierarchyNode*	m_pParent = nullptr;
	vector<CHierarchyNode*>	m_pChildrenNodes;

	KEYFRAME		m_tPrevKeyFrame;
	KEYFRAME		m_tCurKeyFrame;
	KEYFRAME		m_tBlendKeyFrame;

};

END