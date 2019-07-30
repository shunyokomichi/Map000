//=======================================================//
//
// �Q�[������ [game.h]
// Author : Shun Yokomichi
//
//=======================================================//
#ifndef _GAME_H_
#define _GAME_H_

//*******************************************************//
// �w�b�_�t�@�C���̃C���N���[�h
//*******************************************************//
#include "main.h"		// ���C��
#include "manager.h"	// �}�l�[�W���[

//*******************************************************//
// �}�N����`
//*******************************************************//
#define OBJ_MAX (1800)
#define TEXT_OBJ_MAX (3)		//	�e�L�X�g�̒��̃I�u�W�F�N�g�̍ő吔
#define MAX_TEXT		(4)		// Text�̍ő吔

//*******************************************************//
// �O���錾
//*******************************************************//
class CEnemy;
class CMeshField;
class CLife2D;
class CDisModel;
class CDisModel;

//*******************************************************//
// �N���X��`
//*******************************************************//
class CGame
{// �Q�[��
public:
	typedef struct
	{
		int				nCnter;					//	�I�u�W�F�N�g�̐��𐔂���
		D3DXVECTOR3		pos[OBJ_MAX];			//	�I�u�W�F�N�g�|�W�V�����̐ݒ�
		int				nType[OBJ_MAX];			//	�I�u�W�F�N�g��ނ̐ݒ�
		int				ncolType[OBJ_MAX];		//	�I�u�W�F�N�g����̐ݒ�
		D3DXVECTOR3		size[OBJ_MAX];			//	�I�u�W�F�N�g�T�C�Y�̐ݒ�
		bool			bUse[OBJ_MAX];			// �g������
	}TEXTSTATE;

	typedef struct
	{	// TEXT�̏��(���ǉ���͋L�����邱�ƋL�����邱�Ɓ�)
		int nNum[MAX_TEXT];					// �z�u��
		int nTypeNum[MAX_TEXT];				// ���f���̎��
		D3DXVECTOR3 pos[MAX_TEXT][OBJ_MAX];	// �ʒu���
		D3DXVECTOR3 rot[MAX_TEXT][OBJ_MAX];	// ��]���
		D3DXVECTOR3 size[OBJ_MAX];
		D3DXVECTOR3 MeshNum[OBJ_MAX];
		int nType[MAX_TEXT][OBJ_MAX];			// �^�C�v
		char ModeName[MAX_TEXT][20][256];
	}OBJTEXT;



	typedef enum
	{	// �Q�[���̏��(CLEAR & GameOver)
		MODE_NONE = 0,
		MODE_GAMEOVER000,		// ���l�ɕ߂܂�
		MODE_GAMEOVER001,		// �̗̓[��
		MODE_GAMECLEAR,			// ����
		MODE_ITEM,
		MODE_MAX
	}MODEGAME;

	static const int m_MeshFieldMax = 1;
	static const int m_MeshRiverMax = 4;

	CGame();						// �R���X�g���N�^
	~CGame();						// �f�X�g���N�^

	static HRESULT Load(void);		// ���[�h
	static void Unload(void);		// �A�����[�h

	HRESULT Init(void);				// ����������
	void Uninit(void);				// �I������
	void Update(void);				// �X�V����
	void Draw(void);				// �`�揈��

	void ObjectLoad(void);			// �I�u�W�F�N�g�̓ǂݍ���
	void SetStage(void);
	void PlayES(int nSoundNum);		// �T�E���h�̔ԍ��Ŋ������o��

	static void SetMeshField(CMeshField *pMeshField)
	{
		for (int nCnt = 0; nCnt < m_MeshFieldMax; nCnt++)
		{
			if (m_pMeshField[nCnt] == pMeshField)
			{
				m_pMeshField[nCnt] = NULL;
				break;
			}
		}
	}
	static CMeshField *GetMeshField(int nNum) { return m_pMeshField[nNum]; }

	// �Q�[���̏�Ԏ擾�Ɛݒ�
	static void SetGame(MODEGAME pGame) { m_game = pGame; };
	static MODEGAME GetGame() { return m_game; };

	// �e�L�X�g�t�@�C���ǂݍ���
	void TextRoad();
	char *ReadLine(FILE *pFile, char*pDst, int nPatten);
	char *SearchLine(char*pDst);

	// �e�L�X�g�t�@�C����������
	void TextWrite(void);
	int InfSave(int nCntData);

	void Delete(int nMaxNum);		// �폜
	void DeleteSelect(int nMaxNum);	// �폜�I��
	void SelectIcon(void);			// �I���A�C�R���̍��W����

	static char *GetName(int nNum, int nTextNum) { return &m_text.ModeName[nTextNum][nNum][0]; }		// ���f�����̎擾
	static int GetNum(int nTextNum) { return m_text.nTypeNum[nTextNum]; }							// ���f���ő吔�̎擾
	static int GetTabCount(void) { return m_nTabCount; }

private:
	static CMeshField *m_pMeshField[m_MeshFieldMax];
	static int m_nTabCount;

	TEXTSTATE m_textState[TEXT_OBJ_MAX];		//	�e�L�X�g�ǂݍ��ޕϐ�[�ǂݍ��ރI�u�W�F�N�g�̎��]
	static MODEGAME m_game;

	int m_nCnter;

	FILE *pFile;
	char *pStrCur;
	char aLine[256];
	char aStr[256];
	int m_objNum = 0;
	static OBJTEXT m_text;

	CLife2D *m_pLife2D;
	int m_nSelect;
	int m_nObjNum;
	int m_nSelectSet;
	CDisModel *m_pSaid;


protected:

};

#endif