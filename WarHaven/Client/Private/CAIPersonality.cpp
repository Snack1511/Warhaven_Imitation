#include "CAIPersonality.h"
#include "Functor.h"
#include "CAIController.h"
#include "CTable_Conditions.h"
#include "CBehavior.h"
#include "GameInstance.h"
CAIPersonality* CAIPersonality::Create(wstring strPersonalityName, CTable_Conditions* pConditionTable)
{
	CAIPersonality* pInstance = new CAIPersonality(pConditionTable);
	if (FAILED(pInstance->Initailize()))
	{
		Call_MsgBox(L"Failed to Initialize : CAIPersonality");
		SAFE_DELETE(pInstance);
	}
	if (FAILED(pInstance->SetUp_PersonalityName(strPersonalityName)))
	{
		Call_MsgBox(L"Failed to SetUp_PersonalityName : CAIPersonality");
		SAFE_DELETE(pInstance);
	}
	return pInstance;
}
CAIPersonality::CAIPersonality(CTable_Conditions* pConditionTable)
	:m_pConditionTable(pConditionTable)
{
}

CAIPersonality::~CAIPersonality()
{
	Release();
}

HRESULT CAIPersonality::Initailize()
{
	//�ؽ� �����
	m_tPersonalDesc.strPersonalityName = wstring(L"");

	//������ȯ�� �����̿�
	m_tPersonalDesc.fChangeDelayTime = 0.01f;

	//���� ��ȯ�� ������ ����ġ�� --> �տ���
	m_tPersonalDesc.fDelayWeight = 0.f;

	//��ǥ �ν� ����
	m_tPersonalDesc.fSIghtRadius = 30.f;

	//�νĹ��� ����ġ�� --> �տ���
	m_tPersonalDesc.fSightRadiusWeight = 0.f;

	//Ȯ���� HP
	m_tPersonalDesc.fCheckedHP = 30.f;

	//Ȯ�� �� HP�� ����ġ�� --> �տ���
	m_tPersonalDesc.fHPWeight = 0.f;

	//MainPersonality���� --> ���������� ����ġ ����
	m_tPersonalDesc.iMainPersonalityLevel = 1;

	//Ȯ�� ���� ��ü���� ���ÿ� ������ �� ���� ����
	m_tPersonalDesc.eMainPerosnality = eMain_END;

	//���� ���ù� ������ ����
	m_tPersonalDesc.eFightPersonality = eFight_Default;

	//���� �� ����
	m_tPersonalDesc.eCoopPersonality = eCoop_Default;

	//�ൿ�� �ִ�� �ӹ��� �ð�
	ZeroMemory(m_tPersonalDesc.fRemainMaxTime, sizeof(_float) * _uint(eBehaviorType::eCNT));

	// �񺳴��� üũ �뵵
	ZeroMemory(m_tPersonalDesc.fMinMoveAcc, sizeof(_float) * _uint(eBehaviorType::eCNT));
	return S_OK;
}

void CAIPersonality::Release()
{
	for (auto& elem : m_BehaviorList)
		SAFE_DELETE(elem);

	SAFE_DELETE(m_pPatrolBehavior);
}

HRESULT CAIPersonality::SetUp_PersonalityName(wstring strPersonalityName)
{
	m_tPersonalDesc.strPersonalityName = strPersonalityName;
	return S_OK;
}


_float CAIPersonality::Get_Delay()
{
	_float fReturnDelay = m_tPersonalDesc.fChangeDelayTime;
	switch (m_tPersonalDesc.eMainPerosnality)
	{
	case eMain_Elite:
		fReturnDelay -= m_tPersonalDesc.fDelayWeight;
		break;
	case eMain_Dummy:
		fReturnDelay += m_tPersonalDesc.fDelayWeight;
		break;
	case eMain_Crazy:
		fReturnDelay = frandom(m_tPersonalDesc.fChangeDelayTime,
			m_tPersonalDesc.fChangeDelayTime + (m_tPersonalDesc.fDelayWeight * m_tPersonalDesc.iMainPersonalityLevel));
		break;
	}

	return fReturnDelay;
}

_float CAIPersonality::Get_Range()
{
	_float fReturnRange = m_tPersonalDesc.fSIghtRadius;
	switch (m_tPersonalDesc.eMainPerosnality)
	{
	case eMain_Elite:
		fReturnRange += m_tPersonalDesc.fSightRadiusWeight;
		break;
	case eMain_Dummy:
		fReturnRange -= m_tPersonalDesc.fSightRadiusWeight;
		break;
	case eMain_Crazy:
		fReturnRange = frandom(m_tPersonalDesc.fSightRadiusWeight, m_tPersonalDesc.fSightRadiusWeight * m_tPersonalDesc.iMainPersonalityLevel);
		break;
	}

	return fReturnRange;
}

_float CAIPersonality::Get_CheckedHP()
{
	_float fReturnRange = m_tPersonalDesc.fCheckedHP;
	switch (m_tPersonalDesc.eMainPerosnality)
	{
	case eMain_Elite:
		fReturnRange += m_tPersonalDesc.fHPWeight;
		break;
	case eMain_Dummy:
		fReturnRange -= m_tPersonalDesc.fHPWeight;
		break;
	case eMain_Crazy:
		fReturnRange = frandom(m_tPersonalDesc.fHPWeight, m_tPersonalDesc.fHPWeight * m_tPersonalDesc.iMainPersonalityLevel);
		break;
	}

	return fReturnRange;
}

_bool CAIPersonality::Is_LongTimeRemain(eBehaviorType eBhavior)
{
	return m_tPersonalDesc.fRemainMaxTime[_uint(eBhavior)] <= m_fRemainAccTime[_uint(eBhavior)];
}

void CAIPersonality::Update_RemainTime(eBehaviorType eBhavior)
{
	m_fRemainAccTime[_uint(eBhavior)] += fDT(0);
}

void CAIPersonality::Init_RemainTime(eBehaviorType eBhavior)
{
	m_fRemainAccTime[_uint(eBhavior)] = 0.f;
}

_bool CAIPersonality::Check_LessMoveAcc(eBehaviorType eBhavior, _float fCurMoveAcc)
{

	return (m_tPersonalDesc.fMinMoveAcc[_uint(eBhavior)] >= fCurMoveAcc);
}


_float CAIPersonality::Get_LimitRouteDistance()
{
	
	return Get_Range() * 5.f / m_tPersonalDesc.fSIghtRadius;
}

