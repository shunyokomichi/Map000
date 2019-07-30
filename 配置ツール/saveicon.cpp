//=======================================================//
//
// �Z�[�u�A�C�R������ [saveicon.cpp]
// Author : Shun Yokomichi
//
//=======================================================//

//*******************************************************//
// �w�b�_�t�@�C���̃C���N���[�h
//*******************************************************//
#include "saveicon.h"	// �Z�[�u�A�C�R��
#include "manager.h"	// �}�l�[�W���[
#include "renderer.h"	// �����_���[
#include "input.h"		// ����
#include "texture.h"
//=======================================================//
// �}�N����`
//=======================================================//
#define SECOND				(60)								// �b��
#define COL_A_TIME			(SECOND * 2.5)						// �����x�ω��̎���
#define COL_A_CHANGE		(0.02f)								// �����x�ω��̗�

//=======================================================//
// �ÓI�����o�ϐ�
//=======================================================//

//=======================================================//
// �R���X�g���N�^(CSaveIco)
//=======================================================//
CSaveIcon::CSaveIcon() : CScene2D(PRIORITY_4, OBJTYPE_UI)
{
	// �l���f�t�H���g��
	m_nCounter = 0;
}
//=======================================================//
// �m�ۏ���	
//=======================================================//
CSaveIcon *CSaveIcon::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col)
{
	CSaveIcon *pSaveIcon = NULL;			// �|�C���^

	if (pSaveIcon == NULL)
	{// NULL�̏ꍇ
		pSaveIcon = new CSaveIcon;			// �������m��

		if (pSaveIcon != NULL)
		{// NULL�ȊO�̏ꍇ
			pSaveIcon->Init(pos, size, col);		// ����������
		}
	}

	return pSaveIcon;			// �l��Ԃ�
}


//=======================================================//
// ����������(CScene2D)
//=======================================================//
HRESULT CSaveIcon::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col)
{
	// Texure�̓ǂݍ���
	CScene2D::BindTexture(*CTexture::GetTexture(CTexture::TYPE_SAVE_LOGO));

	// 2D�|���S���̏�����
	CScene2D::SetInitAll(pos, size, col, D3DXVECTOR2(1.0f, 1.0f));
	CScene2D::Init();

	// �����x�̏��
	m_Type = TYPE_OPAQUE;

	return S_OK;
}

//=======================================================//
// �I������(CScene2D)
//=======================================================//
void CSaveIcon::Uninit(void)
{
	CScene2D::Uninit();		// �I������
}

//=======================================================//
// �X�V����(CScene2D)
//=======================================================//
void CSaveIcon::Update(void)
{
	D3DXCOLOR col = CScene2D::Getcol();		// �F�̎擾

	switch (m_Type)
	{
	case TYPE_OPAQUE:
		// ��������s������
		m_nCounter++;		// �J�E���^�[
		col.a += COL_A_CHANGE;		// �ω���

		if (col.a >= 1.0f)
		{	// �����x��1�ȏ�Ȃ�
			col.a = 1.0f;
		}
		if (COL_A_TIME < m_nCounter)
		{	// ���Ԉȏ�Ȃ�
			m_nCounter = 0;					// �J�E���^������
			m_Type = TYPE_TRANSPARENCE;		// �^�C�v��ς���
		}
		break;

	case TYPE_TRANSPARENCE:
		// �s�������瓧����
		m_nCounter++;		// �J�E���^�[
		col.a -= COL_A_CHANGE;		// �ω���

		if (col.a <= 0.0f)
		{	// �����x��1�ȏ�Ȃ�
			col.a = 0.0f;
		}
		if (COL_A_TIME < m_nCounter)
		{	// ���Ԉȏ�Ȃ�
			Uninit();
			return;
		}
		break;
	}
	
	// 2D�|���S���̍X�V & �F�̐ݒ�
	CScene2D::Update();
	CScene2D::Setcol(col);
}

//=======================================================//
// �`�揈��(CScene2D)
//=======================================================//
void CSaveIcon::Draw(void)
{
	CScene2D::Draw();	// �`�揈��

}