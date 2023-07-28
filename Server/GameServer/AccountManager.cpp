#include "pch.h"
#include "AccountManager.h"
#include "PlayerManager.h"

AccountManager GAccountMananger;

void AccountManager::AccountThenPlayer()
{
	WRITE_LOCK;
	GPlayerMananger.Lock();
}

void AccountManager::Lock()
{
	WRITE_LOCK;

}
