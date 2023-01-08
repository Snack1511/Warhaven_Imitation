#include "stdafx.h"
#include "CLancerNeedle.h"

#include "UsefulHeaders.h"
#include "CTeamConnector.h"
#include "CCollider_Sphere.h"
#include "HIerarchyNode.h"
#include "CUnit.h"
#include "Easing_Utillity.h"
#include "CUtility_Transform.h"

CLancerNeedle::CLancerNeedle()
{
}

CLancerNeedle::~CLancerNeedle()
{
}

CLancerNeedle* CLancerNeedle::Create(wstring wstrModelFilePath, CHierarchyNode* pOwnerHierarcyNode, CUnit* pOwnerUnit, _uint iNeedleIndex)
{
	CLancerNeedle* pInstance = new CLancerNeedle;


	pInstance->m_iNeedleIndex = iNeedleIndex;

	if (FAILED(pInstance->CLancerNeedle::SetUp_Model(wstrModelFilePath, pOwnerHierarcyNode, pOwnerUnit)))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to SetUp_Model : CLancerNeedle");
		return nullptr;
	}

	if (FAILED(pInstance->CLancerNeedle::Initialize_Prototype()))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to Initialize_Prototype : CLancerNeedle");
		return nullptr;
	}


	return pInstance;
}

void	CLancerNeedle::Needle_CollisionEnter(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType, _float4 vHitPos)
{

}
void	CLancerNeedle::Needle_CollisionStay(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType)
{

}
void	CLancerNeedle::Needle_CollisionExit(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType)
{

}

void CLancerNeedle::On_ChangePhase(LANCERNEEDLE eNeedleState)
{
	// ����Ʈ ��������

	m_eNeedleState = eNeedleState;

	switch (m_eNeedleState)
	{
	case Client::CLancerNeedle::LANCERNEEDLE_START:

		break;

	case Client::CLancerNeedle::LANCERNEEDLE_LOOP:

		break;

	case Client::CLancerNeedle::LANCERNEEDLE_ATTACKBEGIN:


		break;

	case Client::CLancerNeedle::LANCERNEEDLE_ATTACK:

		break;

	case Client::CLancerNeedle::LANCERNEEDLE_STOP:
		
		break;

	default:
		break;
	}

}
void CLancerNeedle::Enable_Needle(_bool bEnable)
{
	if (bEnable)
		ENABLE_GAMEOBJECT(this);

	else
	{
		DISABLE_GAMEOBJECT(this);
		return;
	}
	_float fOffsetZ = 20.f;


	switch (m_iNeedleIndex)
	{
	case 0:
		m_vStartPos = _float4(-41.f, -134.f, -60.2f);
		m_vTargetPos = m_vStartPos;
		m_vTargetPos.z -= fOffsetZ;

		break;

	case 1:
		m_vStartPos = _float4(-41.f, -134.f, -20.5f);
		m_vTargetPos = m_vStartPos;
		m_vTargetPos.z -= fOffsetZ;
		break;


	case 2:
		m_vStartPos = _float4(-41.f, -134.f, 20.f);
		m_vTargetPos = m_vStartPos;
		m_vTargetPos.z += fOffsetZ;

		break;

	case 3:
		m_vStartPos = _float4(-41.f, -134.f, 60.f);
		m_vTargetPos = m_vStartPos;
		m_vTargetPos.z += fOffsetZ;

		break;


	default:
		break;
	}


	

}

