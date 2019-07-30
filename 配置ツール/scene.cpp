//=======================================================//
//
// Scene���� [Scene.cpp]
// Author : Shun Yokomichi
//
//=======================================================//

//*******************************************************//
// �w�b�_�t�@�C���̃C���N���[�h
//*******************************************************//
#include "scene.h"		// �V�[��
#include "manager.h"	// �}�l�[�W���[
#include "renderer.h"	// �����_���[
#include "input.h"		// ����

//*******************************************************//
// �ÓI�����o�ϐ�
//*******************************************************//
CScene	*CScene::m_pTop[PRIORITY_MAX]	= {};	// �擪�I�u�W�F�N�g�ւ̃|�C���^
CScene	*CScene::m_pCur[PRIORITY_MAX]	= {};	// ���݃I�u�W�F�N�g�ւ̃|�C���^

int		CScene::m_nNumAll				= 0;	// ����
int		CScene::m_nNumPri[PRIORITY_MAX] = {};	// ����
int		CScene::m_nStartUp				= 0;	// �X�V�͈�

//=======================================================//
// �R���X�g���N�^(Scene)
//=======================================================//
CScene::CScene(int nPriority, OBJTYPE objType)
{
	m_pPrev = m_pCur[nPriority];	// �O�I�u�W�F�N�g�ݒ�
	m_pNext = NULL;					// ���I�u�W�F�N�g�ݒ�

	m_pCur[nPriority] = this;		// ���݂̃I�u�W�F�N�g�ݒ�

	if (m_pPrev != NULL)
	{	// �O�̃I�u�W�F�N�g��NULL�ȊO
		m_pPrev->m_pNext = m_pCur[nPriority];	// �O�̎��I�u�W�F�N�g�ݒ�
	}

	m_nPriority = nPriority;	// ���ʐݒ�
	m_objType = objType;		// ���
	m_bDeath = false;			// ���S�t���O
	m_nNumAll++;				// �����J�E���g
	m_nNumPri[nPriority]++;		// �����J�E���g

	// �擪�I�u�W�F�N�g��NULL�ȊO
	if (m_pTop[nPriority] == NULL)								{ m_pTop[nPriority] = m_pCur[nPriority]; }			// �擪�I�u�W�F�N�g�ݒ�

	// �擪�I�u�W�F�N�g�ƑO�̃I�u�W�F�N�g������
	else if (m_pTop[nPriority] == m_pCur[nPriority]->m_pPrev)	{ m_pTop[nPriority]->m_pNext = m_pCur[nPriority]; }	// �擪�̎��I�u�W�F�N�g�ݒ�

	char *pObjName	= ConObjtype(objType);
	int nLenght		= (int)strlen(pObjName);
	char cObjName[32];
	for (int nCnt = 0; nCnt < nLenght; nCnt++)
	{
		cObjName[nCnt] = pObjName[nCnt];
	}
	cObjName[nLenght] = '\0';

}

//=======================================================//
// �S�t�F�[�h�ȊO�J������(Scene)
//=======================================================//
void CScene::ReleaseMode(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_FADE; nCntPriority++)
	{	// �J�E���g
		while (m_pTop[nCntPriority] != NULL)
		{	// �擪�I�u�W�F�N�g��NULL�ɂȂ�܂�
			m_pTop[nCntPriority]->Uninit();	// �擪�I�u�W�F�N�g�I��
			m_pTop[nCntPriority]->Release();// �J������
		}
		m_nNumPri[nCntPriority] = 0;		// �v���C�I���e�B����������
	}
	m_nNumAll = 0;							// ����������
}

//=======================================================//
// �S�J������(Scene)
//=======================================================//
void CScene::ReleaseAll(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{	// �J�E���g
		while (m_pTop[nCntPriority] != NULL)
		{	// �擪�I�u�W�F�N�g��NULL�ɂȂ�܂�
			m_pTop[nCntPriority]->Uninit();	// �擪�̃I�u�W�F�N�g�I��
			m_pTop[nCntPriority]->Release();// �J������
		}
		m_nNumPri[nCntPriority] = 0;		// �v���C�I���e�B����������
	}
	m_nNumAll = 0;							// ����������
}

//=======================================================//
// �S�X�V����(Scene)
//=======================================================//
void CScene::UpdateAll(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CScene *pScene	= m_pTop[nCntPriority];						// �X�V�I�u�W�F�N�g�i�擪���L���j
		CScene *pNext	= NULL;										// ���I�u�W�F�N�g�L��

		while (pScene != NULL)
		{	// �X�V�I�u�W�F�N�g��NULL�ȊO
			pNext = pScene->m_pNext;								// ���I�u�W�F�N�g�ݒ�

			// �j�����Ȃ�
			if (pScene->m_bDeath == false) { pScene->Update(); }	// �X�V����

			// �j������
			if (pScene->m_bDeath == true) { pScene->Release(); }	// �J������

			pScene = pNext;											// �X�V�I�u�W�F�N�g�Ɏ��I�u�W�F�N�g�ݒ�
		}
	}
}

