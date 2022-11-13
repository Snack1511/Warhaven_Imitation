#include "CCollider_Sphere.h"

#include "GameInstance.h"
#include "DebugDraw.h"
#include "Prototype_Manager.h"
#include "GameObject.h"
#include "Transform.h"

#include "HIerarchyNode.h"

CCollider_Sphere::CCollider_Sphere(_uint iGroupID)
	: CCollider(iGroupID)
{
}

CCollider_Sphere::CCollider_Sphere(const CCollider_Sphere& origin)
	: CCollider(origin)
	, m_ColInfoList(origin.m_ColInfoList)
{
	_uint iIndex = 0;
	for (auto& elem : m_ColInfoList)
	{
		m_pSphere_Original.push_back(new BoundingSphere(_float3(0.f, 0.f, 0.f), elem.fRadius));
		m_pSphere.push_back(new BoundingSphere(*m_pSphere_Original.back()));
	}
	


}

CCollider_Sphere::~CCollider_Sphere()
{
	for (auto& elem : m_pSphere_Original)
		Safe_Delete(elem);

	for (auto& elem : m_pSphere)
		Safe_Delete(elem);

}

CCollider_Sphere* CCollider_Sphere::Create(_uint iGroupID, _float fRadius, const _uint& iColIndex, _float4 vOffsetPos, _float4x4 matTransformation, CHierarchyNode* pRefBone)
{
	CCollider_Sphere* pCollider = new CCollider_Sphere(iGroupID);

	if (!pCollider)
		return nullptr;

	pCollider->Set_ColIndex(iColIndex);

	COL_INFO_SPHERE tInfo;
	tInfo.fRadius = fRadius;
	tInfo.vOffsetPos = vOffsetPos;
	
	pCollider->m_ColInfoList.push_back(tInfo);

	pCollider->m_eColType = CT_SPHERE;
	pCollider->m_matTransformation = matTransformation;
	pCollider->m_pRefBone = pRefBone;
	pCollider->m_pSphere_Original.push_back(new BoundingSphere(_float3(0.f, 0.f, 0.f), fRadius));
	pCollider->m_pSphere.push_back(new BoundingSphere(*(pCollider->m_pSphere_Original.back())));
	if (FAILED(pCollider->Initialize_Prototype()))
	{
		SAFE_DELETE(pCollider);
		Call_MsgBox(L"Failed to Initialize_Prototype : CCollider_Sphere");
		return nullptr;
	}

	return pCollider;
}

void CCollider_Sphere::Add_Collider(_float fRadius, _float4 vOffsetPos)
{
	COL_INFO_SPHERE tInfo;
	tInfo.fRadius = fRadius;
	tInfo.vOffsetPos = vOffsetPos;

	m_ColInfoList.push_back(tInfo);
}

HRESULT CCollider_Sphere::Initialize()
{
	__super::Initialize();

	

	return S_OK;
}

void CCollider_Sphere::Late_Tick()
{
	//owner의 월드매트릭스, 뼈의 매트릭스, transformation 까지 곱해줘야함. ㅋ;
	
	_float4x4 vFinalMatrix;
	vFinalMatrix.Identity();

	_float4 vFinalPos = _float4(0.f, 0.f, 0.f);
	_uint iIndex = 0;

	//참조 뼈가 있을 경우엔 뼈 행렬 * 트랜스폼 * 월드행렬
	if (m_pRefBone)
	{
		_float4x4 BoneMatrix = m_pRefBone->Get_BoneMatrix();

		for (auto& elem : m_ColInfoList)
		{
			elem.vFinalPos = elem.vOffsetPos.MultiplyCoord(BoneMatrix);

			(*((_float4*)&vFinalMatrix.m[WORLD_RIGHT])) *= elem.fRadius;
			(*((_float4*)&vFinalMatrix.m[WORLD_UP])) *= elem.fRadius;
			(*((_float4*)&vFinalMatrix.m[WORLD_LOOK])) *= elem.fRadius;
			(*((_float4*)&vFinalMatrix.m[WORLD_POS])) = elem.vFinalPos;
			m_pSphere_Original[iIndex]->Transform(*m_pSphere[iIndex++], vFinalMatrix.XMLoad());
		}
	}
	else // 아닌경우 그냥 월드포스 _ 오프세솦스
	{
		_float4x4 WorldMatrix = m_pRefBone->Get_BoneMatrix();

		for (auto& elem : m_ColInfoList)
		{
			elem.vFinalPos = elem.vOffsetPos.MultiplyCoord(WorldMatrix);


			(*((_float4*)&vFinalMatrix.m[WORLD_RIGHT])) *= elem.fRadius;
			(*((_float4*)&vFinalMatrix.m[WORLD_UP])) *= elem.fRadius;
			(*((_float4*)&vFinalMatrix.m[WORLD_LOOK])) *= elem.fRadius;
			(*((_float4*)&vFinalMatrix.m[WORLD_POS])) = elem.vFinalPos;
			m_pSphere_Original[iIndex]->Transform(*m_pSphere[iIndex++], vFinalMatrix.XMLoad());
		}

	}

	
	

}

#ifdef _DEBUG
HRESULT CCollider_Sphere::Render()
{
	DEVICE_CONTEXT->GSSetShader(nullptr, nullptr, 0);

	DEVICE_CONTEXT->IASetInputLayout(m_pInputLayout.Get());

	m_pEffect->SetWorld(XMMatrixIdentity());


	m_pEffect->SetView(GAMEINSTANCE->Get_CurViewMatrix().XMLoad());
	m_pEffect->SetProjection(GAMEINSTANCE->Get_CurProjMatrix().XMLoad());
	m_pEffect->Apply(PDEVICE_CONTEXT);

	_vector		vColor = m_bCol == true ? XMVectorSet(1.f, 0.f, 0.f, 1.f) : XMVectorSet(0.f, 1.f, 0.f, 1.f);

	m_pBatch->Begin();

	for (auto& elem : m_pSphere)
	{
		DX::Draw(m_pBatch, *elem, vColor);
	}

	m_pBatch->End();

	return S_OK;
}
#endif