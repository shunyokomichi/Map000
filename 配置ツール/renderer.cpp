//=======================================================//
//
// レンダラー処理 [Renderer.cpp]
// Author : Shun Yokomichi
//
//=======================================================//

//*******************************************************//
// ヘッダファイルのインクルード
//*******************************************************//
#include "renderer.h"		// レンダラー
#include "manager.h"		// マネージャー
#include "scene.h"			// Scene
#include "input.h"			// 入力
#include "debugproc.h"		// デバック

//*******************************************************//
// 静的メンバ変数
//*******************************************************//

//=======================================================//
// コンストラクタ(CRenderer)
//=======================================================//
CRenderer::CRenderer()
{
	// 初期値設定
	m_pD3D			= NULL;	// オブジェクト
	m_pD3DDevice	= NULL;	// デバイス
	m_bUse			= false;	// デバック表示
}

//=======================================================//
// 初期化処理(CRenderer)
//=======================================================//
HRESULT CRenderer::Init(HWND hWnd, bool bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;	// ディスプレイモード
    D3DDISPLAYMODE d3ddm;			// プレゼンテーションパラメータ

	// Direct3Dオブジェクト作成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == NULL) { return E_FAIL; }

	// 現在のディスプレイモード取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm))) { return E_FAIL; }

	// デバイスのプレゼンテーションパラメータ設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));									// ワークをゼロクリア
	d3dpp.BackBufferCount				= 1;							// バックバッファ数
	d3dpp.BackBufferWidth				= SCREEN_WIDTH;					// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight				= SCREEN_HEIGHT;				// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat				= d3ddm.Format;					// カラーモード指定
	d3dpp.SwapEffect					= D3DSWAPEFFECT_DISCARD;		// 映像信号に同期してフリップ
	d3dpp.EnableAutoDepthStencil		= TRUE;							// デプスバッファ（Ｚバッファ）とステンシルバッファ作成
	d3dpp.AutoDepthStencilFormat		= D3DFMT_D16;					// デプスバッファとして16bitを使う
	d3dpp.Windowed						= true;							// ウィンドウモード(Falseでフルスクリーン,Trueで縮小)
	d3dpp.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;		// リフレッシュレート
	d3dpp.PresentationInterval			= D3DPRESENT_INTERVAL_DEFAULT;	// インターバル

	// デバイス生成
	// ディスプレイアダプタ表示用のデバイス作成
	// 描画と頂点処理をハードウェアで行なう
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &m_pD3DDevice)))
	{
		// 上記設定失敗時
		// 描画をハードウェアで行い、頂点処理はCPUで行なう
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_pD3DDevice)))
		{
			// 上記設定失敗時
			// 描画と頂点処理をCPUで行なう
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_pD3DDevice)))
			{
				// 生成失敗
				return E_FAIL;
			}
		}
	}

	// レンダーステート設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// 裏面カリング
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Zバッファ使用
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンド実行
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);						// ライティングモード有効
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラー指定
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラー指定

	// サンプラーステート設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(U値)設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(V値)設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小フィルタモード設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大フィルタモード設定

	// テクスチャステージステート設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// アルファブレンディング処理設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// 最初のアルファ引数
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// 2番目のアルファ引数

	return S_OK;
}

//=======================================================//
// 終了処理(CRenderer)
//=======================================================//
void CRenderer::Uninit(void)
{
	CScene::ReleaseAll();

	// デバイス破棄
	if (m_pD3DDevice != NULL) { m_pD3DDevice->Release(); m_pD3DDevice = NULL; }

	// Direct3Dオブジェクト破棄
	if (m_pD3D != NULL) { m_pD3D->Release(); m_pD3D = NULL; }
}

//=======================================================//
// 更新処理(CRenderer)
//=======================================================//
void CRenderer::Update(void)
{
	CScene::UpdateAll();	// 更新処理

	//if (CManager::GetInputKeyboard()->GetTrigger(DIK_1))
	//{	// デバック表示するかしないか(true : しない / false : する)
	//	m_bUse = m_bUse ? false : true;
	//}
}

//=======================================================//
// 描画処理(CRenderer)
//=======================================================//
void CRenderer::Draw(void)
{
	// バックバッファ＆Ｚバッファクリア
	m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(100, 149, 237, 0), 1.0f, 0);

	// Direct3Dによる描画開始
	if(SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		CScene::DrawAll();	// 描画処理
		//if (m_bUse == false)
		{
			CDebugProc::Draw();	// 描画処理
		}

		// Direct3Dによる描画終了
		m_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファ入れ替え
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}