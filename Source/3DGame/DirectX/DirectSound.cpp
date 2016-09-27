#include "DirectSound.h"
#include "..\ApplicationSystem\ApplicationData.h"

using std::make_pair;

/**
* @brief �R���X�g���N�^
*/
DirectSound::DirectSound()
{
	// DirectSound�I�u�W�F�N�g�𐶐�
	if (FAILED(DirectSoundCreate8(NULL, &pDS8, NULL))){
		MessageBox(0, "DirectSound�I�u�W�F�N�g�̍쐬�Ɏ��s���܂���", "DirectSoundError!", MB_OK);
		exit(0);
	}
}

/**
* @brief �f�X�g���N�^
*/
DirectSound::~DirectSound()
{
	map<string, IDirectSoundBuffer*>::iterator ite;
	for (ite = m_soundBuffer.begin(); ite != m_soundBuffer.end(); ite++) {
		IDirectSoundBuffer* dsb = ite->second;
		dsb->Stop();
		SafeRelease(dsb);
	}
	m_soundBuffer.clear();

	SafeRelease(pDS8);
}

/**
* @brief WAV�t�@�C���̓ǂݍ���
* @param �T�E���h�t�@�C����
*/
BOOL DirectSound::Load(LPSTR fileName)
{
	// ���Ɏw�肵���t�@�C�����ǂݍ��܂�Ă�����ǂݍ��܂Ȃ�
	auto it = m_soundBuffer.find(fileName);
	if (it != m_soundBuffer.end()){
		return FALSE;
	}

	// �������x���̐ݒ�
	if (FAILED(pDS8->SetCooperativeLevel(AppIns->GetWindowHandle(), DSSCL_EXCLUSIVE))){
		MessageBox(0, "�������x���ݒ莸�s", "", MB_OK);
		return FALSE;
	}
	// Windows�}���`���f�B�AAPI�̃n���h��(Windows�}���`���f�B�AAPI��WAV�t�@�C���֌W�̑���p��API�ł���DirectX�ł͂Ȃ��j
	HMMIO hMmio = NULL;
	// WAV�t�@�C�����@WAV�f�[�^�̃T�C�Y�iWAV�t�@�C����WAV�f�[�^�Ő�߂��Ă���̂ŁA�قڃt�@�C���T�C�Y�Ɠ���j
	DWORD dwWavSize = 0;
	// WAV�̃t�H�[�}�b�g ��j16�r�b�g�A44100Hz�A�X�e���I�Ȃ�
	WAVEFORMATEX* pwfex;
	// �`�����N���
	MMCKINFO ckInfo;
	// �ŏ㕔�`�����N�iRIFF�`�����N�j�ۑ��p
	MMCKINFO riffckInfo;
	PCMWAVEFORMAT pcmWaveForm;

	// WAV�t�@�C�����̃w�b�_�[���i���f�[�^�ȊO�j�̊m�F�Ɠǂݍ���
	hMmio = mmioOpen(fileName, NULL, MMIO_ALLOCBUF | MMIO_READ);
	
	// �t�@�C���|�C���^��RIFF�`�����N�̐擪�ɃZ�b�g����
	if ((MMSYSERR_NOERROR != mmioDescend(hMmio, &riffckInfo, NULL, 0))){
		MessageBox(0, "�ŏ��̃`�����N�i�ŏ㕔�`�����N)�ɐi���ł��܂���", "", MB_OK);
		return FALSE;
	}
	// �`�����N���ɂ��WAV�t�@�C�����ǂ����m�F����
	if ((riffckInfo.ckid != mmioFOURCC('R', 'I', 'F', 'F')) || (riffckInfo.fccType != mmioFOURCC('W', 'A', 'V', 'E'))){
		MessageBox(0, "�����WAV�t�@�C���ł͂���܂���", " lol ", MB_OK);
		return FALSE;
	}
	// �t�@�C���|�C���^��'f' 'm' 't' ' ' �`�����N�ɃZ�b�g����
	ckInfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
	if (MMSYSERR_NOERROR != mmioDescend(hMmio, &ckInfo, &riffckInfo, MMIO_FINDCHUNK)){
		MessageBox(0, "fmt �`�����N��������܂���", "", MB_OK);
		return FALSE;
	}
	// �t�H�[�}�b�g��ǂݍ���
	if (mmioRead(hMmio, (HPSTR)&pcmWaveForm, sizeof(pcmWaveForm)) != sizeof(pcmWaveForm)){
		MessageBox(0, "WAV�t�H�[�}�b�g�̓ǂݍ��ݎ��s", "", MB_OK);
		return FALSE;
	}
	// ���j�APCM�ŁA���A�}���`�`�����l��WAV�͑z��O
	if (pcmWaveForm.wf.wFormatTag == WAVE_FORMAT_PCM){
		pwfex = (WAVEFORMATEX*)new CHAR[sizeof(WAVEFORMATEX)];
		if (NULL == pwfex){
			return FALSE;
		}
		memcpy(pwfex, &pcmWaveForm, sizeof(pcmWaveForm));
		pwfex->cbSize = 0;
	}
	else {
		MessageBox(0, "�W���̃��j�APCM�t�H�[�}�b�g��z�肵�Ă��܂�", "", MB_OK);
		return FALSE;
	}
	if (MMSYSERR_NOERROR != mmioAscend(hMmio, &ckInfo, 0)){
		delete pwfex;
		return FALSE;
	}
	// WAV�t�@�C�����̉��f�[�^�̓ǂݍ���
	ckInfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
	if (MMSYSERR_NOERROR != mmioDescend(hMmio, &ckInfo, &riffckInfo, MMIO_FINDCHUNK)){
		MessageBox(0, "data�`�����N��������܂���", "", MB_OK);
		return FALSE;
	}
	dwWavSize = ckInfo.cksize;
	// DirectSound�Z�J���_���[�o�b�t�@�[�쐬
	DSBUFFERDESC dsbd;
	ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags = DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY;
	dsbd.dwBufferBytes = dwWavSize;
	dsbd.guid3DAlgorithm = DS3DALG_DEFAULT;
	dsbd.lpwfxFormat = pwfex;

	LPDIRECTSOUNDBUFFER buffer = nullptr;

	if (FAILED(pDS8->CreateSoundBuffer(&dsbd, &buffer, NULL))){
		MessageBox(0, "�Z�J���_���o�b�t�@�[�̍쐬�Ɏ��s���܂���", "", MB_OK);
		return FALSE;
	}
	delete pwfex;
	//�g�`�f�[�^���������ޏ����Ƃ��āA�Z�J���_���[�o�b�t�@�[�����b�N����
	VOID* pBuffer = NULL;
	DWORD dwBufferSize = 0;
	if (FAILED(buffer->Lock(0, dwWavSize, &pBuffer, &dwBufferSize, NULL, NULL, 0))){
		MessageBox(0, "�Z�J���_���o�b�t�@�[�̃��b�N�Ɏ��s���܂���", "", MB_OK);
		return FALSE;
	}
	//���b�N�����o�b�t�@�[�T�C�Y�����ۂ̔g�`�f�[�^���傫���ꍇ�B�N���b�V���h�~�B
	DWORD dwSize = dwBufferSize;
	if (dwSize > ckInfo.cksize){
		dwSize = ckInfo.cksize;
	}
	
	//�o�b�t�@�[�ɉ��f�[�^����������   
	FILE* fp;
	fopen_s(&fp, fileName, "rb");
	fseek(fp, riffckInfo.dwDataOffset + sizeof(FOURCC), SEEK_SET);

	BYTE* pWavData = new BYTE[dwSize];
	
	// �܂��́A�ꎞ�I�ȏꏊ(pWavData)�ɔg�`�f�[�^��ǂݍ���
	fread(pWavData, 1, dwSize, fp);

	// �ꎞ�I�ȏꏊ�̔g�`�f�[�^���Z�J���_���o�b�t�@�[�ɃR�s�[����
	for (DWORD i = 0; i < dwSize; i++){
		*((BYTE*)pBuffer + i) = *((BYTE*)pWavData + i); 
	}
	fclose(fp);
	// �ꎞ�I�g�`�f�[�^�̉��
	delete pWavData;
	
	// �A�����b�N
	buffer->Unlock(pBuffer, dwBufferSize, NULL, 0);

	// �}�b�v�ɓo�^
	m_soundBuffer.insert(make_pair(fileName, buffer));

	// ���[�h����
	return TRUE;
}


