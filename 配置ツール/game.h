//=======================================================//
//
// ゲーム処理 [game.h]
// Author : Shun Yokomichi
//
//=======================================================//
#ifndef _GAME_H_
#define _GAME_H_

//*******************************************************//
// ヘッダファイルのインクルード
//*******************************************************//
#include "main.h"		// メイン
#include "manager.h"	// マネージャー

//*******************************************************//
// マクロ定義
//*******************************************************//
#define OBJ_MAX (1800)
#define TEXT_OBJ_MAX (3)		//	テキストの中のオブジェクトの最大数
#define MAX_TEXT		(4)		// Textの最大数

//*******************************************************//
// 前方宣言
//*******************************************************//
class CEnemy;
class CMeshField;
class CLife2D;
class CDisModel;
class CDisModel;

//*******************************************************//
// クラス定義
//*******************************************************//
class CGame
{// ゲーム
public:
	typedef struct
	{
		int				nCnter;					//	オブジェクトの数を数える
		D3DXVECTOR3		pos[OBJ_MAX];			//	オブジェクトポジションの設定
		int				nType[OBJ_MAX];			//	オブジェクト種類の設定
		int				ncolType[OBJ_MAX];		//	オブジェクト判定の設定
		D3DXVECTOR3		size[OBJ_MAX];			//	オブジェクトサイズの設定
		bool			bUse[OBJ_MAX];			// 使ったか
	}TEXTSTATE;

	typedef struct
	{	// TEXTの情報(※追加後は記入すること記入すること※)
		int nNum[MAX_TEXT];					// 配置数
		int nTypeNum[MAX_TEXT];				// モデルの種類
		D3DXVECTOR3 pos[MAX_TEXT][OBJ_MAX];	// 位置情報
		D3DXVECTOR3 rot[MAX_TEXT][OBJ_MAX];	// 回転情報
		D3DXVECTOR3 size[OBJ_MAX];
		D3DXVECTOR3 MeshNum[OBJ_MAX];
		int nType[MAX_TEXT][OBJ_MAX];			// タイプ
		char ModeName[MAX_TEXT][20][256];
	}OBJTEXT;



	typedef enum
	{	// ゲームの状態(CLEAR & GameOver)
		MODE_NONE = 0,
		MODE_GAMEOVER000,		// 巨人に捕まる
		MODE_GAMEOVER001,		// 体力ゼロ
		MODE_GAMECLEAR,			// 生還
		MODE_ITEM,
		MODE_MAX
	}MODEGAME;

	static const int m_MeshFieldMax = 1;
	static const int m_MeshRiverMax = 4;

	CGame();						// コンストラクタ
	~CGame();						// デストラクタ

	static HRESULT Load(void);		// ロード
	static void Unload(void);		// アンロード

	HRESULT Init(void);				// 初期化処理
	void Uninit(void);				// 終了処理
	void Update(void);				// 更新処理
	void Draw(void);				// 描画処理

	void ObjectLoad(void);			// オブジェクトの読み込み
	void SetStage(void);
	void PlayES(int nSoundNum);		// サウンドの番号で環境音を出す

	static void SetMeshField(CMeshField *pMeshField)
	{
		for (int nCnt = 0; nCnt < m_MeshFieldMax; nCnt++)
		{
			if (m_pMeshField[nCnt] == pMeshField)
			{
				m_pMeshField[nCnt] = NULL;
				break;
			}
		}
	}
	static CMeshField *GetMeshField(int nNum) { return m_pMeshField[nNum]; }

	// ゲームの状態取得と設定
	static void SetGame(MODEGAME pGame) { m_game = pGame; };
	static MODEGAME GetGame() { return m_game; };

	// テキストファイル読み込み
	void TextRoad();
	char *ReadLine(FILE *pFile, char*pDst, int nPatten);
	char *SearchLine(char*pDst);

	// テキストファイル書き込み
	void TextWrite(void);
	int InfSave(int nCntData);

	void Delete(int nMaxNum);		// 削除
	void DeleteSelect(int nMaxNum);	// 削除選択
	void SelectIcon(void);			// 選択アイコンの座標決め

	static char *GetName(int nNum, int nTextNum) { return &m_text.ModeName[nTextNum][nNum][0]; }		// モデル名の取得
	static int GetNum(int nTextNum) { return m_text.nTypeNum[nTextNum]; }							// モデル最大数の取得
	static int GetTabCount(void) { return m_nTabCount; }

private:
	static CMeshField *m_pMeshField[m_MeshFieldMax];
	static int m_nTabCount;

	TEXTSTATE m_textState[TEXT_OBJ_MAX];		//	テキスト読み込む変数[読み込むオブジェクトの種類]
	static MODEGAME m_game;

	int m_nCnter;

	FILE *pFile;
	char *pStrCur;
	char aLine[256];
	char aStr[256];
	int m_objNum = 0;
	static OBJTEXT m_text;

	CLife2D *m_pLife2D;
	int m_nSelect;
	int m_nObjNum;
	int m_nSelectSet;
	CDisModel *m_pSaid;


protected:

};

#endif