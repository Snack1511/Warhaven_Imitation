#pragma once
#include "Client_Defines.h"

#define	DECLARE_PLAYERINFO(classname) public:\
static classname* Create() \
{ \
classname* pInstance = new classname(); \
if (FAILED(pInstance->Initialize()))\
{\
	string text = "Failed to Initialize : ";\
	text += typeid(classname).name();\
	wstring wText;\
	wText.assign(text.begin(), text.end());\
	Call_MsgBox(wText.c_str());\
	delete pInstance;\
	pInstance = nullptr;\
}\
return pInstance;\
}\


//실제 mainPlayer 정보 들고있는 클래스
//인벤토리, 초기화 정보 등등
//이 정보를 토대로 매칭되고 플레이어 클래스 만들어질거임

//GameSystem은 16개의 Player Info를 들고있따.
//스테이지 진입 시 섞어서 매칭해준다.

// Warrior, Engineer 는 지휘관 급에서 사용하기 때문에 만약 Define 을 주석건다면 PlayerInfo_Leader 에서 #else 나 #ifndef 로 처리해주세요

#define WARRIOR_TH
#define ARCHER_TH
//#define FIONA_TH
//#define PALADIN_TH
#define ENGINEER_TH
//#define PRIEST_TH
//#define QANDA_TH
//#define LANCER_TH

BEGIN(Client)

class CTeamConnector;
class CSquad;
class CPlayer;  
class CAIPersonality;
class CPlayerInfo abstract
{

public:
	struct PLAYER_INFO
	{
		wstring wstrName;
		wstring wstrCamName;
		_uint	iLevel = 1;

	};

	struct PLAYER_SETUP_DATA
	{
		/* Custom 정보 */
		wstring	wstrHelmetMeshPath[CLASS_END];
		wstring	wstrBodyMeshPath[CLASS_END];
		wstring	wstrWeaponMeshPath[CLASS_END];
	};

	friend class CGameSystem;
	friend class CPlayer;

protected:
	CPlayerInfo();
	virtual ~CPlayerInfo();

public:
	/* 레벨 로딩시 이 함수를 통해서 Player를 만들어냄 */
	CPlayer* Make_Player();

public:
	CPlayer* Get_Player() { return m_pMyPlayer; }
	eTEAM_TYPE	Get_TeamType() { return m_eTeamType; }
	void	Set_TeamType(eTEAM_TYPE eEnum) { m_eTeamType = eEnum; }
	void	Set_Squad(CSquad* pSquad) { m_pMySquad = pSquad; }
	void	Set_TeamConnector(CTeamConnector* pTeam) { m_pMyTeam = pTeam; }
	void	Set_ChosenClass(CLASS_TYPE eEnum) { m_eCurChosenClass = eEnum; }

	CLASS_TYPE	Get_ChonsenClass() { return m_eCurChosenClass; }
	CLASS_TYPE	Get_ChangeHeroClass() { return m_eChangeHeroClass; }

	_uint	Get_UnitType() { return m_iUnitType; }

	wstring Get_PlayerName() { return m_tPlayerInfo.wstrName; }
	wstring Get_LookAtCamName() { return m_tPlayerInfo.wstrCamName; }

	PLAYER_SETUP_DATA	Get_SetUpData() { return m_tPlayerSetUpData; }

public:
	/* 커스텀 추가될 때마다 여기 enum 추가해서 넣어놓기 */
	enum class eCUSTOM_HEAD { eDEFAULT, eHEAD1, eHEAD2, eRABBIT };
	void	Set_CustomHead(CLASS_TYPE eClassType, eCUSTOM_HEAD eHeadEnum);

	enum class eCUSTOM_BODY { eDEFAULT, eBODY1, eBODY2 };
	void	Set_CustomBody(CLASS_TYPE eClassType, eCUSTOM_BODY eBodyEnum);

