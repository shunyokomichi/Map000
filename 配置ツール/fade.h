//=======================================================//
//
// �t�F�[�h���� [Fade.h]
// Author : Shun Yokomichi
//
//=======================================================//
#ifndef _FADE_H_
#define _FADE_H_

//*******************************************************//
// �w�b�_�t�@�C���̃C���N���[�h
//*******************************************************//
#include "main.h"		// ���C��
#include "manager.h"	// �}�l�[�W���[
#include "scene2d.h"	// Scene2D

//*******************************************************//
// �N���X��`
//*******************************************************//
class CFade : public CScene2D
{// CFade�i�e�FCScene2D�j
public:
	typedef enum
	{	// �t�F�[�h���
		FADE_NONE = 0,	// �������Ă��Ȃ�
		FADE_IN,		// �t�F�[�h�C��
		FADE_OUT,		// �t�F�[�h�A�E�g
		FADE_MAX		// �ő吔
	} FADE;

	CFade();		// �R���X�g���N�^
	~CFade() {};	// �f�X�g���N�^(���g����)

	static HRESULT Load(void);								// �e�N�X�`���ǂݍ���
	static void Unload(void);								// �e�N�X�`���J��
	static CFade *Create(CManager::MODE mode, FADE fade);	// ��������

	HRESULT Init(CManager::MODE mode, FADE fade);	// ����������
	void Uninit(void)	{ CScene2D::Uninit(); };	// �I������
	void Update(void);								// �X�V����
	void Draw(void)		{ CScene2D::Draw(); };		// �`�揈��

	static FADE GetFade(void) { return m_fade; }	// �t�F�[�h�擾

private:
	static LPDIRECT3DTEXTURE9	m_pTexture;		// �e�N�X�`�����ւ̃|�C���^
	CManager::MODE				m_modeNext;		// ���[�h�ڍs
	static FADE					m_fade;			// �t�F�[�h
	int							m_nCntTimer;	// �^�C���J�E���^
};
#endif