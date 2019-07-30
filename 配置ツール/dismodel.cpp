//=============================================================================
//
// オブジェクト表示処理 [dismodel.cpp]
// Author : Shun Yokomichi
//
//=============================================================================
#include "dismodel.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "object.h"
#include "selecticon.h"
#include "debugproc.h"
#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_OPERATION		(3)		// 最大の処理数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		*CDisModel::m_pTexture[MAX_MODEL] = {};		// テクスチャ情報へのポインタ
LPD3DXMESH				CDisModel::m_pMesh[MAX_MODEL] = {};			// メッシュ情報へのポインタ
LPD3DXBUFFER			CDisModel::m_pBuffMat[MAX_MODEL] = {};		// マテリアル情報へのポインタ
DWORD					CDisModel::m_nNumMat[MAX_MODEL] = {};		// マテリアル情報の数
int						CDisModel::m_nNum = 0;						// 総合配置数の情報

//*****************************************************************************
// ロード処理
//*****************************************************************************
HRESULT CDisModel::Load(int MaxModel)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		// デバイス取得

	for (int nCntModel = 0; nCntModel < MaxModel; nCntModel++)
	{	// テキストに書いてあるモデルの数分回す
		char *cModelName = {};					// モデル名代入用
		cModelName = CGame::GetName(nCntModel, 0);	// テキストに書いてあるモデル名を代入する

		// Xファイルの読み込み
		D3DXLoadMeshFromX(cModelName, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[nCntModel], NULL, &m_nNumMat[nCntModel], &m_pMesh[nCntModel]);

		D3DXMATERIAL *pmat;							// マテリアルデータへのポインタ
		D3DMATERIAL9 *matDef;						// 現在のマテリアル保存用
		pmat = (D3DXMATERIAL*)m_pBuffMat[nCntModel]->GetBufferPointer();
		matDef = new D3DMATERIAL9[m_nNumMat[nCntModel]];
		m_pTexture[nCntModel] = new LPDIRECT3DTEXTURE9[m_nNumMat[nCntModel]];

		for (DWORD tex = 0; tex < m_nNumMat[nCntModel]; tex++)
		{	// 使っているMaterialの数だけ回す
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
// アンロード処理
//*****************************************************************************
void CDisModel::Unload(void)
{
	for (int nCntParnet = 0; nCntParnet < MAX_MODEL; nCntParnet++)
	{
		// テクスチャの破棄
		for (DWORD tex = 0; tex < m_nNumMat[nCntParnet]; tex++)
		{// カウント
			if (m_pTexture[nCntParnet][tex] != NULL)
			{// NULL以外の場合
				m_pTexture[nCntParnet][tex]->Release();		// 解放
				m_pTexture[nCntParnet][tex] = NULL;			// NULLへ
			}
		}

		delete[] m_pTexture[nCntParnet];					// Texuterの破棄
		m_pTexture[nCntParnet] = NULL;						// NULLに

		// マテリアルの開放
		if (m_pBuffMat[nCntParnet] != NULL)
		{// NULL以外の場合
			m_pBuffMat[nCntParnet]->Release();				// 解放
			m_pBuffMat[nCntParnet] = NULL;					// NULLへ
		}

		// メッシュの開放
		if (m_pMesh[nCntParnet] != NULL)
		{// NULL以外の場合
			m_pMesh[nCntParnet]->Release();					// 解放
			m_pMesh[nCntParnet] = NULL;						// NULLへ
		}

		m_nNumMat[nCntParnet] = 0;				// 初期化
	}
}

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CDisModel::CDisModel() : CSceneX(CScene::PRIORITY_3, CScene::OBJTYPE_SAID)
{
	m_nSelectNum = 0;
	m_nCntRedo = 0;
	for (int nCntData = 0; nCntData < MAX_REDO; nCntData++)
	{
		m_nRedo[nCntData] = {};
	}
}

//*****************************************************************************
// 確保処理
//*****************************************************************************
CDisModel *CDisModel::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int objType)
{
	CDisModel *PSaid = NULL;			// ポインタ

	if (PSaid == NULL)
	{// NULLの場合
		PSaid = new CDisModel;			// メモリ確保

		if (PSaid != NULL)
		{// NULL以外の場合
			PSaid->Init(pos, rot, objType);		// 初期化処理
			PSaid->SetType(XTYPE::TYPE_SAID);	// 表示状態に
			PSaid->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));	// 表示するモデルの色(半透明)
		}
	}

	return PSaid;			// 値を返す
}

//*****************************************************************************
// 初期化処理
//*****************************************************************************
HRESULT CDisModel::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int objType)
{
	// モデルの初期化
	CSceneX::SetInitAll(m_pTexture[objType], m_pMesh[objType], m_pBuffMat[objType], m_nNumMat[objType], pos, rot);
	CSceneX::Init();
	CDisplay::SetSize(SetModelSize(CDisplay::TYPE_SIZE_MIN), SetModelSize(CDisplay::TYPE_SIZE_MAX));
	CDisplay::SetChange(D3DXVECTOR2(1.0f, 1.0f));

	// 位置と向きの設定
	CSceneX::Setpos(pos);
	CSceneX::Setrot(rot);

	return S_OK;
}

//*****************************************************************************
// 終了処理
//*****************************************************************************
void CDisModel::Uninit(void)
{
	CSceneX::Uninit();		// 終了処理
}