//=======================================================//
// �S�`�揈��(Scene)
//=======================================================//
void CScene::DrawAll(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{	// �J�E���g
		CScene *pScene	= m_pTop[nCntPriority];						// �`��I�u�W�F�N�g�i�擪���L���j
		CScene *pNext	= NULL;										// ���I�u�W�F�N�g�L��

		while (pScene != NULL)
		{	// �`��I�u�W�F�N�g��NULL�ȊO
			pNext = pScene->m_pNext;								// ���I�u�W�F�N�g�ݒ�

			// �j�����Ȃ�
			if (pScene->m_bDeath == false)	{ pScene->Draw(); }		// �`�揈��

			// �j������
			if (pScene->m_bDeath == true)	{ pScene->Release(); }	// �J������

			pScene = pNext;											// �`��I�u�W�F�N�g�Ɏ��I�u�W�F�N�g�ݒ�
		}
	}
}

//=======================================================//
// �J������(Scene)
//=======================================================//
void CScene::Release(void)
{
	if (this != NULL)
	{	// ������NULL�ȊO
		int nPriority	= m_nPriority;
		OBJTYPE objType = m_objType;

		if (m_pPrev != NULL)	// m_pTop�ȊO
		{	// �O�I�u�W�F�N�g��NULL�ȊO
			if (m_pTop[nPriority] == m_pPrev)
			{	// �擪�I�u�W�F�N�g���O�I�u�W�F�N�g�Ɠ���
				m_pTop[nPriority]->m_pNext = m_pNext;	// �擪�̎��I�u�W�F�N�g�ݒ�
			}
			m_pPrev->m_pNext = m_pNext;					// �O�̎��I�u�W�F�N�g�ݒ�
		}

		if (m_pNext != NULL)	// m_pCur�ȊO
		{	// ���I�u�W�F�N�g��NULL�ȊO
			if (m_pCur[nPriority] == m_pNext)
			{	// ���݃I�u�W�F�N�g���O�I�u�W�F�N�g�Ɠ���
				m_pCur[nPriority]->m_pPrev = m_pPrev;	// ���݂̑O�I�u�W�F�N�g�ݒ�
			}
			m_pNext->m_pPrev = m_pPrev;					// ���̑O�I�u�W�F�N�g�ݒ�
		}

		// �擪�I�u�W�F�N�g�������Ɠ���
		if (m_pTop[nPriority] == this) { m_pTop[nPriority] = m_pNext; }	// �擪�I�u�W�F�N�g�����ɍX�V

		// ���݃I�u�W�F�N�g�������Ɠ���
		if (m_pCur[nPriority] == this) { m_pCur[nPriority] = m_pPrev; }	// ���݃I�u�W�F�N�g��O�ɍX�V

		m_pPrev = NULL;	// �O�I�u�W�F�N�g
		m_pNext = NULL;	// ���I�u�W�F�N�g

		char *pObjName	= ConObjtype(objType);
		int nLenght		= (int)strlen(pObjName);
		char cObjName[32];
		for (int nCnt = 0; nCnt < nLenght; nCnt++)
		{
			cObjName[nCnt] = pObjName[nCnt];
		}
		cObjName[nLenght] = '\0';

		delete this;			// �j��
		m_nNumPri[nPriority]--;	// �v���C�I���e�B���������炷
		m_nNumAll--;			// ���������炷
	}
}

//=======================================================//
// �I�u�W�F�N�g�^�C�v�����ϊ�����(Scene)
//=======================================================//
char *CScene::ConObjtype(CScene::OBJTYPE objType)
{
	char cName[32];

	switch (objType)
	{
	case OBJTYPE_MESHFIELD		:wsprintf(cName, "Meshfield");		break;	// ���b�V���t�B�[���h
	case OBJTYPE_MESHWALL		:wsprintf(cName, "Meshwall");		break;	// ���b�V���E�H�[��
	case OBJTYPE_MESHCYLINDER	:wsprintf(cName, "Meshcylinder");	break;	// ���b�V���V�����_�[
	case OBJTYPE_BG				:wsprintf(cName, "Bg");				break;	// �w�i
	case OBJTYPE_ENEMY			:wsprintf(cName, "Enemy");			break;	// �G
	case OBJTYPE_PLAYER			:wsprintf(cName, "Player");			break;	// �v���C���[
	case OBJTYPE_BULLET			:wsprintf(cName, "Bullet");			break;	// �e
	case OBJTYPE_EFFECT			:wsprintf(cName, "Effect");			break;	// �G�t�F�N�g
	case OBJTYPE_NUMBER			:wsprintf(cName, "Number");			break;	// ����
	case OBJTYPE_SCORE			:wsprintf(cName, "Score");			break;	// �X�R�A
	case OBJTYPE_RANKING		:wsprintf(cName, "Ranking");		break;	// �����L���O
	case OBJTYPE_MENU			:wsprintf(cName, "Menu");			break;	// ���j���[
	case OBJTYPE_PAUSE			:wsprintf(cName, "Pause");			break;	// �|�[�Y
	case OBJTYPE_TITLE			:wsprintf(cName, "Title");			break;	// �^�C�g��
	case OBJTYPE_FADE			:wsprintf(cName, "Fade");			break;	// �t�F�[�h
	case OBJTYPE_CAMERA			:wsprintf(cName, "Camera");			break;	// �J����
	default						:wsprintf(cName, "Unknown");		break;	// ��L�ȊO
	}
	return &cName[0];
}