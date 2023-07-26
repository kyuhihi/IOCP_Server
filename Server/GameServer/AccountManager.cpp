#include "pch.h"
#include "AccountManager.h"
#include "UserManager.h"

void AccountManager::ProcessLogin()
{
	//accountLock
	lock_guard<mutex> guard(m_mutex);

	//userLock
	User* user = UserManager::Instance()->GetUser(100);

}
