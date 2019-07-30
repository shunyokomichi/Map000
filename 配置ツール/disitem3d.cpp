//=============================================================================
//
// アイテム3D表示処理 [disitem3D.cpp]
// Author : Shun Yokomichi
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS

#include "disitem3d.h"
#include "renderer.h"			// レンダラー
#include "manager.h"			// マネージャー
#include "input.h"				// 入力
#include "model.h"				// モデル
#include "game.h"				// ゲーム
#include "sound.h"				// サウンド
#include "display.h"
#include "debugproc.h"
#include "selecticon.h"
//*********************************************************************
//	グローバル宣言
//*********************************************************************

//*********************************************************************
//	マクロ定義
//*********************************************************************
#define ITEM000_0	"data/MODEL/item/Chest000_0.x"			// モデル名(ふた部分)
#define ITEM000_1	"data/MODEL/item/Chest000_1.x"			// モデル名(箱部分)
#define AREA_RADIUS	(60.0f)									// エリアの半径

//*********************************************************************
//	静的メンバ変数
//*********************************************************************
LPD3DXMESH				CDisItem3D::m_pMesh[MAX_ITEM_PARTS] = {};			// メッシュのポインタ
LPD3DXBUFFER			CDisItem3D::m_pBuffMat[MAX_ITEM_PARTS] = {};		// バッファのポインタ
DWORD					CDisItem3D::m_nNumMat[MAX_ITEM_PARTS] = {};		// マットのポインタ
LPDIRECT3DTEXTURE9		*CDisItem3D::m_pTexture[MAX_ITEM_PARTS] = {};		// テクスチャ情報へのポインタ
int						CDisItem3D::m_nNum = 0;						// 総合配置数の情報

//*********************************************************************
//	アイテムの生成
//*********************************************************************
CDisItem3D *CDisItem3D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type)
{
	CDisItem3D *pItem3D = NULL;

	// シーンの生成
	if (pItem3D == NULL)
	{
		pItem3D = new CDisItem3D;	// 動的確保
		if (pItem3D != NULL)
		{
			pItem3D->Init();				// 初期化へ
			pItem3D->SetModel(pos, rot);	// モデルの位置 & 向き設定
			pItem3D->SetType(type);			// 種類の設定
		}
	}

	return pItem3D;
}

//*********************************************************************
//	コンストラクタ
//*********************************************************************
CDisItem3D::CDisItem3D(int nPriority, OBJTYPE objType): CScene(3, objType)
{
}

//*********************************************************************
//	デストラクタ
//*********************************************************************
CDisItem3D::~CDisItem3D() {}

//*********************************************************************
//		テクスチャの読み込み
//*********************************************************************
HRESULT CDisItem3D::Load(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	// Xファイルの読み込み
	D3DXLoadMeshFromX(ITEM000_0, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[0], NULL, &m_nNumMat[0], &m_pMesh[0]);
	D3DXLoadMeshFromX(ITEM000_1, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[1], NULL, &m_nNumMat[1], &m_pMesh[1]);

	for (int nCnt = 0; nCnt < MAX_ITEM_PARTS; nCnt++)
	{
		D3DXMATERIAL *pmat;							// マテリアルデータへのポインタ
		D3DMATERIAL9 *matDef;						// 現在のマテリアル保存用
		pmat = (D3DXMATERIAL*)m_pBuffMat[nCnt]->GetBufferPointer();
		matDef = new D3DMATERIAL9[m_nNumMat[nCnt]];
		m_pTexture[nCnt] = new LPDIRECT3DTEXTURE9[m_nNumMat[nCnt]];

		for (DWORD tex = 0; tex < m_nNumMat[nCnt]; tex++)
		{// カウント
			matDef[tex] = pmat[tex].MatD3D;
			matDef[tex].Ambient = matDef[tex].Diffuse;
			m_pTexture[nCnt][tex] = NULL;
			if (pmat[tex].pTextureFilename != NULL &&
				lstrlen(pmat[tex].pTextureFilename) > 0)
			{// テクスチャを使用している
				if (FAILED(D3DXCreateTextureFromFile(pDevice,
					pmat[tex].pTextureFilename,
					&m_pTexture[nCnt][tex])))
				{
				}
			}
		}

		delete[] matDef;
		matDef = NULL;
	}

	return S_OK;
}

