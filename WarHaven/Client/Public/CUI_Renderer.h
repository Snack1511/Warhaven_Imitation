#pragma once
#include "Renderer.h"

BEGIN(Engine)

class ENGINE_DLL CUI_Renderer : public CRenderer
{
	DECLARE_PROTOTYPE(CUI_Renderer)

private:
	CUI_Renderer(_uint iGroupID);
	virtual ~CUI_Renderer();

public:
	static CUI_Renderer* Create(_uint iGroupID, const RENDER_GROUP& eRenderGroup,
		const _uint& iCurPass, const _float4& vOffsetPos = _float4(0.f, 0.f, 0.f, 1.f));

public:
	virtual HRESULT		Initialize_Prototype() override;
	virtual HRESULT		Initialize() override;
	virtual void		Start() override;
	virtual void		Tick() override;
	virtual void		Late_Tick() override;
	virtual HRESULT		Render();
	virtual void		Release() override;
};

END