HRESULT CLancerNeedle::SetUp_Model(wstring wstrModelFilePath, CHierarchyNode* pOwnerHierarcyNode, CUnit* pOwnerUnit)
{
	
	CModel* pModel = CModel::Create(CP_BEFORE_RENDERER, TYPE_NONANIM, wstrModelFilePath, XMMatrixScaling(0.01f, 0.01f, 0.01f) * 
		XMMatrixRotationZ(XMConvertToRadians(270.f)) * XMMatrixRotationX(XMConvertToRadians(90.0f))
	);


	if (!pModel)
		return E_FAIL;

	pModel->Initialize();
	Add_Component(pModel);
	pModel->Set_ShaderFlag(SH_LIGHT_BLOOM);
	pModel->Set_ShaderPassToAll(VTXMODEL_PASS_NORMALMAPPING);


	CShader* pShader = CShader::Create(CP_BEFORE_RENDERER, SHADER_VTXMODEL,
		VTXMODEL_DECLARATION::Element, VTXMODEL_DECLARATION::iNumElements);
	pShader->Initialize();
	Add_Component(pShader);

	CModel_Renderer* pRenderer = CModel_Renderer::Create(CP_RENDERER, RENDER_NONALPHA, VTXMODEL_PASS_NORMALMAPPING
		, _float4(0.f, 0.f, 0.f, 1.f));

	pRenderer->Initialize();
	Add_Component<CRenderer>(pRenderer);

	m_pOwnerBone = pOwnerHierarcyNode;
	m_pOwnerUnit = pOwnerUnit;

	return S_OK;
}

HRESULT CLancerNeedle::Initialize_Prototype()
{

	

	return S_OK;
}

HRESULT CLancerNeedle::Initialize()
{
	return S_OK;
}

HRESULT CLancerNeedle::Start()
{
	__super::Start();

	CallBack_CollisionEnter += bind(&CLancerNeedle::Needle_CollisionEnter, this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4);
	CallBack_CollisionStay += bind(&CLancerNeedle::Needle_CollisionStay, this, placeholders::_1, placeholders::_2, placeholders::_3);
	CallBack_CollisionExit += bind(&CLancerNeedle::Needle_CollisionExit, this, placeholders::_1, placeholders::_2, placeholders::_3);

	m_eNeedleState = LANCERNEEDLE_START;
	m_pTransform->Set_Scale(_float4(FLT_MIN, FLT_MIN, FLT_MIN));

	return S_OK;
}

void CLancerNeedle::My_Tick()
{
	__super::My_Tick();
	
	if(KEY(R, HOLD))
	{
		if (KEY(X, HOLD))
			m_vStartPos.x += 1.f;

		if (KEY(Y, HOLD))
			m_vStartPos.y += 1.f;

		if (KEY(Z, HOLD))
			m_vStartPos.z += 1.f;
	}
	else
	{
		if (KEY(X, HOLD))
			m_vStartPos.x -= 1.f;

		if (KEY(Y, HOLD))
			m_vStartPos.y -= 1.f;

		if (KEY(Z, HOLD))
			m_vStartPos.z -= 1.f;
	}
	
}

