//=======================================================//
//
// �T�E���h���� [Sound.h]
// Author : Shun Yokomichi
//
//=======================================================//
#ifndef _SOUND_H_
#define _SOUND_H_

//*******************************************************//
// �w�b�_�t�@�C���̃C���N���[�h
//*******************************************************//
#include "main.h"	// ���C��

//*******************************************************//
// �N���X��`
//*******************************************************//
class CSound
{// CSound
public:
	//*************************************
	// �T�E���h�t�@�C��
	//*************************************
	typedef enum
	{// �T�E���h��
		// BGM
		SOUND_LABEL_BGM000 = 0,	// BGM(�^�C�g��)
		SOUND_LABEL_BGM001,		// BGM(�`���[�g���A��)
		SOUND_LABEL_BGM002,		// BGM(�Q�[��)
		SOUND_LABEL_BGM003,		// BGM(�Q�[��2)
		SOUND_LABEL_BGM004,		// BGM(���U���g)
		SOUND_LABEL_BGM005,		// BGM(���U���g2)

		// SE(�V�X�e��)
		SOUND_LABEL_SE000,		// SE(�|�[�Y���j���[�\��)
		SOUND_LABEL_SE001,		// SE(����)
		SOUND_LABEL_SE002,		// SE(�Z���N�g)
		SOUND_LABEL_SE003,		// SE(�L�����Z��)

		// SE(�v���C���[����)
		SOUND_LABEL_SE004,		// SE(���s(���n))
		SOUND_LABEL_SE005,		// SE(�_�b�V��(���n))
		SOUND_LABEL_SE006,		// SE(�W�����v)
		SOUND_LABEL_SE007,		// SE(���n)
		SOUND_LABEL_SE008,		// SE(���S)

		// SE(����)
		SOUND_LABEL_SE009,		// SE(���l�̑���)
		SOUND_LABEL_MAX,		// �ő吔
	} SOUND_LABEL;

	CSound() {};	// �R���X�g���N�^(���g����)
	~CSound() {};	// �f�X�g���N�^(���g����)

	HRESULT Init(HWND hWnd);
	void Uninit(void);
	HRESULT PlaySound(SOUND_LABEL label);
	void StopSound(SOUND_LABEL label);
	void StopSound(void);
	void ChangeVolume(SOUND_LABEL label, float fVolume);
	void ChagePitch(SOUND_LABEL label, float fPitch);

private:
	typedef struct
	{
		char *pFilename;	// �t�@�C����
		int nCntLoop;		// ���[�v�J�E���g(-1�Ń��[�v�Đ�)
	} SOUNDPARAM;

	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	IXAudio2 *m_pXAudio2									= NULL;	// XAudio2�I�u�W�F�N�g�ւ̃|�C���^
	IXAudio2MasteringVoice *m_pMasteringVoice				= NULL;	// �}�X�^�[�{�C�X�ւ̃|�C���^
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX]	= {};	// �\�[�X�{�C�X�ւ̃|�C���^
	BYTE *m_apDataAudio[SOUND_LABEL_MAX]					= {};	// �I�[�f�B�I�f�[�^�ւ̃|�C���^
	DWORD m_aSizeAudio[SOUND_LABEL_MAX]						= {};	// �I�[�f�B�I�f�[�^�T�C�Y

	// �e���f�ނ̃p�����[�^
	SOUNDPARAM m_aSoundParam[SOUND_LABEL_MAX] =
	{// �f�[�^�p�X(�T�E���h���X�g)
		// BGM
		{ "Data/SOUND/BGM/Title002.wav", -1 },				// BGM0	// �^�C�g��
		{ "Data/SOUND/BGM/Tutorial_Loop.wav", -1 },			// BGM1 // �`���[�g���A��
		{ "Data/SOUND/BGM/Game000.wav", -1 },				// BGM2	// �Q�[��
		{ "Data/SOUND/BGM/Game003.wav", -1 },				// BGM3	// �Q�[��2
		{ "Data/SOUND/BGM/GameClear.wav", -1 },				// BGM4	// ���U���g
		{ "Data/SOUND/BGM/Result000.wav", -1 },				// BGM5 // ���U���g2

		// �V�X�e��SE
		{ "Data/SOUND/SE/SYSTEM/Pause_Open.wav", 0 },		// SE0	// �|�[�Y���j���[�\��
		{ "Data/SOUND/SE/SYSTEM/Pause_Decision.wav", 0 },	// SE1	// ����
		{ "Data/SOUND/SE/SYSTEM/Pause_Select.wav", 0 },		// SE2	// �Z���N�g
		{ "Data/SOUND/SE/SYSTEM/Pause_Cancel.wav", 0 },		// SE3	// �L�����Z��

		// �L�����N�^�[����SE
		{ "Data/SOUND/SE/PLAYER/Walk_Normal.wav", 0 },		// SE4	// ���s(���n)
		{ "Data/SOUND/SE/PLAYER/Dash_Normal.wav", 0 },		// SE5	// �_�b�V��(���n)
		{ "Data/SOUND/SE/PLAYER/Action_Jump.wav", 0 },		// SE6	// �W�����v
		{ "Data/SOUND/SE/PLAYER/Action_Landing.wav", 0 },	// SE7	// ���n
		{ "Data/SOUND/SE/PLAYER/Action_Death.wav", 0 },		// SE8	// ���S

		// ��SE
		{ "Data/SOUND/SE/GAME/Walk_Giant.wav", 0 },			// SE9	// ���l�̑���
	};
};
#endif