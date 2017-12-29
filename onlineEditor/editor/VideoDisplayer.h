#ifndef VIDEODISPLAYER_H__
#define VIDEODISPLAYER_H__

#include <QWidget>
#include <QTimer>
#include <QTimer>
#include <QMutex>
#include <QLabel>
#include <QOpenGLWidget>
#include <QCheckBox>
#include "UserList.h"
//#include "common.h"
#include "../WebrtcDLl/WebrtcDllImpl.h"

class VideoDisplayer : public QOpenGLWidget, public IVideoDisplayer
{
	Q_OBJECT

public:
	VideoDisplayer(bool canMove = false,QWidget *parent=nullptr);
	~VideoDisplayer();

	void setFullDisPlayer(bool bFull);

public slots:
	virtual void displayVideoFrame(unsigned char* pData, int len, int width, int height) override;
	virtual void setHasVideo(bool bVideo) override;
	virtual bool getHasVideo() override;
	virtual void* getWindow() override;
	virtual void setVideoRenderer(IVideoRenderer* pVideoRenderer) override;
	virtual void setUserName(string strName) override;
	//void setDisplayerType(EnumDisplayerType type);
	//EnumDisplayerType getDIisplayerType() const;

	void setWidgetState();
	void onUpdateVideoTimer();
	virtual void updateWidget();
	void setWidgetSize();

signals:	
	void doubleClicked(VideoDisplayer *pSelf);
	void sizeChanged();

protected:
	virtual void paintEvent(QPaintEvent *event);
	virtual void mouseDoubleClickEvent(QMouseEvent *event);
	virtual void closeEvent(QCloseEvent *event);
	
	QPixmap* NinePatch(QString& picName, int iHorzSplit, int iVertSplit, int DstWidth, int DstHeight, QPainter* painter);

private:
	QTimer m_updateVideoTimer;
	QMutex m_Mutex;
	unsigned char* m_pData;
	int m_width;
	int m_height;

	UserList* m_pUserList;

	bool m_canMove;
	bool mouse_press;
	QPoint move_point;
	bool m_bHasVideo;

	QSize m_oMinWidgetSize;
	IVideoRenderer* m_pVideoRenderer;

	string m_strUserName;

	QLabel *m_pUserNameLabel;

	bool m_bIsFullDisPlayer;
	EnumDisplayerType m_enDIsplayerType;
};

#endif // VIDEODISPLAYER_H__