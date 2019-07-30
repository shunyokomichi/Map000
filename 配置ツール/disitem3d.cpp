//=============================================================================
//
// �A�C�e��3D�\������ [disitem3D.cpp]
// Author : Shun Yokomichi
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS

#include "disitem3d.h"
#include "renderer.h"			// �����_���[
#include "manager.h"			// �}�l�[�W���[
#include "input.h"				// ����
#include "model.h"				// ���f��
#include "game.h"				// �Q�[��
#include "sound.h"				// �T�E���h
#include "display.h"
#include "debugproc.h"
#include "selecticon.h"
//*********************************************************************
//	�O���[�o���錾
//*********************************************************************

//*********************************************************************
//	�}�N����`
//*********************************************************************
#define ITEM000_0	"data/MODEL/item/Chest000_0.x"			// ���f����(�ӂ�����)
#define ITEM000_1	"data/MODEL/item/Chest000_1.x"			// ���f����(������)
#define AREA_RADIUS	(60.0f)									// �G���A�̔��a

//*********************************************************************
//	�ÓI�����o�ϐ�
//*********************************************************************
LPD3DXMESH				CDisItem3D::m_pMesh[MAX_ITEM_PARTS] = {};			// ���b�V���̃|�C���^
LPD3DXBUFFER			CDisItem3D::m_pBuffMat[MAX_ITEM_PARTS] = {};		// �o�b�t�@�̃|�C���^
DWORD					CDisItem3D::m_nNumMat[MAX_ITEM_PARTS] = {};		// �}�b�g�̃|�C���^
LPDIRECT3DTEXTURE9		*CDisItem3D::m_pTexture[MAX_ITEM_PARTS] = {};		// �e�N�X�`�����ւ̃|�C���^
int						CDisItem3D::m_nNum = 0;						// �����z�u���̏��

//*********************************************************************
//	�A�C�e���̐���
//*********************************************************************
CDisItem3D *CDisItem3D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type)
{
	CDisItem3D *pItem3D = NULL;

	// �V�[���̐���
	if (pItem3D == NULL)
	{
		pItem3D = new CDisItem3D;	// ���I�m��
		if (pItem3D != NULL)
		{
			pItem3D->Init();				// ��������
			pItem3D->SetModel(pos, rot);	// ���f���̈ʒu & �����ݒ�
			pItem3D->SetType(type);			// ��ނ̐ݒ�
		}
	}

	return pItem3D;
}

//*********************************************************************
//	�R���X�g���N�^
//*********************************************************************
CDisItem3D::CDisItem3D(int nPriority, OBJTYPE objType): CScene(3, objType)
{
}

//*********************************************************************
//	�f�X�g���N�^
//*********************************************************************
CDisItem3D::~CDisItem3D() {}

//*********************************************************************
//		�e�N�X�`���̓ǂݍ���
//*********************************************************************
HRESULT CDisItem3D::Load(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(ITEM000_0, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[0], NULL, &m_nNumMat[0], &m_pMesh[0]);
	D3DXLoadMeshFromX(ITEM000_1, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[1], NULL, &m_nNumMat[1], &m_pMesh[1]);

	for (int nCnt = 0; nCnt < MAX_ITEM_PARTS; nCnt++)
	{
		D3DXMATERIAL *pmat;							// �}�e���A���f�[�^�ւ̃|�C���^
		D3DMATERIAL9 *matDef;						// ���݂̃}�e���A���ۑ��p
		pmat = (D3DXMATERIAL*)m_pBuffMat[nCnt]->GetBufferPointer();
		matDef = new D3DMATERIAL9[m_nNumMat[nCnt]];
		m_pTexture[nCnt] = new LPDIRECT3DTEXTURE9[m_nNumMat[nCnt]];

		for (DWORD tex = 0; tex < m_nNumMat[nCnt]; tex++)
		{// �J�E���g
			matDef[tex] = pmat[tex].MatD3D;
			matDef[tex].Ambient = matDef[tex].Diffuse;
			m_pTexture[nCnt][tex] = NULL;
			if (pmat[tex].pTextureFilename != NULL &&
				lstrlen(pmat[tex].pTextureFilename) > 0)
			{// �e�N�X�`�����g�p���Ă���
				if (FAILED(D3DXCreateTextureFromFile(pDevice,
					pmat[tex].pTextureFilename,
					&m_pTexture[nCnt][tex])))
				{
				}
			}
		}

		delete[] matDef;
		matDef = NULL;
	}

	return S_OK;
}

