#ifndef COMMON_H__
#define COMMON_H__

#include <string>
#include <QString>
#include <QWindow>

enum ENUM_Role
{
	enum_Compere = 1,  //主持人
	enum_Speaker = 2,  //主讲人
	enum_Participant = 3	//参与者
};

enum sendMessageType
{
	E_Normal = 0,
	E_ImageUpLoad = 1,
	E_ImageSend = 2,
};

//终端类型
enum TerminalType
{
	E_Null = 0,
	E_Pc_Win = 1,
	E_Web = 2,
	E_Ios = 3,
	E_Android = 4,
	E_Android_RearviewMirror = 5,
	E_Pc_Mac = 6,
	E_Ios_Pad = 7,
	E_Android_Pad = 8,
	E_Web_Phone = 9
};

extern __int64 g_nUserId;
extern std::string g_strName;
extern ENUM_Role g_oEnumRole;
extern int g_nMeetingType;  //1视频会议  2直播
extern QString g_strMeetingTopic; //会议主题
extern QString g_strRoomNo; //房间号
extern QString g_strMeetingCode; //参会码
extern bool g_bHasCam; //是否有摄像头
extern int g_nMicrophoneIndex; //麦克风索引
extern int g_nSpeakerIndex; //扬声器索引
extern int g_nCameraIndex; //摄像头索引
extern TerminalType g_oEnTerminalType; //终端类型

extern WId g_MainWindowId; //主窗口句柄ID
extern QString g_serverPath;
extern QString g_imageCachePath;

QString toChineseSpell(QString strText);

__int64 string2int64(std::string strParam);
std::string int642string(__int64 nParam);

QString utf82unicode(std::string value);
std::string unicode2utf8(QString value);

std::string roleE2S(ENUM_Role eRole);
ENUM_Role roleS2E(std::string sRole);

void reNameSave(QString strNew);

enum EnumDisplayerType
{
	EN_FullWidget = 0,
	EN_OldScale = 1
};

#endif // COMMON_H__