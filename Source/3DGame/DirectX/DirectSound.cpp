#include "DirectSound.h"
#include "..\ApplicationSystem\ApplicationData.h"

using std::make_pair;

/**
* @brief コンストラクタ
*/
DirectSound::DirectSound()
{
	// DirectSoundオブジェクトを生成
	if (FAILED(DirectSoundCreate8(NULL, &pDS8, NULL))){
		MessageBox(0, "DirectSoundオブジェクトの作成に失敗しました", "DirectSoundError!", MB_OK);
		exit(0);
	}
}

/**
* @brief デストラクタ
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
* @brief WAVファイルの読み込み
* @param サウンドファイル名
*/
BOOL DirectSound::Load(LPSTR fileName)
{
	// 既に指定したファイルが読み込まれていたら読み込まない
	auto it = m_soundBuffer.find(fileName);
	if (it != m_soundBuffer.end()){
		return FALSE;
	}

	// 協調レベルの設定
	if (FAILED(pDS8->SetCooperativeLevel(AppIns->GetWindowHandle(), DSSCL_EXCLUSIVE))){
		MessageBox(0, "協調レベル設定失敗", "", MB_OK);
		return FALSE;
	}
	// WindowsマルチメディアAPIのハンドル(WindowsマルチメディアAPIはWAVファイル関係の操作用のAPIでありDirectXではない）
	HMMIO hMmio = NULL;
	// WAVファイル内　WAVデータのサイズ（WAVファイルはWAVデータで占められているので、ほぼファイルサイズと同一）
	DWORD dwWavSize = 0;
	// WAVのフォーマット 例）16ビット、44100Hz、ステレオなど
	WAVEFORMATEX* pwfex;
	// チャンク情報
	MMCKINFO ckInfo;
	// 最上部チャンク（RIFFチャンク）保存用
	MMCKINFO riffckInfo;
	PCMWAVEFORMAT pcmWaveForm;

	// WAVファイル内のヘッダー情報（音データ以外）の確認と読み込み
	hMmio = mmioOpen(fileName, NULL, MMIO_ALLOCBUF | MMIO_READ);
	
	// ファイルポインタをRIFFチャンクの先頭にセットする
	if ((MMSYSERR_NOERROR != mmioDescend(hMmio, &riffckInfo, NULL, 0))){
		MessageBox(0, "最初のチャンク（最上部チャンク)に進入できません", "", MB_OK);
		return FALSE;
	}
	// チャンク情報によりWAVファイルかどうか確認する
	if ((riffckInfo.ckid != mmioFOURCC('R', 'I', 'F', 'F')) || (riffckInfo.fccType != mmioFOURCC('W', 'A', 'V', 'E'))){
		MessageBox(0, "これはWAVファイルではありません", " lol ", MB_OK);
		return FALSE;
	}
	// ファイルポインタを'f' 'm' 't' ' ' チャンクにセットする
	ckInfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
	if (MMSYSERR_NOERROR != mmioDescend(hMmio, &ckInfo, &riffckInfo, MMIO_FINDCHUNK)){
		MessageBox(0, "fmt チャンクが見つかりません", "", MB_OK);
		return FALSE;
	}
	// フォーマットを読み込む
	if (mmioRead(hMmio, (HPSTR)&pcmWaveForm, sizeof(pcmWaveForm)) != sizeof(pcmWaveForm)){
		MessageBox(0, "WAVフォーマットの読み込み失敗", "", MB_OK);
		return FALSE;
	}
	// リニアPCMで、かつ、マルチチャンネルWAVは想定外
	if (pcmWaveForm.wf.wFormatTag == WAVE_FORMAT_PCM){
		pwfex = (WAVEFORMATEX*)new CHAR[sizeof(WAVEFORMATEX)];
		if (NULL == pwfex){
			return FALSE;
		}
		memcpy(pwfex, &pcmWaveForm, sizeof(pcmWaveForm));
		pwfex->cbSize = 0;
	}
	else {
		MessageBox(0, "標準のリニアPCMフォーマットを想定しています", "", MB_OK);
		return FALSE;
	}
	if (MMSYSERR_NOERROR != mmioAscend(hMmio, &ckInfo, 0)){
		delete pwfex;
		return FALSE;
	}
	// WAVファイル内の音データの読み込み
	ckInfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
	if (MMSYSERR_NOERROR != mmioDescend(hMmio, &ckInfo, &riffckInfo, MMIO_FINDCHUNK)){
		MessageBox(0, "dataチャンクが見つかりません", "", MB_OK);
		return FALSE;
	}
	dwWavSize = ckInfo.cksize;
	// DirectSoundセカンダリーバッファー作成
	DSBUFFERDESC dsbd;
	ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags = DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY;
	dsbd.dwBufferBytes = dwWavSize;
	dsbd.guid3DAlgorithm = DS3DALG_DEFAULT;
	dsbd.lpwfxFormat = pwfex;

	LPDIRECTSOUNDBUFFER buffer = nullptr;

	if (FAILED(pDS8->CreateSoundBuffer(&dsbd, &buffer, NULL))){
		MessageBox(0, "セカンダリバッファーの作成に失敗しました", "", MB_OK);
		return FALSE;
	}
	delete pwfex;
	//波形データを書き込む準備として、セカンダリーバッファーをロックする
	VOID* pBuffer = NULL;
	DWORD dwBufferSize = 0;
	if (FAILED(buffer->Lock(0, dwWavSize, &pBuffer, &dwBufferSize, NULL, NULL, 0))){
		MessageBox(0, "セカンダリバッファーのロックに失敗しました", "", MB_OK);
		return FALSE;
	}
	//ロックしたバッファーサイズが実際の波形データより大きい場合。クラッシュ防止。
	DWORD dwSize = dwBufferSize;
	if (dwSize > ckInfo.cksize){
		dwSize = ckInfo.cksize;
	}
	
	//バッファーに音データを書き込む   
	FILE* fp;
	fopen_s(&fp, fileName, "rb");
	fseek(fp, riffckInfo.dwDataOffset + sizeof(FOURCC), SEEK_SET);

	BYTE* pWavData = new BYTE[dwSize];
	
	// まずは、一時的な場所(pWavData)に波形データを読み込み
	fread(pWavData, 1, dwSize, fp);

	// 一時的な場所の波形データをセカンダリバッファーにコピーする
	for (DWORD i = 0; i < dwSize; i++){
		*((BYTE*)pBuffer + i) = *((BYTE*)pWavData + i); 
	}
	fclose(fp);
	// 一時的波形データの解放
	delete pWavData;
	
	// アンロック
	buffer->Unlock(pBuffer, dwBufferSize, NULL, 0);

	// マップに登録
	m_soundBuffer.insert(make_pair(fileName, buffer));

	// ロード成功
	return TRUE;
}


/**
* @brief 再生
* @param サウンドファイル名
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
* @brief 停止
* @param サウンドファイル名
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
* @brief ループ再生
* @param サウンドファイル名
*/
HRESULT DirectSound::LoopPlay(LPSTR fileName)
{
	m_soundBuffer[fileName]->Play(NULL, 0, DSBPLAY_LOOPING);
	m_soundBuffer[fileName]->SetCurrentPosition(800);
	
	return S_OK;
}


/**
* @brief 再生してるかどうかのチェック
* @param サウンドファイル名
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
* 音の音量調節
*
* @param fileName サウンドファイル名
* @param Volume 音量(0(MIN) ～ 100(MAX))
*/
void DirectSound::SetVolume(LPSTR fileName, int volume)
{
	if (volume < 0) volume = 0;
	else if (volume > 100) volume = 100;
	int Num = (volume - 100) * 100;
	m_soundBuffer[fileName]->SetVolume(Num);
}


/**
* @brief サウンドデータを解放
* @param サウンドファイル名
*/
void DirectSound::SoundSingleFree(LPSTR fileName)
{
	m_soundBuffer[fileName]->Release();
}

