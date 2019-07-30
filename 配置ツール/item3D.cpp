//=============================================================================
//
// �A�C�e��3D���� [item3D.cpp]
// Author : Shun Yokomichi
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS

#include "item3d.h"				// 3D�A�C�e��
#include "renderer.h"			// �����_���[
#include "manager.h"			// �}�l�[�W���[
#include "input.h"				// ����
#include "model.h"				// ���f��
#include "game.h"				// �Q�[��
#include "sound.h"				// �T�E���h
#include <stdio.h>				// �C���N���h�t�@�C��

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
LPD3DXMESH				CItem3D::m_pMesh[MAX_ITEM_PARTS] = {};			// ���b�V���̃|�C���^
LPD3DXBUFFER			CItem3D::m_pBuffMat[MAX_ITEM_PARTS] = {};		// �o�b�t�@�̃|�C���^
DWORD					CItem3D::m_nNumMat[MAX_ITEM_PARTS] = {};		// �}�b�g�̃|�C���^
LPDIRECT3DTEXTURE9		*CItem3D::m_pTexture[MAX_ITEM_PARTS] = {};		// �e�N�X�`�����ւ̃|�C���^
int						CItem3D::m_nNumAll = 0;						// �����z�u���̏��

//*********************************************************************
//	�A�C�e���̐���
//*********************************************************************
CItem3D *CItem3D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type)
{
	CItem3D *pItem3D = NULL;

	// �V�[���̐���
	if (pItem3D == NULL)
	{
		pItem3D = new CItem3D;	// ���I�m��
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
CItem3D::CItem3D(int nPriority, OBJTYPE objType): CScene(3, OBJTYPE_ITEM3D)
{
	// �f�t�H���g�l�ɂ���
	m_motionItem.nParts = 0;		// Parts��
	m_motionItem.nIndex = 0;		// �C���f�b�N��
	m_motionItem.nParent = 0;		// �e�q�֌W

}

//*********************************************************************
//	�f�X�g���N�^
//*********************************************************************
CItem3D::~CItem3D() {}

//*********************************************************************
//		�e�N�X�`���̓ǂݍ���
//*********************************************************************
HRESULT CItem3D::Load(void)
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
void CItem3D::Unload(void)
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
HRESULT CItem3D::Init()
{
	// ���ꂼ��̏�����
	// Text�̓ǂݍ���
	TxtLoad();

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ����
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړ���
	
	// ���f���֌W
	m_nNum = m_nNumAll;			// ���f���̌��ݑ����z�u���̔ԍ�����(�z�u�ԍ��̋L�^)
	m_nNumAll++;				// �����z�u�̉��Z


	return S_OK;
}

//*********************************************************************
// �I������
//*********************************************************************
void CItem3D::Uninit(void)
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

	// �����z�u�̌��Z
	m_nNumAll--;

	CScene::SetDeath();						// �I������
}

//*********************************************************************
// �X�V����
//*********************************************************************
void CItem3D::Update(void)
{
	if (m_nTypeNum == 0) { m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); }	// �J�M�������
	if (m_nTypeNum == 1) { m_col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f); }	// �J�M�������
	if (m_nTypeNum == 2) { m_col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f); }	// �񕜂������
	if (m_nTypeNum == 3) { m_col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f); }	// �����������


	for (int nCntParts = 0; nCntParts < MAX_ITEM_PARTS; nCntParts++)
	{
		if (m_pModel[nCntParts] != NULL && m_nTypeNum != 0)
		{
			m_pModel[nCntParts]->Setcol(m_col);
		}
	}
}

//*********************************************************************
// �`�揈��
//*********************************************************************
void CItem3D::Draw(void)
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
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
	
	for (int nCntParts = 0; nCntParts < MAX_ITEM_PARTS; nCntParts++)
	{	
		// MODEL�̕`��
		m_pModel[nCntParts]->Draw();
	}
}

//*********************************************************************
// �ݒ菈��
//*********************************************************************
void CItem3D::SetModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	m_pos = pos;		// pos����
	m_rot = rot;		// rot����
}

//*********************************************************************
//	Text���ǂݍ��ݏ���
//*********************************************************************
char *CItem3D::ReadLine(FILE *pFile, char*pDst, int nPatten)
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
char *CItem3D::SearchLine(char*pDst)
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
void CItem3D::TxtLoad()
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

