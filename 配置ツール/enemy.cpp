//=======================================================//
//
//	敵処理 [enemy.cpp]
// Author : Shun Yokomichi
//
//=======================================================//

//*******************************************************//
// ヘッダファイルのインクルード
//*******************************************************//
#include "enemy.h"			// 敵
#include "manager.h"		// マネージャー
#include "renderer.h"		// レンダラー
#include "sound.h"			// サウンド
#include "game.h"			// ゲーム
#include "input.h"			// 入力
#include "camera.h"			// カメラ
#include "meshField.h"		// メッシュフィールド
#include "shadow.h"			// 影
#include "input.h"			// 入力
#include "object.h"			// オブジェクト

//*******************************************************//
// マクロ定義
//*******************************************************//
#define MODEL_NAME_0	"data\\MODEL\\Enemy\\enemy000.x"	// 敵モデル(ちょうちんのお化け)
#define MODEL_NAME_1	"data\\MODEL\\Enemy\\enemy001.x"	// 敵モデル(傘のお化け)
#define KNOCKBACK		(30.0f)								// ダメージ時のノックバック距離
#define AREA			(400.0f)							// エリア
#define NORMAL_MOVE		(1.0f)								// 通常移動速度
#define CHASE_MOVE		(1.5f)								// 追尾移動速度
#define MAX_INVINCIBLE_TIME		(30)						// 無敵時の時間
#define MAX_DAMAGE_TIME			(30)						// ダメージ時の時間
#define MOVE					(0.1f)						// 移動速度

//*******************************************************//
// 静的メンバ変数
//*******************************************************//
LPDIRECT3DTEXTURE9		*CEnemy::m_pTexture[MAX_ENEMY] = {};	// テクスチャ情報へのポインタ
LPD3DXMESH				CEnemy::m_pMesh[MAX_ENEMY] = {};		// メッシュ情報へのポインタ
LPD3DXBUFFER			CEnemy::m_pBuffMat[MAX_ENEMY] = {};		// マテリアル情報へのポインタ
DWORD					CEnemy::m_nNumMat[MAX_ENEMY] = {};		// マテリアル情報の数
int						CEnemy::m_nNumAll = 0;						// 総合配置数の情報

//=======================================================//
// コンストラクタ
//=======================================================//
CEnemy::CEnemy() : CSceneX(CScene::PRIORITY_3, CScene::OBJTYPE_ENEMY)
{
	//	デフォルト値に
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置情報(前の位置)保管
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 移動量変数
}

//=======================================================//
// デストラクタ
//=======================================================//
CEnemy::~CEnemy()
{

}

//=======================================================//
// ロード処理
//=======================================================//
HRESULT CEnemy::Load(int MaxModel)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		// デバイス取得

	for (int nCntModel = 0; nCntModel < MaxModel; nCntModel++)
	{	// テキストに書いてあるモデルの数だけ読み込み
		char *cModelName = {};					// モデル名保存用
		cModelName = CGame::GetName(nCntModel, 2);	// テキストから読み込んだモデル名の代入

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

//=======================================================//
// アンロード処理
//=======================================================//
void CEnemy::Unload(void)
{
	for (int nCntModel = 0; nCntModel < 2; nCntModel++)
	{
		for (DWORD tex = 0; tex < m_nNumMat[nCntModel]; tex++)
		{// カウント
			if (m_pTexture[nCntModel][tex] != NULL)
			{// NULL以外の場合
				m_pTexture[nCntModel][tex]->Release();		// 解放
				m_pTexture[nCntModel][tex] = NULL;			// NULLへ
			}
		}
		// マテリアルの開放
		if (m_pBuffMat[nCntModel] != NULL)
		{// NULL以外の場合
			m_pBuffMat[nCntModel]->Release();				// 削除
			m_pBuffMat[nCntModel] = NULL;					// NULLへ
		}

		// マテリアルの開放
		if (m_pMesh[nCntModel] != NULL)
		{// NULL以外の場合
			m_pMesh[nCntModel]->Release();					// 解放
			m_pMesh[nCntModel] = NULL;						// NULLへ
		}

		m_nNumMat[nCntModel] = 0;				// 初期化
	}
}

//=======================================================//
// 確保処理
//=======================================================//
CEnemy *CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	CEnemy *pEnemy = NULL;			// ポインタ

	if (pEnemy == NULL)
	{// NULLの場合
		pEnemy = new CEnemy;			// メモリ確保

		if (pEnemy != NULL)
		{// NULL以外の場合
			pEnemy->Init(pos, rot, nType);		// 初期化処理
		}
	}

	return pEnemy;			// 値を返す
}

//=======================================================//
// 初期化処理
//=======================================================//
HRESULT CEnemy::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	// CSceneXの初期化へ
	CSceneX::SetInitAll(m_pTexture[nType], m_pMesh[nType], m_pBuffMat[nType], m_nNumMat[nType], pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	CSceneX::Init();
	
	// CSceneXに位置と向きを設定
	CSceneX::Setpos(pos);
	CSceneX::Setrot(rot);

	// モデル関係
	m_nTypeNum = nType;		// モデルの種類代入
	m_nNum = m_nNumAll;			// モデルの現在総合配置数の番号を代入(配置番号の記録)
	m_nNumAll++;				// 総合配置の加算


	return S_OK;
}

//=======================================================//
// 終了処理
//=======================================================//
void CEnemy::Uninit(void)
{
	m_nNumAll--;				// 総合配置の加算

	CSceneX::Uninit();	// 終了処理へ
}

//=======================================================//
// 更新処理
//=======================================================//
void CEnemy::Update(void)
{

}

//=======================================================//
// 描画処理
//=======================================================//
void CEnemy::Draw(void)
{
	// Xモデルの描画処理 & 影の設定
	CSceneX::Draw();
}