//=======================================================//
//
// ���C�g���� [Light.cpp]
// Author : Shun Yokomichi
//
//=======================================================//

//*******************************************************//
// �w�b�_�t�@�C���̃C���N���[�h
//*******************************************************//
#include "light.h"		// ���C�g
#include "manager.h"	// �}�l�[�W���[
#include "renderer.h"	// �����_���[
#include "input.h"		// ����

//=======================================================//
// ����������(Light)
//=======================================================//
HRESULT CLight::Init(void)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �ݒ�p�����x�N�g��
	D3DXVECTOR3 vecDir[MAX_LIGHT];

	for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{
		// ���C�g���N���A
		ZeroMemory(&m_Light[nCntLight], sizeof(D3DLIGHT9));

		if (nCntLight == 0)
		{
			// ��ސݒ�
			m_Light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

			// �g�U���ݒ�
			m_Light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// �����ݒ�
			vecDir[nCntLight] = D3DXVECTOR3(-0.45f, -0.38f, 0.35f);
			D3DXVec3Normalize(&vecDir[nCntLight], &vecDir[nCntLight]);	// ���K��
			m_Light[nCntLight].Direction = vecDir[nCntLight];
		}
		else if (nCntLight == 1)
		{
			// ��ސݒ�
			m_Light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

			// �g�U���ݒ�
			m_Light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// �����ݒ�
			vecDir[nCntLight] = D3DXVECTOR3(0.22f, -0.37f, 0.44f);
			D3DXVec3Normalize(&vecDir[nCntLight], &vecDir[nCntLight]);	// ���K��
			m_Light[nCntLight].Direction = vecDir[nCntLight];
		}
		else if (nCntLight == 2)
		{
			// ��ސݒ�
			m_Light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

			// �g�U���ݒ�
			m_Light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// �����ݒ�
			vecDir[nCntLight] = D3DXVECTOR3(-0.22f, -0.37f, 0.44f);
			D3DXVec3Normalize(&vecDir[nCntLight], &vecDir[nCntLight]);	// ���K��
			m_Light[nCntLight].Direction = vecDir[nCntLight];
		}
		else if (nCntLight == 3)
		{
			// ��ސݒ�
			m_Light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

			// �g�U���ݒ�
			m_Light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// �����ݒ�
			vecDir[nCntLight] = D3DXVECTOR3(-0.22f, -0.27f, 0.44f);
			D3DXVec3Normalize(&vecDir[nCntLight], &vecDir[nCntLight]);	// ���K��
			m_Light[nCntLight].Direction = vecDir[nCntLight];
		}

		// ���C�g�ݒ�
		pDevice->SetLight(nCntLight, &m_Light[nCntLight]);

		// ���C�g�L����
		pDevice->LightEnable(nCntLight, TRUE);
	}
	return S_OK;
}