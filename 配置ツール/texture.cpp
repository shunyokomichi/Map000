//**************************************************************
//
//		テクスチャの処理[texture.cpp]
//		Auther : Shun Yokomichi
//
//**************************************************************
#include "texture.h"
#include "manager.h"		// マネージャー
#include "renderer.h"		// レンダラー

//**************************************************************
//			マクロ定義
//**************************************************************

//**************************************************************
//		静的メンバ変数
//**************************************************************
LPDIRECT3DTEXTURE9 *CTexture::m_apTexture = NULL;		// Textureのポインタ
const char *CTexture::m_apFilename[TYPE_MAX] =
{
	"data/TEXTURE/SelectIcon/icon.png",
	"data/TEXTURE/SelectIcon/save.png"
};

//**************************************************************
//		コンストラクタ
//**************************************************************
CTexture::CTexture() {}

//**************************************************************
//		デストラクタ
//**************************************************************
CTexture::~CTexture() {}

//**************************************************************
//		テクスチャの読み込み
//**************************************************************
HRESULT CTexture::Load(void)
{
	// デバイスの取得
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
//		テクスチャの開放
//**************************************************************
void CTexture::Unload(void)
{
	//テクスチャの破棄
	for (int nCntTex = 0; nCntTex < TYPE_MAX; nCntTex++)
	{
		if (m_apTexture[nCntTex] != NULL)
		{	// NULL以外なら
			m_apTexture[nCntTex]->Release();		// 破棄
			m_apTexture[nCntTex] = NULL;			// NULLに
		}
	}

	if (m_apTexture != NULL)
	{
		delete m_apTexture;
		m_apTexture = NULL;
	}

}