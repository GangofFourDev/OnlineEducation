/*
*  Copyright 2012 The WebRTC Project Authors. All rights reserved.
*
*  Use of this source code is governed by a BSD-style license
*  that can be found in the LICENSE file in the root of the source
*  tree. An additional intellectual property rights grant can be found
*  in the file PATENTS.  All contributing project authors may
*  be found in the AUTHORS file in the root of the source tree.
*/
#ifndef WEBRTC_EXAMPLES_PEERCONNECTION_CLIENT_CONDUCTOR_H_
#define WEBRTC_EXAMPLES_PEERCONNECTION_CLIENT_CONDUCTOR_H_

#include "webrtc/api/mediastreaminterface.h"
#include "webrtc/api/peerconnectioninterface.h"
#include "webrtc/base/scoped_ref_ptr.h"
#include "webrtc/base/thread.h"
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>
#include <assert.h>
#include <deque>
#include <map>
#include <set>
#include <queue>
#include <vector>
#include <hash_map>
#include <string>

#include "webrtc/base/common.h"
#include "webrtc/base/json.h"
#include "webrtc/base/logging.h"
#include "webrtc/base/ssladapter.h"
#include "webrtc/base/win32socketinit.h"
#include "webrtc/base/win32socketserver.h"
#include "Poco/Mutex.h"
#include "Poco/ScopedLock.h"
#include "Poco/Timer.h"
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Parser.h"
#include "Poco/JSON/Query.h"
#include "Poco/JSON/JSONException.h"
#include "Poco/JSON/Stringifier.h"
#include "Poco/JSON/Handler.h"
#include "Poco/JSON/Template.h"
#include "Poco/JSON/Array.h"

#include "webrtc/media/engine/webrtcvideocapturerfactory.h"
#include "webrtc/api/test/fakeconstraints.h"
#include "webrtc/base/common.h"
#include "webrtc/base/logging.h"
#include "webrtc/modules/video_capture/video_capture_factory.h"
#include "webrtc/base/common.h"
#include "webrtc/base/logging.h"

#include "webrtc/media/base/screencastid.h"
#include "webrtc/media/base/videocapturerfactory.h"


#include "webrtc\modules\audio_device\include\audio_device.h"
#include "webrtc\media\engine\webrtcvideocapturer.h"

#include "global.h"

using namespace webrtc;

class TEXPORT PeerConnection //: public webrtc::PeerConnectionObserver, public webrtc::CreateSessionDescriptionObserver
{
	PeerConnection();

	void SignalFrameCapturedDisconnect(sigslot::has_slots_interface* pclass);

private:
	cricket::WebRtcVideoCapturer *m_videoCapturer;
};
#endif  // WEBRTC_EXAMPLES_PEERCONNECTION_CLIENT_CONDUCTOR_H_