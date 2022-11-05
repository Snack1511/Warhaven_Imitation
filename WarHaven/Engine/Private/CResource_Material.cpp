#include "CResource_Material.h"

#include "CUtility_File.h"

CResource_Material::CResource_Material()
{
}

CResource_Material::~CResource_Material()
{
}

CResource_Material* CResource_Material::Create(ifstream* pReadFile, string strName)
{
	CResource_Material* pResource = new CResource_Material;

	if (FAILED(pResource->Initialize(pReadFile, strName)))
	{
		Call_MsgBox(L"Failed to Initiailize : CResource_Material");
		SAFE_DELETE(pResource);
	}

	return pResource;
}

HRESULT CResource_Material::Initialize(ifstream* pReadFile, string strName)
{
	__super::Initialize(pReadFile, strName);

	for (_uint i = 0; i < AI_TEXTURE_TYPE_MAX; ++i)
	{
		m_tMatDesc.TexturePaths[i] = CUtility_File::Read_Text(pReadFile);
	}

	m_tMatDesc.vMixColor = Check_FileName(m_strName);

	return S_OK;
}

_float4 CResource_Material::Check_FileName(string strName)
{
	_float4 vReturnColor = _float4(1.f, 1.f, 1.f, 1.f);

	//Model
	if (strName == "MI_CHR_Face")
	{
		vReturnColor = _float4(1.f, 0.9f, 0.8f, 1.f);
		//m_tMatDesc.TexturePaths[aiTextureType_DIFFUSE_ROUGHNESS] = "../bin/resources/textures/materials/T_Faceparts_base_coptes.dds";

	}


	return vReturnColor;
}
