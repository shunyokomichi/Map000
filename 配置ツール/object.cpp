//=============================================================================
//
// オブジェクト処理 [object.cpp]
// Author : Shun Yokomichi
//
//=============================================================================
#include "object.h"			// オブジェクト
#include "manager.h"		// マネージャー
#include "renderer.h"		// レンダラー
#include "input.h"			// 入力
#include "game.h"			//　ゲーム

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		*CObject::m_pTexture[MAX_MODEL] = {};		// テクスチャ情報へのポインタ
LPD3DXMESH				CObject::m_pMesh[MAX_MODEL] = {};			// メッシュ情報へのポインタ
LPD3DXBUFFER			CObject::m_pBuffMat[MAX_MODEL] = {};		// マテリアル情報へのポインタ
DWORD					CObject::m_nNumMat[MAX_MODEL] = {};		// マテリアル情報の数
int						CObject::m_nNumAll = 0;						// 総合配置数の情報

//*****************************************************************************
// コンストラクタ									
//*****************************************************************************
CObject::CObject() : CSceneX(PRIORITY_3, CScene::OBJTYPE_BLOCK)
{
	// デフォルト
	m_nNum = 0;		// 総合配置数
}

//*****************************************************************************
// デストラクタ										(public)	*** CBlock ***
//*****************************************************************************
CObject::~CObject()
{

}

//*****************************************************************************
// ロード処理							(public)	*** CBullet ***
//*****************************************************************************
HRESULT CObject::Load(int MaxModel)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		// デバイス取得

	for (int nCntModel = 0; nCntModel < MaxModel; nCntModel++)
	{	// テキストに書いてあるモデルの数だけ読み込み
		char *cModelName = {};					// モデル名保存用
		cModelName = CGame::GetName(nCntModel, 0);	// テキストから読み込んだモデル名の代入

		// Xファイルの読み込み
		D3DXLoadMeshFromX(cModelName, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[nCntModel], NULL, &m_nNumMat[nCntModel], &m_pMesh[nCntModel]);

		D3DXMATERIAL *pmat;							// マテリアルデータへのポインタ
		D3DMATERIAL9 *matDef;						// 現在のマテリアル保存用
		pmat = (D3DXMATERIAL*)m_pBuffMat[nCntModel]->GetBufferPointer();
		matDef = new D3DMATERIAL9[m_nNumMat[nCntModel]];
		m_pTexture[nCntModel] = new LPDIRECT3DTEXTURE9[m_nNumMat[nCntModel]];

		for (DWORD tex = 0; tex < m_nNumMat[nCntModel]; tex++)
		{	// Texture使っている数分回していく
			matDef[tex] = pmat[tex].MatD3D;
			matDef[tex].Ambient = matDef[tex].Diffuse;
			m_pTexture[nCntModel][tex] = NULL;
			if (pmat[tex].pTextureFilename != NULL &&
				lstrlen(pmat[tex].pTextureFilename) > 0)
			{// テクスチャを使用している
				if (FAILED(D3DXCreateTextureFromFile(pDevice,
					pmat[tex].pTextureFilename,
					&m_pTexture[nCntModel][tex])))
				{
					//MessageBox(NULL, "テクスチャの読み込みに失敗しました", NULL, MB_OK);
				}
			}
		}

		delete[] matDef;
		matDef = NULL;
	}
	return S_OK;
}

//*****************************************************************************
// アンロード処理								(public)	*** CBullet ***
//*****************************************************************************
void CObject::Unload(void)
{
	for (int nCntParnet = 0; nCntParnet < MAX_MODEL; nCntParnet++)
	{
		// テクスチャの破棄
		for (DWORD tex = 0; tex < m_nNumMat[nCntParnet]; tex++)
		{	// 使っているTextureの枚数回す
			if (m_pTexture[nCntParnet][tex] != NULL)
			{	// NULL以外の場合
				m_pTexture[nCntParnet][tex]->Release();		// 解放
				m_pTexture[nCntParnet][tex] = NULL;			// NULLへ
			}
		}

		delete[] m_pTexture[nCntParnet];					// Textureの削除
		m_pTexture[nCntParnet] = NULL;						// NULLに

		// マテリアルの開放
		if (m_pBuffMat[nCntParnet] != NULL)
		{// NULL以外の場合
			m_pBuffMat[nCntParnet]->Release();				// 解放
			m_pBuffMat[nCntParnet] = NULL;					// NULLへ
		}

		// マテリアルの開放
		if (m_pMesh[nCntParnet] != NULL)
		{// NULL以外の場合
			m_pMesh[nCntParnet]->Release();					// 解放
			m_pMesh[nCntParnet] = NULL;						// NULLへ
		}

		m_nNumMat[nCntParnet] = 0;							// 初期化
	}
}

//*****************************************************************************
// 確保処理											(public)	*** CBlock ***
//*****************************************************************************
CObject *CObject::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int objType)
{
	CObject *pObject = NULL;			// ポインタ

	if (pObject == NULL)
	{	// NULLの場合
		pObject = new CObject;			// メモリ確保

		if (pObject != NULL)
		{// NULL以外の場合
			pObject->Init(pos, rot, objType);		// 初期化処理
			pObject->SetType(XTYPE::TYPE_NORMAL);	// 配置するモデルのタイプ
		}
	}

	return pObject;			// 値を返す
}

//*****************************************************************************
// 初期化処理										(public)	*** CBlock ***
//*****************************************************************************
HRESULT CObject::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int objType)
{
	// モデル全体をまとめるcppの初期化
	CSceneX::SetInitAll(m_pTexture[objType], m_pMesh[objType], m_pBuffMat[objType], m_nNumMat[objType], pos, rot);
	CSceneX::Init();

	// 位置と向きの設定
	CSceneX::Setpos(pos);
	CSceneX::Setrot(rot);

	// モデル関係
	m_nTypeNum = objType;		// モデルの種類代入
	m_nNum = m_nNumAll;			// モデルの現在総合配置数の番号を代入(配置番号の記録)
	m_nNumAll++;				// 総合配置の加算

	return S_OK;
}

//*****************************************************************************
// 終了処理											(public)	*** CBlock ***
//*****************************************************************************
void CObject::Uninit(void)
{
	// 総合配置の減算
	m_nNumAll--;

	// モデルの終了処理
	CSceneX::Uninit();
}

//*****************************************************************************
// 更新処理											(public)	*** CBlock ***
//*****************************************************************************
void CObject::Update(void)
{
	CSceneX::Update();		// モデルの更新処理
}

//*****************************************************************************
// 描画処理											(public)	*** CBlock ***
//*****************************************************************************
void CObject::Draw(void)
{
	CSceneX::Draw();		// モデルの描画処理
}
