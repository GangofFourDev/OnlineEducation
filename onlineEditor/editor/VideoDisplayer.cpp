#include "VideoDisplayer.h"
#include <QPainter>
#include <qevent.h>
#include <QPainter>
#include <QCheckBox>
#include <qmath.h>
#include <QApplication>
#include <QDesktopWidget>
#include <QHBoxLayout>
//#include "VideoWdg.h"


VideoDisplayer::VideoDisplayer(bool canMove,QWidget *parent)
	: QOpenGLWidget(parent), m_strUserName(""), m_bIsFullDisPlayer(false)
{
	m_pVideoRenderer = nullptr;
	//setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
	//setWindowFlags( Qt::WindowStaysOnTopHint);
	m_canMove = canMove;
	m_width = 0;
	m_height = 0;
	m_pData = nullptr;
	
	m_bHasVideo = false;


	QDesktopWidget* desktopWidget = QApplication::desktop();
	//获取可用桌面大小
	//QRect deskRect = desktopWidget->availableGeometry();
	//获取设备屏幕大小
	QRect screenRect = desktopWidget->screenGeometry();
	m_oMinWidgetSize = screenRect.size();
	m_oMinWidgetSize -= QSize(2, 2);

	m_pUserNameLabel = new QLabel(this);
	m_pUserNameLabel->setObjectName("VideoDisplayer_uesrName");
	m_pUserNameLabel->setGeometry(5, 0, 130, 30);

	QHBoxLayout *pHMainLayout = new QHBoxLayout;
	pHMainLayout->setContentsMargins(0, 0, 0, 0);
	pHMainLayout->addWidget(new QLabel(this));

	this->setLayout(pHMainLayout);

	connect(&m_updateVideoTimer, SIGNAL(timeout()), this, SLOT(onUpdateVideoTimer()));
	connect(this, SIGNAL(sizeChanged()), this, SLOT(setWidgetSize()));
	m_updateVideoTimer.start(50);
}

VideoDisplayer::~VideoDisplayer()
{
	if (m_pVideoRenderer)
	{
		m_pVideoRenderer->setVideoDisplayer(nullptr);
	}
}

void VideoDisplayer::setFullDisPlayer(bool bFull)
{
	m_bIsFullDisPlayer = bFull;

	if (m_bIsFullDisPlayer)
	{
		setWidgetSize();
	}
}

void VideoDisplayer::displayVideoFrame(unsigned char* pData, int len, int width, int height){
#if 1
	QMutexLocker locker(&m_Mutex);
	if (m_pData == nullptr) {
		m_pData = new unsigned char[len];
		m_width = width;
		m_height = height;
	}
	if (m_width != width || m_height != height) {
		delete[] m_pData;
		m_pData = new unsigned char[len];
		m_width = width;
		m_height = height;

		if (m_bIsFullDisPlayer)
		{
			emit sizeChanged();
		}
	}
	memcpy(m_pData, pData, len);
	//update();
#endif
}

void VideoDisplayer::setHasVideo(bool bVideo)
{
	m_bHasVideo = bVideo;
	repaint();
}

bool VideoDisplayer::getHasVideo()
{
	return m_bHasVideo;
}

void* VideoDisplayer::getWindow()
{
	WId wId = this->winId();
	void* wWindow = reinterpret_cast<void*>(wId);
	return wWindow;
}

void VideoDisplayer::setVideoRenderer(IVideoRenderer* pVideoRenderer)
{
	m_pVideoRenderer = pVideoRenderer;
}

void VideoDisplayer::setUserName(string strName)
{
	m_strUserName = strName;
	m_pUserNameLabel->setText(m_strUserName.c_str());
}

//void VideoDisplayer::setDisplayerType(EnumDisplayerType type)
//{
//	m_enDIsplayerType = type;
//	setWidgetSize();
//}
//
//EnumDisplayerType VideoDisplayer::getDIisplayerType() const
//{
//	return m_enDIsplayerType;
//}

