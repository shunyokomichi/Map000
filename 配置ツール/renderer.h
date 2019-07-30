//=======================================================//
//
// �����_���[���� [Renderer.h]
// Author : Shun Yokomichi
//
//=======================================================//
#ifndef _RENDERER_H_
#define _RENDERER_H_

//*******************************************************//
// �w�b�_�t�@�C���̃C���N���[�h
//*******************************************************//
#include "main.h"	// ���C��

//*******************************************************//
// �O���錾
//*******************************************************//

//*******************************************************//
// �N���X��`
//*******************************************************//
class CRenderer
{// CRenderer
public:
	CRenderer();		// �R���X�g���N�^
	~CRenderer() {};	// �f�X�g���N�^(���g����)

	HRESULT Init(HWND hWnd, bool bWindow);	// ����������
	void Uninit(void);						// �I������
	void Update(void);						// �X�V����
	void Draw(void);						// �`�揈��

	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; }

private:
	LPDIRECT3D9			m_pD3D;			// Direct3D�I�u�W�F�N�g
	LPDIRECT3DDEVICE9	m_pD3DDevice;	// Device�I�u�W�F�N�g(�`��ɕK�v)
	bool				m_bUse;			// �f�o�b�N�̕\��
};
#endif