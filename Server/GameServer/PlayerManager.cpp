#include "pch.h"
#include "PlayerManager.h"
#include "AccountManager.h"

PlayerManager GPlayerMananger;


void PlayerManager::PlayerThenAccount()
{
	WRITE_LOCK;
	
	GAccountMananger.Lock();
}

void PlayerManager::Lock()
{
	WRITE_LOCK;
}
