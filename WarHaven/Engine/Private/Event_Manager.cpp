#include "..\Public\Event_Manager.h"

#include "GameObject.h"
#include "Component.h"
#include "Level.h"

#include "Object_Manager.h"
#include "Level_Manager.h"

IMPLEMENT_SINGLETON(CEvent_Manager)

CEvent_Manager::CEvent_Manager()
{
}

CEvent_Manager::~CEvent_Manager()
{
	m_vecDeadObjects.clear();
	m_vecDeadComponents.clear();
	m_vecEvent.clear();

}

void CEvent_Manager::Tick()
{
	for (size_t i = 0; i < m_vecDeadComponents.size(); ++i)
	{
		if (nullptr != m_vecDeadComponents[i])
			delete m_vecDeadComponents[i];
	}

	m_vecDeadComponents.clear();


	/* Delete all Dead_Object from prev Frame. */
	for (size_t i = 0; i < m_vecDeadObjects.size(); ++i)
	{
		if (nullptr != m_vecDeadObjects[i])
			m_vecDeadObjects[i]->Destroy_Instance();
	}

	m_vecDeadObjects.clear();


	

	for (size_t i = 0; i < m_vecEvent.size(); ++i)
	{
		Execute(m_vecEvent[i]);
	}
	m_vecEvent.clear();
	
}

void CEvent_Manager::Delete_GameObject(CGameObject * pGameObject)
{
	if (!pGameObject)
	{
		assert(0);
		return;
	}

	Add_Event(EVENT_DELETE_OBJECT, pGameObject);
}

void CEvent_Manager::Create_GameObject(CGameObject * pGameObject, const _uint& iGroupIdx)
{
	if (!pGameObject)
	{
		assert(0);
		return;
	}
	Add_Event(EVENT_CREATE_OBJECT, pGameObject, (DWORD_PTR)iGroupIdx);
}

void CEvent_Manager::Delete_Component(CComponent* pComponent, CGameObject* pGameObject)
{
	Add_Event(EVENT_DELETE_COMPONENT, pGameObject, 0, pComponent);
}

void CEvent_Manager::Create_Component(CComponent* pComponent, CGameObject* pGameObject)
{
	Add_Event(EVENT_CREATE_COMPONENT, pGameObject, 0, pComponent);
}

void CEvent_Manager::Enable_GameObject(CGameObject * pGameObject)
{
	if (!pGameObject)
	{
		assert(0);
		return;
	}
	Add_Event(EVENT_ENABLE_OBJECT, pGameObject);
}

void CEvent_Manager::Disable_GameObject(CGameObject * pGameObject)
{
	if (!pGameObject)
	{
		assert(0);
		return;
	}
	Add_Event(EVENT_DISABLE_OBJECT, pGameObject);
}

void CEvent_Manager::Disable_Component(CComponent* pComponent)
{
	Add_Event(EVENT_DISABLE_COMPONENT, nullptr, 0 ,pComponent);
}

void CEvent_Manager::Enable_Component(CComponent* pComponent)
{
	Add_Event(EVENT_ENABLE_COMPONENT, nullptr, 0, pComponent);
}

void CEvent_Manager::Create_StaticObject(CGameObject * pGameObject, _hashcode hcClassName)
{
	if (!pGameObject)
	{
		assert(0);
		return;
	}
	Add_Event(EVENT_CREATE_STATIC, pGameObject, (DWORD_PTR)hcClassName);

}

void CEvent_Manager::Change_Level(CLevel* pLevel)
{
	Add_Event(EVENT_CHANGE_LEVEL, nullptr, 0, nullptr, pLevel);
}

void CEvent_Manager::Clear_All_Event()
{
	m_vecEvent.clear();
	m_vecDeadObjects.clear();
	m_vecDeadComponents.clear();
}

