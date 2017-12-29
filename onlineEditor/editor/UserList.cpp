#include "UserList.h"

#include <assert.h>
#include "Observer.h"

UserList::UserList() : m_pCurrentUserInfo(nullptr)
{
	
}


UserList::~UserList()
{
	clear();
}

void UserList::addUserInfo(UserInfo* pUserInfo)
{
	if (pUserInfo->role() == enum_Compere)
	{
		m_oUserInfoList.insert(m_oUserInfoList.begin(), pUserInfo);
	}
	else
	{
		m_oUserInfoList.push_back(pUserInfo);
	}

	add(pUserInfo);
}

void UserList::addUserInfo(__int64 nUserId, std::string strUserName, bool bAudio, bool bVideo, ENUM_Role enumRole, bool bHasCam, TerminalType terType)
{
	UserInfo* pUserInfo = new UserInfo(nUserId, strUserName, bAudio, bVideo, enumRole, bHasCam, terType);
	addUserInfo(pUserInfo);
}

void UserList::removeUserInfo(__int64 nUserId)
{
	for (auto it = m_oUserInfoList.begin(); it != m_oUserInfoList.end(); ++it)
	{
		if ((*it)->userId() == nUserId)
		{
			delete *it;
			m_oUserInfoList.erase(it);
			break;
		}
	}

	remove(nUserId);
}

UserInfo* UserList::userInfoById(__int64 nUserId)
{
	for (auto it = m_oUserInfoList.begin(); it != m_oUserInfoList.end(); ++it)
	{
		if ((*it)->userId() == nUserId)
		{
			return *it;
		}
	}

	return nullptr;
}

void UserList::updateUserInfoById(__int64 nUserId, std::string strUserName, bool bAudio, bool bVideo, ENUM_Role enumRole, bool bHasCam)
{
	UserInfo* pUserInfo = userInfoById(nUserId);
	if (pUserInfo != nullptr)
	{
		pUserInfo->setUserName(strUserName);
		pUserInfo->setAudio(bAudio);
		pUserInfo->setVideo(bVideo);
		pUserInfo->setRole(enumRole);
		pUserInfo->setHasCam(bHasCam);
		updateUserInfo(pUserInfo);
	}
}

void UserList::updateUserInfo(UserInfo* pUserInfo)
{
	update(pUserInfo);
}

void UserList::clear()
{
	for (auto it = m_oUserInfoList.begin(); it != m_oUserInfoList.end(); ++it)
	{
		delete *it;
	}
	m_oUserInfoList.clear();
	m_pCurrentUserInfo = nullptr;
}

int UserList::count()
{
	return m_oUserInfoList.size();
}

UserInfo* UserList::userInfoByIndex(int nIndex)
{
	assert(nIndex >= 0 && nIndex < m_oUserInfoList.size());
	return m_oUserInfoList[nIndex];
}

UserInfo* UserList::currentUserInfo()
{
	return m_pCurrentUserInfo;
}

void UserList::setCurrentUserInfo(UserInfo* pUserInfo)
{
	m_pCurrentUserInfo = pUserInfo;
}

void UserList::attach(Observer* pObserver)
{
	m_oObserver.push_back(pObserver);
}

void UserList::detach(Observer* pObserver)
{
	for (auto it = m_oObserver.begin(); it != m_oObserver.end(); ++it)
	{
		if (pObserver == *it)
		{
			m_oObserver.erase(it);
			return;
		}
	}
}

void UserList::update(UserInfo* pUserInfo)
{
	for (auto it = m_oObserver.begin(); it != m_oObserver.end(); ++it)
	{
		(*it)->update(pUserInfo);
	}
}

void UserList::add(UserInfo* pUserInfo)
{
	for (auto it = m_oObserver.begin(); it != m_oObserver.end(); ++it)
	{
		(*it)->add(pUserInfo);
	}
}

void UserList::remove(__int64 nUserId)
{
	for (auto it = m_oObserver.begin(); it != m_oObserver.end(); ++it)
	{
		(*it)->remove(nUserId);
	}
}
