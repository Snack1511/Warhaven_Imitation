#pragma once
#include "Client_Defines.h"

BEGIN(Client)

class CState;

class CState_Manager
{
	DECLARE_SINGLETON(CState_Manager);
	

private:
	CState_Manager();
	~CState_Manager();

public:
	CState* Get_State(STATE_TYPE eType)
	{
		if (eType >= STATE_END)
			return nullptr;

		return m_arrStates[eType];
	}

public:
	HRESULT	Initialize();

private:
	void Warrior_State();
	void WarHammer_State();
	void Archer_State();
	void Paladin_State();

	void Valkyrie_State();
	
	void Warrior_SandBagState();
	
	
	void Warrior_State_AI();
	void WarHammer_State_AI();
	void Valkyrie_State_AI();
	void Paladin_State_AI();
	void Archer_State_AI();

	void Spear_State();



private:
	CState* m_arrStates[STATE_END] = {};

private:
	/*For Warrior*/
	//void Warrior_Base_R();
	//void Warrior_Base_L();

	//void Warrior_Base_Attack_R();
	//void Warrior_Base_Attack_L();
	//void Warrior_Base_Attack_Default();

	//void Warrior_Base_Hit();
	//void Warrior_Base_ETC();

	//void Warrior_Others();

};
END