void CEvent_Manager::Clear_Enable_Events()
{
	for (auto iter = m_vecEvent.begin(); iter != m_vecEvent.end();)
	{
		if ((*iter).eEven == EVENT_DISABLE_OBJECT || (*iter).eEven == EVENT_ENABLE_OBJECT)
		{
			iter = m_vecEvent.erase(iter);
		}
		else
			++iter;
	}
}

void CEvent_Manager::Add_Event(const EVENT_TYPE& eEven, CGameObject* _pGameObject, DWORD_PTR lParam , CComponent* _pComponent , CLevel* _pLevel )
{
	m_vecEvent.push_back(EVENT(eEven, lParam, _pGameObject, _pComponent, _pLevel));

	 _uint iSize = m_vecEvent.size();
	 int i = 0;
}

void CEvent_Manager::Execute(const EVENT & tEvent)
{
	switch (tEvent.eEven)
	{
	default:
		break;

	case EVENT_CREATE_OBJECT:
	{
		CGameObject* pGameObject = (tEvent.pGameObject);
		_uint iGroupIdx = (_uint)(tEvent.lParam);
		CObject_Manager::Get_Instance()->Add_Object(pGameObject, iGroupIdx);
		if (FAILED(pGameObject->Start()))
		{
			Call_MsgBox(L"Failed to Start Static GameObject : CEvent_Manager");
			return;
		}

	}
	break;

	case EVENT_CREATE_COMPONENT:
	{
		/*CComponent* pComponent = (CComponent*)(tEvent.lParam);
		CGameObject* pGameObject = (CGameObject*)(tEvent.wParam);
		pGameObject->Add_Component(pComponent);*/
		//pComponent->Start();
	}
	break;

	case EVENT_CREATE_STATIC:
	{
		CGameObject* pGameObject = tEvent.pGameObject;
		_hashcode	iObjectID =(_hashcode)(tEvent.lParam);
		CObject_Manager::Get_Instance()->Add_StaticObject(pGameObject, iObjectID);
		if (FAILED(pGameObject->Start()))
		{
			Call_MsgBox(L"Failed to Start Static GameObject : CEvent_Manager");
			return;
		}

	}
	break;

	case EVENT_DELETE_OBJECT:
	{
		CGameObject* pGameObject = tEvent.pGameObject;
		if (pGameObject->Is_Dead())
			return;

		pGameObject->Set_Dead();

		/* Prevent Overlapped pointer */
		/*for (auto& elem : m_vecDead)
		{
			if (elem == pGameObject)
				return;
		}*/

		m_vecDeadObjects.push_back(pGameObject);
	}
	break;

	case EVENT_DELETE_COMPONENT:
	{
		CComponent* pComponent = tEvent.pComponent;
		if (pComponent->Is_Dead())
			return;

		pComponent->Set_Dead();

		/* Prevent Overlapped pointer */
		/*for (auto& elem : m_vecDead)
		{
			if (elem == pGameObject)
				return;
		}*/

		m_vecDeadComponents.push_back(pComponent);
	}
	break;

	case EVENT_ENABLE_OBJECT:
	{
		CGameObject* pGameObject = tEvent.pGameObject;
		pGameObject->Set_Enable(true);
		pGameObject->Call_Enable();
	}

	break;

	case EVENT_ENABLE_COMPONENT:
	{
		CComponent* pComponent = tEvent.pComponent;
		pComponent->Set_Enable(true);
	}

	break;

	case EVENT_DISABLE_OBJECT:
	{
		CGameObject* pGameObject = tEvent.pGameObject;
		pGameObject->Set_Enable(false);
		pGameObject->Call_Disable();

	}
		break;

	case EVENT_DISABLE_COMPONENT:
	{
		CComponent* pComponent = tEvent.pComponent;
		pComponent->Set_Enable(false);
	}

	break;

	case EVENT_CHANGE_LEVEL:
	{
		CLevel*	pLevel = tEvent.pLevel;
		CLevel_Manager::Get_Instance()->Enter_Level(pLevel);
	}
	break;

	}
}
