//=======================================================//
//
// Scene処理 [Scene.cpp]
// Author : Shun Yokomichi
//
//=======================================================//

//*******************************************************//
// ヘッダファイルのインクルード
//*******************************************************//
#include "scene.h"		// シーン
#include "manager.h"	// マネージャー
#include "renderer.h"	// レンダラー
#include "input.h"		// 入力

//*******************************************************//
// 静的メンバ変数
//*******************************************************//
CScene	*CScene::m_pTop[PRIORITY_MAX]	= {};	// 先頭オブジェクトへのポインタ
CScene	*CScene::m_pCur[PRIORITY_MAX]	= {};	// 現在オブジェクトへのポインタ

int		CScene::m_nNumAll				= 0;	// 総数
int		CScene::m_nNumPri[PRIORITY_MAX] = {};	// 総数
int		CScene::m_nStartUp				= 0;	// 更新範囲

//=======================================================//
// コンストラクタ(Scene)
//=======================================================//
CScene::CScene(int nPriority, OBJTYPE objType)
{
	m_pPrev = m_pCur[nPriority];	// 前オブジェクト設定
	m_pNext = NULL;					// 次オブジェクト設定

	m_pCur[nPriority] = this;		// 現在のオブジェクト設定

	if (m_pPrev != NULL)
	{	// 前のオブジェクトがNULL以外
		m_pPrev->m_pNext = m_pCur[nPriority];	// 前の次オブジェクト設定
	}

	m_nPriority = nPriority;	// 順位設定
	m_objType = objType;		// 種類
	m_bDeath = false;			// 死亡フラグ
	m_nNumAll++;				// 総数カウント
	m_nNumPri[nPriority]++;		// 総数カウント

	// 先頭オブジェクトがNULL以外
	if (m_pTop[nPriority] == NULL)								{ m_pTop[nPriority] = m_pCur[nPriority]; }			// 先頭オブジェクト設定

	// 先頭オブジェクトと前のオブジェクトが同じ
	else if (m_pTop[nPriority] == m_pCur[nPriority]->m_pPrev)	{ m_pTop[nPriority]->m_pNext = m_pCur[nPriority]; }	// 先頭の次オブジェクト設定

	char *pObjName	= ConObjtype(objType);
	int nLenght		= (int)strlen(pObjName);
	char cObjName[32];
	for (int nCnt = 0; nCnt < nLenght; nCnt++)
	{
		cObjName[nCnt] = pObjName[nCnt];
	}
	cObjName[nLenght] = '\0';

}

//=======================================================//
// 全フェード以外開放処理(Scene)
//=======================================================//
void CScene::ReleaseMode(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_FADE; nCntPriority++)
	{	// カウント
		while (m_pTop[nCntPriority] != NULL)
		{	// 先頭オブジェクトがNULLになるまで
			m_pTop[nCntPriority]->Uninit();	// 先頭オブジェクト終了
			m_pTop[nCntPriority]->Release();// 開放処理
		}
		m_nNumPri[nCntPriority] = 0;		// プライオリティ総数初期化
	}
	m_nNumAll = 0;							// 総数初期化
}

//=======================================================//
// 全開放処理(Scene)
//=======================================================//
void CScene::ReleaseAll(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{	// カウント
		while (m_pTop[nCntPriority] != NULL)
		{	// 先頭オブジェクトがNULLになるまで
			m_pTop[nCntPriority]->Uninit();	// 先頭のオブジェクト終了
			m_pTop[nCntPriority]->Release();// 開放処理
		}
		m_nNumPri[nCntPriority] = 0;		// プライオリティ総数初期化
	}
	m_nNumAll = 0;							// 総数初期化
}

//=======================================================//
// 全更新処理(Scene)
//=======================================================//
void CScene::UpdateAll(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CScene *pScene	= m_pTop[nCntPriority];						// 更新オブジェクト（先頭を記憶）
		CScene *pNext	= NULL;										// 次オブジェクト記憶

		while (pScene != NULL)
		{	// 更新オブジェクトがNULL以外
			pNext = pScene->m_pNext;								// 次オブジェクト設定

			// 破棄しない
			if (pScene->m_bDeath == false) { pScene->Update(); }	// 更新処理

			// 破棄する
			if (pScene->m_bDeath == true) { pScene->Release(); }	// 開放処理

			pScene = pNext;											// 更新オブジェクトに次オブジェクト設定
		}
	}
}

