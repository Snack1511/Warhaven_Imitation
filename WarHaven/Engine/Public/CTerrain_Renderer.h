#pragma once
#include "Renderer.h"
BEGIN(Engine)

class CShader;
class CMesh;
class CTexture;

class ENGINE_DLL CTerrain_Renderer :
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
public:
    void Clear_TextureList();
    void Add_Texture(CTexture* pTexture);
    void Update_TextureIndex(_int Sour, _int Dest);
    void Update_BackGround(_int BGIndex);
private:
    //SHADOW_TYPE	m_eShadowType = NOSHADOW;
    ID3D11ShaderResourceView** m_pArrSRVs = nullptr;
    list<ID3D11ShaderResourceView*> m_SRVList;

    _int m_TextureNums = 0;
    _bool m_bSRVUpdateFlag = true;
    _float4* m_pMeshColorFlag = nullptr;
    _int m_iVertNums = 0;
};

END