#include "pch.h"
#include "CorePch.h"

#include <thread>
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>
#include "ThreadManager.h"

#include "RefCounting.h"
#include "Memory.h"
#include "Allocator.h"

using TL = TypeList<class Player, class Mage, class Knight, class Archer>;


class Player
{

public:
	Player()
	{
		INIT_TL(Player);
	}

	virtual ~Player(){}
	DECLARE_TL


};

class Knight : public Player
{
public:

	Knight() { INIT_TL(Knight); }
};

class Mage : public Player
{
public:
	Mage(){ INIT_TL(Mage); }
};

class Archer : public Player
{
public:
	Archer() { INIT_TL(Archer); }
};



int main()
{
	//TypeList<Mage, Knight>::Head  whoAmI;
	//TypeList<Mage, Knight>::Tail  whoAmI2;

	//TypeList<Mage, TypeList<Knight,Archer>>::Head whoAmI3;//mage
	//TypeList<Mage, TypeList<Knight,Archer>>::Tail::Head whoAmI4;//Knight
	//TypeList<Mage, TypeList<Knight,Archer>>::Tail::Tail whoAmI5;//Archer

	//Length<TypeList<Mage, Knight>>::value;

	//TypeAt<TL, 0>::Result whoAmI6;

	//int32 index1 = IndexOf<TL, Mage>::value;

	//bool canConvert1 = Conversion<Player, Knight>::exists;
	//bool canConvert2 = Conversion<Knight, Player>::exists;
	//bool canConvert3 = Conversion<Knight, Mage>::exists;
	
	{
		Player* player = new Player();
		bool canCast = CanCast<Knight*>(player);
		Knight* knight = TypeCast<Knight*>(player);

		delete player;
	}

	for (int32 i = 0; i < 5; i++)
	{
		GThreadManager->Launch([]()
			{
				while (true)
				{
					
				}
			});
	}
	GThreadManager->Join();
}