/**
* @brief �Đ�
* @param �T�E���h�t�@�C����
*/
HRESULT DirectSound::Play(LPSTR fileName)
{
	if (IsPlay(fileName)){
		Stop(fileName);
	}
	m_soundBuffer[fileName]->Play(NULL, 0, NULL);
	m_soundBuffer[fileName]->SetCurrentPosition(800);

	return S_OK;
}


/**
* @brief ��~
* @param �T�E���h�t�@�C����
*/
HRESULT DirectSound::Stop(LPSTR fileName)
{
	if (IsPlay(fileName)){
		m_soundBuffer[fileName]->Stop();
		m_soundBuffer[fileName]->SetCurrentPosition(0);
	}

	return S_OK;
}


/**
* @brief ���[�v�Đ�
* @param �T�E���h�t�@�C����
*/
HRESULT DirectSound::LoopPlay(LPSTR fileName)
{
	m_soundBuffer[fileName]->Play(NULL, 0, DSBPLAY_LOOPING);
	m_soundBuffer[fileName]->SetCurrentPosition(800);
	
	return S_OK;
}


/**
* @brief �Đ����Ă邩�ǂ����̃`�F�b�N
* @param �T�E���h�t�@�C����
*/
BOOL DirectSound::IsPlay(LPSTR fileName)
{
	DWORD dwStatus;
	m_soundBuffer[fileName]->GetStatus(&dwStatus);

	if ((dwStatus & DSBSTATUS_PLAYING) != 0) {
		return TRUE;
	}

	return FALSE;
}


/**
* ���̉��ʒ���
*
* @param fileName �T�E���h�t�@�C����
* @param Volume ����(0(MIN) �` 100(MAX))
*/
void DirectSound::SetVolume(LPSTR fileName, int volume)
{
	if (volume < 0) volume = 0;
	else if (volume > 100) volume = 100;
	int Num = (volume - 100) * 100;
	m_soundBuffer[fileName]->SetVolume(Num);
}


/**
* @brief �T�E���h�f�[�^�����
* @param �T�E���h�t�@�C����
*/
void DirectSound::SoundSingleFree(LPSTR fileName)
{
	m_soundBuffer[fileName]->Release();
}

