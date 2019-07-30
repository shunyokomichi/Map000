//=======================================================//
//
// Scene2D���� [Scene2D.h]
// Author : Shun Yokomichi
//
//=======================================================//
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

//*******************************************************//
// �w�b�_�t�@�C���̃C���N���[�h
//*******************************************************//
#include "main.h"	// ���C��
#include "scene.h"	// Scene

//*******************************************************//
// �N���X��`
//*******************************************************//
class CScene2D : public CScene
{// CScene2D�i�e�FCScene�j
public:
	CScene2D(int nPriority, CScene::OBJTYPE objType);	// �R���X�g���N�^
	~CScene2D() {};										// �f�X�g���N�^(���g����)

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture) { m_pTexture = pTexture; }	// �e�N�X�`�����f

	HRESULT Init(void);	// ����������
	void Uninit(void);	// �I������
	void Update(void);	// �X�V����
	void Draw(void);	// �`�揈��

	void SetInitAll(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV)
	{	// �����l�ݒ�
		m_pos	= pos;		// �ʒu
		m_size	= size;		// �T�C�Y
		m_col	= col;		// �J���[
		m_TexUV = TexUV;	// UV
	}

	void Setpos(D3DXVECTOR3 pos)	{ m_pos = pos; }	// �ʒu�ݒ�
	D3DXVECTOR3 Getpos(void)		{ return m_pos; }	// �ʒu�擾

	void Setrot(float rot)			{ m_rot = rot; }	// �����ݒ�
	float Getrot(void)				{ return m_rot; }	// �����擾

	void Setsize(D3DXVECTOR3 size)	{ m_size = size; }	// �T�C�Y�ݒ�
	D3DXVECTOR3 Getsize(void)		{ return m_size; }	// �T�C�Y�擾

	void Setcol(D3DXCOLOR col)		{ m_col = col; }	// �J���[�ݒ�
	D3DXCOLOR Getcol(void)			{ return m_col; }	// �J���[�擾

	void SetTex(int nAnimLineNum, int nAnimPattern, D3DXVECTOR2 TexMoveUV);	// �A�j���[�V�������ݒ�

private:
	LPDIRECT3DTEXTURE9		m_pTexture;	// �e�N�X�`�����ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// ���_�o�b�t�@���ւ̃|�C���^

	D3DXVECTOR3 m_pos;		// �ʒu
	float		m_rot;		// ����
	D3DXVECTOR3 m_size;		// �T�C�Y
	D3DXCOLOR	m_col;		// �J���[
	D3DXVECTOR2 m_TexUV;	// UV
};
#endif