void VideoDisplayer::paintEvent(QPaintEvent *event) 
{
	QPainter painter(this);
	if (!m_bHasVideo)
	{
		int w = this->width();
		int h = this->height();
		NinePatch(QString(":/skin/Resources/video/smallVideoBG.png"), 5, 5, w, h, &painter);
		QPixmap imgLogo(":/skin/Resources/video/smallvideoLogo.png");
		if (imgLogo.isNull() == false)
		{
			painter.drawPixmap(abs((w - imgLogo.width()) / 2), abs((h - imgLogo.height()) / 2), imgLogo);
		}
		painter.end();
	}
	else
	{
		QMutexLocker locker(&m_Mutex);
		painter.setRenderHint(QPainter::Antialiasing);
		if (m_pData != NULL)
		{
			QImage videoFrame(m_pData, m_width, m_height, QImage::Format_ARGB32);

			/*if (m_bIsFullDisPlayer && m_enDIsplayerType == EN_OldScale)
			{
				if (width() > m_width && height() > m_height)
				{
					painter.fillRect(0, 0, width(), height(), QColor("#000000"));
					painter.drawImage(QRect((width() - m_width) / 2, (height() - m_height) / 2, m_width, m_height),
						videoFrame, QRect(0, 0, m_width, m_height));
				}
				else if (width() > m_width )
				{
					painter.fillRect(0, 0, width(), height(), QColor("#000000"));
					painter.drawImage(QRect((width() - m_width) / 2, 0, m_width, m_height),
						videoFrame, QRect(0, 0, m_width, m_height));
				}
				else if (height() > m_height)
				{
					painter.fillRect(0, 0, width(), height(), QColor("#000000"));
					painter.drawImage(QRect(0, (height() - m_height) / 2, m_width, m_height),
						videoFrame, QRect(0, 0, m_width, m_height));
				}
				else
				{
					painter.drawImage(QRect(0, 0, m_width, m_height), videoFrame, QRect(0, 0, m_width, m_height));
				}
			}
			else
			{*/
				painter.drawImage(QRect(0, 0, width(), height()), videoFrame, QRect(0, 0, m_width, m_height));
			//}
			painter.end();
		}
	}

#if 0
	QMutexLocker locker(&m_Mutex);
	QPainter painter(this);
	if (m_bHasVideo)
	{
		if (m_pData != nullptr) {
			painter.setRenderHint(QPainter::Antialiasing, true);
			int w = this->width();
			int h = this->height();
			float x = (float)m_width / (float)m_height;
			int w2 = h*x;
			QPen pen;
			pen.setColor(QColor(102, 0, 0));
			painter.setPen(pen);
			painter.fillRect(0, 0, w, h, QColor(0, 0, 0));

			QImage videoFrame(m_pData, m_width, m_height, QImage::Format_ARGB32);
			painter.drawImage(QRect((w - w2) / 2, 0, w2, h), videoFrame, QRect(0, 0, m_width, m_height));

		}
	}
	else
	{
		painter.setPen(Qt::NoPen);
		painter.setBrush(Qt::black);
		painter.drawRect(rect());
	}
#endif
}

void VideoDisplayer::setWidgetState()
{
	Qt::WindowStates state = this->windowState();
	if (state & Qt::WindowFullScreen)
	{
		this->setWindowFlags(Qt::SubWindow);
		this->showNormal();
	}
	else
	{		
		this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
		this->showFullScreen();
	}
}

void VideoDisplayer::onUpdateVideoTimer()
{
	if (m_bHasVideo)
	{
		this->updateWidget();
	}
}

void VideoDisplayer::updateWidget()
{
	this->update();
}

void VideoDisplayer::setWidgetSize()
{
	if (m_enDIsplayerType == EN_OldScale)
	{
		this->resize(qMax(m_width, m_oMinWidgetSize.width()), qMax(m_height, m_oMinWidgetSize.height()));
	}
	else
	{
		this->resize(m_oMinWidgetSize.width(), m_oMinWidgetSize.height());
	}
}

