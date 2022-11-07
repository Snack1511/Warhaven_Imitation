#pragma once
#include "CImGui_Window.h"

BEGIN(Engine)
class CUI_Object;
END

BEGIN(Client)

class CWindow_UI : public CImGui_Window
{
private:
	CWindow_UI() = default;
	virtual ~CWindow_UI() = default;

public:
	static CWindow_UI* Create();

public:
	// CImGui_Window을(를) 통해 상속됨
	virtual HRESULT Initialize() override;
	virtual void Tick() override;
	virtual HRESULT Render() override;

private:
	void Read_Folder(const char* pFilePath);
};

END