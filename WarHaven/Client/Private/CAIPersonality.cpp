#include "CAIPersonality.h"
#include "Functor.h"
#include "CAIController.h"
#include "CTable_Conditions.h"
#include "CBehavior.h"
CAIPersonality::CAIPersonality(CTable_Conditions* pConditionTable)
	:m_pConditionTable(pConditionTable)
{
}

CAIPersonality::~CAIPersonality()
{
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