//*********************************************************************
//		テクスチャの開放
//*********************************************************************
void CDisItem3D::Unload(void)
{
	for (int nCntModel = 0; nCntModel < MAX_ITEM_PARTS; nCntModel++)
	{
		for (DWORD tex = 0; tex < m_nNumMat[nCntModel]; tex++)
		{// カウント
			if (m_pTexture[nCntModel][tex] != NULL)
			{// NULL以外の場合
				m_pTexture[nCntModel][tex]->Release();		// 解放
				m_pTexture[nCntModel][tex] = NULL;			// NULLへ
			}
		}

		// メッシュの開放
		if (m_pMesh[nCntModel] != NULL)
		{
			m_pMesh[nCntModel]->Release();
			m_pMesh[nCntModel] = NULL;
		}
		// マテリアルの開放
		if (m_pBuffMat[nCntModel] != NULL)
		{
			m_pBuffMat[nCntModel]->Release();
			m_pBuffMat[nCntModel] = NULL;
		}
	}
}

//*********************************************************************
// 初期化処理
//*********************************************************************
HRESULT CDisItem3D::Init()
{
	// それぞれの初期化
	// Textの読み込み
	TxtLoad();

	CDisplay::SetSize(SetModelSize(CDisplay::TYPE_SIZE_MIN), SetModelSize(CDisplay::TYPE_SIZE_MAX));
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向き
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	CDisplay::SetChange(D3DXVECTOR2(1.0f, 1.0f));

	return S_OK;
}

//*********************************************************************
// 終了処理
//*********************************************************************
void CDisItem3D::Uninit(void)
{
	for (int nCntParts = 0; nCntParts < MAX_ITEM_PARTS; nCntParts++)
	{
		if (m_pModel[nCntParts] != NULL)
		{	// NULL以外なら
			m_pModel[nCntParts]->Uninit();		// 終了処理
			delete m_pModel[nCntParts];			// メモリ開放
			m_pModel[nCntParts] = NULL;			// NULLに
		}
	}

	CScene::SetDeath();						// 終了処理
}

//*********************************************************************
// 更新処理
//*********************************************************************
void CDisItem3D::Update(void)
{
	// 基本操作(移動, 向き, リセット)
	BasicOperation();			// 移動処理
	BasicSetRot();				// 向き設定処理
	ResetButton();				// 位置、回転リセット

	// Enterキーで選択したオブジェクトを生成
	ModelCreate();

	// デバック
	CDebugProc::Print("**********************************************************\n");
	CDebugProc::Print(" 宝箱の配置\n");																	// モデル配置設定
	CDebugProc::Print(" ENTER モデル生成             : %d 番目を生成\n", m_nNum);						// 生成するモデル番号
	// モデル選択処理
	SelectType();

	// 位置設定 & 向き設定
	for (int nCntParts = 0; nCntParts < MAX_ITEM_PARTS; nCntParts++)
	{
		if (m_pModel[nCntParts] != NULL)
		{
			m_pModel[nCntParts]->Setcol(D3DXCOLOR(m_col.r, m_col.g, m_col.b, 0.5f));
		}
	}

	if (CDisplay::m_bUse == true)
	{	// 表示するのが切り替わる時
		CDisplay::m_bUse = false;
		Uninit();
	}

}

//*********************************************************************
// 描画処理
//*********************************************************************
void CDisItem3D::Draw(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス
	// シャドウ投影の処理
	// ローカル変数宣言
	D3DXMATRIX mtxShadow;		// シャドウマトリックス
	D3DXPLANE  planeField;		// 平面の値を入れる変数
	D3DXVECTOR4 vecLight;		// ライトの値
	D3DXVECTOR3 pos, normal;	// 平面を求める時に必要変数

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, CDisplay::m_rot.y, CDisplay::m_rot.x, CDisplay::m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, CDisplay::m_pos.x, CDisplay::m_pos.y, CDisplay::m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	for (int nCntParts = 0; nCntParts < MAX_ITEM_PARTS; nCntParts++)
	{
		if (m_pModel[nCntParts] != NULL)
		{
			// MODELの描画
			m_pModel[nCntParts]->Draw();
		}
	}
}

//*****************************************************************************
// モデル生成
//*****************************************************************************
void CDisItem3D::ModelCreate()
{
	if (CManager::GetInputKeyboard()->GetKeyboardTrigger(DIK_RETURN))
	{
		CItem3D *pObject = NULL;		//	CObjectをNULLに

		if (pObject == NULL)
		{	// NULLなら
			pObject = CItem3D::Create(CDisplay::m_pos, CDisplay::m_rot, (CItem3D::TYPE) m_nNum);		// オブジェクトを生成
			if (m_nNum != 0)
			{
				pObject->SetCol(m_col);
			}
			int nNum = CItem3D::GetNum();											// 最大配置数を取得
		}
	}
}

