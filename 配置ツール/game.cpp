//=======================================================//
//
// �Q�[������ [game.cpp]
// Author : Shun Yokomichi
//
//=======================================================//

//*******************************************************//
// �w�b�_�t�@�C���̃C���N���[�h
//*******************************************************//
#include "game.h"			// �Q�[��
#include "manager.h"		// �}�l�[�W���[
#include "renderer.h"		// �����_���[
#include "sound.h"			// �T�E���h
#include "input.h"			// ����
#include "camera.h"			// �J����
#include "fade.h"			// �t�F�[�h
#include "meshfield.h"		// ���b�V���t�B�[���h
#include "time.h"			// �^�C��
#include "object.h"			// �z�u���f��
#include "scene3d.h"
#include "dismodel.h"			// �\�����f��
#include "disfloor.h"			// �\�����f��
#include "selecticon.h"		// �I���A�C�R��
#include "saveicon.h"		// �Z�[�u�A�C�R��
#include "debugproc.h"		// �f�o�b�N
#include "enemy.h"
#include "disenemy.h"
#include "item3D.h"
#include "disitem3d.h"
//*******************************************************//
// �}�N����`
//*******************************************************//
#define MAX_SOUND_SECOND	(120)		// �b�̍ő�
#define SOUND_SECOND		(120)		// �b�̍ő傩�瑫���b

//*******************************************************//
// �v���g�^�C�v�錾
//*******************************************************//

//*******************************************************//
// �O���[�o���ϐ�
//*******************************************************//

//*******************************************************//
// �ÓI�����o�ϐ�
//*******************************************************//
CMeshField *CGame::m_pMeshField[m_MeshFieldMax] = { NULL };
CGame::MODEGAME CGame::m_game = MODE_NONE;
CGame::OBJTEXT	CGame::m_text = {};
int CGame::m_nTabCount = 0;

//=======================================================//
// �R���X�g���N�^									(public)	*** CGame ***
//=======================================================//
CGame::CGame()
{
	for (int nCntb = 0; nCntb < TEXT_OBJ_MAX; nCntb++)
	{//	�e�L�X�g�̃I�u�W�F�N�g�̎��
		m_textState[nCntb].nCnter = NULL;
		for (int nCnt = 0; nCnt < OBJ_MAX; nCnt++)
		{//	�e�L�X�g�̒��g�̏�����
			m_textState[nCntb].nType[nCnt] = NULL;
			m_textState[nCntb].ncolType[nCnt] = NULL;
			m_textState[nCntb].pos[nCnt] = {};
			m_textState[nCntb].size[nCnt] = {};
			m_textState[nCntb].bUse[nCnt] = false;
		}
	}
	m_nSelect = 0;
	m_pSaid = NULL;
}

//=======================================================//
// �f�X�g���N�^										(public)	*** CGame ***
//=======================================================//
CGame::~CGame()
{

}

//=======================================================//
// ���[�h����							(public)	*** CBullet ***
//=======================================================//
HRESULT CGame::Load(void)
{
	return S_OK;
}

//=======================================================//
// �A�����[�h����								(public)	*** CBullet ***
//=======================================================//
void CGame::Unload(void)
{

}

//=======================================================//
// ����������										(public)	*** CGame ***
//=======================================================//
HRESULT CGame::Init(void)
{
	m_nTabCount = 0;
	ObjectLoad();					//	�A�C�e���̃e�L�X�g�ǂݍ���

	CCamera *pCamera = CManager::GetCamera();		// �J�����擾
	pCamera->SetPos(D3DXVECTOR3(0.0f, 90.0f, -400.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	TextRoad();

	CObject::Load(m_text.nTypeNum[0]);
	CMeshField::Load(m_text.nTypeNum[1]);		// ���b�V���t�B�[���h
	CEnemy::Load(m_text.nTypeNum[2]);			// �G
	CItem3D::Load();
	CDisItem3D::Load();

	for (int nCnt = 0; nCnt < m_text.nNum[0]; nCnt++)
	{	// �u���b�N�̐���
		CObject::Create(m_text.pos[0][nCnt], m_text.rot[0][nCnt], (CObject::OBJTYPE) m_text.nType[0][nCnt]);
	}

	for (int nCnt = 0; nCnt < m_text.nNum[1]; nCnt++)
	{
		CMeshField::Create(CMeshField::MESHTYPE_FIELD, D3DXVECTOR3(10.0f, 0.0f, 10.0f), m_text.pos[1][nCnt], m_text.rot[1][nCnt], m_text.size[nCnt], D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f), CMeshField::MESHTEX_NUMSIZE, m_text.nType[1][nCnt]);
	}

	for (int nCnt = 0; nCnt < m_text.nNum[2]; nCnt++)
	{	// �u���b�N�̐���
		CEnemy::Create(m_text.pos[2][nCnt], m_text.rot[2][nCnt], m_text.nType[2][nCnt]);
	}

	for (int nCnt = 0; nCnt < m_text.nNum[3]; nCnt++)
	{	// �u���b�N�̐���
		CItem3D::Create(m_text.pos[3][nCnt], m_text.rot[3][nCnt], (CItem3D::TYPE)m_text.nType[3][nCnt]);
	}

	// �\�����f���̓ǂݍ��݂Ɛ���
	CDisModel::Load(m_text.nTypeNum[0]);
	CDisModel::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f),D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);

	CDisFloor::Load(1);
	CDsiEnemy::Load(2);

	// �I���A�C�R���̓ǂݍ��݂Ɛ���
	CSelectIcon::Create(D3DXVECTOR3(m_text.pos[0][0].x, m_text.pos[0][0].y + 50.0f, m_text.pos[0][0].z), D3DXVECTOR3(10.0f, 10.0f, 0.0f));

	m_nSelect = 0;
	m_nObjNum = CObject::GetNumAll();
	m_nSelectSet = 0;
	return S_OK;
}

//=======================================================//
// �I������											(public)	*** CGame ***
//=======================================================//
void CGame::Uninit(void)
{
	CMeshField::Unload();
	CDisModel::Unload();
	CDisFloor::Unload();
	CItem3D::Unload();
	CDisItem3D::Unload();

	if (m_pSaid != NULL)
	{
		m_pSaid->Uninit();
		m_pSaid = NULL;
	}

	CScene::ReleaseMode();	// ���[�h�I��
}