//*********************************************************************
//		�e�N�X�`���̊J��
//*********************************************************************
void CDisItem3D::Unload(void)
{
	for (int nCntModel = 0; nCntModel < MAX_ITEM_PARTS; nCntModel++)
	{
		for (DWORD tex = 0; tex < m_nNumMat[nCntModel]; tex++)
		{// �J�E���g
			if (m_pTexture[nCntModel][tex] != NULL)
			{// NULL�ȊO�̏ꍇ
				m_pTexture[nCntModel][tex]->Release();		// ���
				m_pTexture[nCntModel][tex] = NULL;			// NULL��
			}
		}

		// ���b�V���̊J��
		if (m_pMesh[nCntModel] != NULL)
		{
			m_pMesh[nCntModel]->Release();
			m_pMesh[nCntModel] = NULL;
		}
		// �}�e���A���̊J��
		if (m_pBuffMat[nCntModel] != NULL)
		{
			m_pBuffMat[nCntModel]->Release();
			m_pBuffMat[nCntModel] = NULL;
		}
	}
}

//*********************************************************************
// ����������
//*********************************************************************
HRESULT CDisItem3D::Init()
{
	// ���ꂼ��̏�����
	// Text�̓ǂݍ���
	TxtLoad();

	CDisplay::SetSize(SetModelSize(CDisplay::TYPE_SIZE_MIN), SetModelSize(CDisplay::TYPE_SIZE_MAX));
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ����
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	CDisplay::SetChange(D3DXVECTOR2(1.0f, 1.0f));

	return S_OK;
}

//*********************************************************************
// �I������
//*********************************************************************
void CDisItem3D::Uninit(void)
{
	for (int nCntParts = 0; nCntParts < MAX_ITEM_PARTS; nCntParts++)
	{
		if (m_pModel[nCntParts] != NULL)
		{	// NULL�ȊO�Ȃ�
			m_pModel[nCntParts]->Uninit();		// �I������
			delete m_pModel[nCntParts];			// �������J��
			m_pModel[nCntParts] = NULL;			// NULL��
		}
	}

	CScene::SetDeath();						// �I������
}

//*********************************************************************
// �X�V����
//*********************************************************************
void CDisItem3D::Update(void)
{
	// ��{����(�ړ�, ����, ���Z�b�g)
	BasicOperation();			// �ړ�����
	BasicSetRot();				// �����ݒ菈��
	ResetButton();				// �ʒu�A��]���Z�b�g

	// Enter�L�[�őI�������I�u�W�F�N�g�𐶐�
	ModelCreate();

	// �f�o�b�N
	CDebugProc::Print("**********************************************************\n");
	CDebugProc::Print(" �󔠂̔z�u\n");																	// ���f���z�u�ݒ�
	CDebugProc::Print(" ENTER ���f������             : %d �Ԗڂ𐶐�\n", m_nNum);						// �������郂�f���ԍ�
	// ���f���I������
	SelectType();

	// �ʒu�ݒ� & �����ݒ�
	for (int nCntParts = 0; nCntParts < MAX_ITEM_PARTS; nCntParts++)
	{
		if (m_pModel[nCntParts] != NULL)
		{
			m_pModel[nCntParts]->Setcol(D3DXCOLOR(m_col.r, m_col.g, m_col.b, 0.5f));
		}
	}

	if (CDisplay::m_bUse == true)
	{	// �\������̂��؂�ւ�鎞
		CDisplay::m_bUse = false;
		Uninit();
	}

}

