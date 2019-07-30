//=======================================================//
//
// ライト処理 [Light.cpp]
// Author : Shun Yokomichi
//
//=======================================================//

//*******************************************************//
// ヘッダファイルのインクルード
//*******************************************************//
#include "light.h"		// ライト
#include "manager.h"	// マネージャー
#include "renderer.h"	// レンダラー
#include "input.h"		// 入力

//=======================================================//
// 初期化処理(Light)
//=======================================================//
HRESULT CLight::Init(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 設定用方向ベクトル
	D3DXVECTOR3 vecDir[MAX_LIGHT];

	for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{
		// ライトをクリア
		ZeroMemory(&m_Light[nCntLight], sizeof(D3DLIGHT9));

		if (nCntLight == 0)
		{
			// 種類設定
			m_Light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

			// 拡散光設定
			m_Light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// 方向設定
			vecDir[nCntLight] = D3DXVECTOR3(-0.45f, -0.38f, 0.35f);
			D3DXVec3Normalize(&vecDir[nCntLight], &vecDir[nCntLight]);	// 正規化
			m_Light[nCntLight].Direction = vecDir[nCntLight];
		}
		else if (nCntLight == 1)
		{
			// 種類設定
			m_Light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

			// 拡散光設定
			m_Light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// 方向設定
			vecDir[nCntLight] = D3DXVECTOR3(0.22f, -0.37f, 0.44f);
			D3DXVec3Normalize(&vecDir[nCntLight], &vecDir[nCntLight]);	// 正規化
			m_Light[nCntLight].Direction = vecDir[nCntLight];
		}
		else if (nCntLight == 2)
		{
			// 種類設定
			m_Light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

			// 拡散光設定
			m_Light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// 方向設定
			vecDir[nCntLight] = D3DXVECTOR3(-0.22f, -0.37f, 0.44f);
			D3DXVec3Normalize(&vecDir[nCntLight], &vecDir[nCntLight]);	// 正規化
			m_Light[nCntLight].Direction = vecDir[nCntLight];
		}
		else if (nCntLight == 3)
		{
			// 種類設定
			m_Light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

			// 拡散光設定
			m_Light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// 方向設定
			vecDir[nCntLight] = D3DXVECTOR3(-0.22f, -0.27f, 0.44f);
			D3DXVec3Normalize(&vecDir[nCntLight], &vecDir[nCntLight]);	// 正規化
			m_Light[nCntLight].Direction = vecDir[nCntLight];
		}

		// ライト設定
		pDevice->SetLight(nCntLight, &m_Light[nCntLight]);

		// ライト有効化
		pDevice->LightEnable(nCntLight, TRUE);
	}
	return S_OK;
}