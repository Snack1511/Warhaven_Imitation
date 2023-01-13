#include "stdafx.h"
#include "CLancerNeedle.h"

#include "UsefulHeaders.h"
#include "CTeamConnector.h"
#include "CCollider_Sphere.h"
#include "HIerarchyNode.h"
#include "CUnit.h"
#include "Easing_Utillity.h"
#include "CUtility_Transform.h"
#include "CColorController.h"
#include "CEffect.h"
#include "CRectEffects.h"

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

	CUnit* pOtherUnit = nullptr;
	pOtherUnit = dynamic_cast<CUnit*>(pOtherObj);

	if (!pOtherUnit)
		return;

	if (m_pOwnerUnit == pOtherUnit)
		return;

	if (pOtherUnit->Get_Status().fHP >= 0.f)
	{
		if (m_pStinedUnit == pOtherUnit)
			return;

		m_pStinedUnit = pOtherUnit;
		DISABLE_COMPONENT(GET_COMPONENT_FROM(m_pStinedUnit, CCollider_Sphere));

		if (m_pOwnerUnit->Is_MainPlayer())
		{
			CUnit::UNIT_STATUS eStatus = m_pOwnerUnit->Get_Status();
			m_pOwnerUnit->Shake_Camera(eStatus.fCamPower * 1.2f, eStatus.fCamTime * 1.2f);
		}
	}
		

	
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

		GET_COMPONENT(CModel)->Set_RimLightFlag(RGBA(255, 40, 0, 1.f));

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

		/*if (m_pNiddleMesh)
		{
			static_cast<CEffect*>(m_pNiddleMesh)->Set_FadeOut();
			m_pNiddleMesh = nullptr;
		}*/

		/*if (!m_NiddleMesh.empty())
		{
			for (auto& elem : m_NiddleMesh)
			{
				static_cast<CEffect*>(elem)->Set_FadeOut();
			}
			m_NiddleMesh.clear();
		}*/

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

	CCollider_Sphere* pCollider = CCollider_Sphere::Create(CP_AFTER_TRANSFORM, fRadius, COL_BLUEGROGGYATTACK, vOffsetPos, DEFAULT_TRANS_MATRIX);
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

	m_NiddleMesh.clear();
	return S_OK;
}

void CLancerNeedle::My_Tick()
{
	__super::My_Tick();
	
	
}

