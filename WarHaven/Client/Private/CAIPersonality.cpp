#include "CAIPersonality.h"
#include "Functor.h"
#include "CAIController.h"
#include "CTable_Conditions.h"
#include "CBehavior.h"
#include "GameInstance.h"
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

	return S_OK;
}

void CAIPersonality::Release()
{
	for (auto& elem : m_BehaviorList)
		SAFE_DELETE(elem);

	SAFE_DELETE(m_pPatrolBehavior);
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


_float CAIPersonality::Get_LimitRouteDistance()
{
	
	return Get_Range() * 5.f / m_tPersonalDesc.fSIghtRadius;
}

