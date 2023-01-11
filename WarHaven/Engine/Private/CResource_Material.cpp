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

	if (!m_tMatDesc.TexturePaths[8].empty())
		m_tMatDesc.TexturePaths[1] = m_tMatDesc.TexturePaths[8];

	m_tMatDesc.vMixColor = Check_FileName(m_strName);
	

	return S_OK;
}

_float4 CResource_Material::Check_FileName(string strName)
{
	_int iFind = 0;
	_float4 vReturnColor = _float4(1.f, 1.f, 1.f, 1.f);

	//Model
	if (strName == "MI_CHR_Face")
	{
		vReturnColor = _float4(1.f, 0.9f, 0.8f, 1.f);
		//m_tMatDesc.TexturePaths[aiTextureType_DIFFUSE_ROUGHNESS] = "../bin/resources/textures/materials/T_Faceparts_base_coptes.dds";

	}
	/* Fiona */
	else if (strName == "MI_Fiona001_A00_HairSmall")
	{
		m_tMatDesc.TexturePaths[1] = "../bin/resources/textures/AnimModelTextures/T_Fiona_01_Hair_B.dds";
	}
	else if (strName == "MI_Fiona001_A00_HairLarge")
	{
		m_tMatDesc.TexturePaths[1] = "../bin/resources/textures/AnimModelTextures/T_Fiona_01_Hair_B.dds";
	}

	else if (strName == "MI_Fiona001_A00_HairLarge")
	{
		//vReturnColor = _float4(0.8f, 0.3f, 0.15f, 1.f);
		m_tMatDesc.TexturePaths[1] = "../bin/resources/textures/AnimModelTextures/T_Fiona_01_Hair_B.dds";

	}
	else if (strName == "MI_Fiona001_A00_HairBraid")
	{
		//vReturnColor = _float4(0.8f, 0.3f, 0.15f, 1.f);
		m_tMatDesc.TexturePaths[1] = "../bin/resources/textures/AnimModelTextures/T_Fiona_01_Hair_B.dds";

	}
	else if (strName == "MI_Fiona0001_HairProp_A00")
	{
		//vReturnColor = _float4(0.8f, 0.3f, 0.15f, 1.f);
		m_tMatDesc.TexturePaths[1] = "../bin/resources/textures/AnimModelTextures/T_Fiona_01_HairProp_B.dds";

	}
	else if (strName == "MI_Fiona_01_Helm")
	{
		//vReturnColor = _float4(0.8f, 0.3f, 0.15f, 1.f);
		m_tMatDesc.TexturePaths[1] = "../bin/resources/textures/AnimModelTextures/T_Fiona_01_helmet_B.dds";

	}
	else if (strName == "MI_Fiona_01_Cloth")
	{
		//vReturnColor = _float4(0.8f, 0.3f, 0.15f, 1.f);
		m_tMatDesc.TexturePaths[1] = "../bin/resources/textures/AnimModelTextures/T_Fiona_01_Body_B.dds";

	}
	else if (strName == "MI_Fiona_01_Cape")
	{
		//vReturnColor = _float4(0.8f, 0.3f, 0.15f, 1.f);
		m_tMatDesc.TexturePaths[1] = "../bin/resources/textures/AnimModelTextures/T_Fiona_01_Body_B.dds";

	}
	//else if (strName == "MI_Fiona_01_Face")
	//{
	//	//vReturnColor = _float4(0.8f, 0.3f, 0.15f, 1.f);
	//	m_tMatDesc.TexturePaths[1] = "../bin/resources/textures/AnimModelTextures/T_Fiona_01_Body_B.dds";

	//}

	/* Qanda */
	else if (strName == "MI_Hair_Qanda")
	{
		//vReturnColor = _float4(0.8f, 0.3f, 0.15f, 1.f);
		m_tMatDesc.TexturePaths[1] = "../bin/resources/textures/AnimModelTextures/T_Qanda_Hair01_B.dds";

	}
	else if (strName == "ML_Cane0001_A00")
	{
		//vReturnColor = _float4(0.8f, 0.3f, 0.15f, 1.f);
		m_tMatDesc.TexturePaths[1] = "../bin/resources/textures/AnimModelTextures/T_Cane0001_A00_A00_B.dds";

	}
	else if (strName == "ML_Cane0001_A00_02")
	{
		//vReturnColor = _float4(0.8f, 0.3f, 0.15f, 1.f);
		m_tMatDesc.TexturePaths[1] = "../bin/resources/textures/AnimModelTextures/T_Cane0001_A00_A00_B.dds";

	}
	else if (strName == "MI_EyeOcclusion_Qanda")
	{
		m_tMatDesc.TexturePaths[1] = "../bin/resources/textures/AnimModelTextures/T_Eyes_Qanda.dds";
		
	}
	

	//ML_Cane0001_A00

	/* Lancer */
	else if (strName == "MI_Lancer0001_Fur_Out_A00")
	{
		//vReturnColor = _float4(0.8f, 0.3f, 0.15f, 1.f);
		m_tMatDesc.TexturePaths[1] = "../bin/resources/textures/AnimModelTextures/T_Lancer0001_Horse_A00.dds";

	}
	else if (strName == "MI_Lancer0001_Fur_A00")
	{
		//vReturnColor = _float4(0.8f, 0.3f, 0.15f, 1.f);
		m_tMatDesc.TexturePaths[1] = "../bin/resources/textures/AnimModelTextures/T_Lancer0001_Horse_A00.dds";

	}
	else if (strName == "MI_Lance0000_Eye_A00")
	{
		vReturnColor = _float4(0.2f, 0.9f, 0.15f, 1.f);
		m_tMatDesc.TexturePaths[1] = "../bin/resources/textures/AnimModelTextures/T_Archer0001_eye_A00_B.dds";

	}
	else if (strName == "MI_Lance0000_Eyebrows_A00")
	{
		vReturnColor = _float4(0.1f, 1.f, 0.11f, 1.f);
		m_tMatDesc.TexturePaths[1] = "../bin/resources/textures/AnimModelTextures/T_Lance0000_Eyebrows_A00_M.dds";
	}

	else if (strName == "MI_Lance0000_Eyelash_A00")
	{
		//vReturnColor = _float4(0.8f, 0.3f, 0.15f, 1.f);
		m_tMatDesc.TexturePaths[1] = "../bin/resources/textures/AnimModelTextures/T_Warrior0001_Eyelashes_A00.dds";

	}


	else if (strName == "MI_Engineer0001_Eye_A00")
	{
	m_tMatDesc.TexturePaths[1] = "../bin/resources/textures/AnimModelTextures/T_EyeIris_DarkBrown_B.dds";
	}
	


	/* Warrior */
	else if (strName == "MI_Warrior0001_Beard02_A00")
	{
		vReturnColor = _float4(0.015625f, 0.00441f, 0.003061f, 1.f);

	}
	else if (strName == "MI_Warrior0001_Beard03_A00")
	{
		vReturnColor = _float4(0.015f, 0.148605f, 0.148438f, 1.f);

	}
	else if (strName == "MI_Warrior0001_Beard03_A00")
	{
		vReturnColor = _float4(0.005208f, 0.001217f, 0.000883f, 1.f);

	}
	else if (strName == "MI_Warrior0001_Eye_A00")
	{
		//vReturnColor = _float4(0.005208f, 0.001217f, 0.000883f, 1.f);
		m_tMatDesc.TexturePaths[1] = "../bin/resources/textures/AnimModelTextures/T_EyeIris_DarkBrown_B.dds";

	}
	else if (strName == "MI_Warrior0001_Eyebrow_A00")
	{
		vReturnColor = _float4(0.020833f, 0.007304f, 0.00651f, 1.f);
		m_tMatDesc.TexturePaths[1] = "../bin/resources/textures/AnimModelTextures/T_Warrior0001_Eyebrow_A00.dds";

	}
	else if (strName == "MI_Warrior0001_Eyelashes_A00")
	{
		vReturnColor = _float4(0.020833f, 0.007304f, 0.00651f, 1.f);
		m_tMatDesc.TexturePaths[1] = "../bin/resources/textures/AnimModelTextures/T_Warrior0001_Eyelashes_A00.dds";

	}
	else if (strName == "MI_Warrior0001_Face_A00")
	{
		//vReturnColor = _float4(0.020833f, 0.007304f, 0.00651f, 1.f);
		m_tMatDesc.TexturePaths[1] = "../bin/resources/textures/AnimModelTextures/T_Warrior0001_Face_A00_B.dds";
		m_tMatDesc.TexturePaths[aiTextureType_NORMALS] = "../bin/resources/textures/AnimModelTextures/T_Warrior0001_Face_A00_N.dds";

	}
	else if (strName == "MI_Face_Asian_01")
	{
		//vReturnColor = _float4(0.020833f, 0.007304f, 0.00651f, 1.f);
		m_tMatDesc.TexturePaths[1] = "../bin/resources/textures/AnimModelTextures/T_Male_Face_Asian_01_B.dds";
		m_tMatDesc.TexturePaths[aiTextureType_NORMALS] = "../bin/resources/textures/AnimModelTextures/T_Face_Asian_01_N.dds";

	}
	else if (strName == "MI_Eyelash_Male_Mid_Brown")
	{
		vReturnColor = _float4(0.002604f, 0.001848f, 0.001824f, 1.f);
		m_tMatDesc.TexturePaths[1] = "../bin/resources/textures/AnimModelTextures/T_Eyelash_01.dds";
		//m_tMatDesc.TexturePaths[aiTextureType_NORMALS] = "../bin/resources/textures/AnimModelTextures/T_Face_Asian_01_N.dds";

	}
	else if (strName == "MI_Eyelash_Fiona")
	{
		m_tMatDesc.TexturePaths[1] = "../bin/resources/textures/AnimModelTextures/T_Eyelash_01.dds";
	}
	else if (strName == "MI_Face_Fiona")
	{
		//vReturnColor = _float4(0.002604f, 0.001848f, 0.001824f, 1.f);
		m_tMatDesc.TexturePaths[1] = "../bin/resources/textures/AnimModelTextures/T_Face_Fiona_B.dds";
	}
	else if (m_tMatDesc.TexturePaths[1] == "../bin/resources/textures/AnimModelTextures/T_Fiona0004_Body_A00_B.dds")
	{
		//vReturnColor = _float4(0.002604f, 0.001848f, 0.001824f, 1.f);
		m_tMatDesc.TexturePaths[1] = "../bin/resources/textures/AnimModelTextures/T_Fiona0004_Body_A02_B.dds";

	}
	else if (m_tMatDesc.TexturePaths[1] == "../bin/resources/textures/AnimModelTextures/T_Fiona0004_Helm_A00_B.dds")
	{
		//vReturnColor = _float4(0.002604f, 0.001848f, 0.001824f, 1.f);
		m_tMatDesc.TexturePaths[1] = "../bin/resources/textures/AnimModelTextures/T_Fiona0004_Helm_A02_B.dds";

	}
	else if ((_int)(m_tMatDesc.TexturePaths[1].find("Weed")) > 0)
	{
		vReturnColor = 2.f;
	}


	/*else if ((_int)(m_tMatDesc.TexturePaths[1].find("Tree")) > 0)
	{
		vReturnColor = 2.f;
	}
	else if ((_int)(m_tMatDesc.TexturePaths[1].find("Grass")) > 0)
	{
		vReturnColor = 2.f;
	}
	else if ((_int)(m_tMatDesc.TexturePaths[1].find("Bush")) > 0)
	{
		vReturnColor = 2.f;
	}*/

	else if (m_tMatDesc.TexturePaths[1] == "../bin/resources/textures/AnimModelTextures/M_Eye_Occlusion")
	{
		m_tMatDesc.TexturePaths[1] = "../bin/resources/textures/ModelTextures/eye_sclera_right_clr.dds";
	}
	else if (m_tMatDesc.TexturePaths[1] == "../bin/resources/textures/AnimModelTextures/MI_Warrior0001_Eye_Occlusion_A00")
	{
		m_tMatDesc.TexturePaths[1] = "../bin/resources/textures/ModelTextures/eye_sclera_right_clr.dds";
	}

	//else if (m_tMatDesc.TexturePaths[1] == "../bin/resources/textures/AnimModelTextures/MI_Engineer0001_EyeOcc_A00")
	//{
	//	m_tMatDesc.TexturePaths[1] = "../bin/resources/textures/ModelTextures/eye_sclera_right_clr.dds";
	//}
	//else if (m_tMatDesc.TexturePaths[1] == "../bin/resources/textures/AnimModelTextures/MI_Engineer0000_Eyeocc_A00")
	//{
	//	m_tMatDesc.TexturePaths[1] = "../bin/resources/textures/ModelTextures/eye_sclera_right_clr.dds";
	//}


	else if (m_tMatDesc.TexturePaths[1] == "../bin/resources/textures/AnimModelTextures/MI_Priest0001_Eye_Occlusion_A00")
	{
	m_tMatDesc.TexturePaths[1] = "../bin/resources/textures/ModelTextures/T_Eyes_Priest.dds";
	}

	else if (m_tMatDesc.TexturePaths[1] == "../bin/resources/textures/AnimModelTextures/MI_EyeOcclusion_Fiona")
	{
	m_tMatDesc.TexturePaths[1] = "../bin/resources/textures/ModelTextures/eye_sclera_right_clr.dds";
	}
	else if (m_tMatDesc.TexturePaths[1] == "../bin/resources/textures/AnimModelTextures/MI_Eye_Fiona")
	{
	m_tMatDesc.TexturePaths[1] = "../bin/resources/textures/ModelTextures/eye_sclera_right_clr.dds";
	}
	else if (m_tMatDesc.TexturePaths[1] == "../bin/resources/textures/AnimModelTextures/MI_EyeOcclusion_Paladin")
	{
	m_tMatDesc.TexturePaths[1] = "../bin/resources/textures/ModelTextures/eye_sclera_right_clr.dds";
	}

	else if (m_tMatDesc.TexturePaths[1] == "../bin/resources/textures/AnimModelTextures/MI_Engineer0001_EyeOcc_A00")
	{
		m_tMatDesc.TexturePaths[1] = "../bin/resources/textures/ModelTextures/T_Eyes_Priest.dds";
	}
	else if (m_tMatDesc.TexturePaths[1] == "../bin/resources/textures/AnimModelTextures/MI_Engineer0000_Eyeocc_A00")
	{
		m_tMatDesc.TexturePaths[1] = "../bin/resources/textures/ModelTextures/T_Eyes_Priest.dds";
	}

	
	return vReturnColor;
}
