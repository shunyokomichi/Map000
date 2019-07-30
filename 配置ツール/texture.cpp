//**************************************************************
//
//		�e�N�X�`���̏���[texture.cpp]
//		Auther : Shun Yokomichi
//
//**************************************************************
#include "texture.h"
#include "manager.h"		// �}�l�[�W���[
#include "renderer.h"		// �����_���[

//**************************************************************
//			�}�N����`
//**************************************************************

//**************************************************************
//		�ÓI�����o�ϐ�
//**************************************************************
LPDIRECT3DTEXTURE9 *CTexture::m_apTexture = NULL;		// Texture�̃|�C���^
const char *CTexture::m_apFilename[TYPE_MAX] =
{
	"data/TEXTURE/SelectIcon/icon.png",
	"data/TEXTURE/SelectIcon/save.png"
};

//**************************************************************
//		�R���X�g���N�^
//**************************************************************
CTexture::CTexture() {}

//**************************************************************
//		�f�X�g���N�^
//**************************************************************
CTexture::~CTexture() {}

//**************************************************************
//		�e�N�X�`���̓ǂݍ���
//**************************************************************
HRESULT CTexture::Load(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	m_apTexture = new LPDIRECT3DTEXTURE9[TYPE_MAX];

	for (int nCntTex = 0; nCntTex < TYPE_MAX; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice, m_apFilename[nCntTex], &m_apTexture[nCntTex]);
	}
	return S_OK;
}

//**************************************************************
//		�e�N�X�`���̊J��
//**************************************************************
void CTexture::Unload(void)
{
	//�e�N�X�`���̔j��
	for (int nCntTex = 0; nCntTex < TYPE_MAX; nCntTex++)
	{
		if (m_apTexture[nCntTex] != NULL)
		{	// NULL�ȊO�Ȃ�
			m_apTexture[nCntTex]->Release();		// �j��
			m_apTexture[nCntTex] = NULL;			// NULL��
		}
	}

	if (m_apTexture != NULL)
	{
		delete m_apTexture;
		m_apTexture = NULL;
	}

}