#pragma once
#include "CImGui_Window.h"


BEGIN(Client)
class CWindow_Shader final
	: public CImGui_Window
{
private:
	CWindow_Shader();
	virtual ~CWindow_Shader();

public:
	static CWindow_Shader* Create();

public:
	// CImGui_Window을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void Tick() override;
	virtual HRESULT Render() override;
};
END
