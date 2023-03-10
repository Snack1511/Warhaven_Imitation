#include "..\Public\Channel.h"
#include "HIerarchyNode.h"
#include "CAnimator.h"
#include "Channel.h"
#include "Model.h"

CChannel::CChannel()
{
}

CChannel::~CChannel()
{
	Release();
}

CChannel* CChannel::Create(CHANNEL_DESC pAIChannel)
{
	CChannel* pInstance = new CChannel();

	if (FAILED(pInstance->Initialize(pAIChannel)))
	{
		Call_MsgBox(L"Failed to Initialize : CChannel");
		SAFE_DELETE(pInstance);
	}

	return pInstance;
}

HRESULT CChannel::Set_HierarchyNode(CModel* pModel)
{
	m_pHierarchyNode = pModel->Find_HierarchyNode(m_szName);

	if (nullptr == m_pHierarchyNode)
		return E_FAIL;

	return S_OK;
}

void CChannel::Reset_KeyFrame(_bool bDivide, ANIM_DIVIDE eDivideType)
{
	//한번 쭉 재생되고나서 다시 키프레임 0으로 돌려야함

	//근데 블렌드되고 있는 중이면 내꺼만 0으로 돌려야함
	if (bDivide)
	{
		if (eDivideType == m_pHierarchyNode->Get_BoneType() || m_pHierarchyNode->Get_BoneType() == ANIM_DIVIDE::eDEFAULT)
			m_iCurrentKeyFrame = 0;

	}
	else
		m_iCurrentKeyFrame = 0;

	m_bBlend = false;

}

HRESULT CChannel::Initialize(CHANNEL_DESC tChannelDesc)
{
	strcpy_s(m_szName, tChannelDesc.strName.c_str());
	
	m_iNumKeyframes = tChannelDesc.iNumKeyFrames;
	m_pKeyFrames = tChannelDesc.pKeyFrames;

	return S_OK;
}

void CChannel::Update_TransformationMatrices(_float fCurrentTime, _bool bDivide, ANIM_DIVIDE eAnimDivide)
{
	//CenterNode는 위치는 하체에서 받아와야함

	KEYFRAME& tPrevFrame = m_pHierarchyNode->Get_PrevKeyFrame();
	KEYFRAME& tCurFrame = m_pHierarchyNode->Get_CurKeyFrame();
	KEYFRAME& tBlendFrame = m_pHierarchyNode->Get_BlendKeyFrame();
	if (bDivide)
	{
		if (m_pHierarchyNode->Get_BoneType() != eAnimDivide && m_pHierarchyNode->Get_BoneType() != ANIM_DIVIDE::eDEFAULT)
		{
			//if (m_pHierarchyNode->Is_CenterNode())
			//{
			//	//내가 CenterNode면 위치는 하체꺼 가져오기
			//	_vector			vScale, vRotation, vPosition;


			//	if (fCurrentTime > m_pKeyFrames[m_iNumKeyframes - 1].fTime)
			//	{
			//		vPosition = XMLoadFloat3(&m_pKeyFrames[m_iCurrentKeyFrame].vPosition);
			//		vPosition = XMVectorSetW(vPosition, 1.f);
			//	}

			//	else
			//	{
			//		while (fCurrentTime > m_pKeyFrames[m_iCurrentKeyFrame + 1].fTime)
			//		{
			//			++m_iCurrentKeyFrame;
			//			if (m_bBlend)
			//				m_bBlend = false;
			//		}


			//		_float fRatio = (fCurrentTime - m_pKeyFrames[m_iCurrentKeyFrame].fTime)
			//			/ (m_pKeyFrames[m_iCurrentKeyFrame + 1].fTime - m_pKeyFrames[m_iCurrentKeyFrame].fTime);

			//		_vector			vSourPosition;
			//		_vector			vDestPosition;
			//		vSourPosition = XMLoadFloat3(&m_pKeyFrames[m_iCurrentKeyFrame].vPosition);



			//		vDestPosition = XMLoadFloat3(&m_pKeyFrames[m_iCurrentKeyFrame + 1].vPosition);
			//		vPosition = XMVectorLerp(vSourPosition, vDestPosition, fRatio);
			//		vPosition = XMVectorSetW(vPosition, 1.f);
			//	}

			//	XMStoreFloat3(&tCurFrame.vPosition, vPosition);


			//	if (nullptr != m_pHierarchyNode)
			//	{
			//		m_pHierarchyNode->Set_Position(vPosition);
			//	}
			//}
			//else
			{
				//내뼈가 아니면 시간만 조용히 갱신해놓고 튀기
				while (fCurrentTime > m_pKeyFrames[m_iCurrentKeyFrame + 1].fTime)
				{
					++m_iCurrentKeyFrame;
				}
			}
			

			





			return;
		}

		
	}
	else
	{
		tBlendFrame = tCurFrame;
	}

	//만약 블렌드 중이면 중간에 끼어들어야 함

	


	_vector			vScale, vRotation, vPosition;
	

	if (fCurrentTime > m_pKeyFrames[m_iNumKeyframes-1].fTime)
	{
		vScale = XMLoadFloat3(&m_pKeyFrames[m_iCurrentKeyFrame].vScale);
		vRotation = XMLoadFloat4(&m_pKeyFrames[m_iCurrentKeyFrame].vRotation);
		vPosition = XMLoadFloat3(&m_pKeyFrames[m_iCurrentKeyFrame].vPosition);
		vPosition = XMVectorSetW(vPosition, 1.f);

		/*if (bDivide)
		{
			if (m_pHierarchyNode->Get_BoneType() == ANIM_DIVIDE::eBODYLOWER)
			{

				cout << fCurrentTime << " 목표 시간 : " << m_pKeyFrames[m_iNumKeyframes - 1].fTime << endl;
			}
		}*/
	}

	else
	{
		while (fCurrentTime > m_pKeyFrames[m_iCurrentKeyFrame + 1].fTime)
		{
			++m_iCurrentKeyFrame;
			if (m_bBlend)
				m_bBlend = false;
		}


		_float fRatio = (fCurrentTime - m_pKeyFrames[m_iCurrentKeyFrame].fTime)
			/ (m_pKeyFrames[m_iCurrentKeyFrame + 1].fTime - m_pKeyFrames[m_iCurrentKeyFrame].fTime);

		_vector			vSourScale, vSourRotation, vSourPosition;
		_vector			vDestScale, vDestRotation, vDestPosition;

		if (m_bBlend)
		{
			vSourScale = XMLoadFloat3(&tBlendFrame.vScale);
			vSourRotation = XMLoadFloat4(&tBlendFrame.vRotation);
			vSourPosition = XMLoadFloat3(&tBlendFrame.vPosition);
		}
		else
		{
			vSourScale = XMLoadFloat3(&m_pKeyFrames[m_iCurrentKeyFrame].vScale);
			vSourRotation = XMLoadFloat4(&m_pKeyFrames[m_iCurrentKeyFrame].vRotation);
			vSourPosition = XMLoadFloat3(&m_pKeyFrames[m_iCurrentKeyFrame].vPosition);
		}

		

		vDestScale = XMLoadFloat3(&m_pKeyFrames[m_iCurrentKeyFrame + 1].vScale);
		vDestRotation = XMLoadFloat4(&m_pKeyFrames[m_iCurrentKeyFrame + 1].vRotation);
		vDestPosition = XMLoadFloat3(&m_pKeyFrames[m_iCurrentKeyFrame + 1].vPosition);

		vScale = XMVectorLerp(vSourScale, vDestScale, fRatio);
		vRotation = XMQuaternionSlerp(vSourRotation, vDestRotation, fRatio);
		vPosition = XMVectorLerp(vSourPosition, vDestPosition, fRatio);
		vPosition = XMVectorSetW(vPosition, 1.f);
	}

	XMStoreFloat3(&tCurFrame.vScale, vScale);
	XMStoreFloat4(&tCurFrame.vRotation, vRotation);

	//상하체분리중에 센터노드는 포지션 갱신하면 안댐
	//if (!(bDivide && m_pHierarchyNode->Is_CenterNode()))
		XMStoreFloat3(&tCurFrame.vPosition, vPosition);



	_matrix		TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vPosition);
	_float4x4 TransMat;
	XMStoreFloat4x4(&TransMat, TransformationMatrix);

	if (nullptr != m_pHierarchyNode)
	{
		/*if (bDivide && m_pHierarchyNode->Is_CenterNode())
			m_pHierarchyNode->Set_Translation_NoPos(TransMat);
		else*/
			m_pHierarchyNode->Set_TransformationMatrix(TransMat);
	}
	else
	{
		int i = 0;
	}
}

