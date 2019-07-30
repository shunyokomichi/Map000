//=======================================================//
//
// ���f������ [Model.h]
// Author :
//
//=======================================================//
#ifndef _MODEL_H_
#define _MODEL_H_

//*******************************************************//
// �w�b�_�t�@�C���̃C���N���[�h
//*******************************************************//
#include "main.h"	// ���C��

//*******************************************************//
// �N���X��`
//*******************************************************//
class CModel
{// CModel
public:
	CModel();		// �R���X�g���N�^
	~CModel() {};	// �f�X�g���N�^(���g����)

	HRESULT Init(void);		// ����������
	void Uninit(void) {};	// �I������(���g����)
	void Update(void);		// �X�V����
	void Draw(void);		// �`�揈��

	void SetInitAll(LPDIRECT3DTEXTURE9 *pTexture, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
	{	// �����l�ݒ�
		m_pTexture = pTexture;	// �e�N�X�`��
		m_pMesh = pMesh;		// ���b�V��
		m_pBuffMat = pBuffMat;	// �}�e���A���o�b�t�@
		m_nNumMat = nNumMat;	// �}�e���A����
		m_pos = pos;			// �ʒu
		m_rot = rot;			// ����
	}

	D3DXMATRIX GetmtxWorld(void)	{ return m_mtxWorld; }	// ���[���h�}�g���b�N�X�擾
	void SetParent(CModel *pModel)	{ m_pParent = pModel; }	// �e���f���ݒ�

	void Setpos(D3DXVECTOR3 pos)	{ m_pos = pos; }	// �ʒu�ݒ�
	D3DXVECTOR3 Getpos(void)		{ return m_pos; }	// �ʒu�擾

	void Setrot(D3DXVECTOR3 rot)	{ m_rot = rot; }	// �����ݒ�
	D3DXVECTOR3 Getrot(void)		{ return m_rot; }	// �����擾

	D3DXVECTOR3 Getsize(int nNum)	{ if (nNum == 0) return m_vtxMax;	else return m_vtxMin; }	// �T�C�Y�擾
	void Setcol(D3DXCOLOR col)		{ m_col = col; m_bcolChange = true; }						// �J���[�ݒ�

private:
	LPDIRECT3DTEXTURE9	*m_pTexture;		// �e�N�X�`�����ւ̃|�C���^
	LPD3DXMESH			m_pMesh;			// ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER		m_pBuffMat;			// �}�e���A�����ւ̃|�C���^
	DWORD				m_nNumMat;			// �}�e���A�����
	D3DXMATRIX			m_mtxWorld;			// ���[���h�}�g���b�N�X
	CModel				*m_pParent;			// �e�q�֌W

	D3DXVECTOR3			m_vtxMin, m_vtxMax;	// ���_�ŏ��l,�ő�l
	D3DXVECTOR3			m_pos, m_rot;		// �ʒu,����
	D3DXCOLOR			m_col;				// �J���[
	bool				m_bcolChange;		// �J���[�ύX
};
#endif