//*********************************************************************
// �`�揈��
//*********************************************************************
void CDisItem3D::Draw(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X
	// �V���h�E���e�̏���
	// ���[�J���ϐ��錾
	D3DXMATRIX mtxShadow;		// �V���h�E�}�g���b�N�X
	D3DXPLANE  planeField;		// ���ʂ̒l������ϐ�
	D3DXVECTOR4 vecLight;		// ���C�g�̒l
	D3DXVECTOR3 pos, normal;	// ���ʂ����߂鎞�ɕK�v�ϐ�

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, CDisplay::m_rot.y, CDisplay::m_rot.x, CDisplay::m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, CDisplay::m_pos.x, CDisplay::m_pos.y, CDisplay::m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	for (int nCntParts = 0; nCntParts < MAX_ITEM_PARTS; nCntParts++)
	{
		if (m_pModel[nCntParts] != NULL)
		{
			// MODEL�̕`��
			m_pModel[nCntParts]->Draw();
		}
	}
}

//*****************************************************************************
// ���f������
//*****************************************************************************
void CDisItem3D::ModelCreate()
{
	if (CManager::GetInputKeyboard()->GetKeyboardTrigger(DIK_RETURN))
	{
		CItem3D *pObject = NULL;		//	CObject��NULL��

		if (pObject == NULL)
		{	// NULL�Ȃ�
			pObject = CItem3D::Create(CDisplay::m_pos, CDisplay::m_rot, (CItem3D::TYPE) m_nNum);		// �I�u�W�F�N�g�𐶐�
			if (m_nNum != 0)
			{
				pObject->SetCol(m_col);
			}
			int nNum = CItem3D::GetNum();											// �ő�z�u�����擾
		}
	}
}

//*****************************************************************************
// ���f���I��
//*****************************************************************************
void CDisItem3D::SelectType(void)
{
	if (CManager::GetInputKeyboard()->GetKeyboardTrigger(DIK_LEFT) == true)
	{	// ���L�[�����̏ꍇ
		m_nNum = (m_nNum + ((int)CDisItem3D::TYPE_MAX - 1)) % (int)CDisItem3D::TYPE_MAX;		// �I�����f����ς���
	}
	else if (CManager::GetInputKeyboard()->GetKeyboardTrigger(DIK_RIGHT) == true)
	{	// ���L�[�����̏ꍇ
		m_nNum = (m_nNum + 1) % (int)CDisItem3D::TYPE_MAX;		// �I�����f����ς���
	}

	SetCol(m_nNum);

	CDebugProc::Print(" �� or �� ���g�I��             : %d / %d ��\n", m_nNum, (int)CDisItem3D::TYPE_MAX);		// �I��ł郂�f���ԍ�
	if (m_nNum == 0) { CDebugProc::Print("                                       ���g����\n"); }
	if (m_nNum == 1) { CDebugProc::Print("                                       �J�M�������Ă�\n"); }
	if (m_nNum == 2) { CDebugProc::Print("                                       �񕜂���\n"); }
	if (m_nNum == 3) { CDebugProc::Print("                                       ��������\n"); }

}

//*****************************************************************************
// �F�ݒ�
//*****************************************************************************
void CDisItem3D::SetCol(int nNum)
{
	if (nNum == 0) { m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); }	// �J�M�������
	if (nNum == 1) { m_col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);}	// �J�M�������
	if (nNum == 2) { m_col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f); }	// �񕜂������
	if (nNum == 3) { m_col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f); }	// �����������
}


//*********************************************************************
// �ݒ菈��
//*********************************************************************
void CDisItem3D::SetModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	m_pos = pos;		// pos����
	m_rot = rot;		// rot����
}

