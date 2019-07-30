//=======================================================//
//
// シーン処理 [Scene.h]
// Author : Shun Yokomichi
//
//=======================================================//
#ifndef _SCENE_H_
#define _SCENE_H_

//*******************************************************//
// ヘッダファイルのインクルード
//*******************************************************//
#include "main.h"

//*******************************************************//
// クラス定義
//*******************************************************//
class CScene
{// CScene
public:
	typedef enum
	{	// 優先順位
		PRIORITY_BG = 0,	// 背景
		PRIORITY_1,			// 1
		PRIORITY_2,			// 2
		PRIORITY_3,			// 3
		PRIORITY_4,			// 4
		PRIORITY_5,			// 5
		PRIORITY_UI,		// UI
		PRIORITY_PAUSE,		// ポーズ
		PRIORITY_FADE,		// フェード
		PRIORITY_MAX		// 最大数
	}PRIORITY;

	typedef enum
	{// オブジェクトタイプ
		OBJTYPE_NONE = 0,		// 何もしていない
		OBJTYPE_MESHFIELD,		// メッシュフィールド
		OBJTYPE_MESHWALL,		// メッシュウォール
		OBJTYPE_MESHCYLINDER,	// メッシュシリンダー
		OBJTYPE_BG,				// 背景
		OBJTYPE_SAID,			// 表示
		OBJTYPE_ENEMY,			// 敵
		OBJTYPE_PLAYER,			// プレイヤー
		OBJTYPE_BULLET,			// 弾
		OBJTYPE_SELECTICON,		// 選択アイコン
		OBJTYPE_EFFECT,			// エフェクト
		OBJTYPE_ITEM,			// アイテム
		OBJTYPE_ITEMICON,		// アイテムアイコン
		OBJTYPE_ITEM3D,			// アイテム3D
		OBJTYPE_BLOCK,			// ブロック
		OBJTYPE_OBJECT3D,		// ビルボード
		OBJTYPE_UI,				// UI
		OBJTYPE_NUMBER,			// 数字
		OBJTYPE_SCORE,			// スコア
		OBJTYPE_RANKING,		// ランキング
		OBJTYPE_MENU,			// メニュー
		OBJTYPE_PAUSE,			// ポーズ
		OBJTYPE_TITLE,			// タイトル
		OBJTYPE_FADE,			// フェード
		OBJTYPE_CAMERA,			// カメラ
		OBJTYPE_RESULT,			// リザルト
		OBJTYPE_MAX				// 最大数
	}OBJTYPE;

	CScene(int nPriority, OBJTYPE objType);	// コンストラクタ
	virtual ~CScene() {};					// デストラクタ(中身無し)

	static void ReleaseMode(void);			// フェード以外破棄処理
	static void ReleaseAll(void);			// 全破棄処理
	static void UpdateAll(void);			// 全更新処理
	static void DrawAll(void);				// 全描画処理

	virtual HRESULT Init(void)	= 0;		// 初期化処理
	virtual void Uninit(void)	= 0;		// 終了処理
	virtual void Update(void)	= 0;		// 更新処理
	virtual void Draw(void)		= 0;		// 描画処理

	static int GetNumAll(void)				{ return m_nNumAll; }				// 総数取得
	static int GetNumPri(int nPriority)		{ return m_nNumPri[nPriority]; }	// プライオリティ総数取得

	void SetObjType(OBJTYPE objType)		{ m_objType = objType; }			// オブジェクトタイプ設定
	OBJTYPE GetObjType(void)				{ return m_objType; }				// オブジェクトタイプ取得

	static void SetStartUp(int nStartUp)	{ m_nStartUp = nStartUp; }			// 更新設定

	// オブジェクトタイプ計算用
	static CScene *GetScene(PRIORITY nPriority) { return m_pTop[nPriority]; }
	CScene *GetSceneNext(void)					{ return m_pNext; }

private:
	void Release(void);	// 開放処理
	char *ConObjtype(OBJTYPE objType);

	static CScene *m_pTop[PRIORITY_MAX];	// 先頭オブジェクトへのポインタ
	static CScene *m_pCur[PRIORITY_MAX];	// 現在オブジェクトへのポインタ

	CScene *m_pPrev;						// 前オブジェクトへのポインタ
	CScene *m_pNext;						// 次オブジェクトへのポインタ

	int		m_nPriority;					// 種類
	OBJTYPE m_objType;						// 種類
	bool	m_bDeath;						// 死亡フラグ

	static int m_nNumAll;					// 総数
	static int m_nNumPri[PRIORITY_MAX];		// プライオリティ総数
	static int m_nStartUp;					// 更新開始番号

protected:
	void SetDeath(void) { m_bDeath = true; }	// 破棄
};
#endif