#pragma once
#include "CImGui_Window.h"


BEGIN(Client)
class CWindow_Select final
	: public CImGui_Window
{
private:
	CWindow_Select();
	virtual ~CWindow_Select();

public:
	static CWindow_Select* Create();

public:
	// CImGui_Window��(��) ���� ��ӵ�
	virtual HRESULT Initialize() override;
	virtual void Tick() override;
	virtual HRESULT Render() override;
};
END
