#pragma once
#include "CImGui_Window.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)
class CWindow_PhysX final
	: public CImGui_Window
{
private:
	CWindow_PhysX();
	virtual ~CWindow_PhysX();

public:
	static CWindow_PhysX* Create();

public:
	// CImGui_Window을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void Tick() override;
	virtual HRESULT Render() override;

private:
	list<CGameObject*>	m_GameObjects;
	CGameObject* m_pCurGameObject = nullptr;

private:
	void	Create_TestObject();
};
END