//*****************************************************************************
// 更新処理
//*****************************************************************************
void CDisModel::Update(void)
{
	if (CGame::GetTabCount() == 1)
	{
		// 基本操作(移動, 向き, リセット)
		BasicOperation();			// 移動処理
		BasicSetRot();				// 向き設定処理
		ResetButton();				// 位置、回転リセット
		Select();					// モデル選択
		ModelCreate();				// 選択したオブジェクトを生成
	}

	// 位置設定 & 向き設定
	CSceneX::Setpos(CDisplay::m_pos);
	CSceneX::Setrot(CDisplay::m_rot);
	CSceneX::Setcol(CSceneX::GetCol());
	CDisplay::SetSize(SetModelSize(CDisplay::TYPE_SIZE_MIN), SetModelSize(CDisplay::TYPE_SIZE_MAX));

	if (CDisplay::m_bUse == true)
	{	// 表示するのが切り替わる時
		CDisplay::m_bUse = false;
		Uninit();
	}
}

//*****************************************************************************
// 描画処理
//*****************************************************************************
void CDisModel::Draw(void)
{
	CSceneX::Draw();	// 描画処理
}

//*****************************************************************************
// モデル生成
//*****************************************************************************
void CDisModel::ModelCreate()
{
	CInputMouse *pMouse = CManager::GetInputMouse();	// 入力情報の取得(マウス)

	//m_nCntRedo = SelectNum(m_nSelectNum, MAX_REDO);

	if (pMouse->GetMouseTrigger(1))
	{
		CObject *pObject = NULL;		//	CObjectをNULLに

		if (pObject == NULL)
		{	// NULLなら
			pObject = CObject::Create(CDisplay::m_pos, CDisplay::m_rot, (CObject::OBJTYPE) m_nNum);		// オブジェクトを生成
			int nNum = CObject::GetNum();											// 最大配置数を取得

			if (nNum == 1)
			{	// 最大配置数が1の場合
				CSceneX *pSceneX = ((CSceneX*)pObject);		// CSceneXへキャスト(型の変更)
				D3DXVECTOR3 size = pSceneX->Getsize(0);		// サイズの取得(vtxMaxを取得)
				CSelectIcon::Setpos(D3DXVECTOR3(CDisplay::m_pos.x, size.y + CDisplay::m_pos.y + 20.0f, CDisplay::m_pos.z));		// Xモデルの位置取得
			}
		}
	}
}

//*****************************************************************************
// 処理選択
//*****************************************************************************
void CDisModel::Select(void)
{
	//m_nSelectNum = SelectNum(m_nSelectNum, MAX_OPERATION);

	if (CGame::GetTabCount() == 1)
	{
		SelectModel(); 
	}
}

//*****************************************************************************
// モデル選択
//*****************************************************************************
void CDisModel::SelectModel(void)
{
	int m_MaxModel = CGame::GetNum(0);		// 読み込んでいるモデルの最大数を取得

	if (CManager::GetInputKeyboard()->GetKeyboardTrigger(DIK_1))
	{	// 左クリック押下の場合
		m_nNum = (m_nNum + (m_MaxModel - 1)) % m_MaxModel;		// 選択モデルを変える
		CSceneX::SetInitAll(m_pTexture[m_nNum], m_pMesh[m_nNum], m_pBuffMat[m_nNum], m_nNumMat[m_nNum], D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));	// モデルの初期設定
		CDisplay::SetSize(SetModelSize(CDisplay::TYPE_SIZE_MIN), SetModelSize(CDisplay::TYPE_SIZE_MAX));
	}
	else if (CManager::GetInputKeyboard()->GetKeyboardTrigger(DIK_2))
	{	// 右クリック押下の場合
		m_nNum = (m_nNum + 1) % m_MaxModel;		// 選択モデルを変える
		CSceneX::SetInitAll(m_pTexture[m_nNum], m_pMesh[m_nNum], m_pBuffMat[m_nNum], m_nNumMat[m_nNum], D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));	// モデルの初期設定
		CDisplay::SetSize(SetModelSize(CDisplay::TYPE_SIZE_MIN), SetModelSize(CDisplay::TYPE_SIZE_MAX));
	}

	CDebugProc::Print("**********************************************************\n");
	CDebugProc::Print(" モデルの配置\n");																	// モデル配置設定
	CDebugProc::Print(" ENTER モデル生成             : %d 番目を生成\n", m_nNum);						// 生成するモデル番号
	CDebugProc::Print(" ← or → モデル選択            : %d / %d 個\n", m_nNum, CGame::GetNum(0));		// 選んでるモデル番号

}

//*****************************************************************************
// サイズ分取得
//*****************************************************************************
D3DXVECTOR3 CDisModel::SetModelSize(int nSize)
{
	D3DXVECTOR3 size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// モデルのサイズ取得用
	CSceneX *pSceneX = NULL;							// CSceneXのポインタ

	if (pSceneX == NULL)
	{	// NULLなら
		pSceneX = new CSceneX(PRIORITY_3, CScene::OBJTYPE_OBJECT3D);	// 動的確保

		if (pSceneX != NULL)
		{	// NULLじゃないなら
			pSceneX->SetSize(m_pMesh[m_nNum]);		// サイズの設定
			size = pSceneX->Getsize(nSize);			// モデルのサイズ取得(0 : 最大値 / 1 : 最小値)
			pSceneX->Uninit();						// 終了処理
			pSceneX = NULL;							// NULLに
		}
	}
	return size;
}
