//=======================================================//
//
// �G���� [enemy.h]
// Author : Shun Yokomichi
//
//=======================================================//
#ifndef _ENEMY_H_
#define _ENEMY_H_

//*******************************************************//
// �w�b�_�t�@�C���̃C���N���[�h
//*******************************************************//
#include "main.h"		// ���C��
#include "scenex.h"		// �V�[��x

//*******************************************************//
// �}�N����`
//*******************************************************//
#define MAX_ENEMY		(2)		//�G�̍ő吔

//*******************************************************//
// �O���錾
//*******************************************************//

//*******************************************************//
// �N���X��`
//*******************************************************//
class CEnemy : public CSceneX
{// �u���b�N�e�FCSceneX�j
public:
	typedef enum
	{
		STATE_NORMAL = 0,		// �ʏ�
		STATE_DAMAGE,			// �_���[�W
		STATE_INVINCIBLE,		// ���G
		STATE_MAX				// ����
	}STATE;

	CEnemy();										// �R���X�g���N�^
	~CEnemy();										// �f�X�g���N�^

	static HRESULT Load(int MaxModel);						// ���[�h
	static void Unload(void);						// �A�����[�h
	static CEnemy *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);		// ����

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);			// ����������
	void Uninit(void);										// �I������
	void Update(void);										// �X�V����
	void Draw(void);										// �`�揈��

	static int GetNum() { return m_nNumAll; }			// ���f���̑������̎擾
	int GetType() { return m_nTypeNum; }				// ���f���̃^�C�v�̎擾
	int GetModelNum() { return m_nNum; }				// ���f���̔z�u�ԍ��̎擾
	void SetNum(int nNum) { m_nNum = nNum; }			// ���f���̔ԍ��ݒ�


private:
	static LPDIRECT3DTEXTURE9	*m_pTexture[MAX_ENEMY];	// �e�N�X�`�����ւ̃|�C���^
	static LPD3DXMESH			m_pMesh[MAX_ENEMY];		// ���b�V�����ւ̃|�C���^
	static LPD3DXBUFFER			m_pBuffMat[MAX_ENEMY];	// �}�e���A�����ւ̃|�C���^
	static DWORD				m_nNumMat[MAX_ENEMY];	// �}�e���A�����̐�

	D3DXVECTOR3					m_posOld;			// (�O��)�ʒu
	D3DXVECTOR3					m_move;				// �ړ���

	static int m_nNumAll;			// �����z�u��
	int m_nTypeNum;					// �^�C�v�ԍ�
	int m_nNum;						// �z�u��


protected:

};

#endif