void CLancerNeedle::My_LateTick()
{
	__super::My_LateTick();

	switch (m_eNeedleState)
	{
	case Client::CLancerNeedle::LANCERNEEDLE_START:
		

		Chase_OwnerBoneMatrix();

		m_vScale.x = 1.f;
		m_vScale.y += fDT(0) * 2.f;
		m_vScale.z = 1.f;

		if (m_vScale.y >= 1.f)
		{
			XMStoreFloat4x4(&m_OwnerBoneOffsetMatrix, XMMatrixIdentity());
			
			
			m_OwnerBoneOffsetMatrix.m[3][0] = frandom(-0.5f * (_float)m_iNeedleIndex, -0.5f * (_float)m_iNeedleIndex);
			m_OwnerBoneOffsetMatrix.m[3][1] = frandom(-0.2f, -0.2f);
			m_OwnerBoneOffsetMatrix.m[3][2] = frandom(-0.5f * (_float)m_iNeedleIndex, -0.5f * (_float)m_iNeedleIndex);

			if (KEY(J, TAP))
				m_eNeedleState = LANCERNEEDLE_ATTACKBEGIN;

			m_pTransform->Set_Scale(_float4(1.f, 1.f, 1.f));
		}
		else
		{
			m_pTransform->Set_Scale(m_vScale);
		}


		break;

	
	case Client::CLancerNeedle::LANCERNEEDLE_ATTACKBEGIN:
	{
		if (!m_pOwnerBone)
			return;

		m_fCurAcc += fDT(0);

		if (m_fCurAcc > m_fTotalTime)
		{
			m_eNeedleState = LANCERNEEDLE_ATTACK;
			m_fCurAcc = 0.f;
			break;
		}

		/* �θ� ���(�����)�� ���ϱ� ���� ũ, ��, �� �ϴ� ���� */
		_float4x4 matLocal;
		matLocal.Identity();

	// 1. ���� 

		_float vNewAngle = CEasing_Utillity::sinfOut(0.f, 270.f, m_fCurAcc, m_fTotalTime);
		//���� ȸ����� ���� ���� (��������)
		CUtility_Transform::Turn_ByAngle(matLocal, _float4(0.f, 0.f, 1.f, 0.f), vNewAngle);

	// 2. ������
		_float4 vNewPos = CEasing_Utillity::QuadIn(m_vStartPos, m_vTargetPos, m_fCurAcc, m_fTotalTime);
		memcpy(matLocal.m[3], &vNewPos, sizeof(_float4));

		// 
	// 3. ����
	// 
	// 4. �θ�
			CHierarchyNode* pNode = GET_COMPONENT_FROM(m_pOwnerUnit, CModel)->Find_HierarchyNode("0B_Head");//("0B_Head");

			if (pNode == nullptr)
				return;

			/* �θ� ���*/
			_float4x4		matBone = pNode->Get_BoneMatrix();

			/* �������(ũ����) * �θ���� = ������� */
			_float4x4		matFinal = matLocal * matBone;
			//_float4x4		mat
			m_pTransform->Get_Transform().matMyWorld = matFinal;
			m_pTransform->Make_WorldMatrix();
		}

		break;
	
	case Client::CLancerNeedle::LANCERNEEDLE_ATTACK:
	{
		if (KEY(J, TAP))
			m_eNeedleState = LANCERNEEDLE_ATTACKBEGIN;

		CHierarchyNode* pNode = GET_COMPONENT_FROM(m_pOwnerUnit, CModel)->Find_HierarchyNode("0B_Head");//("0B_Head");

		if (pNode == nullptr)
			return;

		_float4x4		matBone = pNode->Get_BoneMatrix();

		_float4x4 matWorldInv = pNode->Get_BoneMatrix().Inverse();
		_float4x4 matOffset = pNode->Get_BoneMatrix() * matWorldInv;

		matBone = matOffset * matBone;

		_float4 vPos = m_vTargetPos.MultiplyCoord(matBone);



		m_pTransform->Set_World(WORLD_POS, vPos);
		CUtility_Transform::Turn_ByAngle(matBone, _float4(0.f, 1.f, 0.f, 0.f), 0.f);
		m_pTransform->Make_WorldMatrix();

	}
		break;
	
	case Client::CLancerNeedle::LANCERNEEDLE_STOP:

		// ������ Disable
		m_fCurAcc += fDT(0);

		if (m_fCurAcc < 0.2f)
			DISABLE_GAMEOBJECT(this);

		break;
	case Client::CLancerNeedle::LANCERNEEDLE_END:
		break;
	default:
		break;
	}


}

void CLancerNeedle::OnEnable()
{
	__super::OnEnable();
	m_eNeedleState = LANCERNEEDLE_START;
	m_pTransform->Set_Scale(_float4(FLT_MIN, FLT_MIN, FLT_MIN));
}

void CLancerNeedle::OnDisable()
{
	__super::OnDisable();

}

void CLancerNeedle::Chase_OwnerBoneMatrix()
{
	if (m_pOwnerBone)
	{
		CHierarchyNode* pNode = GET_COMPONENT_FROM(m_pOwnerUnit, CModel)->Find_HierarchyNode("0B_Head");//("0B_Head");

		if (pNode == nullptr)
			return;

		_float4x4		matBone = pNode->Get_BoneMatrix();

		_float4x4 matWorldInv = pNode->Get_BoneMatrix().Inverse();
		_float4x4 matOffset = pNode->Get_BoneMatrix() * matWorldInv;

		matBone = matOffset * matBone;

		_float4 vPos = m_vStartPos;

		vPos = vPos.MultiplyCoord(matBone);
		
		matBone.m[3][0] = vPos.x;
		matBone.m[3][1] = vPos.y;
		matBone.m[3][2] = vPos.z;

		m_pTransform->Get_Transform().matMyWorld = matBone;

		m_pTransform->Set_World(WORLD_POS, vPos);

		m_pTransform->Make_WorldMatrix();

	}
}
