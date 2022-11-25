#pragma once
#include "CImGui_Window.h"


BEGIN(Client)
class CWindow_Level final
	: public CImGui_Window
{
private:
	CWindow_Level();
	virtual ~CWindow_Level();

public:
	static CWindow_Level* Create();

public:
	// CImGui_Window��(��) ���� ��ӵ�
	virtual HRESULT Initialize() override;
	virtual void Tick() override;
	virtual HRESULT Render() override;
};
END
