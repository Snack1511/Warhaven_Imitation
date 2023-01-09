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
	m_pOwnerUnit->CallBack_CollisionEnter(pOtherObj, eOtherColType, eMyColType, vHitPos);
	pOtherObj->CallBack_CollisionEnter(m_pOwnerUnit, eMyColType, eOtherColType, vHitPos);

	CUnit* pOtherUnit = dynamic_cast<CUnit*>(pOtherObj);

	if (m_pOwnerUnit == pOtherUnit && !pOtherUnit)
		return;

	if (pOtherUnit->Get_Status().fHP >= 0.f)
	{
		m_pStinedUnit = pOtherUnit;
	}



	/*m_pStingBone = GET_COMPONENT_FROM(m_tHitInfo.pOtherUnit, CModel)->Find_HierarchyNode(m_tHitInfo.strStingBoneName.c_str());
	m_eAnimType = ANIM_HIT;
	m_iAnimIndex = m_iHitStingIndex[HIT_STATE_N];
	*/	
	

	
}
void	CLancerNeedle::Needle_CollisionStay(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType)
{
	m_pOwnerUnit->CallBack_CollisionStay(pOtherObj, eOtherColType, eMyColType);
	pOtherObj->CallBack_CollisionStay(m_pOwnerUnit, eMyColType, eOtherColType);
}
void	CLancerNeedle::Needle_CollisionExit(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType)
{
	m_pOwnerUnit->CallBack_CollisionExit(pOtherObj, eOtherColType, eMyColType);
	pOtherObj->CallBack_CollisionExit(m_pOwnerUnit, eMyColType, eOtherColType);
}

