//=======================================================//
//
// Scene3D���� [Scene3D.h]
// Author : Shun Yokomichi
//
//=======================================================//
#ifndef _SCENE3D_H_
#define _SCENE3D_H_

//*******************************************************//
// �w�b�_�t�@�C���̃C���N���[�h
//*******************************************************//
#include "main.h"	// ���C��
#include "scene.h"	// Scene

//*******************************************************//
// �N���X��`
//*******************************************************//
class CScene3D : public CScene
{// CScene3D�i�e�FCScene�j
public:
	CScene3D(int nPriority, CScene::OBJTYPE objType);	// �R���X�g���N�^
	~CScene3D() {};										// �f�X�g���N�^(���g����)

	typedef enum
	{// Scene3D�^�C�v
		SCENE3DTYPE_NORMAL = 0,		// �ʏ�
		SCENE3DTYPE_BILLBOARD,		// �r���{�[�h
		SCENE3DTYPE_BILLEFFECTNONE,	// �r���{�[�h�G�t�F�N�g�p���Z�����Ȃ�
		SCENE3DTYPE_BILLEFFECT,		// �r���{�[�h�G�t�F�N�g�p���Z��������
		SCENE3DTYPE_MAX				// �ő吔
	}SCENE3DTYPE;

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture) { m_pTexture = pTexture; }	// �e�N�X�`�����f

	HRESULT Init(void);	// ����������
	void Uninit(void);	// �I������
	void Update(void);	// �X�V����
	void Draw(void);	// �`�揈��

	void SetInitAll(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, SCENE3DTYPE scene3dType)
	{	// �����l�ݒ�
		m_pos			= pos;			// �ʒu
		m_rot			= rot;			// ����
		m_size			= size;			// �T�C�Y
		m_col			= col;			// �J���[
		m_TexUV			= TexUV;		// �e�N�X�`��UV
		m_scene3dType	= scene3dType;	// Scene3D�^�C�v
	}

	D3DXMATRIX GetmtxWorld(void) { return m_mtxWorld; }	// ���[���h�}�g���b�N�X�擾
	void SetParent(CScene3D *pScene3D) { m_pParent = pScene3D; }	// �e���f���ݒ�

	void Setpos(D3DXVECTOR3 pos)	{ m_pos = pos; }	// �ʒu�ݒ�
	D3DXVECTOR3 Getpos(void)		{ return m_pos; }	// �ʒu�擾

	void Setrot(D3DXVECTOR3 rot)	{ m_rot = rot; }	// �����ݒ�
	D3DXVECTOR3 Getrot(void)		{ return m_rot; }	// �����擾

	void Setsize(D3DXVECTOR3 size)	{ m_size = size; }	// �T�C�Y�ݒ�
	D3DXVECTOR3 Getsize(void)		{ return m_size; }	// �T�C�Y�擾

	void Setcol(D3DXCOLOR col)		{ m_col = col; }	// �J���[�ݒ�
	D3DXCOLOR Getcol(void)			{ return m_col; }	// �J���[�擾

	void SetPosleft(bool bLeft) { m_bLeft = bLeft; };	// ���S���W�����̏ꍇ
	bool GetPosLeft(void) { return m_bLeft; };			// ���S���W�����̏ꍇ�擾

	void SetBillRot(float rot);							// �r���{�[�h�����ݒ�

	void SetTex(int nAnimLineNum, int nAnimPattern, D3DXVECTOR2 TexUV, D3DXVECTOR2 TexMoveUV);	// �A�j���[�V�������ݒ�

	D3DXVECTOR2 GetTex(void) { return m_TexUV; }	// UV�擾
	D3DXVECTOR3 GetVtx(int nNum);					// ���_
	D3DXVECTOR3 GetNor(int nNum);					// �@��

	float GetHeight(D3DXVECTOR3 pos);				// �����擾

private:
	LPDIRECT3DTEXTURE9		m_pTexture;	// �e�N�X�`�����ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// ���_�o�b�t�@���ւ̃|�C���^

	D3DXMATRIX	m_mtxWorld;		// ���[���h�}�g���b�N�X
	D3DXVECTOR3 m_pos;			// �ʒu
	D3DXVECTOR3 m_rot;			// ����
	D3DXVECTOR3 m_size;			// �T�C�Y
	D3DXCOLOR	m_col;			// �J���[
	D3DXVECTOR2 m_TexUV;		// UV
	SCENE3DTYPE m_scene3dType;	// Scene3D�^�C�v
	CScene3D				*m_pParent;			// �e�q�֌W

	bool m_bLeft;
};
#endif