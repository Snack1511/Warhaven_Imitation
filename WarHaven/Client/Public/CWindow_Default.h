#pragma once
#include "CImGui_Window.h"


BEGIN(Client)
class CWindow_Default final
	: public CImGui_Window
{
private:
	CWindow_Default();
	virtual ~CWindow_Default();

public:
	static CWindow_Default* Create();

public:
	// CImGui_Window��(��) ���� ��ӵ�
	virtual HRESULT Initialize() override;
	virtual void Tick() override;
	virtual HRESULT Render() override;
};
END
