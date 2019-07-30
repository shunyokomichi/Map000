//=======================================================//
//
// �I��\������ [Scene2D.cpp]
// Author : Shun Yokomichi
//
//=======================================================//

//*******************************************************//
// �w�b�_�t�@�C���̃C���N���[�h
//*******************************************************//
#include "selecticon.h"	// �I���A�C�R��
#include "manager.h"	// �}�l�[�W���[
#include "renderer.h"	// �����_���[
#include "input.h"		// ����
#include "texture.h"

//*******************************************************//
// �}�N����`
//*******************************************************//
#define POS_Y_MOVE		(1.0f)									// �����ő��

//*******************************************************//
// �ÓI�����o�ϐ��錾
//*******************************************************//
D3DXVECTOR3				CSelectIcon::m_pos		 = {};		// �ʒu���

//=======================================================//
// �R���X�g���N�^(CScene2D)
//=======================================================//
CSelectIcon::CSelectIcon() : CScene3D(PRIORITY_4, CScene::OBJTYPE_SELECTICON)
{
	// �f�t�H���g
	m_Type = TYPE_NONE;		// �I���A�C�R���̏��(�����x)
	m_fPosY = 0.0f;			// Y���ۑ��p
}

//=======================================================//
// �m�ۏ���	
//=======================================================//
CSelectIcon *CSelectIcon::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CSelectIcon *pSelectIcon = NULL;			// �|�C���^

	if (pSelectIcon == NULL)
	{// NULL�̏ꍇ
		pSelectIcon = new CSelectIcon;			// �������m��

		if (pSelectIcon != NULL)
		{// NULL�ȊO�̏ꍇ
			pSelectIcon->Init(pos, size);		// ����������
		}
	}
	return pSelectIcon;			// �l��Ԃ�
}

//=======================================================//
// ����������(CScene2D)
//=======================================================//
HRESULT CSelectIcon::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// Texture�̓ǂݍ���
	CScene3D::BindTexture(*CTexture::GetTexture(CTexture::TYPE_SELECT_ICON));

	// 3D�|���S���̏�����
	CScene3D::SetInitAll(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), size, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f), SCENE3DTYPE_BILLBOARD);
	CScene3D::Init();

	// �ʒu�̑��
	m_pos = pos;

	m_Type = TYPE_UP;		// �グ���Ԃ�

	return S_OK;
}

//=======================================================//
// �I������(CScene2D)
//=======================================================//
void CSelectIcon::Uninit(void)
{
	CScene3D::Uninit();		//CScene2D��Uninit��
}

//=======================================================//
// �X�V����(CScene2D)
//=======================================================//
void CSelectIcon::Update(void)
{
	switch (m_Type)
	{
	case TYPE_UP:
		// Y�����グ����
		m_fPosY += 0.02f;

		if (m_fPosY <= POS_Y_MOVE)
		{	// �w�肵���l�ȉ��Ȃ�
			m_pos.y += m_fPosY / 3;		// ���Z
		}
		else
		{
			m_Type = TYPE_DWON;		// �^�C�v�̕ύX(Y��Dwon��Ԃ�)
		}
		break;

	case TYPE_DWON:
		//�@Y������������
		m_fPosY -= 0.02f;

		if (m_fPosY >= 0.0f)
		{	// �w�肵���l�ȏ�Ȃ�
			m_pos.y -= m_fPosY / 3;		// ���Z
		}
		else
		{
			m_Type = TYPE_UP;		// �^�C�v�̕ύX(Y��Dwon��Ԃ�)
		}
		break;
	}

	CScene3D::Setpos(m_pos);	// �ʒu���X�V
	CScene3D::Update();			// 3D�|���S���̍X�V
}

//=======================================================//
// �`�揈��(CScene2D)
//=======================================================//
void CSelectIcon::Draw(void)
{
	CScene3D::Draw();		// �`�揈��
}