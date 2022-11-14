#pragma once
#include "Engine_Defines.h"

BEGIN(Engine)

class CAnimator;
class CModel;
class CResource_Animation;

class CAnimation final
{
private:
	CAnimation();
	virtual ~CAnimation();

	friend class CAnimator;

public:
	static CAnimation* Create(CResource_Animation* pAIAnimation);

public:
	HRESULT	Set_HierarchyNodes(CModel* pModel);
	void	OnInterpolate();
	void	OnSwitchAnim();
	void	Reset();
	_float	Calculate_Duration(_uint iKeyFrame);
	ANIM_DIVIDE	Get_AnimDivideType() { return m_eAnimDivide; };

public:
	HRESULT Initialize(CResource_Animation* pAIAnimation);
	void	Update_Matrices(_bool bDivide);
	void	Release();

private:
	ANIM_DIVIDE		m_eAnimDivide = ANIM_DIVIDE::eDEFAULT;

private:
	char			m_szName[MAX_PATH] = "";
	_uint			m_iNumChannels = 0; /* ���� �ִϸ��̼��� ����ؾ��� ���� ����. */
	vector<class CChannel*>		m_Channels; /* ���� �ִԿ��� ���Ǵ� ���� ���¸� ������ ��ü�̴�. */

	_bool			m_bInterpolation = false;

	_float			m_fDuration = 0.f;
	_float			m_fTickPerSecond = 0.f;
	_float			m_fAnimSpeed = 1.f;

	_float			m_fTimeAcc = 0.f;
	_float			m_fInterpolationTimeAcc = 0.f;
	_bool			m_isFinished = false;

	_float			m_fInterpolationTime = 0.1f;

};

END