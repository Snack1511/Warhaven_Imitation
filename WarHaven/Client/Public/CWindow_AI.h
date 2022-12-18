#pragma once
#include "Client_Defines.h"
#include "CImGui_Window.h"
BEGIN(Client)
class CPlayer;
class CAIPersonality;
class CBehavior;
class CTeamConnector;
class CSquad;
class CTable_Conditions;
class CWindow_AI :
    public CImGui_Window
{
protected:
	CWindow_AI();
	virtual ~CWindow_AI();
public:
	static CWindow_AI* Create();
public:
	virtual void On_Enable() override;
	virtual void On_Disable()override;
public:
	// CImGui_Window을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void Tick() override;
	virtual HRESULT Render() override;
public:
	void On_Grouping(const char* GroupName, const ImVec2& Size, const function<void(const ImVec2&)> func);
public:
	void Func_PlayerSetting();
	void Func_AISetting();
private:
	void ListUp_RedPlayer(const ImVec2& Size);
	void ListUp_BluePlayer(const ImVec2& Size);
	void ListUp_Player(const char* ListID, const ImVec2& Size, CPlayer*& pCurSelectPlayer, CTeamConnector* TeamConnector, eTEAM_TYPE eTeamType);
private:
	void Func_ChangeBehavior();
	void ListUp_Behaviors(const char* ListID, const ImVec2& Size, list<CBehavior*>& BehaviorList);

	void ListUp_BehaviorConditions(const char* szListName, const char* ListID, const ImVec2& Size, CBehavior* pBehavior, wstring& rhsConditionName, _uint iConditionType);
private:
	void Display_Data(string strTitle, string strData, const ImVec4& vTitleColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f), const ImVec4& vDataColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
private:
	void Add_Team(eTEAM_TYPE eTeamType);
private:
	CPlayer* Add_Player(_bool bLeader, wstring strPlayerInfoName, CSquad* pSquad, eTEAM_TYPE eTeamType);
	void Delete_Player(CPlayer* pSelectPlayer, CSquad* pSquad);
private:
	CAIPersonality* Create_Personality(string strPersonalityName);
	void Save_Personality(CAIPersonality* pCurSelectPersonality);
	CAIPersonality* Load_Personality(string strPersonalityName);
private:
	void Clear_SelectPlayerData();
	void SetUp_SelectPersonality();
	void Update_Personality();
private:
	void		Create_SubWindow(const char* szWindowName, const ImVec2& Pos, const ImVec2& Size, function<void(CWindow_AI&)> func);
private:
	_bool m_bHoverWindow = false;
private:
	ImVec2 m_vMainWndSize = ImVec2(0.f, 0.f);
	CTeamConnector* m_pTeamConnector[_uint(eTEAM_TYPE::eCOUNT)] = {nullptr};
	vector<wstring>* m_pVecPlayerInfoName = nullptr;
	CTable_Conditions* m_pTableCondition = nullptr;
private:
	CPlayer* m_pCurSelectRedPlayer = nullptr;
	CPlayer* m_pCurSelectBluePlayer = nullptr;
	CPlayer* m_pCurSelectPlayer = nullptr;
	CAIPersonality* m_pCurSelectPersonality = nullptr;
	CBehavior* m_pCurSelectBehavior = nullptr;
	_uint m_iCurPlayerNameIndex = 0;
	wstring m_strCurSelectWhenCondition = L"";
	wstring m_strCurSelectWhatCondition = L"";
};

END
/*
	CPlayer
	--> 플레이어 생성 삭제시 누수 생김
	--> 플레이어 생성시 레벨에 적용안됨 --> Ready_Object함수를 대체할 거를 만들어야 할듯
	CAIPersonality --> 기본 생성루틴 정의
	CAIPersonality --> 로드루틴 정의
	CAIPersonality --> 세이브루틴 정의
	CAIPersonality에 CBehavior추가 제거루틴 정의 --> ListUp_Behaviors에서
	선택된 CBehavior의 조건 추가 제거루틴 정의 --> ListUp_BehaviorConditions에서
	선택된 CBehavior의 가중치 셋팅 --> Func_ChangeBehavior에서



*/