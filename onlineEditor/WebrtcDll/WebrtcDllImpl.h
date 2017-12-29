#ifndef WEBRTCDLLIMPL_H__
#define WEBRTCDLLIMPL_H__

#include <windows.h>
#include <sstream>
#include <string>
//#include "INotification.h"
using namespace std;

//class ISdpEvent
//{
//public:
//	virtual void onCandicate(std::string sdp_mid, int sdp_mlineindex, std::string sdp) = 0;
//	virtual void onSdp(std::string type, std::string sdp) = 0;
//};
//
//class ISdpCallback
//{
//public:
//	virtual void onResult(string error, string sdp) = 0;
//};

class IVideoRenderer;
class IVideoDisplayer
{
public:
	virtual void displayVideoFrame(unsigned char* pData, int len, int width, int height) = 0;
	virtual void setHasVideo(bool bVideo) = 0;
	virtual void* getWindow() = 0;
	virtual void setVideoRenderer(IVideoRenderer* pVideoRenderer) = 0;
	virtual bool getHasVideo() = 0;
	virtual void setUserName(string strName) = 0;
	virtual void updateWidget() = 0;
};

class IVideoRenderer
{
public:
	virtual ~IVideoRenderer() {};
	virtual void setVideoDisplayer(IVideoDisplayer* pVideoDisplayer) = 0;
};

class IVideoInfo
{
public:
	virtual IVideoDisplayer* getVideoDisplayer() = 0;
	virtual void setVideoDisplayer(IVideoDisplayer* pVideoDisplayer) = 0;
	//如果用户发言 strUserId 为用户编号 bVideo 为true
	//如果用户停止 strUserId 为"" bVideo 为false
	virtual void setUserId(__int64 nUserId, bool bVideo) = 0;
	virtual bool getHasVideo() = 0;
};


//class IWebrtcSDKEvent
//{
//public:
//	virtual void log(char* strLog) = 0;
//	virtual IVideoInfo* getVideoInfo(bool bLocal = true, bool isMeg = true) = 0;
//	virtual void onNotification(INotification* data) = 0;
//};

//class IWebrtcSDK
//{
//public:
//	virtual boolean init(const char* serverUrl) = 0;
//	virtual void login(__int64 userId, const char*userName, const char* roomId, const char* role, bool bHasCam) = 0;
//	virtual void logout() = 0;
//	virtual void reConnect() = 0;
//	virtual bool isConnect() = 0;
//
//	virtual bool startSpeak(bool bAudio, bool bVideo) = 0;
//	virtual void stopSpeak() = 0;
//	virtual void removeStream(__int64 userId) = 0;
//	virtual void ping() = 0;
//	virtual void onPing() = 0;
//	virtual void onNotification(INotification* data) = 0;
//	virtual void broadcastMsg(const char* type, __int64 toUserId, const char* msg = "") = 0;
//	virtual void shareScreen(uint64_t id, bool bDesktop = false, int nDesktopIndex = -1, HWND excludeWin = nullptr) = 0;
//
//	virtual void release() = 0;
//
//	virtual void setUserRole(__int64 userId, const char* role) = 0;
//	virtual void setDisplayName(__int64 userId, const char* displayName) = 0;
//	virtual void setUserCamera(__int64 userId, bool hasCam) = 0;
//	virtual int camCount() = 0;
//	virtual int updateDevice(int nCameraIndex, int nMicrophoneIndex, int nSpeakerIndex) = 0;
//};
//
//IWebrtcSDK* GetWebrtcSDK(IWebrtcSDKEvent*event);

//class IAudioDevice
//{
//public:
//	virtual bool init() = 0;
//	virtual bool deviceCount(int& micCount, int& speakerCount) = 0;
//	virtual void release() = 0;
//	virtual bool testMic(int micIndex, int speakerIndex) = 0;
//	virtual bool testSpeaker(int speakerIndex, const char* fileName) = 0;
//	virtual bool stopTest() = 0;
//	virtual void setSpeakerVolume(uint32_t valve) = 0;
//	virtual void setMicrophoneVolume(uint32_t volume) = 0;
//	virtual void getMicGUIDFromIndex(int micIndex, char sGUID[256]) = 0;
//	virtual void getSpeakGUIDFromIndex(int micIndex, char sGUID[256]) = 0;
//};
//
//class IAudioDeviceEvent
//{
//public:
//	virtual void onAddRecordingDevice(int index, char* name, char* guid) = 0;
//	virtual void onAddPlayoutDevice(int index, char* name, char* guid) = 0;
//	//麦克最大音量
//	virtual void onRecordingEvent_MaxVolume(short nMaxVolume) = 0;
//	//扬声器最大音量
//	virtual void onPlayoutEvent_MaxVolume(short nMaxVolume) = 0;
//};
//
//IAudioDevice* GetAudioDevice(IAudioDeviceEvent* event);
//
//
//class IVideoDevice
//{
//public:
//	virtual bool init() = 0;
//	virtual void release() = 0;
//	virtual bool showCamera(IVideoDisplayer* pVideoDisplayer, int index = 0, int w = 640, int h = 480) = 0;
//	virtual void closeCamera() = 0;
//};
//
//class IVideoDeviceEvent
//{
//public:
//	virtual void onAddCameraDevice(int index, char* name, char* guid) = 0;
//};
//
//IVideoDevice* GetVideoDevice(IVideoDeviceEvent* event);

#endif // WEBRTCDLLIMPL_H__