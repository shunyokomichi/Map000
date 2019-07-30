//=======================================================//
//
// �V�[������ [Scene.h]
// Author : Shun Yokomichi
//
//=======================================================//
#ifndef _SCENE_H_
#define _SCENE_H_

//*******************************************************//
// �w�b�_�t�@�C���̃C���N���[�h
//*******************************************************//
#include "main.h"

//*******************************************************//
// �N���X��`
//*******************************************************//
class CScene
{// CScene
public:
	typedef enum
	{	// �D�揇��
		PRIORITY_BG = 0,	// �w�i
		PRIORITY_1,			// 1
		PRIORITY_2,			// 2
		PRIORITY_3,			// 3
		PRIORITY_4,			// 4
		PRIORITY_5,			// 5
		PRIORITY_UI,		// UI
		PRIORITY_PAUSE,		// �|�[�Y
		PRIORITY_FADE,		// �t�F�[�h
		PRIORITY_MAX		// �ő吔
	}PRIORITY;

	typedef enum
	{// �I�u�W�F�N�g�^�C�v
		OBJTYPE_NONE = 0,		// �������Ă��Ȃ�
		OBJTYPE_MESHFIELD,		// ���b�V���t�B�[���h
		OBJTYPE_MESHWALL,		// ���b�V���E�H�[��
		OBJTYPE_MESHCYLINDER,	// ���b�V���V�����_�[
		OBJTYPE_BG,				// �w�i
		OBJTYPE_SAID,			// �\��
		OBJTYPE_ENEMY,			// �G
		OBJTYPE_PLAYER,			// �v���C���[
		OBJTYPE_BULLET,			// �e
		OBJTYPE_SELECTICON,		// �I���A�C�R��
		OBJTYPE_EFFECT,			// �G�t�F�N�g
		OBJTYPE_ITEM,			// �A�C�e��
		OBJTYPE_ITEMICON,		// �A�C�e���A�C�R��
		OBJTYPE_ITEM3D,			// �A�C�e��3D
		OBJTYPE_BLOCK,			// �u���b�N
		OBJTYPE_OBJECT3D,		// �r���{�[�h
		OBJTYPE_UI,				// UI
		OBJTYPE_NUMBER,			// ����
		OBJTYPE_SCORE,			// �X�R�A
		OBJTYPE_RANKING,		// �����L���O
		OBJTYPE_MENU,			// ���j���[
		OBJTYPE_PAUSE,			// �|�[�Y
		OBJTYPE_TITLE,			// �^�C�g��
		OBJTYPE_FADE,			// �t�F�[�h
		OBJTYPE_CAMERA,			// �J����
		OBJTYPE_RESULT,			// ���U���g
		OBJTYPE_MAX				// �ő吔
	}OBJTYPE;

	CScene(int nPriority, OBJTYPE objType);	// �R���X�g���N�^
	virtual ~CScene() {};					// �f�X�g���N�^(���g����)

	static void ReleaseMode(void);			// �t�F�[�h�ȊO�j������
	static void ReleaseAll(void);			// �S�j������
	static void UpdateAll(void);			// �S�X�V����
	static void DrawAll(void);				// �S�`�揈��

	virtual HRESULT Init(void)	= 0;		// ����������
	virtual void Uninit(void)	= 0;		// �I������
	virtual void Update(void)	= 0;		// �X�V����
	virtual void Draw(void)		= 0;		// �`�揈��

	static int GetNumAll(void)				{ return m_nNumAll; }				// �����擾
	static int GetNumPri(int nPriority)		{ return m_nNumPri[nPriority]; }	// �v���C�I���e�B�����擾

	void SetObjType(OBJTYPE objType)		{ m_objType = objType; }			// �I�u�W�F�N�g�^�C�v�ݒ�
	OBJTYPE GetObjType(void)				{ return m_objType; }				// �I�u�W�F�N�g�^�C�v�擾

	static void SetStartUp(int nStartUp)	{ m_nStartUp = nStartUp; }			// �X�V�ݒ�

	// �I�u�W�F�N�g�^�C�v�v�Z�p
	static CScene *GetScene(PRIORITY nPriority) { return m_pTop[nPriority]; }
	CScene *GetSceneNext(void)					{ return m_pNext; }

private:
	void Release(void);	// �J������
	char *ConObjtype(OBJTYPE objType);

	static CScene *m_pTop[PRIORITY_MAX];	// �擪�I�u�W�F�N�g�ւ̃|�C���^
	static CScene *m_pCur[PRIORITY_MAX];	// ���݃I�u�W�F�N�g�ւ̃|�C���^

	CScene *m_pPrev;						// �O�I�u�W�F�N�g�ւ̃|�C���^
	CScene *m_pNext;						// ���I�u�W�F�N�g�ւ̃|�C���^

	int		m_nPriority;					// ���
	OBJTYPE m_objType;						// ���
	bool	m_bDeath;						// ���S�t���O

	static int m_nNumAll;					// ����
	static int m_nNumPri[PRIORITY_MAX];		// �v���C�I���e�B����
	static int m_nStartUp;					// �X�V�J�n�ԍ�

protected:
	void SetDeath(void) { m_bDeath = true; }	// �j��
};
#endif