void CLancerNeedle::My_LateTick()
{
	__super::My_LateTick();

	_float4x4		matFinal;
	matFinal.Identity();

	switch (m_eNeedleState)
	{
	case Client::CLancerNeedle::LANCERNEEDLE_START:

		Chase_OwnerBoneMatrix();

		if (m_bStartNeedle)
		{
			CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Turn_Needle", this, ZERO_VECTOR);

			/*if(!m_pNiddleMesh)
				m_pNiddleMesh = CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"Needle_Mesh_0"), this, ZERO_VECTOR);*/

			if (m_NiddleMesh.empty())
			{
				m_NiddleMesh = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Needle_Mesh", this, ZERO_VECTOR);
			}

			m_bStartNeedle = false;
		}


		m_fCurAcc += fDT(0);

		m_vScale = CEasing_Utillity::SinIn(m_vScale, _float4(1.f, 1.f, 1.f), m_fCurAcc, 0.3f);

		if (m_vScale.x >= 1.f)
		{
			m_pTransform->Set_Scale(_float4(1.f, 1.f, 1.f));
			On_ChangePhase(LANCERNEEDLE_LOOP);
		}
		else
		{
			m_pTransform->Set_Scale(m_vScale);
		}


		break;
	case Client::CLancerNeedle::LANCERNEEDLE_LOOP:

		Chase_OwnerBoneMatrix();

		if (KEY(C, TAP))
			CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Turn_Needle", this, ZERO_VECTOR);

		break;
	
	case Client::CLancerNeedle::LANCERNEEDLE_ATTACKBEGIN:
	{

		if (!m_pOwnerBone)
			return;
		
		m_fCurAcc += fDT(0);

		if (m_fCurAcc > m_fTotalTime)
		{
			m_fCurAcc = m_fTotalTime;
			//이펙트
		}

		if (m_bBeginNeedle)
		{
			if (m_NiddleBegin.empty())
			{
				m_NiddleBegin = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Needle_Begin", this, ZERO_VECTOR);
			}
			m_bBeginNeedle = false;
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
			matFinal = matLocal * matBone;// *matOffset;

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

		CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Needle_End", this, ZERO_VECTOR);
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

	Dragging_Unit();
}

void CLancerNeedle::OnEnable()
{
	__super::OnEnable();

	if (!m_pOwnerUnit->Get_OwnerPlayer()->Get_Team())
		m_pCollider->Set_ColIndex(COL_BLUEGROGGYATTACK);
	else
	{
		eTEAM_TYPE eTeamType = m_pOwnerUnit->Get_OwnerPlayer()->Get_Team()->Get_TeamType();

		if (eTeamType == eTEAM_TYPE::eBLUE)
			m_pCollider->Set_ColIndex(COL_BLUEGROGGYATTACK);
		else
			m_pCollider->Set_ColIndex(COL_REDGROGGYATTACK);
	}

	DISABLE_COMPONENT(GET_COMPONENT(CCollider_Sphere));
	m_vScale = _float4(FLT_MIN, FLT_MIN, FLT_MIN);
	m_eNeedleState = LANCERNEEDLE_START;
	m_pTransform->Set_Scale(m_vScale);
	m_pStinedUnit = nullptr;

	GET_COMPONENT(CModel)->Set_RimLightFlag(RGBA(255, 40, 0, 0.f));
	//CColorController::COLORDESC tColorDesc;
	//ZeroMemory(&tColorDesc, sizeof(CColorController::COLORDESC));

	//tColorDesc.eFadeStyle = CColorController::TIME;
	//tColorDesc.fFadeInStartTime = 0.f;
	//tColorDesc.fFadeInTime = 0.2f;
	//tColorDesc.fFadeOutStartTime = 10.f;
	//tColorDesc.fFadeOutTime = 0.2f;
	//tColorDesc.vTargetColor = _float4(1.f, 0.5f, 0.f, 1.f);

	//tColorDesc.iMeshPartType = 0;
	//GET_COMPONENT(CColorController)->Add_ColorControll(tColorDesc);
}

void CLancerNeedle::OnDisable()
{
	__super::OnDisable();
	DISABLE_COMPONENT(GET_COMPONENT(CCollider_Sphere));
	m_vScale = _float4(FLT_MIN, FLT_MIN, FLT_MIN);
	
	if (m_pStinedUnit)
	{
		m_pStinedUnit->On_Die();
		m_pStinedUnit = nullptr;
	}
	
	m_bStartNeedle = true;
	m_bBeginNeedle = true;

	/*if(m_pNiddleMesh)
	{
		static_cast<CEffect*>(m_pNiddleMesh)->Set_FadeOut();
		m_pNiddleMesh = nullptr;
	}*/

	if (!m_NiddleMesh.empty())
	{
		for (auto& elem : m_NiddleMesh)
		{
			static_cast<CEffect*>(elem)->Set_FadeOut();
		}
		m_NiddleMesh.clear();
	}

	if (!m_NiddleBegin.empty())
	{
		for (auto& elem : m_NiddleBegin)
		{
			static_cast<CRectEffects*>(elem)->Set_AllFadeOut();
		}
		m_NiddleBegin.clear();
	}
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

void CLancerNeedle::Dragging_Unit()
{
	if (m_pStinedUnit)
	{
		if (!m_pStinedUnit->Is_Valid())
		{
			m_pStinedUnit = nullptr;
			return;
		}



		_float4 vStartPos = m_vStartPos;
		vStartPos.y -= 20.f;

		_float4x4 matLocal;
		matLocal.Identity();

		_float4 vNewPos = CEasing_Utillity::QuadIn(vStartPos, m_vTargetPos, m_fTotalTime, m_fTotalTime);
		memcpy(matLocal.m[3], &vNewPos, sizeof(_float4));

		CHierarchyNode* pNode = GET_COMPONENT_FROM(m_pOwnerUnit, CModel)->Find_HierarchyNode("0B_Head");//("0B_Head");

		_float4x4		matBone = pNode->Get_BoneMatrix();

		CTransform* pOtherTransform = m_pStinedUnit->Get_Transform();
		matBone = matLocal * matBone;

		_float4 vRight = m_pTransform->Get_World(WORLD_RIGHT);
		_float4 vLook = m_pTransform->Get_World(WORLD_LOOK);


		pOtherTransform->Get_Transform().matMyWorld = matBone;
		CUtility_Transform::Rotation(pOtherTransform, _float4(0.f, 1.f, 0.f, 0.f), 270.f);

		_float4 vOtherPos = m_pTransform->Get_World(WORLD_POS);
		
		CLASS_TYPE eClass = m_pStinedUnit->Get_OwnerPlayer()->Get_CurClass();

		switch (eClass)
		{
		case Client::WARRIOR:
			vOtherPos.y -= 0.55f;
			break;

		case Client::SPEAR:
			vOtherPos.y -= 0.55f;
			break;

		case Client::ARCHER:
			vOtherPos.y -= 0.55f;
			break;

		case Client::PALADIN:
			vOtherPos.y -= 0.55f;
			break;

		case Client::PRIEST:
			vOtherPos.y -= 0.55f;
			break;

		case Client::ENGINEER:
			vOtherPos.y -= 0.8f;
			break;

		case Client::FIONA:

			break;

		case Client::QANDA:
			vOtherPos.y -= 0.55f;
			break;

		case Client::HOEDT:

			break;

		case Client::LANCER:

			break;

		default:
			vOtherPos.y -= 0.55f;
			break;

		}
		
		pOtherTransform->Set_World(WORLD_POS, vOtherPos);
		pOtherTransform->Make_WorldMatrix();
	}
}
