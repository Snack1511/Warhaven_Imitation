#include "..\Public\Animation.h"

#include "GameInstance.h"
#include "Channel.h"
#include "Model.h"
#include "CAnimator.h"

#include "CResource_Animation.h"
#include "HIerarchyNode.h"

CAnimation::CAnimation()
{
}

CAnimation::~CAnimation()
{
	Release();
}

CAnimation* CAnimation::Create(CResource_Animation* pAIAnimation)
{
	CAnimation* pInstance = new CAnimation();

	if (FAILED(pInstance->Initialize(pAIAnimation)))
	{
		Call_MsgBox(L"Failed to Created : CAnimation");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}
HRESULT CAnimation::Set_HierarchyNodes(CModel* pModel)
{
	for (auto& elem : m_Channels)
	{
		if (FAILED(elem->Set_HierarchyNode(pModel)))
			return E_FAIL;
	}

	return S_OK;
}

void CAnimation::OnInterpolate()
{
	m_bInterpolation = true;
	m_fInterpolationTimeAcc = 0.f;
}

void CAnimation::OnSwitchAnim()
{
}

void CAnimation::Reset(_bool bDivide, ANIM_DIVIDE eDivideType)
{
	m_isFinished = false;
	m_fTimeAcc = 0.f;
	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		m_Channels[i]->Reset_KeyFrame(bDivide, m_eAnimDivide);
		//m_Channels[i]->m_pHierarchyNode->Set_PrevKeyFrame(m_Channels[i]->m_pHierarchyNode->Get_CurKeyFrame());

	}
}

_float CAnimation::Calculate_Duration(_uint iKeyFrame)
{
	if (m_Channels[0]->m_iNumKeyframes < iKeyFrame)
		return 0.f;

	_float fResult = 0.f;

	for (_uint i = 0; i < iKeyFrame; ++i)
	{
		fResult += m_Channels[0]->m_pKeyFrames[i].fTime;
	}
	return fResult;
}

HRESULT CAnimation::Initialize(CResource_Animation* pAnimResource)
{
	ANIM_DESC& tAnimDesc = pAnimResource->Get_AnimDesc();

	strcpy_s(m_szName, pAnimResource->Get_Name().c_str());

	m_iNumChannels = tAnimDesc.iNumChannels;

	/* ���� �ִϸ��̼��� ����ϴµ� �ɸ��� �ð�. */
	m_fDuration = tAnimDesc.fDuration;
	m_fTickPerSecond = tAnimDesc.fTickPerSecond;

	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		CChannel* pChannel = CChannel::Create(tAnimDesc.pChannels[i]);
		if (nullptr == pChannel)
			return E_FAIL;

		m_Channels.push_back(pChannel);
	}
	m_fInterpolationTime = 0.1f;

	return S_OK;
}

_bool CAnimation::Update_Matrices(_bool bDivide)
{
	if (m_isFinished)
	{
		Reset(bDivide, m_eAnimDivide);

		if (m_eAnimDivide == ANIM_DIVIDE::eBODYUPPER)
			return false;
	}

	
	if (m_bInterpolation)
	{

		m_fInterpolationTimeAcc += fDT(0);

		if (m_fInterpolationTimeAcc >= m_fInterpolationTime)
		{
			m_bInterpolation = false;
			m_fInterpolationTimeAcc = 0.f;
			return true;
		}

		for (_uint i = 0; i < m_iNumChannels; ++i)
		{
			m_Channels[i]->Interpolate_Matrix(m_fInterpolationTimeAcc, m_fInterpolationTime, bDivide, m_eAnimDivide);
		}

		return true;
	}

	m_fTimeAcc += m_fTickPerSecond * m_fAnimSpeed * fDT(0);

	if (m_fTimeAcc >= m_fDuration)
	{
		m_isFinished = true;
		return true;
	}

	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		
		m_Channels[i]->Update_TransformationMatrices(m_fTimeAcc, bDivide, m_eAnimDivide);
	}

	return true;
}

void CAnimation::Release()
{
	for (auto& pChannel : m_Channels)
		SAFE_DELETE(pChannel);

	m_Channels.clear();
}