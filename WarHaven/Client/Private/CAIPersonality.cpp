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
	//해시 저장용
	m_tPersonalDesc.strPersonalityName = wstring(L"");

	//상태전환시 딜레이용
	m_tPersonalDesc.fChangeDelayTime = 0.01f;

	//상태 전환시 딜레이 가중치용 --> 합연산
	m_tPersonalDesc.fDelayWeight = 0.f;

	//목표 인식 범위
	m_tPersonalDesc.fSIghtRadius = 30.f;

	//인식범위 가중치용 --> 합연산
	m_tPersonalDesc.fSightRadiusWeight = 0.f;

	//확인할 HP
	m_tPersonalDesc.fCheckedHP = 30.f;

	//확인 할 HP의 가중치용 --> 합연산
	m_tPersonalDesc.fHPWeight = 0.f;

	//MainPersonality레벨 --> 곱연산으로 가중치 붙음
	m_tPersonalDesc.iMainPersonalityLevel = 1;

	//확률 계산시 전체적인 선택에 영향을 줄 메인 성향
	m_tPersonalDesc.eMainPerosnality = eMain_END;

	//유닛 선택및 전투시 영향
	m_tPersonalDesc.eFightPersonality = eFight_Default;

	//팀업 시 영향
	m_tPersonalDesc.eCoopPersonality = eCoop_Default;

	//행동별 최대로 머무를 시간
	ZeroMemory(m_tPersonalDesc.fRemainMaxTime, sizeof(_float) * _uint(eBehaviorType::eCNT));

	// 비볐는지 체크 용도
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

