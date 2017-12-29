#ifndef VIDEORENDERER_H__
#define VIDEORENDERER_H__

#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>
#include <assert.h>
#include <deque>
#include <map>
#include <set>
#include <string>
#include "webrtc/base/scoped_ref_ptr.h"
#include <map>
#include <string>
#include "webrtc/api/mediastreaminterface.h"
#include "webrtc/base/win32.h"
#include "webrtc/media/base/mediachannel.h"
#include "webrtc/media/base/videocommon.h"
#include "webrtc/media/base/videoframe.h"
#include "WebrtcDllImpl.h"

template <typename T>
class AutoLock 
{
public:
	explicit AutoLock(T* obj) : obj_(obj) { obj_->Lock(); }
	~AutoLock() { obj_->Unlock(); }
protected:
	T* obj_;
};

class VideoRenderer : public rtc::VideoSinkInterface<cricket::VideoFrame>, public IVideoRenderer 
{
public:
	VideoRenderer(IVideoDisplayer* pVideoDisplayer);
	~VideoRenderer();
public:
	void OnFrame(const cricket::VideoFrame& frame) override;

	void Lock() {
		::EnterCriticalSection(&buffer_lock_);
	}

	void Unlock() {
		::LeaveCriticalSection(&buffer_lock_);
	}
public:
	virtual void setVideoDisplayer(IVideoDisplayer* pVideoDisplayer) override;
	void stopRender();
private:
	IVideoDisplayer* m_pVideoDisplayer;
	unsigned char* m_pData;
	int m_width;
	int m_height;

	CRITICAL_SECTION buffer_lock_;
};

#endif // VIDEORENDERER_H__