//*****************************************************************************
// �T�C�Y���擾
//*****************************************************************************
D3DXVECTOR3 CDisItem3D::SetModelSize(int nSize)
{
	D3DXVECTOR3 size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ���f���̃T�C�Y�擾�p
	CSceneX *pSceneX = NULL;							// CSceneX�̃|�C���^

	if (pSceneX == NULL)
	{	// NULL�Ȃ�
		pSceneX = new CSceneX(PRIORITY_3, CScene::OBJTYPE_ITEM3D);	// ���I�m��

		if (pSceneX != NULL)
		{	// NULL����Ȃ��Ȃ�
			pSceneX->SetSize(m_pMesh[0]);		// �T�C�Y�̐ݒ�
			size = pSceneX->Getsize(nSize);			// ���f���̃T�C�Y�擾(0 : �ő�l / 1 : �ŏ��l)
			pSceneX->Uninit();						// �I������
			pSceneX = NULL;							// NULL��
		}
	}
	return size;
}


//*********************************************************************
//	Text���ǂݍ��ݏ���
//*********************************************************************
char *CDisItem3D::ReadLine(FILE *pFile, char*pDst, int nPatten)
{
	bool bLine = false;		// �ǂݍ��݂���߂邩�ǂ����̃t���O
	while (bLine == false)
	{
		fgets(&aLine[0], 256, pFile);	// 1�s�ǂݍ���
		while (1)
		{
			if (memcmp(pDst, "\t", 1) == 0)
			{	// �^�u�L�[�Ȃ�
				pDst += 1; // 1�����i�߂�
				while (1)
				{
					if (memcmp(pDst, "\t", 1) == 0)
					{	// �^�u�Ȃ�
						pDst += 1;	// 1�����i�߂�
					}
					else
					{
						break;		// �I��
					}
				}
			}
			if (memcmp(pDst, "\0", 1) == 0)
			{	// �X�y�[�X�Ȃ�
				pDst += 1;	// 1�����i�߂�
				while (1)
				{
					if (memcmp(pDst, "\0", 1) == 0)
					{	// �X�y�[�X�Ȃ�
						pDst += 1;	// 1�����i�߂�
					}
					else
					{
						break;		// �I��
					}
				}
			}
			else if (memcmp(pDst, "\n", 1) == 0)
			{	// ���s�Ȃ�
				bLine = true;	// �t���O��true��(�ǂݍ��ݏI��)
				break;
			}
			else if (memcmp(pDst, "#", 1) == 0)
			{	// #�Ȃ�
				bLine = true;	// �R�����g����(�ǂݍ��ݏI��)
				break;
			}
			else
			{	// ����ȊO�̏ꍇ
				bLine = true;	// �ǂݍ��ݏI��
				break;
			}
		}
	}
	return pDst;	// �ǂݍ��񂾂P�s��Ԃ�

}

//*********************************************************************
//	�ǂݍ��݂����f�[�^�̓ǂݍ��ݏ���(pos, rot�Ȃ�)
//*********************************************************************
char *CDisItem3D::SearchLine(char*pDst)
{
	while (1)
	{
		if (memcmp(pDst, ",", 1) == 0)
		{	// �R���}�Ȃ�
			pDst += 1;	// 1�����i�߂�
			break;
		}
		else if (memcmp(pDst, "\0", 1) == 0)
		{	// �X�y�[�X�Ȃ�
			pDst += 1;		// 1�����i�߂�
			break;
		}
		else
		{	// ����ȊO�Ȃ�
			pDst += 1;		// �P�����i�߂�
		}
	}

	return pDst;	// �ǂݍ���1�s��ς���
}

