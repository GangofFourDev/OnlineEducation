#ifndef OBSERVER_H__
#define OBSERVER_H__

class UserInfo;
class Observer
{
public:
	virtual ~Observer() {};
public:
	virtual void update(UserInfo* pUserInfo) = 0;
	virtual void add(UserInfo* pUserInfo) {};
	virtual void remove(__int64 nUserId) {};
};

#endif // OBSERVER_H__