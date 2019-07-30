//=======================================================//
//
// �Z�[�u�A�C�R������ [saveicon.h]
// Author : Shun Yokomichi
//
//=======================================================//
#ifndef _SAVEICON_H_
#define _SAVEICON_H_

//*******************************************************//
// �w�b�_�t�@�C���̃C���N���[�h
//*******************************************************//
#include "scene2d.h"	// 2D�|���S��(�e)

//*******************************************************//
// �N���X��`
//*******************************************************//
class CSaveIcon : public CScene2D
{// CScene2D�i�e�FCScene�j
public:
	typedef enum
	{
		TYPE_NONE = 0,		// �����Ȃ����
		TYPE_TRANSPARENCE,	// �s�������瓧��(����)
		TYPE_OPAQUE,		// ��������s����(����)
		TYPE_MAX			// ����
	}TYPE;

	CSaveIcon();											// �R���X�g���N�^
	~CSaveIcon() {};										// �f�X�g���N�^(���g����)
	static CSaveIcon *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col);		// ����


	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col);	// ����������
	void Uninit(void);												// �I������
	void Update(void);												// �X�V����
	void Draw(void);												// �`�揈��

private:
	TYPE	m_Type;								// �����x�ω��̎��
	int m_nCounter;								// �����x�̃J�E���^
};
#endif