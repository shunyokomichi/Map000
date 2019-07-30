//=============================================================================
//
// 床表示処理 [disfloor.cpp]
// Author : Shun Yokomichi
//
//=============================================================================
#include "disfloor.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "selecticon.h"
#include "debugproc.h"
#include "game.h"
#include "meshfield.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_OPERATION (3)								// 処理の最大数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		CDisFloor::m_pTexture[MAX_MODEL] = {};		// テクスチャ情報へのポインタ
int						CDisFloor::m_nNum = 0;						// 総合配置数の情報

//*****************************************************************************
// ロード処理
//*****************************************************************************
HRESULT CDisFloor::Load(int MaxModel)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		// デバイス取得

	for (int nCntModel = 0; nCntModel < MaxModel; nCntModel++)
	{	// テキストに書いてあるモデルの数分回す
		char *cModelName = {};					// モデル名代入用
		cModelName = CGame::GetName(nCntModel, 1);	// テキストに書いてあるモデル名を代入する

		D3DXCreateTextureFromFile(pDevice, cModelName, &m_pTexture[nCntModel]);
	}
	return S_OK;
}

//*****************************************************************************
// アンロード処理
//*****************************************************************************
void CDisFloor::Unload(void)
{
	for (int nCntParnet = 0; nCntParnet < MAX_MODEL; nCntParnet++)
	{
		// テクスチャの破棄
		if (m_pTexture[nCntParnet] != NULL)
		{// NULL以外の場合
			m_pTexture[nCntParnet]->Release();		// 解放
			m_pTexture[nCntParnet] = NULL;			// NULLへ
		}
	}
}

//*****************************************************************************
// コンストラクタ
//*****************************************************************************
CDisFloor::CDisFloor() : CSceneMesh(CScene::PRIORITY_3, CScene::OBJTYPE_SAID)
{
}
//*****************************************************************************
// 確保処理
//*****************************************************************************
CDisFloor *CDisFloor::Create(D3DXVECTOR3 Num, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, MESHTEX meshTex, int nTex)
{
	CDisFloor *PSaid = NULL;			// ポインタ

	if (PSaid == NULL)
	{// NULLの場合
		PSaid = new CDisFloor;			// メモリ確保

		if (PSaid != NULL)
		{// NULL以外の場合
			PSaid->Init(Num, pos, rot, size, col, TexUV, meshTex, nTex);		// 初期化処理
		}
	}

	return PSaid;			// 値を返す
}

//*****************************************************************************
// 初期化処理
//*****************************************************************************
HRESULT CDisFloor::Init(D3DXVECTOR3 Num, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, MESHTEX meshTex, int nTex)
{
	// メッシュフィールドの初期化
	CSceneMesh::Init();
	CSceneMesh::SetInitAll(CMeshField::MESHTYPE_FIELD, Num, pos, rot, size,col, TexUV, CMeshField::MESHTEX_NUMSIZE);
	CSceneMesh::BindTexture(m_pTexture[nTex]);
	m_SelectNum = 0;
	CDisplay::m_size = size;
	m_MeshSize = Num;
	CDisplay::SetSize(size, size);
	CDisplay::SetChange(D3DXVECTOR2(-1.0f, 1.0f));

	return S_OK;
}

//*****************************************************************************
// 終了処理
//*****************************************************************************
void CDisFloor::Uninit(void)
{
	CSceneMesh::Uninit();		// 終了処理
}

//*****************************************************************************
// 更新処理
//*****************************************************************************
void CDisFloor::Update(void)
{
	if (CGame::GetTabCount() == 1)
	{
		// 基本操作(移動, 向き, リセット)
		BasicOperation();			// 移動処理
		BasicSetRot();				// 向き設定処理
		ResetButton();				// 位置、回転リセット
		SelectNum(m_SelectNum);		// 選択番号決め
		FloorCreate();				// 選択したオブジェクトを生成
	}

	// 位置設定 & 向き設定
	CSceneMesh::Setpos(CDisplay::m_pos);
	CSceneMesh::Setrot(CDisplay::m_rot);
	CSceneMesh::SetSize(CDisplay::m_size, m_MeshSize);
	CDisplay::SetSize(CDisplay::m_size, CDisplay::m_size);

	if(CDisplay::m_bUse == true)
	{	// 表示するのが切り替わる時
		CDisplay::m_bUse = false;
		Uninit();
	}
}

//*****************************************************************************
// 描画処理
//*****************************************************************************
void CDisFloor::Draw(void)
{
	CSceneMesh::Draw();	// 描画処理
}

//*****************************************************************************
// Texture選択
//*****************************************************************************
void CDisFloor::FloorCreate()
{
	CInputMouse *pMouse = CManager::GetInputMouse();	// 入力情報の取得(マウス)

	if (pMouse->GetMouseTrigger(1))
	{
		CSceneMesh *pMesh = NULL;		//	CObjectをNULLに

		if (pMesh == NULL)
		{	// NULLなら
			pMesh = CMeshField::Create(CMeshField::MESHTYPE_FIELD, m_MeshSize, CDisplay::m_pos, CDisplay::m_rot, CDisplay::m_size, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f), CMeshField::MESHTEX_NUMSIZE, m_nNum);
			int nNum = CMeshField::GetNum();											// 最大配置数を取得
			if (nNum == 1)
			{	// 最大配置数が1の場合
				CSceneMesh *pSceneMesh = ((CSceneMesh*)pMesh);		// CSceneXへキャスト(型の変更)
				D3DXVECTOR3 size = pSceneMesh->Getsize();		// サイズの取得(vtxMaxを取得)
				CSelectIcon::Setpos(D3DXVECTOR3(CDisplay::m_pos.x, size.y + CDisplay::m_pos.y + 10.0f, CDisplay::m_pos.z));		// Xモデルの位置取得
			}
		}
	}
}

