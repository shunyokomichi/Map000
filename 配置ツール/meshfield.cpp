//=======================================================//
//
// メッシュフィールド処理 [meshfield.cpp]
// Author : Shun Yokomichi
//
//=======================================================//

//*******************************************************//
// ヘッダファイルのインクルード
//*******************************************************//
#include "meshfield.h"	// メッシュフィールド
#include "manager.h"	// マネージャー
#include "renderer.h"	// レンダラー
#include "input.h"		// 入力
#include "camera.h"		// カメラ
#include "game.h"		// ゲーム

//*******************************************************//
// マクロ定義
//*******************************************************//
#define TEXTURE_NAME_1	"data\\TEXTURE\\Object\\yuka.jpg"	// テクスチャ

//*******************************************************//
// 静的メンバ変数
//*******************************************************//
LPDIRECT3DTEXTURE9	CMeshField::m_pTexture[MAX_TEX_FIELD] = {};	// テクスチャ情報へのポインタ
int					CMeshField::m_nNumAll = 0;		// 総合配置数の情報

//=======================================================//
//	読み込み処理(CMeshField)
//=======================================================//
HRESULT CMeshField::Load(int nMaxTex)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	for (int nCnttex = 0; nCnttex < nMaxTex; nCnttex++)
	{
		char *cModelName = {};					// モデル名保存用
		cModelName = CGame::GetName(nCnttex, 1);	// テキストから読み込んだモデル名の代入

		if (m_pTexture[nCnttex] == NULL)
		{
			// テクスチャ生成
			D3DXCreateTextureFromFile(pDevice, cModelName, &m_pTexture[nCnttex]);
		}
	}

	return S_OK;
}

//=======================================================//
// 開放処理(CMeshField)
//=======================================================//
void CMeshField::Unload(void)
{
	// テクスチャの破棄
	for (int nCnttex = 0; nCnttex < MAX_TEX_FIELD; nCnttex++)
	{
		if (m_pTexture[nCnttex] != NULL)
		{
			m_pTexture[nCnttex]->Release();
			m_pTexture[nCnttex] = NULL;
		}
	}
}

//=======================================================//
// 生成処理(CMeshField)
//=======================================================//
CMeshField *CMeshField::Create(MESHTYPE meshType, D3DXVECTOR3 num, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, MESHTEX meshTex, int nTex)
{
	// ポインタ生成
	CMeshField *pMeshField = NULL;

	// NULLの場合、シーン生成
	if (pMeshField == NULL) { pMeshField = new CMeshField; }

	if (pMeshField != NULL)
	{	// NULL以外の場合、初期化処理（位置、大きさ、色、UV）
		pMeshField->Init(meshType, num, pos, rot, size, col, TexUV, meshTex);
		pMeshField->BindTexture(m_pTexture[nTex]);	// テクスチャ設定
		pMeshField->m_nTex = nTex;
	}

	return pMeshField;
}

//=======================================================//
// 初期化処理(CMeshField)
//=======================================================//
HRESULT CMeshField::Init(MESHTYPE meshType, D3DXVECTOR3 num, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, MESHTEX meshTex)
{
	CSceneMesh::SetInitAll(meshType, num, pos, rot, size, col, D3DXVECTOR2(TexUV.x, TexUV.y), meshTex);
	CSceneMesh::Init();

	m_nNum = m_nNumAll;			// モデルの現在総合配置数の番号を代入(配置番号の記録)
	m_nNumAll++;				// 総合配置の加算

	return S_OK;
}