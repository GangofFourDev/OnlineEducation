#include "VideoRenderer.h"
#include <algorithm>
VideoRenderer::VideoRenderer( IVideoDisplayer* pVideoDisplayer)
{
	::InitializeCriticalSection(&buffer_lock_);
	m_pVideoDisplayer = pVideoDisplayer;
	//m_pVideoDisplayer->setVideoRenderer(this);
	m_pData = nullptr;
}


VideoRenderer::~VideoRenderer()
{
	stopRender();
	::DeleteCriticalSection(&buffer_lock_);
}

void VideoRenderer::OnFrame(const cricket::VideoFrame& video_frame)
{
	AutoLock<VideoRenderer> lock(this);

	if (m_pVideoDisplayer) {
		const cricket::VideoFrame* frame = video_frame.GetCopyWithRotationApplied();
		int w = frame->width();
		int h = frame->height();

		if (m_pData == nullptr) {
			m_pData = new unsigned char[w * h * 4];
			m_width = w;
			m_height = h;
		}

		if (m_width != w || m_height != h) {
			delete[] m_pData;
			m_pData = new unsigned char[w*h * 4];
			m_width = w;
			m_height = h;
		}
		frame->ConvertToRgbBuffer(
			cricket::FOURCC_ARGB,
			m_pData,
			w*h * 4,
			w * 4);
		m_pVideoDisplayer->displayVideoFrame(m_pData, w*h * 4, w, h);
	}
}

void VideoRenderer::stopRender()
{
	AutoLock<VideoRenderer> lock(this);
	if (m_pVideoDisplayer)
	{
		m_pVideoDisplayer->setVideoRenderer(nullptr);
		m_pVideoDisplayer = nullptr;
	}

	if (m_pData) {
		delete[] m_pData;
		m_pData = nullptr;
	}
}

void VideoRenderer::setVideoDisplayer(IVideoDisplayer* pVideoDisplayer)
{
	AutoLock<VideoRenderer> lock(this);
	m_pVideoDisplayer = pVideoDisplayer;
}
