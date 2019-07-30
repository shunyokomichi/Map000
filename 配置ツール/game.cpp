//=======================================================//
//
// ゲーム処理 [game.cpp]
// Author : Shun Yokomichi
//
//=======================================================//

//*******************************************************//
// ヘッダファイルのインクルード
//*******************************************************//
#include "game.h"			// ゲーム
#include "manager.h"		// マネージャー
#include "renderer.h"		// レンダラー
#include "sound.h"			// サウンド
#include "input.h"			// 入力
#include "camera.h"			// カメラ
#include "fade.h"			// フェード
#include "meshfield.h"		// メッシュフィールド
#include "time.h"			// タイム
#include "object.h"			// 配置モデル
#include "scene3d.h"
#include "dismodel.h"			// 表示モデル
#include "disfloor.h"			// 表示モデル
#include "selecticon.h"		// 選択アイコン
#include "saveicon.h"		// セーブアイコン
#include "debugproc.h"		// デバック
#include "enemy.h"
#include "disenemy.h"
#include "item3D.h"
#include "disitem3d.h"
//*******************************************************//
// マクロ定義
//*******************************************************//
#define MAX_SOUND_SECOND	(120)		// 秒の最大
#define SOUND_SECOND		(120)		// 秒の最大から足す秒

//*******************************************************//
// プロトタイプ宣言
//*******************************************************//

//*******************************************************//
// グローバル変数
//*******************************************************//

//*******************************************************//
// 静的メンバ変数
//*******************************************************//
CMeshField *CGame::m_pMeshField[m_MeshFieldMax] = { NULL };
CGame::MODEGAME CGame::m_game = MODE_NONE;
CGame::OBJTEXT	CGame::m_text = {};
int CGame::m_nTabCount = 0;

//=======================================================//
// コンストラクタ									(public)	*** CGame ***
//=======================================================//
CGame::CGame()
{
	for (int nCntb = 0; nCntb < TEXT_OBJ_MAX; nCntb++)
	{//	テキストのオブジェクトの種類
		m_textState[nCntb].nCnter = NULL;
		for (int nCnt = 0; nCnt < OBJ_MAX; nCnt++)
		{//	テキストの中身の初期化
			m_textState[nCntb].nType[nCnt] = NULL;
			m_textState[nCntb].ncolType[nCnt] = NULL;
			m_textState[nCntb].pos[nCnt] = {};
			m_textState[nCntb].size[nCnt] = {};
			m_textState[nCntb].bUse[nCnt] = false;
		}
	}
	m_nSelect = 0;
	m_pSaid = NULL;
}

//=======================================================//
// デストラクタ										(public)	*** CGame ***
//=======================================================//
CGame::~CGame()
{

}

//=======================================================//
// ロード処理							(public)	*** CBullet ***
//=======================================================//
HRESULT CGame::Load(void)
{
	return S_OK;
}

//=======================================================//
// アンロード処理								(public)	*** CBullet ***
//=======================================================//
void CGame::Unload(void)
{

}

//=======================================================//
// 初期化処理										(public)	*** CGame ***
//=======================================================//
HRESULT CGame::Init(void)
{
	m_nTabCount = 0;
	ObjectLoad();					//	アイテムのテキスト読み込み

	CCamera *pCamera = CManager::GetCamera();		// カメラ取得
	pCamera->SetPos(D3DXVECTOR3(0.0f, 90.0f, -400.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	TextRoad();

	CObject::Load(m_text.nTypeNum[0]);
	CMeshField::Load(m_text.nTypeNum[1]);		// メッシュフィールド
	CEnemy::Load(m_text.nTypeNum[2]);			// 敵
	CItem3D::Load();
	CDisItem3D::Load();

	for (int nCnt = 0; nCnt < m_text.nNum[0]; nCnt++)
	{	// ブロックの生成
		CObject::Create(m_text.pos[0][nCnt], m_text.rot[0][nCnt], (CObject::OBJTYPE) m_text.nType[0][nCnt]);
	}

	for (int nCnt = 0; nCnt < m_text.nNum[1]; nCnt++)
	{
		CMeshField::Create(CMeshField::MESHTYPE_FIELD, D3DXVECTOR3(10.0f, 0.0f, 10.0f), m_text.pos[1][nCnt], m_text.rot[1][nCnt], m_text.size[nCnt], D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f), CMeshField::MESHTEX_NUMSIZE, m_text.nType[1][nCnt]);
	}

	for (int nCnt = 0; nCnt < m_text.nNum[2]; nCnt++)
	{	// ブロックの生成
		CEnemy::Create(m_text.pos[2][nCnt], m_text.rot[2][nCnt], m_text.nType[2][nCnt]);
	}

	for (int nCnt = 0; nCnt < m_text.nNum[3]; nCnt++)
	{	// ブロックの生成
		CItem3D::Create(m_text.pos[3][nCnt], m_text.rot[3][nCnt], (CItem3D::TYPE)m_text.nType[3][nCnt]);
	}

	// 表示モデルの読み込みと生成
	CDisModel::Load(m_text.nTypeNum[0]);
	CDisModel::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f),D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);

	CDisFloor::Load(1);
	CDsiEnemy::Load(2);

	// 選択アイコンの読み込みと生成
	CSelectIcon::Create(D3DXVECTOR3(m_text.pos[0][0].x, m_text.pos[0][0].y + 50.0f, m_text.pos[0][0].z), D3DXVECTOR3(10.0f, 10.0f, 0.0f));

	m_nSelect = 0;
	m_nObjNum = CObject::GetNumAll();
	m_nSelectSet = 0;
	return S_OK;
}

//=======================================================//
// 終了処理											(public)	*** CGame ***
//=======================================================//
void CGame::Uninit(void)
{
	CMeshField::Unload();
	CDisModel::Unload();
	CDisFloor::Unload();
	CItem3D::Unload();
	CDisItem3D::Unload();

	if (m_pSaid != NULL)
	{
		m_pSaid->Uninit();
		m_pSaid = NULL;
	}

	CScene::ReleaseMode();	// モード終了
}