void CChannel::Interpolate_Matrix(_float fCurrentTime, _float fMaxTime, _bool bDivide, ANIM_DIVIDE eAnimDivide)
{
	//Divide가 true면 나랑 같은 타입의 뼈만 쓰까주야함
	if (bDivide)
	{
		if (m_pHierarchyNode->Get_BoneType() != eAnimDivide && m_pHierarchyNode->Get_BoneType() != ANIM_DIVIDE::eDEFAULT)
			return;
	}


	//전 키프레임
	KEYFRAME& tPrevFrame = m_pHierarchyNode->Get_PrevKeyFrame();

	//이게 매프레임 마다 저장용 
	KEYFRAME& tCurFrame = m_pHierarchyNode->Get_CurKeyFrame();

	//현재 매트릭스
	_vector			vScale, vRotation, vPosition;

	_float fRatio = fCurrentTime / fMaxTime;

	_vector			vSourScale, vSourRotation, vSourPosition;
	_vector			vDestScale, vDestRotation, vDestPosition;

	vSourScale = XMLoadFloat3(&tPrevFrame.vScale);
	vSourRotation = XMLoadFloat4(&tPrevFrame.vRotation);
	vSourPosition = XMLoadFloat3(&tPrevFrame.vPosition);

	vDestScale = XMLoadFloat3(&m_pKeyFrames[m_iCurrentKeyFrame].vScale);
	vDestRotation = XMLoadFloat4(&m_pKeyFrames[m_iCurrentKeyFrame].vRotation);
	vDestPosition = XMLoadFloat3(&m_pKeyFrames[m_iCurrentKeyFrame].vPosition);

	vScale = XMVectorLerp(vSourScale, vDestScale, fRatio);
	vRotation = XMQuaternionSlerp(vSourRotation, vDestRotation, fRatio);
	vPosition = XMVectorLerp(vSourPosition, vDestPosition, fRatio);
	vPosition = XMVectorSetW(vPosition, 1.f);

	//이전 애니메이션의 그 프레임 상태를 받아와서
	// 그걸로 해야해.

	XMStoreFloat3(&tCurFrame.vScale, vScale);
	XMStoreFloat4(&tCurFrame.vRotation, vRotation);
	XMStoreFloat3(&tCurFrame.vPosition, vPosition);

	_matrix		TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vPosition);
	_float4x4 TransMat;
	XMStoreFloat4x4(&TransMat, TransformationMatrix);

	if (nullptr != m_pHierarchyNode)
		m_pHierarchyNode->Set_TransformationMatrix(TransMat);
	else
	{
		int i = 0;
	}

}

void CChannel::Release()
{
}

