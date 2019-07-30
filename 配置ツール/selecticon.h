//=======================================================//
//
// �I��\������ [selecticon.h]
// Author : Shun Yokomichi
//
//=======================================================//
#ifndef _SELECTICON_H_
#define _SELECTICON_H_

//*******************************************************//
// �w�b�_�t�@�C���̃C���N���[�h
//*******************************************************//
#include "scene3d.h"		// 3D�|���S��

//*******************************************************//
// �N���X��`
//*******************************************************//
class CSelectIcon : public CScene3D
{
public:
	typedef enum
	{
		TYPE_NONE = 0,		// �����Ȃ����
		TYPE_UP,			// Y�����グ�Ă�����
		TYPE_DWON,			// Y���������Ă�����
		TYPE_MAX			// ����
	}TYPE;

	CSelectIcon();											// �R���X�g���N�^
	~CSelectIcon() {};										// �f�X�g���N�^(���g����)
	static CSelectIcon *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);		// ����

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 siz);	// ����������
	void Uninit(void);								// �I������
	void Update(void);								// �X�V����
	void Draw(void);								// �`�揈��

	// �擾 & �ݒ�̊֐�
	static void Setpos(D3DXVECTOR3 pos)	{ m_pos = pos; }	// �ʒu�ݒ�
	D3DXVECTOR3 Getpos(void)			{ return m_pos; }		// �ʒu�擾

private:
	static D3DXVECTOR3 m_pos;				// �ʒu
	TYPE m_Type;							// Y���̕ω��̃^�C�v
	int m_nCounter;							// �J�E���^�[
	float	m_fPosY;
};
#endif