//*****************************************************************************
// モデル選択
//*****************************************************************************
void CDisItem3D::SelectType(void)
{
	if (CManager::GetInputKeyboard()->GetKeyboardTrigger(DIK_LEFT) == true)
	{	// ←キー押下の場合
		m_nNum = (m_nNum + ((int)CDisItem3D::TYPE_MAX - 1)) % (int)CDisItem3D::TYPE_MAX;		// 選択モデルを変える
	}
	else if (CManager::GetInputKeyboard()->GetKeyboardTrigger(DIK_RIGHT) == true)
	{	// →キー押下の場合
		m_nNum = (m_nNum + 1) % (int)CDisItem3D::TYPE_MAX;		// 選択モデルを変える
	}

	SetCol(m_nNum);

	CDebugProc::Print(" ← or → 中身選択             : %d / %d 個\n", m_nNum, (int)CDisItem3D::TYPE_MAX);		// 選んでるモデル番号
	if (m_nNum == 0) { CDebugProc::Print("                                       中身無し\n"); }
	if (m_nNum == 1) { CDebugProc::Print("                                       カギが入ってる\n"); }
	if (m_nNum == 2) { CDebugProc::Print("                                       回復する\n"); }
	if (m_nNum == 3) { CDebugProc::Print("                                       爆発する\n"); }

}

//*****************************************************************************
// 色設定
//*****************************************************************************
void CDisItem3D::SetCol(int nNum)
{
	if (nNum == 0) { m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); }	// カギがある宝箱
	if (nNum == 1) { m_col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);}	// カギがある宝箱
	if (nNum == 2) { m_col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f); }	// 回復がある宝箱
	if (nNum == 3) { m_col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f); }	// 爆発がある宝箱
}


//*********************************************************************
// 設定処理
//*********************************************************************
void CDisItem3D::SetModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	m_pos = pos;		// posを代入
	m_rot = rot;		// rotを代入
}

//*****************************************************************************
// サイズ分取得
//*****************************************************************************
D3DXVECTOR3 CDisItem3D::SetModelSize(int nSize)
{
	D3DXVECTOR3 size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// モデルのサイズ取得用
	CSceneX *pSceneX = NULL;							// CSceneXのポインタ

	if (pSceneX == NULL)
	{	// NULLなら
		pSceneX = new CSceneX(PRIORITY_3, CScene::OBJTYPE_ITEM3D);	// 動的確保

		if (pSceneX != NULL)
		{	// NULLじゃないなら
			pSceneX->SetSize(m_pMesh[0]);		// サイズの設定
			size = pSceneX->Getsize(nSize);			// モデルのサイズ取得(0 : 最大値 / 1 : 最小値)
			pSceneX->Uninit();						// 終了処理
			pSceneX = NULL;							// NULLに
		}
	}
	return size;
}


//*********************************************************************
//	Text内読み込み処理
//*********************************************************************
char *CDisItem3D::ReadLine(FILE *pFile, char*pDst, int nPatten)
{
	bool bLine = false;		// 読み込みをやめるかどうかのフラグ
	while (bLine == false)
	{
		fgets(&aLine[0], 256, pFile);	// 1行読み込む
		while (1)
		{
			if (memcmp(pDst, "\t", 1) == 0)
			{	// タブキーなら
				pDst += 1; // 1文字進める
				while (1)
				{
					if (memcmp(pDst, "\t", 1) == 0)
					{	// タブなら
						pDst += 1;	// 1文字進める
					}
					else
					{
						break;		// 終了
					}
				}
			}
			if (memcmp(pDst, "\0", 1) == 0)
			{	// スペースなら
				pDst += 1;	// 1文字進める
				while (1)
				{
					if (memcmp(pDst, "\0", 1) == 0)
					{	// スペースなら
						pDst += 1;	// 1文字進めす
					}
					else
					{
						break;		// 終了
					}
				}
			}
			else if (memcmp(pDst, "\n", 1) == 0)
			{	// 改行なら
				bLine = true;	// フラグをtrueに(読み込み終了)
				break;
			}
			else if (memcmp(pDst, "#", 1) == 0)
			{	// #なら
				bLine = true;	// コメント扱い(読み込み終了)
				break;
			}
			else
			{	// それ以外の場合
				bLine = true;	// 読み込み終了
				break;
			}
		}
	}
	return pDst;	// 読み込んだ１行を返す

}

//*********************************************************************
//	読み込みたいデータの読み込み処理(pos, rotなど)
//*********************************************************************
char *CDisItem3D::SearchLine(char*pDst)
{
	while (1)
	{
		if (memcmp(pDst, ",", 1) == 0)
		{	// コンマなら
			pDst += 1;	// 1文字進める
			break;
		}
		else if (memcmp(pDst, "\0", 1) == 0)
		{	// スペースなら
			pDst += 1;		// 1文字進める
			break;
		}
		else
		{	// それ以外なら
			pDst += 1;		// １文字進める
		}
	}

	return pDst;	// 読み込んだ1行を変える
}

