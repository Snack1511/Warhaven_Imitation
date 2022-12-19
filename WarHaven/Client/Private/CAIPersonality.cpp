#include "CAIPersonality.h"
#include "Functor.h"
#include "CAIController.h"
#include "CTable_Conditions.h"
#include "CBehavior.h"
#include "GameInstance.h"
CAIPersonality* CAIPersonality::Create(CTable_Conditions* pConditionTable)
{
	CAIPersonality* pInstance = new CAIPersonality(pConditionTable);
	if (FAILED(pInstance->Initailize()))
	{
		Call_MsgBox(L"Failed to Initialize : CAIPersonality");
		SAFE_DELETE(pInstance);
	}
	if (FAILED(pInstance->SetUp_PersonalityName(wstring(L"Empty_Personlity"))))
	{
		Call_MsgBox(L"Failed to SetUp_PersonalityName : CAIPersonality");
		SAFE_DELETE(pInstance);
	}
	return pInstance;
}
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
void CAIPersonality::Load(wstring strPersonalityName)
{
	ifstream readFile;
	//D:\PersonalData\MyProject\jusin128thFinalTeamPotpolio\WarHaven\Client\Bin\Data\GameSystem\Perosnality
	string strPath = "../bin/Data/GameSystem/Perosnality/";
	strPath += CFunctor::To_String(strPersonalityName);
	strPath += ".bin";
	readFile.open(strPath, ios::binary);


	if (!readFile.is_open())
	{
		wstring strData = strPersonalityName;
		strData += L"_Load Failed";
		Call_MsgBox(strData.c_str());
		return;
	}

	Clear_Behavior();

	m_tPersonalDesc.strPersonalityName = strPersonalityName;

	readFile.read((char*)&m_tPersonalDesc.tPersonalityData, sizeof(PersonalData));

	_uint iBehaviorCount = _uint(m_BehaviorList.size());
	readFile.read((char*)&iBehaviorCount, sizeof(_uint));

	for (_uint i = 0; i < iBehaviorCount; ++i)
	{
		_uint iBehaviorNameLenght = 0;
		readFile.read((char*)&iBehaviorNameLenght, sizeof(_uint));
		char szName[MAXCHAR] = "";
		readFile.read(szName, sizeof(char)* iBehaviorNameLenght);
		string strName = szName;

		_uint iPriority = 0;
		readFile.read((char*)&iPriority, sizeof(_uint));

		CBehavior* pBehavior = nullptr;
		pBehavior = m_pConditionTable->Find_Behavior(CFunctor::To_Wstring(strName))->Clone();
		pBehavior->Initialize();
		pBehavior->Set_Priority(iPriority);
		m_BehaviorList.push_back(pBehavior);
	}

}
void CAIPersonality::Save()
{
	ofstream writeFile;
	//D:\PersonalData\MyProject\jusin128thFinalTeamPotpolio\WarHaven\Client\Bin\Data\GameSystem\Perosnality
	string strPath = "../bin/Data/GameSystem/Perosnality/";
	strPath += CFunctor::To_String(m_tPersonalDesc.strPersonalityName);
	strPath += ".bin";
	writeFile.open(strPath, ios::binary);

	if (!writeFile.is_open())
	{
		wstring strData = m_tPersonalDesc.strPersonalityName;
		strData += L"_Save Failed";
		Call_MsgBox(strData.c_str());
		return;
	}
	
	writeFile.write((char*)&m_tPersonalDesc.tPersonalityData, sizeof(PersonalData));

	_uint iBehaviorCount = _uint(m_BehaviorList.size());
	writeFile.write((char*)&iBehaviorCount, sizeof(_uint));

	for (auto& ListValue : m_BehaviorList)
	{
		string strName = CFunctor::To_String(ListValue->Get_BehaviorName());
		_uint iPriority = ListValue->Get_Priority();

		_uint iBehaviorNameLenght = _uint(strName.length()) + 1;
		writeFile.write((char*)&iBehaviorNameLenght, sizeof(_uint));

		char szName[MAXCHAR] = "";
		strcat_s(szName, sizeof(char) * MAXCHAR, strName.c_str());
		strcat_s(szName, sizeof(char) * MAXCHAR, "\0");
		writeFile.write(szName, sizeof(char)* iBehaviorNameLenght);

		writeFile.write((char*)&iPriority, sizeof(_uint));
	}
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
	m_tPersonalDesc.tPersonalityData.fChangeDelayTime = 0.01f;

	//상태 전환시 딜레이 가중치용 --> 합연산
	m_tPersonalDesc.tPersonalityData.fDelayWeight = 0.f;

	//목표 인식 범위
	m_tPersonalDesc.tPersonalityData.fSIghtRadius = 30.f;

	//인식범위 가중치용 --> 합연산
	m_tPersonalDesc.tPersonalityData.fSightRadiusWeight = 0.f;

	//확인할 HP
	m_tPersonalDesc.tPersonalityData.fCheckedHP = 30.f;

	//확인 할 HP의 가중치용 --> 합연산
	m_tPersonalDesc.tPersonalityData.fHPWeight = 0.f;

	//MainPersonality레벨 --> 곱연산으로 가중치 붙음
	m_tPersonalDesc.tPersonalityData.iMainPersonalityLevel = 1;

	//확률 계산시 전체적인 선택에 영향을 줄 메인 성향
	m_tPersonalDesc.tPersonalityData.eMainPerosnality = eMain_END;

	//유닛 선택및 전투시 영향
	m_tPersonalDesc.tPersonalityData.eFightPersonality = eFight_Default;

	//팀업 시 영향
	m_tPersonalDesc.tPersonalityData.eCoopPersonality = eCoop_Default;

	//행동별 최대로 머무를 시간
	ZeroMemory(m_tPersonalDesc.tPersonalityData.fRemainMaxTime, sizeof(_float) * _uint(eBehaviorType::eCNT));

	// 비볐는지 체크 용도
	ZeroMemory(m_tPersonalDesc.tPersonalityData.fMinMoveAcc, sizeof(_float) * _uint(eBehaviorType::eCNT));
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
	_float fReturnDelay = m_tPersonalDesc.tPersonalityData.fChangeDelayTime;
	switch (m_tPersonalDesc.tPersonalityData.eMainPerosnality)
	{
	case eMain_Elite:
		fReturnDelay -= m_tPersonalDesc.tPersonalityData.fDelayWeight;
		break;
	case eMain_Dummy:
		fReturnDelay += m_tPersonalDesc.tPersonalityData.fDelayWeight;
		break;
	case eMain_Crazy:
		fReturnDelay = frandom(m_tPersonalDesc.tPersonalityData.fChangeDelayTime,
			m_tPersonalDesc.tPersonalityData.fChangeDelayTime + (m_tPersonalDesc.tPersonalityData.fDelayWeight * m_tPersonalDesc.tPersonalityData.iMainPersonalityLevel));
		break;
	}

	return fReturnDelay;
}

