#include "vaultmp.h"

#include <winsock2.h>
#include <cstdio>
#include <shlwapi.h>
#include <shlobj.h>
#include <list>
#include <vector>
#include <string>
#include <thread>
#include <chrono>

#include "RakNet/RakPeerInterface.h"
#include "RakNet/MessageIdentifiers.h"
#include "RakNet/BitStream.h"
#include "RakNet/RakSleep.h"

#include "Data.h"
#include "Player.h"
#include "Container.h"
#include "Interface.h"
#include "Lockable.h"
#include "Game.h"
#include "VaultException.h"
#include "NetworkClient.h"
#include "GameFactory.h"

#ifdef VAULTMP_DEBUG
#include "Debug.h"
#endif

using namespace RakNet;
using namespace Data;
using namespace std;

typedef pair<string, unsigned int> Savegame;
typedef vector<pair<string, unsigned int> > ModList;

/**
 * \brief Starting point to run a vaultmp game
 */

class Bethesda
{
		friend class NetworkClient;
		friend class Game;

	private:
		static unsigned char game;
		static string password;
		static Savegame savegame;
		static ModList modfiles;
		static char module[32];
		static bool initialized;

		/**
		 * \brief Starts up the game
		 */
		static void Initialize();

		/**
		 * \brief Kills the game process
		 */
		static void Terminate();

		/**
		 * \brief Handles translated command results from the game
		 */
		static void CommandHandler(signed int key, vector<double>& info, double result, bool error);

		/**
		 * \brief Lookup the ID of a given process name
		 */
		static DWORD lookupProgramID(const char process[]);
#ifdef VAULTMP_DEBUG
		static Debug* debug;
#endif

		Bethesda();
	public:

		/**
		 * \brief Initializes Vault-Tec Multiplayer Mod
		 */
		static void InitializeVaultMP(RakPeerInterface* peer, SystemAddress server, string name, string pwd, unsigned char game);

};