void VideoDisplayer::mouseDoubleClickEvent(QMouseEvent *event)
{
	emit doubleClicked(this);
}

QPixmap* VideoDisplayer::NinePatch(QString& picName, int iHorzSplit, int iVertSplit, int DstWidth, int DstHeight, QPainter* painter)
{
	if (DstWidth > 1920 || DstHeight > 1080)
		return nullptr;
	QPixmap* pix = new QPixmap(picName);
	int pixWidth = pix->width();
	int pixHeight = pix->height();

	QPixmap pix_1 = pix->copy(0, 0, iHorzSplit, iVertSplit);
	QPixmap pix_2 = pix->copy(iHorzSplit, 0, pixWidth - iHorzSplit * 2, iVertSplit);
	QPixmap pix_3 = pix->copy(pixWidth - iHorzSplit, 0, iHorzSplit, iVertSplit);

	QPixmap pix_4 = pix->copy(0, iVertSplit, iHorzSplit, pixHeight - iVertSplit * 2);
	QPixmap pix_5 = pix->copy(iHorzSplit, iVertSplit, pixWidth - iHorzSplit * 2, pixHeight - iVertSplit * 2);
	QPixmap pix_6 = pix->copy(pixWidth - iHorzSplit, iVertSplit, iHorzSplit, pixHeight - iVertSplit * 2);

	QPixmap pix_7 = pix->copy(0, pixHeight - iVertSplit, iHorzSplit, iVertSplit);
	QPixmap pix_8 = pix->copy(iHorzSplit, pixHeight - iVertSplit, pixWidth - iHorzSplit * 2, pixWidth - iHorzSplit * 2);
	QPixmap pix_9 = pix->copy(pixWidth - iHorzSplit, pixHeight - iVertSplit, iHorzSplit, iVertSplit);

	pix_2 = pix_2.scaled(DstWidth - iHorzSplit * 2, iVertSplit, Qt::IgnoreAspectRatio);//保持高度拉宽
	pix_4 = pix_4.scaled(iHorzSplit, DstHeight - iVertSplit * 2, Qt::IgnoreAspectRatio);//保持宽度拉高
	pix_5 = pix_5.scaled(DstWidth - iHorzSplit * 2, DstHeight - iVertSplit * 2, Qt::IgnoreAspectRatio);
	pix_6 = pix_6.scaled(iHorzSplit, DstHeight - iVertSplit * 2, Qt::IgnoreAspectRatio);//保持宽度拉高
	pix_8 = pix_8.scaled(DstWidth - iHorzSplit * 2, iVertSplit);//保持高度拉宽


	QPixmap* resultImg = nullptr;
	try
	{
		//resultImg = new QPixmap(DstWidth, DstHeight);
	}
	catch (...)
	{
	}

	//QPainter* painter = new QPainter(resultImg);
	//if (resultImg != nullptr && !resultImg->isNull()) {
	if(true){
		painter->drawPixmap(0, 0, pix_1);
		painter->drawPixmap(iHorzSplit, 0, pix_2);
		painter->drawPixmap(DstWidth - iHorzSplit, 0, pix_3);

		painter->drawPixmap(0, iVertSplit, pix_4);
		painter->drawPixmap(iHorzSplit, iVertSplit, pix_5);
		painter->drawPixmap(DstWidth - iHorzSplit, iVertSplit, pix_6);

		painter->drawPixmap(0, DstHeight - iVertSplit, pix_7);
		painter->drawPixmap(iHorzSplit, DstHeight - iVertSplit, pix_8);
		painter->drawPixmap(DstWidth - iHorzSplit, DstHeight - iVertSplit, pix_9);
		//painter->end();
	}
	return nullptr;
}

void VideoDisplayer::closeEvent(QCloseEvent *event)
{
	//该窗体不能单独关闭
	event->ignore();
}
