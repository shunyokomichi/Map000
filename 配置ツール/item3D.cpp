//=============================================================================
//
// アイテム3D処理 [item3D.cpp]
// Author : Shun Yokomichi
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS

#include "item3d.h"				// 3Dアイテム
#include "renderer.h"			// レンダラー
#include "manager.h"			// マネージャー
#include "input.h"				// 入力
#include "model.h"				// モデル
#include "game.h"				// ゲーム
#include "sound.h"				// サウンド
#include <stdio.h>				// インクルドファイル

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
LPD3DXMESH				CItem3D::m_pMesh[MAX_ITEM_PARTS] = {};			// メッシュのポインタ
LPD3DXBUFFER			CItem3D::m_pBuffMat[MAX_ITEM_PARTS] = {};		// バッファのポインタ
DWORD					CItem3D::m_nNumMat[MAX_ITEM_PARTS] = {};		// マットのポインタ
LPDIRECT3DTEXTURE9		*CItem3D::m_pTexture[MAX_ITEM_PARTS] = {};		// テクスチャ情報へのポインタ
int						CItem3D::m_nNumAll = 0;						// 総合配置数の情報

//*********************************************************************
//	アイテムの生成
//*********************************************************************
CItem3D *CItem3D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type)
{
	CItem3D *pItem3D = NULL;

	// シーンの生成
	if (pItem3D == NULL)
	{
		pItem3D = new CItem3D;	// 動的確保
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
CItem3D::CItem3D(int nPriority, OBJTYPE objType): CScene(3, OBJTYPE_ITEM3D)
{
	// デフォルト値にする
	m_motionItem.nParts = 0;		// Parts数
	m_motionItem.nIndex = 0;		// インデック数
	m_motionItem.nParent = 0;		// 親子関係

}

//*********************************************************************
//	デストラクタ
//*********************************************************************
CItem3D::~CItem3D() {}

//*********************************************************************
//		テクスチャの読み込み
//*********************************************************************
HRESULT CItem3D::Load(void)
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
void CItem3D::Unload(void)
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
HRESULT CItem3D::Init()
{
	// それぞれの初期化
	// Textの読み込み
	TxtLoad();

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向き
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量
	
	// モデル関係
	m_nNum = m_nNumAll;			// モデルの現在総合配置数の番号を代入(配置番号の記録)
	m_nNumAll++;				// 総合配置の加算


	return S_OK;
}

//*********************************************************************
// 終了処理
//*********************************************************************
void CItem3D::Uninit(void)
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

	// 総合配置の減算
	m_nNumAll--;

	CScene::SetDeath();						// 終了処理
}

//*********************************************************************
// 更新処理
//*********************************************************************
void CItem3D::Update(void)
{
	if (m_nTypeNum == 0) { m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); }	// カギがある宝箱
	if (m_nTypeNum == 1) { m_col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f); }	// カギがある宝箱
	if (m_nTypeNum == 2) { m_col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f); }	// 回復がある宝箱
	if (m_nTypeNum == 3) { m_col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f); }	// 爆発がある宝箱


	for (int nCntParts = 0; nCntParts < MAX_ITEM_PARTS; nCntParts++)
	{
		if (m_pModel[nCntParts] != NULL && m_nTypeNum != 0)
		{
			m_pModel[nCntParts]->Setcol(m_col);
		}
	}
}

//*********************************************************************
// 描画処理
//*********************************************************************
void CItem3D::Draw(void)
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
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
	
	for (int nCntParts = 0; nCntParts < MAX_ITEM_PARTS; nCntParts++)
	{	
		// MODELの描画
		m_pModel[nCntParts]->Draw();
	}
}

//*********************************************************************
// 設定処理
//*********************************************************************
void CItem3D::SetModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	m_pos = pos;		// posを代入
	m_rot = rot;		// rotを代入
}

//*********************************************************************
//	Text内読み込み処理
//*********************************************************************
char *CItem3D::ReadLine(FILE *pFile, char*pDst, int nPatten)
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
char *CItem3D::SearchLine(char*pDst)
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
void CItem3D::TxtLoad()
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

