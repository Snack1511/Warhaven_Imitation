#pragma once
#include "Engine_Defines.h"

/* �� �ִϸ��̼Ǹ��� ���� �Ҵ�Ǿ� �����ȴ�. */

BEGIN(Engine)
class CAnimator;
class CModel;

class CChannel final
{
private:
	CChannel();
	virtual ~CChannel();

	friend class CAnimation;
	friend class CAnimator;

public:
	CChannel* Clone() { return new CChannel(*this); }

public:
	static CChannel* Create(CHANNEL_DESC pAIChannel);

public:
	HRESULT	Set_HierarchyNode(CModel* pModel);

public:
	void Reset_KeyFrame() { m_iCurrentKeyFrame = 0; }
	_uint	Get_CurKeyFrame() { return m_iCurrentKeyFrame; }


public:
	HRESULT Initialize(CHANNEL_DESC pAIChannel);
	void Update_TransformationMatrices(_float fCurrentTime);
	void Interpolate_Matrix(_float fCurrentTime, _float fMaxTime);
	void Release();

private:
	char						m_szName[MAX_PATH] = "";
	_uint						m_iNumKeyframes = 0;
	KEYFRAME*					m_pKeyFrames = nullptr;
	class CHierarchyNode*		m_pHierarchyNode = nullptr;
	_uint						m_iCurrentKeyFrame = 0;



};

END