//*********************************************************************
//	読み込みたいデータの読み込み処理(pos, rotなど)
//*********************************************************************
void CDisItem3D::TxtLoad()
{
	pFile = fopen("data/ItemMotion.txt", "r");		// ファイル名, r : 読み込み

	if (pFile != NULL)
	{	// ファイルがNULL以外なら
		pStrCur = ReadLine(pFile, &aLine[0], 0);
		strcpy(&aStr[0], pStrCur);

		if (memcmp(&aStr[0], "CHARACTERSET", 12) == 0)
		{	// 指定文に入ったら
			do
			{	// 指定文が出るまで回す
				pStrCur = ReadLine(pFile, &aLine[0], 0);
				strcpy(&aStr[0], pStrCur);
			} while (memcmp(&aStr[0], "NUM_PARTS = ", 12) != 0);	// 開放条件が満たせば解除
		}

		// 文字を進める
		pStrCur += 12;
		strcpy(&aStr[0], pStrCur);
		m_motionItem.nParts = atoi(&aStr[0]);	// パーツ数を代入

		do
		{
			pStrCur = ReadLine(pFile, &aLine[0], 0);
			strcpy(&aStr[0], pStrCur);

			if (memcmp(&aStr[0], "START", 5) == 0)
			{	// 指定文に入ったら
				for (int nCnt = 0; nCnt < m_motionItem.nParts;)
				{
					pStrCur = ReadLine(pFile, &aLine[0], 0);
					strcpy(&aStr[0], pStrCur);

					if (memcmp(&aStr[0], "PARTSSET", 8) == 0)
					{	// 指定文に入ったら
						do
						{
							pStrCur = ReadLine(pFile, &aLine[0], 0);
							strcpy(&aStr[0], pStrCur);

							if (memcmp(&aStr[0], "INDEX = ", 8) == 0)
							{	// インデック
								pStrCur += 8;
								strcpy(&aStr[0], pStrCur);
								m_motionItem.nIndex = (int)atof(pStrCur);
							}
							else if (memcmp(&aStr[0], "PARENT = ", 9) == 0)
							{	// ペアレント(親子関係設定)
								pStrCur += 9;
								strcpy(&aStr[0], pStrCur);
								m_motionItem.nParent = (int)atof(pStrCur);
							}
							else if (memcmp(&aStr[0], "POS = ", 6) == 0)
							{	// 位置情報
								pStrCur += 6;
								strcpy(&aStr[0], pStrCur);
								m_pos1.x = (float)atof(pStrCur);		// pos.xの情報

								pStrCur = SearchLine(pStrCur);
								strcpy(&aStr[0], pStrCur);
								m_pos1.y = (float)atof(pStrCur);		// pos.yの情報

								pStrCur = SearchLine(pStrCur);
								strcpy(&aStr[0], pStrCur);
								m_pos1.z = (float)atof(pStrCur);		// pos.zの情報
							}
							else if (memcmp(&aStr[0], "ROT = ", 6) == 0)
							{	// 回転情報
								pStrCur += 6;
								strcpy(&aStr[0], pStrCur);
								m_rot1.x = (float)atof(pStrCur);		// rot.xの情報

								pStrCur = SearchLine(pStrCur);
								strcpy(&aStr[0], pStrCur);
								m_rot1.y = (float)atof(pStrCur);		// rot.yの情報

								pStrCur = SearchLine(pStrCur);
								strcpy(&aStr[0], pStrCur);
								m_rot1.z = (float)atof(pStrCur);		// rot.zの情報
							}

						} while (memcmp(&aStr[0], "END_PARTSSET", 12) != 0);
						if (m_pModel[nCnt] == NULL)
						{	// NULL以外なら(モデルの生成)
							m_pModel[nCnt] = new CModel;
							m_pModel[nCnt]->SetInitAll(m_pTexture[m_motionItem.nIndex], m_pMesh[m_motionItem.nIndex], m_pBuffMat[m_motionItem.nIndex], m_nNumMat[m_motionItem.nIndex], m_pos1, m_rot1);
							m_pModel[nCnt]->Init();
						}

						// 親子関係の設定
						if (m_motionItem.nParent == -1)
						{	// 親
							m_pModel[nCnt]->SetParent(NULL);			// 親MODEL (NULL)
						}
						else
						{	// 子
							m_pModel[nCnt]->SetParent(m_pModel[m_motionItem.nParent]);			// 子MODEL (親マトリックス)
						}
						nCnt++;
					}
				}
			}
		} while (memcmp(&aStr[0], "END", 3) != 0);
	}
	fclose(pFile);

}