	enum class eCUSTOM_WEAPON { eDEFAULT, eWEAPON1 };
	void	Set_CustomWeapon(CLASS_TYPE eClassType, eCUSTOM_WEAPON eWeaponEnum);

public:
	/* m_vecPrefClassType 안에서 랜덤으로 골라서 뱉는 함수 */
	CLASS_TYPE	Choose_Character();

public:
	virtual HRESULT	Initialize() PURE;
	virtual void Release();

public:
	/* 
		외부에서 AI설정을 변경하는 용도	
		AI툴에서 사용하기 위해 생성 
	*/
	void Set_Personality(CAIPersonality* pPersonality);

protected:
	/* GameObject들 포인터 */
	CPlayer* m_pMyPlayer = nullptr;
	CSquad* m_pMySquad = nullptr;
	CTeamConnector* m_pMyTeam = nullptr;

protected:
	/*AI들 전용 변수*/
	CAIPersonality* m_pPersonality = nullptr;


protected:
	eTEAM_TYPE	m_eTeamType = eTEAM_TYPE::eBLUE;
	CLASS_TYPE	m_eCurChosenClass = WARRIOR;

	CLASS_TYPE	m_eChangeHeroClass = FIONA;

protected:
	PLAYER_INFO	m_tPlayerInfo;
	_bool	m_bIsMainPlayer = false;

	_uint	m_iUnitType = 0;

protected:
	/* 이 안에 있는 클래스중 하나를 랜덤으로 선택해서 플레이 한다. */
	vector<CLASS_TYPE>		m_vecPrefClassType;

protected:
	/* 이 구조체에 담긴 경로로 Player 클래스와 그 안의 Unit 클래스들이 만들어짐 */
	PLAYER_SETUP_DATA	m_tPlayerSetUpData;

protected:
	/* Player 클래스 만들기 전에 세팅된 정보들이 유효한지 확인 */
	_bool	Can_Make_Player(const PLAYER_INFO& tInfo);

protected:
	virtual HRESULT	SetUp_AIPersonality();

private:
	void	Set_CustomHead_Warrior(eCUSTOM_HEAD eHeadEnum);
	void	Set_CustomBody_Warrior(eCUSTOM_BODY eBodyEnum);
	void	Set_CustomWeapon_Warrior(eCUSTOM_WEAPON eWeaponEnum);

	void	Set_CustomHead_Paladin(eCUSTOM_HEAD eHeadEnum);
	void	Set_CustomBody_Paladin(eCUSTOM_BODY eBodyEnum);
	void	Set_CustomWeapon_Paladin(eCUSTOM_WEAPON eWeaponEnum);

	void	Set_CustomHead_Arhcer(eCUSTOM_HEAD eHeadEnum);
	void	Set_CustomBody_Arhcer(eCUSTOM_BODY eBodyEnum);
	void	Set_CustomWeapon_Arhcer(eCUSTOM_WEAPON eWeaponEnum);

	void	Set_CustomHead_Engineer(eCUSTOM_HEAD eHeadEnum);
	void	Set_CustomBody_Engineer(eCUSTOM_BODY eBodyEnum);
	void	Set_CustomWeapon_Engineer(eCUSTOM_WEAPON eWeaponEnum);

	void	Set_CustomHead_Fiona(eCUSTOM_HEAD eHeadEnum);
	void	Set_CustomBody_Fiona(eCUSTOM_BODY eBodyEnum);
	void	Set_CustomWeapon_Fiona(eCUSTOM_WEAPON eWeaponEnum);

	void	Set_CustomHead_Qanda(eCUSTOM_HEAD eHeadEnum);
	void	Set_CustomBody_Qanda(eCUSTOM_BODY eBodyEnum);
	void	Set_CustomWeapon_Qanda(eCUSTOM_WEAPON eWeaponEnum);

	void	Set_CustomHead_Priest(eCUSTOM_HEAD eHeadEnum);
	void	Set_CustomBody_Priest(eCUSTOM_BODY eBodyEnum);
	void	Set_CustomWeapon_Priest(eCUSTOM_WEAPON eWeaponEnum);

	void	Set_CustomHead_Lancer(eCUSTOM_HEAD eHeadEnum);
	void	Set_CustomBody_Lancer(eCUSTOM_BODY eBodyEnum);
	void	Set_CustomWeapon_Lancer(eCUSTOM_WEAPON eWeaponEnum);

};

END