//=======================================================//
//
// �e���� [Shadow.h]
// Author : Shun Yokomichi
//
//=======================================================//
#ifndef _SHADOW_H_
#define _SHADOW_H_

//*******************************************************//
// �w�b�_�t�@�C���̃C���N���[�h
//*******************************************************//
#include "main.h"

//*******************************************************//
// �N���X��`
//*******************************************************//
class CShadow
{// CShadow
public:
	CShadow() {};							// �R���X�g���N�^(���g����)
	~CShadow() {};							// �f�X�g���N�^(���g����)

	HRESULT Init(void)	{ return S_OK; };	// ����������
	void Uninit(void)	{};					// �I������(���g����)
	void Update(void)	{};					// �X�V����(���g����)
	void Draw(void)		{};					// �`�揈��(���g����)

	static void SetShadow(D3DXMATRIX &mtxWorld, LPD3DXMESH &pMesh, DWORD &nNumMat, D3DXVECTOR3 pos);
};
#endif