//=======================================================//
// 全描画処理(Scene)
//=======================================================//
void CScene::DrawAll(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{	// カウント
		CScene *pScene	= m_pTop[nCntPriority];						// 描画オブジェクト（先頭を記憶）
		CScene *pNext	= NULL;										// 次オブジェクト記憶

		while (pScene != NULL)
		{	// 描画オブジェクトがNULL以外
			pNext = pScene->m_pNext;								// 次オブジェクト設定

			// 破棄しない
			if (pScene->m_bDeath == false)	{ pScene->Draw(); }		// 描画処理

			// 破棄する
			if (pScene->m_bDeath == true)	{ pScene->Release(); }	// 開放処理

			pScene = pNext;											// 描画オブジェクトに次オブジェクト設定
		}
	}
}

//=======================================================//
// 開放処理(Scene)
//=======================================================//
void CScene::Release(void)
{
	if (this != NULL)
	{	// 自分がNULL以外
		int nPriority	= m_nPriority;
		OBJTYPE objType = m_objType;

		if (m_pPrev != NULL)	// m_pTop以外
		{	// 前オブジェクトがNULL以外
			if (m_pTop[nPriority] == m_pPrev)
			{	// 先頭オブジェクトが前オブジェクトと同じ
				m_pTop[nPriority]->m_pNext = m_pNext;	// 先頭の次オブジェクト設定
			}
			m_pPrev->m_pNext = m_pNext;					// 前の次オブジェクト設定
		}

		if (m_pNext != NULL)	// m_pCur以外
		{	// 次オブジェクトがNULL以外
			if (m_pCur[nPriority] == m_pNext)
			{	// 現在オブジェクトが前オブジェクトと同じ
				m_pCur[nPriority]->m_pPrev = m_pPrev;	// 現在の前オブジェクト設定
			}
			m_pNext->m_pPrev = m_pPrev;					// 次の前オブジェクト設定
		}

		// 先頭オブジェクトが自分と同じ
		if (m_pTop[nPriority] == this) { m_pTop[nPriority] = m_pNext; }	// 先頭オブジェクトを次に更新

		// 現在オブジェクトが自分と同じ
		if (m_pCur[nPriority] == this) { m_pCur[nPriority] = m_pPrev; }	// 現在オブジェクトを前に更新

		m_pPrev = NULL;	// 前オブジェクト
		m_pNext = NULL;	// 次オブジェクト

		char *pObjName	= ConObjtype(objType);
		int nLenght		= (int)strlen(pObjName);
		char cObjName[32];
		for (int nCnt = 0; nCnt < nLenght; nCnt++)
		{
			cObjName[nCnt] = pObjName[nCnt];
		}
		cObjName[nLenght] = '\0';

		delete this;			// 破棄
		m_nNumPri[nPriority]--;	// プライオリティ総数を減らす
		m_nNumAll--;			// 総数を減らす
	}
}

//=======================================================//
// オブジェクトタイプ文字変換処理(Scene)
//=======================================================//
char *CScene::ConObjtype(CScene::OBJTYPE objType)
{
	char cName[32];

	switch (objType)
	{
	case OBJTYPE_MESHFIELD		:wsprintf(cName, "Meshfield");		break;	// メッシュフィールド
	case OBJTYPE_MESHWALL		:wsprintf(cName, "Meshwall");		break;	// メッシュウォール
	case OBJTYPE_MESHCYLINDER	:wsprintf(cName, "Meshcylinder");	break;	// メッシュシリンダー
	case OBJTYPE_BG				:wsprintf(cName, "Bg");				break;	// 背景
	case OBJTYPE_ENEMY			:wsprintf(cName, "Enemy");			break;	// 敵
	case OBJTYPE_PLAYER			:wsprintf(cName, "Player");			break;	// プレイヤー
	case OBJTYPE_BULLET			:wsprintf(cName, "Bullet");			break;	// 弾
	case OBJTYPE_EFFECT			:wsprintf(cName, "Effect");			break;	// エフェクト
	case OBJTYPE_NUMBER			:wsprintf(cName, "Number");			break;	// 数字
	case OBJTYPE_SCORE			:wsprintf(cName, "Score");			break;	// スコア
	case OBJTYPE_RANKING		:wsprintf(cName, "Ranking");		break;	// ランキング
	case OBJTYPE_MENU			:wsprintf(cName, "Menu");			break;	// メニュー
	case OBJTYPE_PAUSE			:wsprintf(cName, "Pause");			break;	// ポーズ
	case OBJTYPE_TITLE			:wsprintf(cName, "Title");			break;	// タイトル
	case OBJTYPE_FADE			:wsprintf(cName, "Fade");			break;	// フェード
	case OBJTYPE_CAMERA			:wsprintf(cName, "Camera");			break;	// カメラ
	default						:wsprintf(cName, "Unknown");		break;	// 上記以外
	}
	return &cName[0];
}