//*********************************************************************
//	�ǂݍ��݂����f�[�^�̓ǂݍ��ݏ���(pos, rot�Ȃ�)
//*********************************************************************
void CDisItem3D::TxtLoad()
{
	pFile = fopen("data/ItemMotion.txt", "r");		// �t�@�C����, r : �ǂݍ���

	if (pFile != NULL)
	{	// �t�@�C����NULL�ȊO�Ȃ�
		pStrCur = ReadLine(pFile, &aLine[0], 0);
		strcpy(&aStr[0], pStrCur);

		if (memcmp(&aStr[0], "CHARACTERSET", 12) == 0)
		{	// �w�蕶�ɓ�������
			do
			{	// �w�蕶���o��܂ŉ�
				pStrCur = ReadLine(pFile, &aLine[0], 0);
				strcpy(&aStr[0], pStrCur);
			} while (memcmp(&aStr[0], "NUM_PARTS = ", 12) != 0);	// �J���������������Ή���
		}

		// ������i�߂�
		pStrCur += 12;
		strcpy(&aStr[0], pStrCur);
		m_motionItem.nParts = atoi(&aStr[0]);	// �p�[�c������

		do
		{
			pStrCur = ReadLine(pFile, &aLine[0], 0);
			strcpy(&aStr[0], pStrCur);

			if (memcmp(&aStr[0], "START", 5) == 0)
			{	// �w�蕶�ɓ�������
				for (int nCnt = 0; nCnt < m_motionItem.nParts;)
				{
					pStrCur = ReadLine(pFile, &aLine[0], 0);
					strcpy(&aStr[0], pStrCur);

					if (memcmp(&aStr[0], "PARTSSET", 8) == 0)
					{	// �w�蕶�ɓ�������
						do
						{
							pStrCur = ReadLine(pFile, &aLine[0], 0);
							strcpy(&aStr[0], pStrCur);

							if (memcmp(&aStr[0], "INDEX = ", 8) == 0)
							{	// �C���f�b�N
								pStrCur += 8;
								strcpy(&aStr[0], pStrCur);
								m_motionItem.nIndex = (int)atof(pStrCur);
							}
							else if (memcmp(&aStr[0], "PARENT = ", 9) == 0)
							{	// �y�A�����g(�e�q�֌W�ݒ�)
								pStrCur += 9;
								strcpy(&aStr[0], pStrCur);
								m_motionItem.nParent = (int)atof(pStrCur);
							}
							else if (memcmp(&aStr[0], "POS = ", 6) == 0)
							{	// �ʒu���
								pStrCur += 6;
								strcpy(&aStr[0], pStrCur);
								m_pos1.x = (float)atof(pStrCur);		// pos.x�̏��

								pStrCur = SearchLine(pStrCur);
								strcpy(&aStr[0], pStrCur);
								m_pos1.y = (float)atof(pStrCur);		// pos.y�̏��

								pStrCur = SearchLine(pStrCur);
								strcpy(&aStr[0], pStrCur);
								m_pos1.z = (float)atof(pStrCur);		// pos.z�̏��
							}
							else if (memcmp(&aStr[0], "ROT = ", 6) == 0)
							{	// ��]���
								pStrCur += 6;
								strcpy(&aStr[0], pStrCur);
								m_rot1.x = (float)atof(pStrCur);		// rot.x�̏��

								pStrCur = SearchLine(pStrCur);
								strcpy(&aStr[0], pStrCur);
								m_rot1.y = (float)atof(pStrCur);		// rot.y�̏��

								pStrCur = SearchLine(pStrCur);
								strcpy(&aStr[0], pStrCur);
								m_rot1.z = (float)atof(pStrCur);		// rot.z�̏��
							}

						} while (memcmp(&aStr[0], "END_PARTSSET", 12) != 0);
						if (m_pModel[nCnt] == NULL)
						{	// NULL�ȊO�Ȃ�(���f���̐���)
							m_pModel[nCnt] = new CModel;
							m_pModel[nCnt]->SetInitAll(m_pTexture[m_motionItem.nIndex], m_pMesh[m_motionItem.nIndex], m_pBuffMat[m_motionItem.nIndex], m_nNumMat[m_motionItem.nIndex], m_pos1, m_rot1);
							m_pModel[nCnt]->Init();
						}

						// �e�q�֌W�̐ݒ�
						if (m_motionItem.nParent == -1)
						{	// �e
							m_pModel[nCnt]->SetParent(NULL);			// �eMODEL (NULL)
						}
						else
						{	// �q
							m_pModel[nCnt]->SetParent(m_pModel[m_motionItem.nParent]);			// �qMODEL (�e�}�g���b�N�X)
						}
						nCnt++;
					}
				}
			}
		} while (memcmp(&aStr[0], "END", 3) != 0);
	}
	fclose(pFile);

}