_float CAIPersonality::Get_Range()
{
	_float fReturnRange = m_tPersonalDesc.tPersonalityData.fSIghtRadius;
	switch (m_tPersonalDesc.tPersonalityData.eMainPerosnality)
	{
	case eMain_Elite:
		fReturnRange += m_tPersonalDesc.tPersonalityData.fSightRadiusWeight;
		break;
	case eMain_Dummy:
		fReturnRange -= m_tPersonalDesc.tPersonalityData.fSightRadiusWeight;
		break;
	case eMain_Crazy:
		fReturnRange = frandom(m_tPersonalDesc.tPersonalityData.fSightRadiusWeight, m_tPersonalDesc.tPersonalityData.fSightRadiusWeight * m_tPersonalDesc.tPersonalityData.iMainPersonalityLevel);
		break;
	}

	return fReturnRange;
}

_float CAIPersonality::Get_CheckedHP()
{
	_float fReturnRange = m_tPersonalDesc.tPersonalityData.fCheckedHP;
	switch (m_tPersonalDesc.tPersonalityData.eMainPerosnality)
	{
	case eMain_Elite:
		fReturnRange += m_tPersonalDesc.tPersonalityData.fHPWeight;
		break;
	case eMain_Dummy:
		fReturnRange -= m_tPersonalDesc.tPersonalityData.fHPWeight;
		break;
	case eMain_Crazy:
		fReturnRange = frandom(m_tPersonalDesc.tPersonalityData.fHPWeight, m_tPersonalDesc.tPersonalityData.fHPWeight * m_tPersonalDesc.tPersonalityData.iMainPersonalityLevel);
		break;
	}

	return fReturnRange;
}

_bool CAIPersonality::Is_LongTimeRemain(eBehaviorType eBhavior)
{
	return m_tPersonalDesc.tPersonalityData.fRemainMaxTime[_uint(eBhavior)] <= m_fRemainAccTime[_uint(eBhavior)];
}

void CAIPersonality::Update_RemainTime(eBehaviorType eBhavior)
{
	m_fRemainAccTime[_uint(eBhavior)] += fDT(0);
}

void CAIPersonality::Init_RemainTime(eBehaviorType eBhavior)
{
	m_fRemainAccTime[_uint(eBhavior)] = 0.f;
}

CBehavior* CAIPersonality::Add_Behavior(wstring strBehaviorName)
{
	CBehavior* pBehavior = nullptr;
	pBehavior = m_pConditionTable->Find_Behavior(strBehaviorName)->Clone();
	pBehavior->Initialize();
	m_BehaviorList.push_back(pBehavior);
	return pBehavior;
}

void CAIPersonality::Delete_Behavior(CBehavior* pBehavior)
{
	m_BehaviorList.remove(pBehavior);
	SAFE_DELETE(pBehavior);
	m_bBehaviorListUpdateFlag = true;
}

void CAIPersonality::Clear_Behavior()
{
	for (auto& elem : m_BehaviorList)
		SAFE_DELETE(elem);
	m_BehaviorList.clear();
	m_bBehaviorListUpdateFlag = true;
}

CBehavior* CAIPersonality::Get_LastBehavior()
{
	CBehavior* pBehavior = m_BehaviorList.back();
	return pBehavior;
}

void CAIPersonality::Sort_BehaviorWithPriority()
{
	m_BehaviorList.sort([](CBehavior* Sour, CBehavior* Dest)
		{
			if (Sour->Get_Priority() < Dest->Get_Priority())
				return true;
			else return false;
		});
	m_bBehaviorListUpdateFlag = true;
}

void CAIPersonality::Complete_Update()
{
	m_bBehaviorListUpdateFlag = false; 
}

_bool CAIPersonality::Check_LessMoveAcc(eBehaviorType eBhavior, _float fCurMoveAcc)
{
	return (m_tPersonalDesc.tPersonalityData.fMinMoveAcc[_uint(eBhavior)] >= fCurMoveAcc);
}


_float CAIPersonality::Get_LimitRouteDistance()
{
	
	return Get_Range() * 5.f / m_tPersonalDesc.tPersonalityData.fSIghtRadius;
}

