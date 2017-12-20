#include "PeerConnection.h"


using webrtc::FakeConstraints;
using webrtc::MediaConstraintsInterface;

#define DTLS_ON  true
#define DTLS_OFF false

PeerConnection::PeerConnection()
{
	m_videoCapturer = new cricket::WebRtcVideoCapturer();
}

void PeerConnection::SignalFrameCapturedDisconnect(sigslot::has_slots_interface* pclass)
{
	m_videoCapturer->SignalFrameCaptured.disconnect(pclass);
}

//void PeerConnection::OnSignalingChange(PeerConnectionInterface::SignalingState new_state)
//{
//	throw std::logic_error("The method or operation is not implemented.");
//}
//
//void PeerConnection::OnRenegotiationNeeded()
//{
//	throw std::logic_error("The method or operation is not implemented.");
//}
//
//void PeerConnection::OnIceConnectionChange(PeerConnectionInterface::IceConnectionState new_state)
//{
//	throw std::logic_error("The method or operation is not implemented.");
//}
//
//void PeerConnection::OnIceGatheringChange(PeerConnectionInterface::IceGatheringState new_state)
//{
//	throw std::logic_error("The method or operation is not implemented.");
//}
//
//void PeerConnection::OnIceCandidate(const IceCandidateInterface* candidate)
//{
//	throw std::logic_error("The method or operation is not implemented.");
//}
//
//void PeerConnection::OnSuccess(SessionDescriptionInterface* desc)
//{
//	throw std::logic_error("The method or operation is not implemented.");
//}
//
//void PeerConnection::OnFailure(const std::string& error)
//{
//	throw std::logic_error("The method or operation is not implemented.");
//}
//
//int PeerConnection::AddRef() const
//{
//	throw std::logic_error("The method or operation is not implemented.");
//}
//
//int PeerConnection::Release() const
//{
//	throw std::logic_error("The method or operation is not implemented.");
//}