//=======================================================//
// 更新処理											(public)	*** CGame ***
//=======================================================//
void CGame::Update(void)
{
	CDebugProc::Print(" TABキー押下で操作切替              : 0 / カメラ操作 , 1 / オブジェクト操作\n");

	if (CManager::GetInputKeyboard()->GetKeyboardTrigger(DIK_TAB))
	{	// Tabキーで操作変更
		m_nTabCount = (m_nTabCount + 1) % 2;
	}

	if (m_nTabCount == 1)
	{
		int nNum = 0;
		if (m_nSelectSet == 0)
		{
			nNum = CObject::GetNum();		// 現段階の最大数の取得
		}
		else if (m_nSelectSet == 1)
		{
			nNum = CMeshField::GetNum();
		}
		else if (m_nSelectSet == 2)
		{
			nNum = CEnemy::GetNum();
		}
		else if (m_nSelectSet == 3)
		{
			nNum = CItem3D::GetNum();
		}

		if (CManager::GetInputKeyboard()->GetKeyboardTrigger(DIK_RSHIFT))
		{
			m_nSelectSet = (m_nSelectSet + 1) % 4;

			if (m_nSelectSet == 0)
			{	// モデルの生成
				CDisItem3D::ChangeUninit();
				CDisModel::Create(CDisplay::GetPos(), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
			}
			else if (m_nSelectSet == 1)
			{	// 床の生成
				CDisModel::ChangeUninit();
				CDisFloor::Create(D3DXVECTOR3(110.0f, 0.0f, 15.0f), CDisplay::GetPos(), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXVECTOR3(10.0f, 0.0f, 10.0f), D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.7f), D3DXVECTOR2(1.0f, 1.0f), CSceneMesh::MESHTEX_NUMSIZE, 0);
			}
			else if (m_nSelectSet == 2)
			{	// 敵の生成
				CDisFloor::ChangeUninit();
				CDsiEnemy::Create(CDisplay::GetPos(), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
			}
			else if (m_nSelectSet == 3)
			{
				CDsiEnemy::ChangeUninit();
				CDisItem3D::Create(CDisplay::GetPos(), D3DXVECTOR3(0.0f, 0.0f, 0.0f), (CDisItem3D::TYPE)0);
			}
		}

		if (CManager::GetInputKeyboard()->GetKeyboardTrigger(DIK_F9))
		{	// セーブ
			TextWrite();
		}

		// モデル削除の選択
		DeleteSelect(nNum);

		// 削除
		if (CManager::GetInputKeyboard()->GetKeyboardTrigger(DIK_BACKSPACE))
		{	// BackSpaceキーを押下した場合
			Delete(nNum);	// ブロックの削除へ
		}

		CDebugProc::Print(" 現在選択している作成数                : %d個\n", nNum);
		CDebugProc::Print(" 選択番号  N or V                         : %d / %d番目\n", m_nSelect, nNum);
		CDebugProc::Print(" BACK SPACE 削除                       : %d番目を削除\n", m_nSelect);
		CDebugProc::Print(" セーブ                                        : F9キー押下\n");
		CDebugProc::Print(" 非表示にする                               : F1キー押下\n");
		CDebugProc::Print(" 右shiftキー押下(生成する物の選択) : %dを選択\n", m_nSelectSet);
	}

}

//=======================================================//
// 描画処理
//=======================================================//
void CGame::Draw(void)
{
}

//=======================================================//
// テキスト読み込み
//=======================================================//
void CGame::ObjectLoad(void)
{
}

void CGame::SetStage(void)
{
	// カメラ設定
	//D3DXVECTOR3 posVDest = CManager::GetCamera()->GetPosVDest();	// 視点取得

}

//=======================================================//
// 環境音処理											(public)	*** CGame ***
//=======================================================//
void CGame::PlayES(int nSoundNum)
{
	CSound *pSound = CManager::GetSound();
	if (nSoundNum == 0)
	{	// 風の音
		//pSound->PlaySoundA(CSound::SOUND_LABEL_SE022);
		nSoundNum = -1;
	}
}

//*************************************************************
//	Text読み込み
//*************************************************************
void CGame::TextRoad()
{
	int nTypeNum = 0;		// 種類数の変数
	int nCntWord = 0;		// 文字のカウント
	int nTextCnt = 0;		// Textのカウント

	pFile = fopen("data/TEXT/obj/Map.txt", "r");		// ファイル名, r : 読み込み

	if (pFile != NULL)
	{	// 指定されたファイルが開いた場合
		do
		{	// 指定した文字まで回す
			pStrCur = ReadLine(pFile, &aLine[0], 0);
			strcpy(&aStr[0], pStrCur);

		} while (memcmp(&aStr[0], "MAX_OBJ = ", 10) != 0);	// 指定した文字が0以外なら解除

		// 文字を進める
		pStrCur += 10;
		strcpy(&aStr[0], pStrCur);
		m_text.nNum[nTextCnt] = atoi(&aStr[0]);	// 配置最大数を代入

		do
		{	// 指定した文字まで回す
			pStrCur = ReadLine(pFile, &aLine[0], 0);
			strcpy(&aStr[0], pStrCur);

		} while (memcmp(&aStr[0], "MAX_TYPE = ", 11) != 0);	// 指定した文字が0以外なら解除

		// 文字を進める
		pStrCur += 11;
		strcpy(&aStr[0], pStrCur);
		m_text.nTypeNum[nTextCnt] = atoi(&aStr[0]);	// 配置最大数を代入

		do
		{	// 指定した文字まで回す
			pStrCur = ReadLine(pFile, &aLine[0], 0);
			strcpy(&aStr[0], pStrCur);

			if (memcmp(&aStr[0], "MODEL_FILENAME = ", 17) == 0)
			{
				pStrCur += 17;
				strcpy(&aStr[0], pStrCur);
				do
				{	// モデル名をいれていく
					m_text.ModeName[nTextCnt][nTypeNum][nCntWord] = (char)pStrCur[nCntWord];
					nCntWord++;		// カウント増加
				} while (memcmp(&aStr[nCntWord], "\n", 1) != 0);

				nCntWord = 0;	// 文字カウントを初期化
				nTypeNum++;		// モデルの種類を増加
			}

		} while (memcmp(&aStr[0], "UNLOAD", 6) != 0);	// 指定した文字が0以外なら解除
		nTextCnt++;


		// 床の配置数読み込み & 代入
		do
		{	// 指定した文字まで回す
			pStrCur = ReadLine(pFile, &aLine[0], 0);
			strcpy(&aStr[0], pStrCur);

		} while (memcmp(&aStr[0], "MAX_FLOOR = ", 12) != 0);	// 指定した文字が0以外なら解除

		// 文字を進める
		pStrCur += 12;
		strcpy(&aStr[0], pStrCur);
		m_text.nNum[nTextCnt] = atoi(&aStr[0]);	// 配置最大数を代入

		// 床のTexture種類の読み込み & 代入
		do
		{	// 指定した文字まで回す
			pStrCur = ReadLine(pFile, &aLine[0], 0);
			strcpy(&aStr[0], pStrCur);

		} while (memcmp(&aStr[0], "MAX_TYPE_TEX = ", 15) != 0);	// 指定した文字が0以外なら解除

		// 文字を進める
		pStrCur += 15;
		strcpy(&aStr[0], pStrCur);
		m_text.nTypeNum[nTextCnt] = atoi(&aStr[0]);	// 配置最大数を代入
		nTypeNum = 0;

		// 床のTextuure名の読み込み & 代入
		do
		{	// 指定した文字まで回す
			pStrCur = ReadLine(pFile, &aLine[0], 0);
			strcpy(&aStr[0], pStrCur);

			if (memcmp(&aStr[0], "TEXTURE_NAME = ", 15) == 0)
			{
				pStrCur += 15;
				strcpy(&aStr[0], pStrCur);
				do
				{	// モデル名をいれていく
					m_text.ModeName[nTextCnt][nTypeNum][nCntWord] = (char)pStrCur[nCntWord];
					nCntWord++;		// カウント増加
				} while (memcmp(&aStr[nCntWord], "\n", 1) != 0);

				nCntWord = 0;	// 文字カウントを初期化
				nTypeNum++;		// モデルの種類を増加
			}

		} while (memcmp(&aStr[0], "UNLOAD", 6) != 0);	// 指定した文字が0以外なら解除

		nTextCnt++;

		do
		{	// 指定した文字まで回す
			pStrCur = ReadLine(pFile, &aLine[0], 0);
			strcpy(&aStr[0], pStrCur);

		} while (memcmp(&aStr[0], "MAX_ENEMY = ", 12) != 0);	// 指定した文字が0以外なら解除

		// 文字を進める
		pStrCur += 12;
		strcpy(&aStr[0], pStrCur);
		m_text.nNum[nTextCnt] = atoi(&aStr[0]);	// 配置最大数を代入

		do
		{	// 指定した文字まで回す
			pStrCur = ReadLine(pFile, &aLine[0], 0);
			strcpy(&aStr[0], pStrCur);

		} while (memcmp(&aStr[0], "MAX_TYPE_ENEMY = ", 17) != 0);	// 指定した文字が0以外なら解除

		// 文字を進める
		pStrCur += 17;
		strcpy(&aStr[0], pStrCur);
		m_text.nTypeNum[nTextCnt] = atoi(&aStr[0]);	// 配置最大数を代入
		nTypeNum = 0;
		do
		{	// 指定した文字まで回す
			pStrCur = ReadLine(pFile, &aLine[0], 0);
			strcpy(&aStr[0], pStrCur);

			if (memcmp(&aStr[0], "MODEL_NAME = ", 13) == 0)
			{
				pStrCur += 13;
				strcpy(&aStr[0], pStrCur);
				do
				{	// モデル名をいれていく
					m_text.ModeName[nTextCnt][nTypeNum][nCntWord] = (char)pStrCur[nCntWord];
					nCntWord++;		// カウント増加
				} while (memcmp(&aStr[nCntWord], "\n", 1) != 0);

				nCntWord = 0;	// 文字カウントを初期化
				nTypeNum++;		// モデルの種類を増加
			}

		} while (memcmp(&aStr[0], "UNLOAD", 6) != 0);	// 指定した文字が0以外なら解除

		nTextCnt++;

		do
		{	// 指定した文字まで回す
			pStrCur = ReadLine(pFile, &aLine[0], 0);
			strcpy(&aStr[0], pStrCur);

		} while (memcmp(&aStr[0], "MAX_ITEM = ", 11) != 0);	// 指定した文字が0以外なら解除
		// 文字を進める
		pStrCur += 11;
		strcpy(&aStr[0], pStrCur);
		m_text.nNum[nTextCnt] = atoi(&aStr[0]);	// 配置最大数を代入

		do
		{	// 指定した文字まで回す
			pStrCur = ReadLine(pFile, &aLine[0], 0);
			strcpy(&aStr[0], pStrCur);

		} while (memcmp(&aStr[0], "MAX_TYPE_ITEM = ", 16) != 0);	// 指定した文字が0以外なら解除

		// 文字を進める
		pStrCur += 16;
		strcpy(&aStr[0], pStrCur);
		m_text.nTypeNum[nTextCnt] = atoi(&aStr[0]);	// 配置最大数を代入
		nTextCnt = 0;
		do
		{
			do
			{	// 指定した文字まで回す
				pStrCur = ReadLine(pFile, &aLine[0], 0);
				strcpy(&aStr[0], pStrCur);

				if (memcmp(&aStr[0], "ARRANGE", 7) == 0)
				{	// 指定文字に入ったら
					for (int nCnt = 0; nCnt < m_text.nNum[nTextCnt];)
					{	// 配置数回す
						pStrCur = ReadLine(pFile, &aLine[0], 0);
						strcpy(&aStr[0], pStrCur);

						if (memcmp(&aStr[0], "OBJSET", 6) == 0)
						{	// 指定数に入ったら
							do
							{	// オブジェクト配置情報の読み込み
								pStrCur = ReadLine(pFile, &aLine[0], 0);
								strcpy(&aStr[0], pStrCur);

								if (memcmp(&aStr[0], "TYPE = ", 7) == 0)
								{	// タイプ情報なら
									pStrCur += 7;
									strcpy(&aStr[0], pStrCur);
									m_text.nType[nTextCnt][nCnt] = (int)atof(pStrCur);
								}
								else if (memcmp(&aStr[0], "POS = ", 6) == 0)
								{	// 位置情報なら
									pStrCur += 6;
									strcpy(&aStr[0], pStrCur);
									m_text.pos[nTextCnt][nCnt].x = (float)atof(pStrCur);		// pos.xの情報

									pStrCur = SearchLine(pStrCur);
									strcpy(&aStr[0], pStrCur);
									m_text.pos[nTextCnt][nCnt].y = (float)atof(pStrCur);		// pos.yの情報

									pStrCur = SearchLine(pStrCur);
									strcpy(&aStr[0], pStrCur);
									m_text.pos[nTextCnt][nCnt].z = (float)atof(pStrCur);		// pos.zの情報
								}
								else if (memcmp(&aStr[0], "ROT = ", 6) == 0)
								{	// 回転情報なら
									pStrCur += 6;
									strcpy(&aStr[0], pStrCur);
									m_text.rot[nTextCnt][nCnt].x = (float)atof(pStrCur);		// rot.xの情報

									pStrCur = SearchLine(pStrCur);
									strcpy(&aStr[0], pStrCur);
									m_text.rot[nTextCnt][nCnt].y = (float)atof(pStrCur);		// rot.yの情報

									pStrCur = SearchLine(pStrCur);
									strcpy(&aStr[0], pStrCur);
									m_text.rot[nTextCnt][nCnt].z = (float)atof(pStrCur);		// rot.zの情報
								}
								else if (memcmp(&aStr[0], "SIZE = ", 7) == 0)
								{	// 回転情報なら
									pStrCur += 7;
									strcpy(&aStr[0], pStrCur);
									m_text.size[nCnt].x = (float)atof(pStrCur);		// rot.xの情報

									pStrCur = SearchLine(pStrCur);
									strcpy(&aStr[0], pStrCur);
									m_text.size[nCnt].y = (float)atof(pStrCur);		// rot.yの情報

									pStrCur = SearchLine(pStrCur);
									strcpy(&aStr[0], pStrCur);
									m_text.size[nCnt].z = (float)atof(pStrCur);		// rot.zの情報
								}


							} while (memcmp(&aStr[0], "OBJEND", 6) != 0);	// 指定した文字が0以外なら解除
							nCnt++;		// for文のカウンターの増加
						}
					}
				}
			} while (memcmp(&aStr[0], "END_ARRANGE", 11) != 0);	// 指定した文字が0以外なら解除
			nTextCnt++;
		} while (nTextCnt != 4);
	}
	fclose(pFile);

}

//*************************************************************
//	Text内読み込み処理
//*************************************************************
char *CGame::ReadLine(FILE *pFile, char*pDst, int nPatten)
{
	bool bLine = false;
	while (bLine == false)
	{
		fgets(&aLine[0], 256, pFile);
		while (1)
		{
			if (memcmp(pDst, "\t", 1) == 0)
			{	//
				pDst += 1;
				while (1)
				{
					if (memcmp(pDst, "\t", 1) == 0)
					{
						pDst += 1;
					}
					else
					{
						break;
					}
				}
			}
			if (memcmp(pDst, "\0", 1) == 0)
			{
				pDst += 1;
				while (1)
				{
					if (memcmp(pDst, "\0", 1) == 0)
					{
						pDst += 1;
					}
					else
					{
						break;
					}
				}
			}
			else if (memcmp(pDst, "\n", 1) == 0)
			{
				bLine = true;
				break;
			}
			else if (memcmp(pDst, "#", 1) == 0)
			{
				bLine = true;
				break;
			}
			else
			{
				bLine = true;
				break;
			}
		}
	}
	return pDst;
}

//*************************************************************
//	読み込みたいデータの読み込み処理(pos, rotなど)
//*************************************************************
char *CGame::SearchLine(char*pDst)
{
	while (1)
	{
		if (memcmp(pDst, ",", 1) == 0)
		{	// NULL
			pDst += 1;
			break;
		}
		else if (memcmp(pDst, "\0", 1) == 0)
		{
			pDst += 1;
			break;
		}
		else
		{
			pDst += 1;
		}
	}		// TABの場合

	return pDst;
}

//*************************************************************
//	書き込みに必要な情報の引き出し
//*************************************************************
int CGame::InfSave(int nCntData)
{
	int nNum = 0;

	if (nCntData == 0 || nCntData == 2 || nCntData == 3)
	{
		// シーン
		CScene *pScene = CScene::GetScene(CScene::PRIORITY_3);	// 現在　
		CScene *pSceneNext = NULL;								// 次

		while (pScene != NULL)
		{	// NULL以外の場合
			pSceneNext = pScene->GetSceneNext();				// 次を保存
			if (pScene->GetObjType() == CScene::OBJTYPE_BLOCK && nCntData == 0|| pScene->GetObjType() == CScene::OBJTYPE_ENEMY && nCntData == 2 ||
				pScene->GetObjType() == CScene::OBJTYPE_ITEM3D&& nCntData == 3)
			{
				CSceneX *pBlock = (CSceneX*)pScene;			// SceneXにキャスト
				D3DXVECTOR3 ModelPos = pBlock->Getpos();	// Xモデルの位置を取得
				D3DXVECTOR3 ModelRot = pBlock->Getrot();	// Xモデルの向きを取得

				if (nCntData == 0)
				{
					CObject *pObject = ((CObject*)pScene);		// CObjectへキャスト(型の変更)
					if (pObject != NULL)
					{
						int nType = pObject->GetType();
						m_text.nType[nCntData][nNum] = nType;					// モデル番号を記録

					}
				}
				else if (nCntData == 2)
				{
					CEnemy *pEnemy = ((CEnemy*)pScene);		// CObjectへキャスト(型の変更)
					if (pEnemy != NULL)
					{
						int nType = pEnemy->GetType();
						m_text.nType[nCntData][nNum] = nType;					// モデル番号を記録

					}
				}
				else if (nCntData == 3)
				{
					CItem3D *pItem = (CItem3D*)pScene;			// SceneXにキャスト
					ModelPos = pItem->GetPos();	// Xモデルの位置を取得
					ModelRot = pItem->GetRot();	// Xモデルの向きを取得

					int nType = pItem->GetType();
					m_text.nType[nCntData][nNum] = nType;					// モデル番号を記録

				}

				m_text.pos[nCntData][nNum] = ModelPos;				// 位置情報を記録
				m_text.rot[nCntData][nNum] = ModelRot;				// 向き情報を記録
				nNum++;												// 数の加算
			}

			pScene = pSceneNext;		// 次を設定
		}
	}

	if (nCntData == 1)
	{
		// シーン
		CScene *pSceneField = CScene::GetScene(CScene::PRIORITY_1);	// 現在　
		CScene *pSceneNextField = NULL;								// 次

		while (pSceneField != NULL)
		{	// NULL以外の場合
			pSceneNextField = pSceneField->GetSceneNext();				// 次を保存
			if (pSceneField->GetObjType() == CScene::OBJTYPE_MESHFIELD)
			{
				CSceneMesh *pField = (CSceneMesh*)pSceneField;			// SceneXにキャスト
				D3DXVECTOR3 Pos = pField->Getpos();	// Xモデルの位置を取得
				D3DXVECTOR3 Rot = pField->Getrot();	// Xモデルの向きを取得
				D3DXVECTOR3 size = pField->Getsize();

				CMeshField *pMesh = ((CMeshField*)pSceneField);		// CObjectへキャスト(型の変更)
				if (pMesh != NULL)
				{
					int nType = pMesh->GetTex();
					m_text.nType[nCntData][nNum] = nType;					// モデル番号を記録

				}

				m_text.pos[nCntData][nNum] = Pos;				// 位置情報を記録
				m_text.rot[nCntData][nNum] = Rot;				// 向き情報を記録
				m_text.size[nNum] = size;
				nNum++;												// 数の加算
			}

			pSceneField = pSceneNextField;		// 次を設定
		}
	}
	return nNum;
}

//*************************************************************
//	Text書き込み
//*************************************************************
void CGame::TextWrite(void)
{
	int nNum[4] = {};		// 表示している総数

	for (int nCntData = 0; nCntData < 4; nCntData++)
	{
		nNum[nCntData] = InfSave(nCntData);
	}

	// メッシュフィールドの情報代入

	pFile = fopen("data/TEXT/obj/Map.txt", "w");		// ファイル名, r : 読み込み

	if (pFile != NULL)
	{
		CSaveIcon::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 30.0f, 0.0f), D3DXVECTOR3(250.0f, 100.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
	}

	// テキスト名をテキスト上に表示
	fprintf(pFile, "#=====================================================\n");
	fprintf(pFile, "#\n");
	fprintf(pFile, "#		オブジェクトの配置テキスト\n");
	fprintf(pFile, "#\n");
	fprintf(pFile, "#=====================================================\n");
	fprintf(pFile, "\n");

	// オブジェクトの配置数をテキスト上に表示
	fprintf(pFile, "#=====================================================\n");
	fprintf(pFile, "#		オブジェクトの配置数\n");
	fprintf(pFile, "#=====================================================\n");
	fprintf(pFile, "MAX_OBJ = %d		# 配置数が設定値より少ないと開かないので要注意\n", nNum[0]);
	fprintf(pFile, "\n");

	// モデルの種類
	fprintf(pFile, "#=====================================================\n");
	fprintf(pFile, "#		オブジェクトの種類\n");
	fprintf(pFile, "#=====================================================\n");
	fprintf(pFile, "MAX_TYPE = %d		# 最大モデル数は%dです。足りない場合はCObject.hのマクロの数値を変えてください。\n", m_text.nTypeNum[0], MAX_MODEL);
	fprintf(pFile, "\n");

	// オブジェクトの種類をテキスト上に表示
	fprintf(pFile, "#=====================================================\n");
	fprintf(pFile, "#		オブジェクト名\n");
	fprintf(pFile, "#=====================================================\n");
	fprintf(pFile, "LOAD\n");
	for (int nCntName = 0; nCntName < m_text.nTypeNum[0]; nCntName++)
	{
		fprintf(pFile, "	MODEL_FILENAME = %s\n", &m_text.ModeName[0][nCntName][0]);
	}
	fprintf(pFile, "UNLOAD\n\n");

	// オブジェクトの配置数をテキスト上に表示
	fprintf(pFile, "#=====================================================\n");
	fprintf(pFile, "#		床の配置数\n");
	fprintf(pFile, "#=====================================================\n");
	fprintf(pFile, "MAX_FLOOR = %d		# 配置数が設定値より少ないと開かないので要注意\n", nNum[1]);
	fprintf(pFile, "\n");

	// モデルの種類
	fprintf(pFile, "#=====================================================\n");
	fprintf(pFile, "#		床の種類\n");
	fprintf(pFile, "#=====================================================\n");
	fprintf(pFile, "MAX_TYPE_TEX = %d		# 最大モデル数は%dです。足りない場合はCObject.hのマクロの数値を変えてください。\n", m_text.nTypeNum[1], MAX_MODEL);
	fprintf(pFile, "\n");

	// オブジェクトの種類をテキスト上に表示
	fprintf(pFile, "#=====================================================\n");
	fprintf(pFile, "#		床のTexture名\n");
	fprintf(pFile, "#=====================================================\n");
	fprintf(pFile, "LOAD\n");
	for (int nCntName = 0; nCntName < m_text.nTypeNum[1]; nCntName++)
	{
		fprintf(pFile, "	TEXTURE_NAME = %s\n", &m_text.ModeName[1][nCntName][0]);
	}
	fprintf(pFile, "UNLOAD\n\n");

	// オブジェクトの配置数をテキスト上に表示
	fprintf(pFile, "#=====================================================\n");
	fprintf(pFile, "#		敵の配置数\n");
	fprintf(pFile, "#=====================================================\n");
	fprintf(pFile, "MAX_ENEMY = %d		# 配置数が設定値より少ないと開かないので要注意\n", nNum[2]);
	fprintf(pFile, "\n");

	// モデルの種類
	fprintf(pFile, "#=====================================================\n");
	fprintf(pFile, "#		敵の種類\n");
	fprintf(pFile, "#=====================================================\n");
	fprintf(pFile, "MAX_TYPE_ENEMY = %d		# 最大モデル数は%dです。足りない場合はCObject.hのマクロの数値を変えてください。\n", m_text.nTypeNum[2], MAX_MODEL);
	fprintf(pFile, "\n");

	// オブジェクトの種類をテキスト上に表示
	fprintf(pFile, "#=====================================================\n");
	fprintf(pFile, "#		敵のモデル名\n");
	fprintf(pFile, "#=====================================================\n");
	fprintf(pFile, "LOAD\n");
	for (int nCntName = 0; nCntName < m_text.nTypeNum[2]; nCntName++)
	{
		fprintf(pFile, "	MODEL_NAME = %s\n", &m_text.ModeName[2][nCntName][0]);
	}
	fprintf(pFile, "UNLOAD\n\n");

	// オブジェクトの配置数をテキスト上に表示
	fprintf(pFile, "#=====================================================\n");
	fprintf(pFile, "#		宝箱の配置数\n");
	fprintf(pFile, "#=====================================================\n");
	fprintf(pFile, "MAX_ITEM = %d		# 配置数が設定値より少ないと開かないので要注意\n", nNum[3]);
	fprintf(pFile, "\n");

	// モデルの種類
	fprintf(pFile, "#=====================================================\n");
	fprintf(pFile, "#		宝箱の種類\n");
	fprintf(pFile, "#=====================================================\n");
	fprintf(pFile, "MAX_TYPE_ITEM = %d		# 最大モデル数は%dです。足りない場合はCObject.hのマクロの数値を変えてください。\n", m_text.nTypeNum[3], MAX_MODEL);
	fprintf(pFile, "\n");


	// オブジェクトの配置をテキスト上に表示
	fprintf(pFile, "#=====================================================\n");
	fprintf(pFile, "#		オブジェクトの配置\n");
	fprintf(pFile, "#=====================================================\n");
	fprintf(pFile, "ARRANGE			# 配置情報の読み込み開始(消さないこと)\n");

	for (int nCntModel = 0; nCntModel < nNum[0]; nCntModel++)
	{	// 表示したモデル分回す
		fprintf(pFile, "	OBJSET		# [%d]\n", nCntModel);
		fprintf(pFile, "		TYPE = %d				# モデルタイプ\n", m_text.nType[0][nCntModel]);
		fprintf(pFile, "		POS = %0.1f,%0.1f,%0.1f		# 位置情報\n", m_text.pos[0][nCntModel].x, m_text.pos[0][nCntModel].y, m_text.pos[0][nCntModel].z);
		fprintf(pFile, "		ROT = %0.9f,%0.9f,%0.9f		# 回転情報\n", m_text.rot[0][nCntModel].x, m_text.rot[0][nCntModel].y, m_text.rot[0][nCntModel].z);
		fprintf(pFile, "	OBJEND\n");
		fprintf(pFile, "\n");
	}

	// ファイル読み込み終了の1行表示
	fprintf(pFile, "END_ARRANGE		# 配置情報の読み込み終了(消さないこと)\n");

	// オブジェクトの配置をテキスト上に表示
	fprintf(pFile, "#=====================================================\n");
	fprintf(pFile, "#		床の配置\n");
	fprintf(pFile, "#=====================================================\n");
	fprintf(pFile, "ARRANGE			# 配置情報の読み込み開始(消さないこと)\n");

	for (int nCntModel = 0; nCntModel < nNum[1]; nCntModel++)
	{	// 表示したモデル分回す
		fprintf(pFile, "	OBJSET		# [%d]\n", nCntModel);
		fprintf(pFile, "		TYPE = %d				# モデルタイプ\n", m_text.nType[1][nCntModel]);
		fprintf(pFile, "		POS = %0.1f,%0.1f,%0.1f		# 位置情報\n", m_text.pos[1][nCntModel].x, m_text.pos[1][nCntModel].y, m_text.pos[1][nCntModel].z);
		fprintf(pFile, "		ROT = %0.9f,%0.9f,%0.9f		# 回転情報\n", m_text.rot[1][nCntModel].x, m_text.rot[1][nCntModel].y, m_text.rot[1][nCntModel].z);
		fprintf(pFile, "		SIZE = %0.9f,%0.9f,%0.9f	# サイズ情報\n", m_text.size[nCntModel].x, m_text.size[nCntModel].y, m_text.size[nCntModel].z);
		fprintf(pFile, "	OBJEND\n");
		fprintf(pFile, "\n");
	}
	// ファイル読み込み終了の1行表示
	fprintf(pFile, "END_ARRANGE		# 配置情報の読み込み終了(消さないこと)\n");

	// オブジェクトの配置をテキスト上に表示
	fprintf(pFile, "#=====================================================\n");
	fprintf(pFile, "#		敵の配置\n");
	fprintf(pFile, "#=====================================================\n");
	fprintf(pFile, "ARRANGE			# 配置情報の読み込み開始(消さないこと)\n");

	for (int nCntModel = 0; nCntModel < nNum[2]; nCntModel++)
	{	// 表示したモデル分回す
		fprintf(pFile, "	OBJSET		# [%d]\n", nCntModel);
		fprintf(pFile, "		TYPE = %d				# モデルタイプ\n", m_text.nType[2][nCntModel]);
		fprintf(pFile, "		POS = %0.1f,%0.1f,%0.1f		# 位置情報\n", m_text.pos[2][nCntModel].x, m_text.pos[2][nCntModel].y, m_text.pos[2][nCntModel].z);
		fprintf(pFile, "		ROT = %0.9f,%0.9f,%0.9f		# 回転情報\n", m_text.rot[2][nCntModel].x, m_text.rot[2][nCntModel].y, m_text.rot[2][nCntModel].z);
		fprintf(pFile, "	OBJEND\n");
		fprintf(pFile, "\n");
	}
	// ファイル読み込み終了の1行表示
	fprintf(pFile, "END_ARRANGE		# 配置情報の読み込み終了(消さないこと)\n");

	// オブジェクトの配置をテキスト上に表示
	fprintf(pFile, "#=====================================================\n");
	fprintf(pFile, "#		宝箱の配置\n");
	fprintf(pFile, "#=====================================================\n");
	fprintf(pFile, "ARRANGE			# 配置情報の読み込み開始(消さないこと)\n");

	for (int nCntModel = 0; nCntModel < nNum[3]; nCntModel++)
	{	// 表示したモデル分回す
		fprintf(pFile, "	OBJSET		# [%d]\n", nCntModel);
		fprintf(pFile, "		TYPE = %d				# モデルタイプ\n", m_text.nType[3][nCntModel]);
		fprintf(pFile, "		POS = %0.1f,%0.1f,%0.1f		# 位置情報\n", m_text.pos[3][nCntModel].x, m_text.pos[3][nCntModel].y, m_text.pos[3][nCntModel].z);
		fprintf(pFile, "		ROT = %0.9f,%0.9f,%0.9f		# 回転情報\n", m_text.rot[3][nCntModel].x, m_text.rot[3][nCntModel].y, m_text.rot[3][nCntModel].z);
		fprintf(pFile, "	OBJEND\n");
		fprintf(pFile, "\n");
	}

	// ファイル読み込み終了の1行表示
	fprintf(pFile, "END_ARRANGE		# 配置情報の読み込み終了(消さないこと)\n");

	fclose(pFile);		// ファイルを閉じる

}

//*************************************************************
//	削除
//*************************************************************
void CGame::Delete(int nMaxNum)
{
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;								// 次

	if (m_nSelectSet == 0 || m_nSelectSet == 2 || m_nSelect == 3)
	{
		pScene = CScene::GetScene(CScene::PRIORITY_3);	// 現在　
	}
	else  if (m_nSelectSet == 1)
	{
		pScene = CScene::GetScene(CScene::PRIORITY_1);	// 現在　
	}
	while (pScene != NULL)
	{	// NULL以外の場合
		pSceneNext = pScene->GetSceneNext();				// 次を保存
		if (pScene->GetObjType() == CScene::OBJTYPE_BLOCK && m_nSelectSet == 0)
		{	// オブジェクトタイプがブロックなら
			CObject *pObject = ((CObject*)pScene);		// CObjectへキャスト(型の変更)
			int nObjNum = pObject->GetModelNum();		// CObjectの自信の番号を記録

			if (m_nSelect == nObjNum)
			{	// 同じ番号なら
				m_nObjNum = nObjNum;	// 番号の記録
				if (nMaxNum - 1 == m_nSelect)
				{	// (最大数 - 1)と選択番号が同じ
					m_nSelect--;		// 選択番号を下げる
				}
				pObject->Uninit();		// ブロックのUninit
			}

			if (m_nObjNum < nObjNum )
			{	// 記録した番号よりブロックの番号が大きい場合
				pObject->SetNum(nObjNum - 1);		// ブロックの番号を１ずつ減らす
			}
		}
		else if (pScene->GetObjType() == CScene::OBJTYPE_MESHFIELD )
		{
			CMeshField *pMesh = ((CMeshField*)pScene);		// CObjectへキャスト(型の変更)
			int nObjNum = pMesh->GetModelNum();		// CObjectの自信の番号を記録

			if (m_nSelect == nObjNum)
			{	// 同じ番号なら
				m_nObjNum = nObjNum;	// 番号の記録
				if (nMaxNum - 1 == m_nSelect)
				{	// (最大数 - 1)と選択番号が同じ
					m_nSelect--;		// 選択番号を下げる
				}
				pMesh->Uninit();		// ブロックのUninit
			}

			if (m_nObjNum < nObjNum)
			{	// 記録した番号よりブロックの番号が大きい場合
				pMesh->SetNum(nObjNum - 1);		// ブロックの番号を１ずつ減らす
			}
		}
		if (pScene->GetObjType() == CScene::OBJTYPE_ENEMY&& m_nSelectSet == 2)
		{	// オブジェクトタイプがブロックなら
			CEnemy *pEnemy = ((CEnemy*)pScene);		// CObjectへキャスト(型の変更)
			int nObjNum = pEnemy->GetModelNum();		// CObjectの自信の番号を記録

			if (m_nSelect == nObjNum)
			{	// 同じ番号なら
				m_nObjNum = nObjNum;	// 番号の記録
				if (nMaxNum - 1 == m_nSelect)
				{	// (最大数 - 1)と選択番号が同じ
					m_nSelect--;		// 選択番号を下げる
				}
				pEnemy->Uninit();		// ブロックのUninit
			}

			if (m_nObjNum < nObjNum)
			{	// 記録した番号よりブロックの番号が大きい場合
				pEnemy->SetNum(nObjNum - 1);		// ブロックの番号を１ずつ減らす
			}
		}
		if (pScene->GetObjType() == CScene::OBJTYPE_ITEM3D&& m_nSelectSet == 3)
		{	// オブジェクトタイプがブロックなら
			CItem3D *pItem = ((CItem3D*)pScene);		// CObjectへキャスト(型の変更)
			int nObjNum = pItem->GetModelNum();		// CObjectの自信の番号を記録

			if (m_nSelect == nObjNum)
			{	// 同じ番号なら
				m_nObjNum = nObjNum;	// 番号の記録
				if (nMaxNum - 1 == m_nSelect)
				{	// (最大数 - 1)と選択番号が同じ
					m_nSelect--;		// 選択番号を下げる
				}
				pItem->Uninit();		// ブロックのUninit
			}

			if (m_nObjNum < nObjNum)
			{	// 記録した番号よりブロックの番号が大きい場合
				pItem->SetNum(nObjNum - 1);		// ブロックの番号を１ずつ減らす
			}
		}

		pScene = pSceneNext;		// 次を設定
	}

	SelectIcon();					// 次のモデルの座標を代入
}

//*************************************************************
//	選択削除
//*************************************************************
void CGame::DeleteSelect(int nMaxNum)
{
	int MaxNum = nMaxNum;

	if (m_nSelect <= -1)
	{	// 選択番号が-1以下の場合
		m_nSelect = 0;
	}
	// 選択
	if (CManager::GetInputKeyboard()->GetKeyboardTrigger(DIK_N))
	{	// Nキー押下で進める
		if (MaxNum == 0) { MaxNum = 1; }
		m_nSelect = (m_nSelect + 1) % MaxNum;
		SelectIcon();							// 選択アイコン座標代入
	}
	else if (CManager::GetInputKeyboard()->GetKeyboardTrigger(DIK_V))
	{	// Vキー押下で戻す
		if (MaxNum == 0) { MaxNum = 1; }
		m_nSelect = (m_nSelect + (MaxNum - 1)) % MaxNum;
		SelectIcon();							// 選択アイコン座標代入
	}

}

//*************************************************************
//	選択アイコン座標代入処理
//*************************************************************
void CGame::SelectIcon(void)
{
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;								// 次

	if (m_nSelectSet == 0 || m_nSelectSet == 2 || m_nSelect == 3)
	{
		pScene = CScene::GetScene(CScene::PRIORITY_3);	// 現在　
	}
	else if (m_nSelectSet == 1)
	{
		pScene = CScene::GetScene(CScene::PRIORITY_1);	// 現在　
	}
	while (pScene != NULL)
	{	// NULL以外の場合
		pSceneNext = pScene->GetSceneNext();				// 次を保存
		if (pScene->GetObjType() == CScene::OBJTYPE_BLOCK && m_nSelectSet == 0)
		{	// オブジェクトタイプがブロックなら
			CObject *pObject = ((CObject*)pScene);		// CObjectへキャスト(型の変更)
			int nObjNum = pObject->GetModelNum();		// CObjectの自信の番号を記録

			if (m_nSelect == nObjNum)
			{	// 同じ番号なら
				CSceneX *pSceneX = ((CSceneX*)pObject);		// CSceneXへキャスト(型の変更)
				D3DXVECTOR3 ModelPos = pSceneX->Getpos();
				D3DXVECTOR3 size = pSceneX->Getsize(0);
				CSelectIcon::Setpos(D3DXVECTOR3(ModelPos.x, size.y + ModelPos.y + 20.0f, ModelPos.z));		// Xモデルの位置取得
			}
		}
		if (pScene->GetObjType() == CScene::OBJTYPE_ENEMY&& m_nSelectSet == 2)
		{	// オブジェクトタイプがブロックなら
			CEnemy *pEnemy = ((CEnemy*)pScene);		// CObjectへキャスト(型の変更)
			int nObjNum = pEnemy->GetModelNum();		// CObjectの自信の番号を記録

			if (m_nSelect == nObjNum)
			{	// 同じ番号なら
				CSceneX *pSceneX = ((CSceneX*)pEnemy);		// CSceneXへキャスト(型の変更)
				D3DXVECTOR3 ModelPos = pSceneX->Getpos();
				D3DXVECTOR3 size = pSceneX->Getsize(0);
				CSelectIcon::Setpos(D3DXVECTOR3(ModelPos.x, size.y + ModelPos.y + 20.0f, ModelPos.z));		// Xモデルの位置取得
			}
		}
		if (pScene->GetObjType() == CScene::OBJTYPE_ITEM3D)
		{	// オブジェクトタイプがブロックなら
			CItem3D *pItem = ((CItem3D*)pScene);		// CObjectへキャスト(型の変更)
			int nObjNum = pItem->GetModelNum();		// CObjectの自信の番号を記録

			if (m_nSelect == nObjNum)
			{	// 同じ番号なら
				D3DXVECTOR3 ModelPos = pItem->GetPos();
				CSelectIcon::Setpos(D3DXVECTOR3(ModelPos.x, ModelPos.y + 20.0f, ModelPos.z));		// Xモデルの位置取得
			}
		}
		if (pScene->GetObjType() == CScene::OBJTYPE_MESHFIELD)
		{	// オブジェクトタイプがブロックなら
			CMeshField *pMesh = ((CMeshField*)pScene);		// CObjectへキャスト(型の変更)
			int nObjNum = pMesh->GetModelNum();		// CObjectの自信の番号を記録

			if (m_nSelect == nObjNum)
			{	// 同じ番号なら
				pMesh->SetCol(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
				CSelectIcon::Setpos(D3DXVECTOR3(pMesh->Getpos().x, pMesh->Getpos().y + 20.0f, pMesh->Getpos().z));		// Xモデルの位置取得
			}
		}
		pScene = pSceneNext;		// 次を設定
	}
}