//=======================================================//
// �X�V����											(public)	*** CGame ***
//=======================================================//
void CGame::Update(void)
{
	CDebugProc::Print(" TAB�L�[�����ő���ؑ�              : 0 / �J�������� , 1 / �I�u�W�F�N�g����\n");

	if (CManager::GetInputKeyboard()->GetKeyboardTrigger(DIK_TAB))
	{	// Tab�L�[�ő���ύX
		m_nTabCount = (m_nTabCount + 1) % 2;
	}

	if (m_nTabCount == 1)
	{
		int nNum = 0;
		if (m_nSelectSet == 0)
		{
			nNum = CObject::GetNum();		// ���i�K�̍ő吔�̎擾
		}
		else if (m_nSelectSet == 1)
		{
			nNum = CMeshField::GetNum();
		}
		else if (m_nSelectSet == 2)
		{
			nNum = CEnemy::GetNum();
		}
		else if (m_nSelectSet == 3)
		{
			nNum = CItem3D::GetNum();
		}

		if (CManager::GetInputKeyboard()->GetKeyboardTrigger(DIK_RSHIFT))
		{
			m_nSelectSet = (m_nSelectSet + 1) % 4;

			if (m_nSelectSet == 0)
			{	// ���f���̐���
				CDisItem3D::ChangeUninit();
				CDisModel::Create(CDisplay::GetPos(), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
			}
			else if (m_nSelectSet == 1)
			{	// ���̐���
				CDisModel::ChangeUninit();
				CDisFloor::Create(D3DXVECTOR3(110.0f, 0.0f, 15.0f), CDisplay::GetPos(), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXVECTOR3(10.0f, 0.0f, 10.0f), D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.7f), D3DXVECTOR2(1.0f, 1.0f), CSceneMesh::MESHTEX_NUMSIZE, 0);
			}
			else if (m_nSelectSet == 2)
			{	// �G�̐���
				CDisFloor::ChangeUninit();
				CDsiEnemy::Create(CDisplay::GetPos(), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
			}
			else if (m_nSelectSet == 3)
			{
				CDsiEnemy::ChangeUninit();
				CDisItem3D::Create(CDisplay::GetPos(), D3DXVECTOR3(0.0f, 0.0f, 0.0f), (CDisItem3D::TYPE)0);
			}
		}

		if (CManager::GetInputKeyboard()->GetKeyboardTrigger(DIK_F9))
		{	// �Z�[�u
			TextWrite();
		}

		// ���f���폜�̑I��
		DeleteSelect(nNum);

		// �폜
		if (CManager::GetInputKeyboard()->GetKeyboardTrigger(DIK_BACKSPACE))
		{	// BackSpace�L�[�����������ꍇ
			Delete(nNum);	// �u���b�N�̍폜��
		}

		CDebugProc::Print(" ���ݑI�����Ă���쐬��                : %d��\n", nNum);
		CDebugProc::Print(" �I��ԍ�  N or V                         : %d / %d�Ԗ�\n", m_nSelect, nNum);
		CDebugProc::Print(" BACK SPACE �폜                       : %d�Ԗڂ��폜\n", m_nSelect);
		CDebugProc::Print(" �Z�[�u                                        : F9�L�[����\n");
		CDebugProc::Print(" ��\���ɂ���                               : F1�L�[����\n");
		CDebugProc::Print(" �Eshift�L�[����(�������镨�̑I��) : %d��I��\n", m_nSelectSet);
	}

}

//=======================================================//
// �`�揈��
//=======================================================//
void CGame::Draw(void)
{
}

//=======================================================//
// �e�L�X�g�ǂݍ���
//=======================================================//
void CGame::ObjectLoad(void)
{
}

void CGame::SetStage(void)
{
	// �J�����ݒ�
	//D3DXVECTOR3 posVDest = CManager::GetCamera()->GetPosVDest();	// ���_�擾

}

//=======================================================//
// ��������											(public)	*** CGame ***
//=======================================================//
void CGame::PlayES(int nSoundNum)
{
	CSound *pSound = CManager::GetSound();
	if (nSoundNum == 0)
	{	// ���̉�
		//pSound->PlaySoundA(CSound::SOUND_LABEL_SE022);
		nSoundNum = -1;
	}
}

//*************************************************************
//	Text�ǂݍ���
//*************************************************************
void CGame::TextRoad()
{
	int nTypeNum = 0;		// ��ސ��̕ϐ�
	int nCntWord = 0;		// �����̃J�E���g
	int nTextCnt = 0;		// Text�̃J�E���g

	pFile = fopen("data/TEXT/obj/Map.txt", "r");		// �t�@�C����, r : �ǂݍ���

	if (pFile != NULL)
	{	// �w�肳�ꂽ�t�@�C�����J�����ꍇ
		do
		{	// �w�肵�������܂ŉ�
			pStrCur = ReadLine(pFile, &aLine[0], 0);
			strcpy(&aStr[0], pStrCur);

		} while (memcmp(&aStr[0], "MAX_OBJ = ", 10) != 0);	// �w�肵��������0�ȊO�Ȃ����

		// ������i�߂�
		pStrCur += 10;
		strcpy(&aStr[0], pStrCur);
		m_text.nNum[nTextCnt] = atoi(&aStr[0]);	// �z�u�ő吔����

		do
		{	// �w�肵�������܂ŉ�
			pStrCur = ReadLine(pFile, &aLine[0], 0);
			strcpy(&aStr[0], pStrCur);

		} while (memcmp(&aStr[0], "MAX_TYPE = ", 11) != 0);	// �w�肵��������0�ȊO�Ȃ����

		// ������i�߂�
		pStrCur += 11;
		strcpy(&aStr[0], pStrCur);
		m_text.nTypeNum[nTextCnt] = atoi(&aStr[0]);	// �z�u�ő吔����

		do
		{	// �w�肵�������܂ŉ�
			pStrCur = ReadLine(pFile, &aLine[0], 0);
			strcpy(&aStr[0], pStrCur);

			if (memcmp(&aStr[0], "MODEL_FILENAME = ", 17) == 0)
			{
				pStrCur += 17;
				strcpy(&aStr[0], pStrCur);
				do
				{	// ���f����������Ă���
					m_text.ModeName[nTextCnt][nTypeNum][nCntWord] = (char)pStrCur[nCntWord];
					nCntWord++;		// �J�E���g����
				} while (memcmp(&aStr[nCntWord], "\n", 1) != 0);

				nCntWord = 0;	// �����J�E���g��������
				nTypeNum++;		// ���f���̎�ނ𑝉�
			}

		} while (memcmp(&aStr[0], "UNLOAD", 6) != 0);	// �w�肵��������0�ȊO�Ȃ����
		nTextCnt++;


		// ���̔z�u���ǂݍ��� & ���
		do
		{	// �w�肵�������܂ŉ�
			pStrCur = ReadLine(pFile, &aLine[0], 0);
			strcpy(&aStr[0], pStrCur);

		} while (memcmp(&aStr[0], "MAX_FLOOR = ", 12) != 0);	// �w�肵��������0�ȊO�Ȃ����

		// ������i�߂�
		pStrCur += 12;
		strcpy(&aStr[0], pStrCur);
		m_text.nNum[nTextCnt] = atoi(&aStr[0]);	// �z�u�ő吔����

		// ����Texture��ނ̓ǂݍ��� & ���
		do
		{	// �w�肵�������܂ŉ�
			pStrCur = ReadLine(pFile, &aLine[0], 0);
			strcpy(&aStr[0], pStrCur);

		} while (memcmp(&aStr[0], "MAX_TYPE_TEX = ", 15) != 0);	// �w�肵��������0�ȊO�Ȃ����

		// ������i�߂�
		pStrCur += 15;
		strcpy(&aStr[0], pStrCur);
		m_text.nTypeNum[nTextCnt] = atoi(&aStr[0]);	// �z�u�ő吔����
		nTypeNum = 0;

		// ����Textuure���̓ǂݍ��� & ���
		do
		{	// �w�肵�������܂ŉ�
			pStrCur = ReadLine(pFile, &aLine[0], 0);
			strcpy(&aStr[0], pStrCur);

			if (memcmp(&aStr[0], "TEXTURE_NAME = ", 15) == 0)
			{
				pStrCur += 15;
				strcpy(&aStr[0], pStrCur);
				do
				{	// ���f����������Ă���
					m_text.ModeName[nTextCnt][nTypeNum][nCntWord] = (char)pStrCur[nCntWord];
					nCntWord++;		// �J�E���g����
				} while (memcmp(&aStr[nCntWord], "\n", 1) != 0);

				nCntWord = 0;	// �����J�E���g��������
				nTypeNum++;		// ���f���̎�ނ𑝉�
			}

		} while (memcmp(&aStr[0], "UNLOAD", 6) != 0);	// �w�肵��������0�ȊO�Ȃ����

		nTextCnt++;

		do
		{	// �w�肵�������܂ŉ�
			pStrCur = ReadLine(pFile, &aLine[0], 0);
			strcpy(&aStr[0], pStrCur);

		} while (memcmp(&aStr[0], "MAX_ENEMY = ", 12) != 0);	// �w�肵��������0�ȊO�Ȃ����

		// ������i�߂�
		pStrCur += 12;
		strcpy(&aStr[0], pStrCur);
		m_text.nNum[nTextCnt] = atoi(&aStr[0]);	// �z�u�ő吔����

		do
		{	// �w�肵�������܂ŉ�
			pStrCur = ReadLine(pFile, &aLine[0], 0);
			strcpy(&aStr[0], pStrCur);

		} while (memcmp(&aStr[0], "MAX_TYPE_ENEMY = ", 17) != 0);	// �w�肵��������0�ȊO�Ȃ����

		// ������i�߂�
		pStrCur += 17;
		strcpy(&aStr[0], pStrCur);
		m_text.nTypeNum[nTextCnt] = atoi(&aStr[0]);	// �z�u�ő吔����
		nTypeNum = 0;
		do
		{	// �w�肵�������܂ŉ�
			pStrCur = ReadLine(pFile, &aLine[0], 0);
			strcpy(&aStr[0], pStrCur);

			if (memcmp(&aStr[0], "MODEL_NAME = ", 13) == 0)
			{
				pStrCur += 13;
				strcpy(&aStr[0], pStrCur);
				do
				{	// ���f����������Ă���
					m_text.ModeName[nTextCnt][nTypeNum][nCntWord] = (char)pStrCur[nCntWord];
					nCntWord++;		// �J�E���g����
				} while (memcmp(&aStr[nCntWord], "\n", 1) != 0);

				nCntWord = 0;	// �����J�E���g��������
				nTypeNum++;		// ���f���̎�ނ𑝉�
			}

		} while (memcmp(&aStr[0], "UNLOAD", 6) != 0);	// �w�肵��������0�ȊO�Ȃ����

		nTextCnt++;

		do
		{	// �w�肵�������܂ŉ�
			pStrCur = ReadLine(pFile, &aLine[0], 0);
			strcpy(&aStr[0], pStrCur);

		} while (memcmp(&aStr[0], "MAX_ITEM = ", 11) != 0);	// �w�肵��������0�ȊO�Ȃ����
		// ������i�߂�
		pStrCur += 11;
		strcpy(&aStr[0], pStrCur);
		m_text.nNum[nTextCnt] = atoi(&aStr[0]);	// �z�u�ő吔����

		do
		{	// �w�肵�������܂ŉ�
			pStrCur = ReadLine(pFile, &aLine[0], 0);
			strcpy(&aStr[0], pStrCur);

		} while (memcmp(&aStr[0], "MAX_TYPE_ITEM = ", 16) != 0);	// �w�肵��������0�ȊO�Ȃ����

		// ������i�߂�
		pStrCur += 16;
		strcpy(&aStr[0], pStrCur);
		m_text.nTypeNum[nTextCnt] = atoi(&aStr[0]);	// �z�u�ő吔����
		nTextCnt = 0;
		do
		{
			do
			{	// �w�肵�������܂ŉ�
				pStrCur = ReadLine(pFile, &aLine[0], 0);
				strcpy(&aStr[0], pStrCur);

				if (memcmp(&aStr[0], "ARRANGE", 7) == 0)
				{	// �w�蕶���ɓ�������
					for (int nCnt = 0; nCnt < m_text.nNum[nTextCnt];)
					{	// �z�u����
						pStrCur = ReadLine(pFile, &aLine[0], 0);
						strcpy(&aStr[0], pStrCur);

						if (memcmp(&aStr[0], "OBJSET", 6) == 0)
						{	// �w�萔�ɓ�������
							do
							{	// �I�u�W�F�N�g�z�u���̓ǂݍ���
								pStrCur = ReadLine(pFile, &aLine[0], 0);
								strcpy(&aStr[0], pStrCur);

								if (memcmp(&aStr[0], "TYPE = ", 7) == 0)
								{	// �^�C�v���Ȃ�
									pStrCur += 7;
									strcpy(&aStr[0], pStrCur);
									m_text.nType[nTextCnt][nCnt] = (int)atof(pStrCur);
								}
								else if (memcmp(&aStr[0], "POS = ", 6) == 0)
								{	// �ʒu���Ȃ�
									pStrCur += 6;
									strcpy(&aStr[0], pStrCur);
									m_text.pos[nTextCnt][nCnt].x = (float)atof(pStrCur);		// pos.x�̏��

									pStrCur = SearchLine(pStrCur);
									strcpy(&aStr[0], pStrCur);
									m_text.pos[nTextCnt][nCnt].y = (float)atof(pStrCur);		// pos.y�̏��

									pStrCur = SearchLine(pStrCur);
									strcpy(&aStr[0], pStrCur);
									m_text.pos[nTextCnt][nCnt].z = (float)atof(pStrCur);		// pos.z�̏��
								}
								else if (memcmp(&aStr[0], "ROT = ", 6) == 0)
								{	// ��]���Ȃ�
									pStrCur += 6;
									strcpy(&aStr[0], pStrCur);
									m_text.rot[nTextCnt][nCnt].x = (float)atof(pStrCur);		// rot.x�̏��

									pStrCur = SearchLine(pStrCur);
									strcpy(&aStr[0], pStrCur);
									m_text.rot[nTextCnt][nCnt].y = (float)atof(pStrCur);		// rot.y�̏��

									pStrCur = SearchLine(pStrCur);
									strcpy(&aStr[0], pStrCur);
									m_text.rot[nTextCnt][nCnt].z = (float)atof(pStrCur);		// rot.z�̏��
								}
								else if (memcmp(&aStr[0], "SIZE = ", 7) == 0)
								{	// ��]���Ȃ�
									pStrCur += 7;
									strcpy(&aStr[0], pStrCur);
									m_text.size[nCnt].x = (float)atof(pStrCur);		// rot.x�̏��

									pStrCur = SearchLine(pStrCur);
									strcpy(&aStr[0], pStrCur);
									m_text.size[nCnt].y = (float)atof(pStrCur);		// rot.y�̏��

									pStrCur = SearchLine(pStrCur);
									strcpy(&aStr[0], pStrCur);
									m_text.size[nCnt].z = (float)atof(pStrCur);		// rot.z�̏��
								}


							} while (memcmp(&aStr[0], "OBJEND", 6) != 0);	// �w�肵��������0�ȊO�Ȃ����
							nCnt++;		// for���̃J�E���^�[�̑���
						}
					}
				}
			} while (memcmp(&aStr[0], "END_ARRANGE", 11) != 0);	// �w�肵��������0�ȊO�Ȃ����
			nTextCnt++;
		} while (nTextCnt != 4);
	}
	fclose(pFile);

}

//*************************************************************
//	Text���ǂݍ��ݏ���
//*************************************************************
char *CGame::ReadLine(FILE *pFile, char*pDst, int nPatten)
{
	bool bLine = false;
	while (bLine == false)
	{
		fgets(&aLine[0], 256, pFile);
		while (1)
		{
			if (memcmp(pDst, "\t", 1) == 0)
			{	//
				pDst += 1;
				while (1)
				{
					if (memcmp(pDst, "\t", 1) == 0)
					{
						pDst += 1;
					}
					else
					{
						break;
					}
				}
			}
			if (memcmp(pDst, "\0", 1) == 0)
			{
				pDst += 1;
				while (1)
				{
					if (memcmp(pDst, "\0", 1) == 0)
					{
						pDst += 1;
					}
					else
					{
						break;
					}
				}
			}
			else if (memcmp(pDst, "\n", 1) == 0)
			{
				bLine = true;
				break;
			}
			else if (memcmp(pDst, "#", 1) == 0)
			{
				bLine = true;
				break;
			}
			else
			{
				bLine = true;
				break;
			}
		}
	}
	return pDst;
}

//*************************************************************
//	�ǂݍ��݂����f�[�^�̓ǂݍ��ݏ���(pos, rot�Ȃ�)
//*************************************************************
char *CGame::SearchLine(char*pDst)
{
	while (1)
	{
		if (memcmp(pDst, ",", 1) == 0)
		{	// NULL
			pDst += 1;
			break;
		}
		else if (memcmp(pDst, "\0", 1) == 0)
		{
			pDst += 1;
			break;
		}
		else
		{
			pDst += 1;
		}
	}		// TAB�̏ꍇ

	return pDst;
}

//*************************************************************
//	�������݂ɕK�v�ȏ��̈����o��
//*************************************************************
int CGame::InfSave(int nCntData)
{
	int nNum = 0;

	if (nCntData == 0 || nCntData == 2 || nCntData == 3)
	{
		// �V�[��
		CScene *pScene = CScene::GetScene(CScene::PRIORITY_3);	// ���݁@
		CScene *pSceneNext = NULL;								// ��

		while (pScene != NULL)
		{	// NULL�ȊO�̏ꍇ
			pSceneNext = pScene->GetSceneNext();				// ����ۑ�
			if (pScene->GetObjType() == CScene::OBJTYPE_BLOCK && nCntData == 0|| pScene->GetObjType() == CScene::OBJTYPE_ENEMY && nCntData == 2 ||
				pScene->GetObjType() == CScene::OBJTYPE_ITEM3D&& nCntData == 3)
			{
				CSceneX *pBlock = (CSceneX*)pScene;			// SceneX�ɃL���X�g
				D3DXVECTOR3 ModelPos = pBlock->Getpos();	// X���f���̈ʒu���擾
				D3DXVECTOR3 ModelRot = pBlock->Getrot();	// X���f���̌������擾

				if (nCntData == 0)
				{
					CObject *pObject = ((CObject*)pScene);		// CObject�փL���X�g(�^�̕ύX)
					if (pObject != NULL)
					{
						int nType = pObject->GetType();
						m_text.nType[nCntData][nNum] = nType;					// ���f���ԍ����L�^

					}
				}
				else if (nCntData == 2)
				{
					CEnemy *pEnemy = ((CEnemy*)pScene);		// CObject�փL���X�g(�^�̕ύX)
					if (pEnemy != NULL)
					{
						int nType = pEnemy->GetType();
						m_text.nType[nCntData][nNum] = nType;					// ���f���ԍ����L�^

					}
				}
				else if (nCntData == 3)
				{
					CItem3D *pItem = (CItem3D*)pScene;			// SceneX�ɃL���X�g
					ModelPos = pItem->GetPos();	// X���f���̈ʒu���擾
					ModelRot = pItem->GetRot();	// X���f���̌������擾

					int nType = pItem->GetType();
					m_text.nType[nCntData][nNum] = nType;					// ���f���ԍ����L�^

				}

				m_text.pos[nCntData][nNum] = ModelPos;				// �ʒu�����L�^
				m_text.rot[nCntData][nNum] = ModelRot;				// ���������L�^
				nNum++;												// ���̉��Z
			}

			pScene = pSceneNext;		// ����ݒ�
		}
	}

	if (nCntData == 1)
	{
		// �V�[��
		CScene *pSceneField = CScene::GetScene(CScene::PRIORITY_1);	// ���݁@
		CScene *pSceneNextField = NULL;								// ��

		while (pSceneField != NULL)
		{	// NULL�ȊO�̏ꍇ
			pSceneNextField = pSceneField->GetSceneNext();				// ����ۑ�
			if (pSceneField->GetObjType() == CScene::OBJTYPE_MESHFIELD)
			{
				CSceneMesh *pField = (CSceneMesh*)pSceneField;			// SceneX�ɃL���X�g
				D3DXVECTOR3 Pos = pField->Getpos();	// X���f���̈ʒu���擾
				D3DXVECTOR3 Rot = pField->Getrot();	// X���f���̌������擾
				D3DXVECTOR3 size = pField->Getsize();

				CMeshField *pMesh = ((CMeshField*)pSceneField);		// CObject�փL���X�g(�^�̕ύX)
				if (pMesh != NULL)
				{
					int nType = pMesh->GetTex();
					m_text.nType[nCntData][nNum] = nType;					// ���f���ԍ����L�^

				}

				m_text.pos[nCntData][nNum] = Pos;				// �ʒu�����L�^
				m_text.rot[nCntData][nNum] = Rot;				// ���������L�^
				m_text.size[nNum] = size;
				nNum++;												// ���̉��Z
			}

			pSceneField = pSceneNextField;		// ����ݒ�
		}
	}
	return nNum;
}

//*************************************************************
//	Text��������
//*************************************************************
void CGame::TextWrite(void)
{
	int nNum[4] = {};		// �\�����Ă��鑍��

	for (int nCntData = 0; nCntData < 4; nCntData++)
	{
		nNum[nCntData] = InfSave(nCntData);
	}

	// ���b�V���t�B�[���h�̏����

	pFile = fopen("data/TEXT/obj/Map.txt", "w");		// �t�@�C����, r : �ǂݍ���

	if (pFile != NULL)
	{
		CSaveIcon::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 30.0f, 0.0f), D3DXVECTOR3(250.0f, 100.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
	}

	// �e�L�X�g�����e�L�X�g��ɕ\��
	fprintf(pFile, "#=====================================================\n");
	fprintf(pFile, "#\n");
	fprintf(pFile, "#		�I�u�W�F�N�g�̔z�u�e�L�X�g\n");
	fprintf(pFile, "#\n");
	fprintf(pFile, "#=====================================================\n");
	fprintf(pFile, "\n");

	// �I�u�W�F�N�g�̔z�u�����e�L�X�g��ɕ\��
	fprintf(pFile, "#=====================================================\n");
	fprintf(pFile, "#		�I�u�W�F�N�g�̔z�u��\n");
	fprintf(pFile, "#=====================================================\n");
	fprintf(pFile, "MAX_OBJ = %d		# �z�u�����ݒ�l��菭�Ȃ��ƊJ���Ȃ��̂ŗv����\n", nNum[0]);
	fprintf(pFile, "\n");

	// ���f���̎��
	fprintf(pFile, "#=====================================================\n");
	fprintf(pFile, "#		�I�u�W�F�N�g�̎��\n");
	fprintf(pFile, "#=====================================================\n");
	fprintf(pFile, "MAX_TYPE = %d		# �ő僂�f������%d�ł��B����Ȃ��ꍇ��CObject.h�̃}�N���̐��l��ς��Ă��������B\n", m_text.nTypeNum[0], MAX_MODEL);
	fprintf(pFile, "\n");

	// �I�u�W�F�N�g�̎�ނ��e�L�X�g��ɕ\��
	fprintf(pFile, "#=====================================================\n");
	fprintf(pFile, "#		�I�u�W�F�N�g��\n");
	fprintf(pFile, "#=====================================================\n");
	fprintf(pFile, "LOAD\n");
	for (int nCntName = 0; nCntName < m_text.nTypeNum[0]; nCntName++)
	{
		fprintf(pFile, "	MODEL_FILENAME = %s\n", &m_text.ModeName[0][nCntName][0]);
	}
	fprintf(pFile, "UNLOAD\n\n");

	// �I�u�W�F�N�g�̔z�u�����e�L�X�g��ɕ\��
	fprintf(pFile, "#=====================================================\n");
	fprintf(pFile, "#		���̔z�u��\n");
	fprintf(pFile, "#=====================================================\n");
	fprintf(pFile, "MAX_FLOOR = %d		# �z�u�����ݒ�l��菭�Ȃ��ƊJ���Ȃ��̂ŗv����\n", nNum[1]);
	fprintf(pFile, "\n");

	// ���f���̎��
	fprintf(pFile, "#=====================================================\n");
	fprintf(pFile, "#		���̎��\n");
	fprintf(pFile, "#=====================================================\n");
	fprintf(pFile, "MAX_TYPE_TEX = %d		# �ő僂�f������%d�ł��B����Ȃ��ꍇ��CObject.h�̃}�N���̐��l��ς��Ă��������B\n", m_text.nTypeNum[1], MAX_MODEL);
	fprintf(pFile, "\n");

	// �I�u�W�F�N�g�̎�ނ��e�L�X�g��ɕ\��
	fprintf(pFile, "#=====================================================\n");
	fprintf(pFile, "#		����Texture��\n");
	fprintf(pFile, "#=====================================================\n");
	fprintf(pFile, "LOAD\n");
	for (int nCntName = 0; nCntName < m_text.nTypeNum[1]; nCntName++)
	{
		fprintf(pFile, "	TEXTURE_NAME = %s\n", &m_text.ModeName[1][nCntName][0]);
	}
	fprintf(pFile, "UNLOAD\n\n");

	// �I�u�W�F�N�g�̔z�u�����e�L�X�g��ɕ\��
	fprintf(pFile, "#=====================================================\n");
	fprintf(pFile, "#		�G�̔z�u��\n");
	fprintf(pFile, "#=====================================================\n");
	fprintf(pFile, "MAX_ENEMY = %d		# �z�u�����ݒ�l��菭�Ȃ��ƊJ���Ȃ��̂ŗv����\n", nNum[2]);
	fprintf(pFile, "\n");

	// ���f���̎��
	fprintf(pFile, "#=====================================================\n");
	fprintf(pFile, "#		�G�̎��\n");
	fprintf(pFile, "#=====================================================\n");
	fprintf(pFile, "MAX_TYPE_ENEMY = %d		# �ő僂�f������%d�ł��B����Ȃ��ꍇ��CObject.h�̃}�N���̐��l��ς��Ă��������B\n", m_text.nTypeNum[2], MAX_MODEL);
	fprintf(pFile, "\n");

	// �I�u�W�F�N�g�̎�ނ��e�L�X�g��ɕ\��
	fprintf(pFile, "#=====================================================\n");
	fprintf(pFile, "#		�G�̃��f����\n");
	fprintf(pFile, "#=====================================================\n");
	fprintf(pFile, "LOAD\n");
	for (int nCntName = 0; nCntName < m_text.nTypeNum[2]; nCntName++)
	{
		fprintf(pFile, "	MODEL_NAME = %s\n", &m_text.ModeName[2][nCntName][0]);
	}
	fprintf(pFile, "UNLOAD\n\n");

	// �I�u�W�F�N�g�̔z�u�����e�L�X�g��ɕ\��
	fprintf(pFile, "#=====================================================\n");
	fprintf(pFile, "#		�󔠂̔z�u��\n");
	fprintf(pFile, "#=====================================================\n");
	fprintf(pFile, "MAX_ITEM = %d		# �z�u�����ݒ�l��菭�Ȃ��ƊJ���Ȃ��̂ŗv����\n", nNum[3]);
	fprintf(pFile, "\n");

	// ���f���̎��
	fprintf(pFile, "#=====================================================\n");
	fprintf(pFile, "#		�󔠂̎��\n");
	fprintf(pFile, "#=====================================================\n");
	fprintf(pFile, "MAX_TYPE_ITEM = %d		# �ő僂�f������%d�ł��B����Ȃ��ꍇ��CObject.h�̃}�N���̐��l��ς��Ă��������B\n", m_text.nTypeNum[3], MAX_MODEL);
	fprintf(pFile, "\n");


	// �I�u�W�F�N�g�̔z�u���e�L�X�g��ɕ\��
	fprintf(pFile, "#=====================================================\n");
	fprintf(pFile, "#		�I�u�W�F�N�g�̔z�u\n");
	fprintf(pFile, "#=====================================================\n");
	fprintf(pFile, "ARRANGE			# �z�u���̓ǂݍ��݊J�n(�����Ȃ�����)\n");

	for (int nCntModel = 0; nCntModel < nNum[0]; nCntModel++)
	{	// �\���������f������
		fprintf(pFile, "	OBJSET		# [%d]\n", nCntModel);
		fprintf(pFile, "		TYPE = %d				# ���f���^�C�v\n", m_text.nType[0][nCntModel]);
		fprintf(pFile, "		POS = %0.1f,%0.1f,%0.1f		# �ʒu���\n", m_text.pos[0][nCntModel].x, m_text.pos[0][nCntModel].y, m_text.pos[0][nCntModel].z);
		fprintf(pFile, "		ROT = %0.9f,%0.9f,%0.9f		# ��]���\n", m_text.rot[0][nCntModel].x, m_text.rot[0][nCntModel].y, m_text.rot[0][nCntModel].z);
		fprintf(pFile, "	OBJEND\n");
		fprintf(pFile, "\n");
	}

	// �t�@�C���ǂݍ��ݏI����1�s�\��
	fprintf(pFile, "END_ARRANGE		# �z�u���̓ǂݍ��ݏI��(�����Ȃ�����)\n");

	// �I�u�W�F�N�g�̔z�u���e�L�X�g��ɕ\��
	fprintf(pFile, "#=====================================================\n");
	fprintf(pFile, "#		���̔z�u\n");
	fprintf(pFile, "#=====================================================\n");
	fprintf(pFile, "ARRANGE			# �z�u���̓ǂݍ��݊J�n(�����Ȃ�����)\n");

	for (int nCntModel = 0; nCntModel < nNum[1]; nCntModel++)
	{	// �\���������f������
		fprintf(pFile, "	OBJSET		# [%d]\n", nCntModel);
		fprintf(pFile, "		TYPE = %d				# ���f���^�C�v\n", m_text.nType[1][nCntModel]);
		fprintf(pFile, "		POS = %0.1f,%0.1f,%0.1f		# �ʒu���\n", m_text.pos[1][nCntModel].x, m_text.pos[1][nCntModel].y, m_text.pos[1][nCntModel].z);
		fprintf(pFile, "		ROT = %0.9f,%0.9f,%0.9f		# ��]���\n", m_text.rot[1][nCntModel].x, m_text.rot[1][nCntModel].y, m_text.rot[1][nCntModel].z);
		fprintf(pFile, "		SIZE = %0.9f,%0.9f,%0.9f	# �T�C�Y���\n", m_text.size[nCntModel].x, m_text.size[nCntModel].y, m_text.size[nCntModel].z);
		fprintf(pFile, "	OBJEND\n");
		fprintf(pFile, "\n");
	}
	// �t�@�C���ǂݍ��ݏI����1�s�\��
	fprintf(pFile, "END_ARRANGE		# �z�u���̓ǂݍ��ݏI��(�����Ȃ�����)\n");

	// �I�u�W�F�N�g�̔z�u���e�L�X�g��ɕ\��
	fprintf(pFile, "#=====================================================\n");
	fprintf(pFile, "#		�G�̔z�u\n");
	fprintf(pFile, "#=====================================================\n");
	fprintf(pFile, "ARRANGE			# �z�u���̓ǂݍ��݊J�n(�����Ȃ�����)\n");

	for (int nCntModel = 0; nCntModel < nNum[2]; nCntModel++)
	{	// �\���������f������
		fprintf(pFile, "	OBJSET		# [%d]\n", nCntModel);
		fprintf(pFile, "		TYPE = %d				# ���f���^�C�v\n", m_text.nType[2][nCntModel]);
		fprintf(pFile, "		POS = %0.1f,%0.1f,%0.1f		# �ʒu���\n", m_text.pos[2][nCntModel].x, m_text.pos[2][nCntModel].y, m_text.pos[2][nCntModel].z);
		fprintf(pFile, "		ROT = %0.9f,%0.9f,%0.9f		# ��]���\n", m_text.rot[2][nCntModel].x, m_text.rot[2][nCntModel].y, m_text.rot[2][nCntModel].z);
		fprintf(pFile, "	OBJEND\n");
		fprintf(pFile, "\n");
	}
	// �t�@�C���ǂݍ��ݏI����1�s�\��
	fprintf(pFile, "END_ARRANGE		# �z�u���̓ǂݍ��ݏI��(�����Ȃ�����)\n");

	// �I�u�W�F�N�g�̔z�u���e�L�X�g��ɕ\��
	fprintf(pFile, "#=====================================================\n");
	fprintf(pFile, "#		�󔠂̔z�u\n");
	fprintf(pFile, "#=====================================================\n");
	fprintf(pFile, "ARRANGE			# �z�u���̓ǂݍ��݊J�n(�����Ȃ�����)\n");

	for (int nCntModel = 0; nCntModel < nNum[3]; nCntModel++)
	{	// �\���������f������
		fprintf(pFile, "	OBJSET		# [%d]\n", nCntModel);
		fprintf(pFile, "		TYPE = %d				# ���f���^�C�v\n", m_text.nType[3][nCntModel]);
		fprintf(pFile, "		POS = %0.1f,%0.1f,%0.1f		# �ʒu���\n", m_text.pos[3][nCntModel].x, m_text.pos[3][nCntModel].y, m_text.pos[3][nCntModel].z);
		fprintf(pFile, "		ROT = %0.9f,%0.9f,%0.9f		# ��]���\n", m_text.rot[3][nCntModel].x, m_text.rot[3][nCntModel].y, m_text.rot[3][nCntModel].z);
		fprintf(pFile, "	OBJEND\n");
		fprintf(pFile, "\n");
	}

	// �t�@�C���ǂݍ��ݏI����1�s�\��
	fprintf(pFile, "END_ARRANGE		# �z�u���̓ǂݍ��ݏI��(�����Ȃ�����)\n");

	fclose(pFile);		// �t�@�C�������

}

//*************************************************************
//	�폜
//*************************************************************
void CGame::Delete(int nMaxNum)
{
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;								// ��

	if (m_nSelectSet == 0 || m_nSelectSet == 2 || m_nSelect == 3)
	{
		pScene = CScene::GetScene(CScene::PRIORITY_3);	// ���݁@
	}
	else  if (m_nSelectSet == 1)
	{
		pScene = CScene::GetScene(CScene::PRIORITY_1);	// ���݁@
	}
	while (pScene != NULL)
	{	// NULL�ȊO�̏ꍇ
		pSceneNext = pScene->GetSceneNext();				// ����ۑ�
		if (pScene->GetObjType() == CScene::OBJTYPE_BLOCK && m_nSelectSet == 0)
		{	// �I�u�W�F�N�g�^�C�v���u���b�N�Ȃ�
			CObject *pObject = ((CObject*)pScene);		// CObject�փL���X�g(�^�̕ύX)
			int nObjNum = pObject->GetModelNum();		// CObject�̎��M�̔ԍ����L�^

			if (m_nSelect == nObjNum)
			{	// �����ԍ��Ȃ�
				m_nObjNum = nObjNum;	// �ԍ��̋L�^
				if (nMaxNum - 1 == m_nSelect)
				{	// (�ő吔 - 1)�ƑI��ԍ�������
					m_nSelect--;		// �I��ԍ���������
				}
				pObject->Uninit();		// �u���b�N��Uninit
			}

			if (m_nObjNum < nObjNum )
			{	// �L�^�����ԍ����u���b�N�̔ԍ����傫���ꍇ
				pObject->SetNum(nObjNum - 1);		// �u���b�N�̔ԍ����P�����炷
			}
		}
		else if (pScene->GetObjType() == CScene::OBJTYPE_MESHFIELD )
		{
			CMeshField *pMesh = ((CMeshField*)pScene);		// CObject�փL���X�g(�^�̕ύX)
			int nObjNum = pMesh->GetModelNum();		// CObject�̎��M�̔ԍ����L�^

			if (m_nSelect == nObjNum)
			{	// �����ԍ��Ȃ�
				m_nObjNum = nObjNum;	// �ԍ��̋L�^
				if (nMaxNum - 1 == m_nSelect)
				{	// (�ő吔 - 1)�ƑI��ԍ�������
					m_nSelect--;		// �I��ԍ���������
				}
				pMesh->Uninit();		// �u���b�N��Uninit
			}

			if (m_nObjNum < nObjNum)
			{	// �L�^�����ԍ����u���b�N�̔ԍ����傫���ꍇ
				pMesh->SetNum(nObjNum - 1);		// �u���b�N�̔ԍ����P�����炷
			}
		}
		if (pScene->GetObjType() == CScene::OBJTYPE_ENEMY&& m_nSelectSet == 2)
		{	// �I�u�W�F�N�g�^�C�v���u���b�N�Ȃ�
			CEnemy *pEnemy = ((CEnemy*)pScene);		// CObject�փL���X�g(�^�̕ύX)
			int nObjNum = pEnemy->GetModelNum();		// CObject�̎��M�̔ԍ����L�^

			if (m_nSelect == nObjNum)
			{	// �����ԍ��Ȃ�
				m_nObjNum = nObjNum;	// �ԍ��̋L�^
				if (nMaxNum - 1 == m_nSelect)
				{	// (�ő吔 - 1)�ƑI��ԍ�������
					m_nSelect--;		// �I��ԍ���������
				}
				pEnemy->Uninit();		// �u���b�N��Uninit
			}

			if (m_nObjNum < nObjNum)
			{	// �L�^�����ԍ����u���b�N�̔ԍ����傫���ꍇ
				pEnemy->SetNum(nObjNum - 1);		// �u���b�N�̔ԍ����P�����炷
			}
		}
		if (pScene->GetObjType() == CScene::OBJTYPE_ITEM3D&& m_nSelectSet == 3)
		{	// �I�u�W�F�N�g�^�C�v���u���b�N�Ȃ�
			CItem3D *pItem = ((CItem3D*)pScene);		// CObject�փL���X�g(�^�̕ύX)
			int nObjNum = pItem->GetModelNum();		// CObject�̎��M�̔ԍ����L�^

			if (m_nSelect == nObjNum)
			{	// �����ԍ��Ȃ�
				m_nObjNum = nObjNum;	// �ԍ��̋L�^
				if (nMaxNum - 1 == m_nSelect)
				{	// (�ő吔 - 1)�ƑI��ԍ�������
					m_nSelect--;		// �I��ԍ���������
				}
				pItem->Uninit();		// �u���b�N��Uninit
			}

			if (m_nObjNum < nObjNum)
			{	// �L�^�����ԍ����u���b�N�̔ԍ����傫���ꍇ
				pItem->SetNum(nObjNum - 1);		// �u���b�N�̔ԍ����P�����炷
			}
		}

		pScene = pSceneNext;		// ����ݒ�
	}

	SelectIcon();					// ���̃��f���̍��W����
}

//*************************************************************
//	�I���폜
//*************************************************************
void CGame::DeleteSelect(int nMaxNum)
{
	int MaxNum = nMaxNum;

	if (m_nSelect <= -1)
	{	// �I��ԍ���-1�ȉ��̏ꍇ
		m_nSelect = 0;
	}
	// �I��
	if (CManager::GetInputKeyboard()->GetKeyboardTrigger(DIK_N))
	{	// N�L�[�����Ői�߂�
		if (MaxNum == 0) { MaxNum = 1; }
		m_nSelect = (m_nSelect + 1) % MaxNum;
		SelectIcon();							// �I���A�C�R�����W���
	}
	else if (CManager::GetInputKeyboard()->GetKeyboardTrigger(DIK_V))
	{	// V�L�[�����Ŗ߂�
		if (MaxNum == 0) { MaxNum = 1; }
		m_nSelect = (m_nSelect + (MaxNum - 1)) % MaxNum;
		SelectIcon();							// �I���A�C�R�����W���
	}

}

//*************************************************************
//	�I���A�C�R�����W�������
//*************************************************************
void CGame::SelectIcon(void)
{
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;								// ��

	if (m_nSelectSet == 0 || m_nSelectSet == 2 || m_nSelect == 3)
	{
		pScene = CScene::GetScene(CScene::PRIORITY_3);	// ���݁@
	}
	else if (m_nSelectSet == 1)
	{
		pScene = CScene::GetScene(CScene::PRIORITY_1);	// ���݁@
	}
	while (pScene != NULL)
	{	// NULL�ȊO�̏ꍇ
		pSceneNext = pScene->GetSceneNext();				// ����ۑ�
		if (pScene->GetObjType() == CScene::OBJTYPE_BLOCK && m_nSelectSet == 0)
		{	// �I�u�W�F�N�g�^�C�v���u���b�N�Ȃ�
			CObject *pObject = ((CObject*)pScene);		// CObject�փL���X�g(�^�̕ύX)
			int nObjNum = pObject->GetModelNum();		// CObject�̎��M�̔ԍ����L�^

			if (m_nSelect == nObjNum)
			{	// �����ԍ��Ȃ�
				CSceneX *pSceneX = ((CSceneX*)pObject);		// CSceneX�փL���X�g(�^�̕ύX)
				D3DXVECTOR3 ModelPos = pSceneX->Getpos();
				D3DXVECTOR3 size = pSceneX->Getsize(0);
				CSelectIcon::Setpos(D3DXVECTOR3(ModelPos.x, size.y + ModelPos.y + 20.0f, ModelPos.z));		// X���f���̈ʒu�擾
			}
		}
		if (pScene->GetObjType() == CScene::OBJTYPE_ENEMY&& m_nSelectSet == 2)
		{	// �I�u�W�F�N�g�^�C�v���u���b�N�Ȃ�
			CEnemy *pEnemy = ((CEnemy*)pScene);		// CObject�փL���X�g(�^�̕ύX)
			int nObjNum = pEnemy->GetModelNum();		// CObject�̎��M�̔ԍ����L�^

			if (m_nSelect == nObjNum)
			{	// �����ԍ��Ȃ�
				CSceneX *pSceneX = ((CSceneX*)pEnemy);		// CSceneX�փL���X�g(�^�̕ύX)
				D3DXVECTOR3 ModelPos = pSceneX->Getpos();
				D3DXVECTOR3 size = pSceneX->Getsize(0);
				CSelectIcon::Setpos(D3DXVECTOR3(ModelPos.x, size.y + ModelPos.y + 20.0f, ModelPos.z));		// X���f���̈ʒu�擾
			}
		}
		if (pScene->GetObjType() == CScene::OBJTYPE_ITEM3D)
		{	// �I�u�W�F�N�g�^�C�v���u���b�N�Ȃ�
			CItem3D *pItem = ((CItem3D*)pScene);		// CObject�փL���X�g(�^�̕ύX)
			int nObjNum = pItem->GetModelNum();		// CObject�̎��M�̔ԍ����L�^

			if (m_nSelect == nObjNum)
			{	// �����ԍ��Ȃ�
				D3DXVECTOR3 ModelPos = pItem->GetPos();
				CSelectIcon::Setpos(D3DXVECTOR3(ModelPos.x, ModelPos.y + 20.0f, ModelPos.z));		// X���f���̈ʒu�擾
			}
		}
		if (pScene->GetObjType() == CScene::OBJTYPE_MESHFIELD)
		{	// �I�u�W�F�N�g�^�C�v���u���b�N�Ȃ�
			CMeshField *pMesh = ((CMeshField*)pScene);		// CObject�փL���X�g(�^�̕ύX)
			int nObjNum = pMesh->GetModelNum();		// CObject�̎��M�̔ԍ����L�^

			if (m_nSelect == nObjNum)
			{	// �����ԍ��Ȃ�
				pMesh->SetCol(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
				CSelectIcon::Setpos(D3DXVECTOR3(pMesh->Getpos().x, pMesh->Getpos().y + 20.0f, pMesh->Getpos().z));		// X���f���̈ʒu�擾
			}
		}
		pScene = pSceneNext;		// ����ݒ�
	}
}