//*****************************************************************************
// Texture選択
//*****************************************************************************
void CDisFloor::SelectTex(void)
{
	int m_MaxTex = CGame::GetNum(1);		// 読み込んでいるモデルの最大数を取得

	if (CManager::GetInputKeyboard()->GetKeyboardTrigger(DIK_1) == true)
	{	// ←キー押下の場合
		m_nNum = (m_nNum + (m_MaxTex - 1)) % m_MaxTex;		// 選択モデルを変える
		CSceneMesh::BindTexture(m_pTexture[m_nNum]);
	}
	else if (CManager::GetInputKeyboard()->GetKeyboardTrigger(DIK_2) == true)
	{	// →キー押下の場合
		m_nNum = (m_nNum + 1) % m_MaxTex;		// 選択モデルを変える
		CSceneMesh::BindTexture(m_pTexture[m_nNum]);
	}

	// デバック
	CDebugProc::Print(" Textureの変更\n");
	CDebugProc::Print(" → or ←キー押下              : 番号選択\n");
	CDebugProc::Print(" Texture番号                    : %d / %d 番目を選択中\n", m_nNum, m_MaxTex);

}

//*****************************************************************************
// サイズ変更
//*****************************************************************************
void CDisFloor::SetSizeChange()
{
	CDebugProc::Print(" ***********************************************************\n");

	CDebugProc::Print(" サイズ変更\n");

	if (CManager::GetInputKeyboard()->GetKeyboardPress(DIK_UP) == true)
	{	// ↑キー押下の場合
		CDisplay::m_size.z += 1.0f;
	}
	else if (CManager::GetInputKeyboard()->GetKeyboardPress(DIK_DOWN) == true)
	{	// ↓キー押下の場合
		CDisplay::m_size.z -= 1.0f;
	}
	CDebugProc::Print(" ↑ or ↓キー押下              : Zサイズ変更\n");

	if (CManager::GetInputKeyboard()->GetKeyboardPress(DIK_RIGHT) == true)
	{	// →キー押下の場合
		CDisplay::m_size.x += 1.0f;
	}
	else if (CManager::GetInputKeyboard()->GetKeyboardPress(DIK_LEFT) == true)
	{	// ←キー押下の場合
		CDisplay::m_size.x -= 1.0f;
	}
	CDebugProc::Print(" → or ←キー押下              : Xサイズ変更\n");
	

	if (CDisplay::m_size.x <= 1.0f)
	{	// サイズ(X)が０より小さい場合
		CDisplay::m_size.x = 1.0f;
	}
	if (CDisplay::m_size.z <= 1.0f)
	{	// サイズ(Z)が０より小さい場合
		CDisplay::m_size.z = 1.0f;
	}

	CDebugProc::Print(" サイズ                            : X (%.1f) / Z (%.1f)\n", CDisplay::m_size.x, CDisplay::m_size.z);

}

//*****************************************************************************
// 選択番号の処理
//*****************************************************************************
void CDisFloor::SelectNum(int nNum)
{
	m_SelectNum = CDisplay::SelectNum(m_SelectNum, 2, DIK_3, DIK_3);
	// デバック
	CDebugProc::Print(" ***********************************************************\n");
	CDebugProc::Print("床の生成\n");
	CDebugProc::Print("３押下でサイズか面数の設定を選べる\n");
	CDebugProc::Print(" ***********************************************************\n");

	SelectOperation();		// 番号によって処理を変える
}

//*****************************************************************************
// メッシュフィールドの面数処理
//*****************************************************************************
void CDisFloor::MeshSize()
{
	CDebugProc::Print(" ***********************************************************\n");
	CDebugProc::Print(" 面数の設定\n");

	if (CManager::GetInputKeyboard()->GetKeyboardPress(DIK_UP) == true)
	{	// ↑キー押下の場合
		m_MeshSize.z++;
	}
	else if (CManager::GetInputKeyboard()->GetKeyboardPress(DIK_DOWN) == true)
	{	// ↓キー押下の場合
		m_MeshSize.z--;
	}
	else if (CManager::GetInputKeyboard()->GetKeyboardPress(DIK_RIGHT) == true)
	{	// →キー押下の場合
		m_MeshSize.x++;
	}
	else if (CManager::GetInputKeyboard()->GetKeyboardPress(DIK_LEFT) == true)
	{	// ←キー押下の場合
		m_MeshSize.x--;
	}

	if (m_MeshSize.x <= 0)
	{	// サイズ(X)が０より小さい場合
		m_MeshSize.x = 1;
	}
	if (m_MeshSize.z <= 0)
	{	// サイズ(Z)が０より小さい場合
		m_MeshSize.z = 1;
	}

	// デバック
	CDebugProc::Print(" → or ←キー押下              : X面数の変更\n");
	CDebugProc::Print(" ↑ or ↓キー押下              : Z面数の変更\n");
	CDebugProc::Print(" 面数                          : X (%.1f) / Z (%.1f)\n", m_MeshSize.x, m_MeshSize.z);

}

//*****************************************************************************
// 選択番号により処理を決める
//*****************************************************************************
void CDisFloor::SelectOperation()
{
	SelectTex();			// Texture番号を変える

	if (m_SelectNum == 0)
	{
		SetSizeChange();		// サイズ変更の処理
	}
	else if (m_SelectNum == 1)
	{
		MeshSize();
	}
}