void CLancerNeedle::On_ChangePhase(LANCERNEEDLE eNeedleState)
{
	// 이펙트 생성시점

	m_eNeedleState = eNeedleState;
	m_fCurAcc = 0.f;
	//m_fCurAcc = 0.f;

	_float4 vStartPos = m_vStartPos;

	_float fOffsetX = 40.f;
	_float fOffsetDown = 30.f;
	_float fOffsetZ = 40.f;

	switch (m_eNeedleState)
	{
	case Client::CLancerNeedle::LANCERNEEDLE_START:

		break;

	case Client::CLancerNeedle::LANCERNEEDLE_LOOP:

		break;

	case Client::CLancerNeedle::LANCERNEEDLE_ATTACKBEGIN:
		m_pTransform->Set_Scale(_float4(1.f, 1.f, 1.f));
		ENABLE_COMPONENT(GET_COMPONENT(CCollider_Sphere));

		switch (m_iNeedleIndex)
		{
		case 0:
			m_vStartPos = _float4(-41.f, -134.f, -60.f);
			m_vTargetPos = m_vStartPos;
			m_vTargetPos.x -= fOffsetDown + fOffsetX * 2.f;
			m_vTargetPos.z -= fOffsetZ;



			break;

		case 1:
			m_vStartPos = _float4(-41.f, -134.f, -20.f);
			m_vTargetPos = m_vStartPos;
			m_vTargetPos.x -= fOffsetDown + fOffsetX + (fOffsetX / 2.f);
			m_vTargetPos.z -= fOffsetZ;

			break;


		case 2:
			m_vStartPos = _float4(-41.f, -134.f, 20.f);
			m_vTargetPos = m_vStartPos;
			m_vTargetPos.x -= fOffsetDown + fOffsetX + (fOffsetX / 5.f - 5.f);
			m_vTargetPos.z += fOffsetZ;


			break;

		case 3:
			m_vStartPos = _float4(-41.f, -134.f, 60.f);
			m_vTargetPos = m_vStartPos;
			m_vTargetPos.x -= fOffsetDown + fOffsetX + (fOffsetX / 2.f + -10.f);
			m_vTargetPos.z += fOffsetZ;

			break;


		default:
			break;
		}

		break;

	case Client::CLancerNeedle::LANCERNEEDLE_ATTACK:


		if (m_iNeedleIndex == 0)
			m_vTargetPos.x -= fOffsetX;

		else if (m_iNeedleIndex == 1)
			m_vTargetPos.x -= fOffsetX / 2.f;

		else if (m_iNeedleIndex == 2)
			m_vTargetPos.x -= fOffsetX / 5.f - 5.f;

		else if (m_iNeedleIndex == 3)
			m_vTargetPos.x += fOffsetX / 2.f + -10.f;

		break;

	case Client::CLancerNeedle::LANCERNEEDLE_STOP:
		//DISABLE_COMPONENT(GET_COMPONENT(CCollider_Sphere));
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

	_float fOffsetX = 50.f;
	_float fOffsetZ = 30.f;


	switch (m_iNeedleIndex)
	{
	case 0:
		m_vStartPos = _float4(-41.f, -134.f, -60.2f);
		m_vTargetPos = m_vStartPos;
		m_vTargetPos.y = -164.f;
		m_vTargetPos.z -= fOffsetZ;
		


		break;

	case 1:
		m_vStartPos = _float4(-41.f, -134.f, -20.5f);
		m_vTargetPos = m_vStartPos;
		m_vTargetPos.y = -164.f;
		m_vTargetPos.z -= fOffsetZ;

		break;


	case 2:
		m_vStartPos = _float4(-41.f, -134.f, 20.f);
		m_vTargetPos = m_vStartPos;
		m_vTargetPos.y = -164.f;
		m_vTargetPos.z += fOffsetZ;


		break;

	case 3:
		m_vStartPos = _float4(-41.f, -134.f, 60.f);
		m_vTargetPos = m_vStartPos;
		m_vTargetPos.y = -164.f;
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

	//CColorController* pCController = CColorController::Create(CP_BEFORE_RENDERER);

	//if (!pCController)
	//	return E_FAIL;

	//Add_Component(pCController);

	_float fRadius = 0.25f;
	_float4 vOffsetPos = ZERO_VECTOR;
	//vOffsetPos.z += fRadius;
	//vOffsetPos.z += fRadius;
	//vOffsetPos.z += fRadius;

	CCollider_Sphere* pCollider = CCollider_Sphere::Create(CP_AFTER_TRANSFORM, fRadius, COL_BLUEFLYATTACKGUARDBREAK, vOffsetPos, DEFAULT_TRANS_MATRIX);
	vOffsetPos.x += fRadius;
	pCollider->Add_Collider(fRadius, vOffsetPos);
	vOffsetPos.x += fRadius;
	pCollider->Add_Collider(fRadius, vOffsetPos);
	vOffsetPos.x += fRadius;
	pCollider->Add_Collider(fRadius, vOffsetPos);
	vOffsetPos.x += fRadius;
	pCollider->Add_Collider(fRadius, vOffsetPos);


	vOffsetPos = ZERO_VECTOR;

	vOffsetPos.x -= fRadius;
	pCollider->Add_Collider(fRadius, vOffsetPos);
	vOffsetPos.x -= fRadius;
	pCollider->Add_Collider(fRadius, vOffsetPos);
	vOffsetPos.x -= fRadius;
	pCollider->Add_Collider(fRadius, vOffsetPos);
	vOffsetPos.x -= fRadius;
	pCollider->Add_Collider(fRadius, vOffsetPos);
	vOffsetPos.x -= fRadius;
	pCollider->Add_Collider(fRadius, vOffsetPos);
	vOffsetPos.x -= fRadius;
	pCollider->Add_Collider(fRadius, vOffsetPos);

	Add_Component(pCollider);

	m_pCollider = pCollider;

	if (!m_pCollider)
		return E_FAIL;

	//if (FAILED(SetUp_Colliders(COL_BLUEATTACK)))
	//	return E_FAIL;

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

}

void CLancerNeedle::My_LateTick()
{
	__super::My_LateTick();

	if (m_pStinedUnit)
	{
		_float4 vPos = m_pTransform->Get_World(WORLD_POS);
		vPos.y -= 0.5f;

		m_pStinedUnit->Get_Transform()->Set_World(WORLD_POS, vPos);
	}

	switch (m_eNeedleState)
	{
	case Client::CLancerNeedle::LANCERNEEDLE_START:
		

		Chase_OwnerBoneMatrix();

		m_vScale += fDT(0) * 10.f;

		if (m_vScale.x >= 1.f)
		{
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
			m_fCurAcc = m_fTotalTime;

		}

			

		/* 부모 행렬(뼈행렬)을 곱하기 전에 크, 자, 이 하는 과정 */
		_float4x4 matLocal;
		matLocal.Identity();

	// 1. 자전 

		_float vNewAngle = CEasing_Utillity::sinfOut(m_fAngleStartValue, m_fAngleTargetValue, m_fCurAcc, m_fTotalTime);
		//먼저 회전행렬 만들어서 시작 (자전부터)
		CUtility_Transform::Turn_ByAngle(matLocal, _float4(0.f, 0.f, 1.f, 0.f), vNewAngle);

	// 2. 오프셋
		
		_float4 vNewPos = CEasing_Utillity::QuadIn(m_vStartPos, m_vTargetPos, m_fCurAcc, m_fTotalTime);
		memcpy(matLocal.m[3], &vNewPos, sizeof(_float4));


	// 4. 부모
			CHierarchyNode* pNode = GET_COMPONENT_FROM(m_pOwnerUnit, CModel)->Find_HierarchyNode("0B_Head");//("0B_Head");

			if (pNode == nullptr)
				return;

			/* 부모 행렬*/
			_float4x4		matBone = pNode->Get_BoneMatrix();

			

			/* 로컬행렬(크자이) * 부모행렬 = 최종행렬 */
			_float4x4		matFinal = matLocal * matBone;// *matOffset;

			//_float4x4		mat
			m_pTransform->Get_Transform().matMyWorld = matFinal;
			m_pTransform->Make_WorldMatrix();
		}

		break;
	
	case Client::CLancerNeedle::LANCERNEEDLE_ATTACK:
	{
		//if (m_fCurAcc < m_fTotalTime)
		//	m_fCurAcc += fDT(0);
		//else
		//	m_fCurAcc = m_fTotalTime;

		//if (KEY(J, TAP))
		//	On_ChangePhase(LANCERNEEDLE_ATTACKBEGIN);

		//_float4x4 matLocal;
		//matLocal.Identity();

		//_float vNewAngle = CEasing_Utillity::sinfOut(m_fAngleStartValue, m_fAngleTargetValue, m_fCurAcc, m_fTotalTime);
		////먼저 회전행렬 만들어서 시작 (자전부터)
		//CUtility_Transform::Turn_ByAngle(matLocal, _float4(0.f, 0.f, 1.f, 0.f), vNewAngle);

		//// 2. 오프셋
		//_float4 vNewPos = CEasing_Utillity::QuadIn(m_vLerpPos, m_vTargetPos, m_fCurAcc, m_fTotalTime);
		//memcpy(matLocal.m[3], &vNewPos, sizeof(_float4));

		//CHierarchyNode* pNode = GET_COMPONENT_FROM(m_pOwnerUnit, CModel)->Find_HierarchyNode("0B_Head");//("0B_Head");

		///* 부모 행렬*/
		//_float4x4		matBone = pNode->Get_BoneMatrix();

		///* 로컬행렬(크자이) * 부모행렬 = 최종행렬 */
		//_float4x4		matFinal = matLocal * matBone;
		////_float4x4		mat
		//m_pTransform->Get_Transform().matMyWorld = matFinal;
		//m_pTransform->Make_WorldMatrix();

	}
		break;
	
	case Client::CLancerNeedle::LANCERNEEDLE_STOP:

		// 끝나면 Disable
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
	DISABLE_COMPONENT(GET_COMPONENT(CCollider_Sphere));
	m_vScale = _float4(FLT_MIN, FLT_MIN, FLT_MIN);
	m_eNeedleState = LANCERNEEDLE_START;
	m_pTransform->Set_Scale(m_vScale);
}

void CLancerNeedle::OnDisable()
{
	__super::OnDisable();
	DISABLE_COMPONENT(GET_COMPONENT(CCollider_Sphere));
	m_vScale = _float4(FLT_MIN, FLT_MIN, FLT_MIN);
	m_pStinedUnit = nullptr;

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
