//=======================================================//
//
// 敵処理 [enemy.h]
// Author : Shun Yokomichi
//
//=======================================================//
#ifndef _ENEMY_H_
#define _ENEMY_H_

//*******************************************************//
// ヘッダファイルのインクルード
//*******************************************************//
#include "main.h"		// メイン
#include "scenex.h"		// シーンx

//*******************************************************//
// マクロ定義
//*******************************************************//
#define MAX_ENEMY		(2)		//敵の最大数

//*******************************************************//
// 前方宣言
//*******************************************************//

//*******************************************************//
// クラス定義
//*******************************************************//
class CEnemy : public CSceneX
{// ブロック親：CSceneX）
public:
	typedef enum
	{
		STATE_NORMAL = 0,		// 通常
		STATE_DAMAGE,			// ダメージ
		STATE_INVINCIBLE,		// 無敵
		STATE_MAX				// 総数
	}STATE;

	CEnemy();										// コンストラクタ
	~CEnemy();										// デストラクタ

	static HRESULT Load(int MaxModel);						// ロード
	static void Unload(void);						// アンロード
	static CEnemy *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);		// 生成

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);			// 初期化処理
	void Uninit(void);										// 終了処理
	void Update(void);										// 更新処理
	void Draw(void);										// 描画処理

	static int GetNum() { return m_nNumAll; }			// モデルの総合数の取得
	int GetType() { return m_nTypeNum; }				// モデルのタイプの取得
	int GetModelNum() { return m_nNum; }				// モデルの配置番号の取得
	void SetNum(int nNum) { m_nNum = nNum; }			// モデルの番号設定


private:
	static LPDIRECT3DTEXTURE9	*m_pTexture[MAX_ENEMY];	// テクスチャ情報へのポインタ
	static LPD3DXMESH			m_pMesh[MAX_ENEMY];		// メッシュ情報へのポインタ
	static LPD3DXBUFFER			m_pBuffMat[MAX_ENEMY];	// マテリアル情報へのポインタ
	static DWORD				m_nNumMat[MAX_ENEMY];	// マテリアル情報の数

	D3DXVECTOR3					m_posOld;			// (前の)位置
	D3DXVECTOR3					m_move;				// 移動量

	static int m_nNumAll;			// 総合配置数
	int m_nTypeNum;					// タイプ番号
	int m_nNum;						// 配置数


protected:

};

#endif