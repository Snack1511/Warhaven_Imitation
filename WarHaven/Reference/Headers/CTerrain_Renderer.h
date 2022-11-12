#pragma once
#include "Renderer.h"
class CTerrain_Renderer :
    public CRenderer
{
    DECLARE_PROTOTYPE(CTerrain_Renderer)

private:
    CTerrain_Renderer(_uint iGroupID);
    virtual ~CTerrain_Renderer();

public:
    static CTerrain_Renderer* Create(_uint iGroupID, const RENDER_GROUP& eRenderGroup,
        const _uint& iCurPass, const _float4& vOffsetPos = _float4(0.f, 0.f, 0.f, 1.f));
public:
    virtual HRESULT		Render();
private:
    //SHADOW_TYPE	m_eShadowType = NOSHADOW;
};

