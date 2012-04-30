#include "NetworkServer.h"
#include "Dedicated.h"

#ifdef VAULTMP_DEBUG
Debug* NetworkServer::debug = NULL;
#endif

#ifdef VAULTMP_DEBUG
void NetworkServer::SetDebugHandler(Debug* debug)
{
	NetworkServer::debug = debug;

	if (debug)
		debug->Print("Attached debug handler to Network class", true);
}
#endif

NetworkResponse NetworkServer::ProcessEvent(unsigned char id)
{
	NetworkResponse response;

	switch (id)
	{
		case ID_EVENT_SERVER_ERROR:
		{
			pDefault* packet = PacketFactory::CreatePacket(ID_GAME_END, ID_REASON_ERROR);
			response = Network::CompleteResponse(Network::CreateResponse(packet,
																		 (unsigned char) HIGH_PRIORITY,
																		 (unsigned char) RELIABLE_ORDERED,
																		 CHANNEL_GAME,
																		 Client::GetNetworkList(NULL)));
			break;
		}

		default:
			throw VaultException("Unhandled event type %d", id);
	}

	return response;
}

NetworkResponse NetworkServer::ProcessPacket(Packet* data)
{
	NetworkResponse response;
	pDefault* packet;

	switch (data->data[0])
	{
		case ID_CONNECTION_REQUEST_ACCEPTED:
		{
			Utils::timestamp();
			printf("Connected to MasterServer (%s)\n", data->systemAddress.ToString());
#ifdef VAULTMP_DEBUG
			debug->PrintFormat("Connected to MasterServer (%s)", true, data->systemAddress.ToString());
#endif

			Dedicated::master = data->systemAddress;
			Dedicated::Announce(true);
			break;
		}

		case ID_DISCONNECTION_NOTIFICATION:
		case ID_CONNECTION_LOST:
		{
			Utils::timestamp();

			switch (data->data[0])
			{
				case ID_DISCONNECTION_NOTIFICATION:
					printf("Client disconnected (%s)\n", data->systemAddress.ToString());
#ifdef VAULTMP_DEBUG
					debug->PrintFormat("Client disconnected (%s)", true, data->systemAddress.ToString());
#endif
					break;

				case ID_CONNECTION_LOST:
					printf("Lost connection (%s)\n", data->systemAddress.ToString());
#ifdef VAULTMP_DEBUG
					debug->PrintFormat("Lost connection (%s)", true, data->systemAddress.ToString());
#endif
					break;
			}

			response = Server::Disconnect(data->guid, ID_REASON_ERROR);
			break;
		}

		case ID_NEW_INCOMING_CONNECTION:
		{
			Utils::timestamp();
			printf("New incoming connection from %s\n", data->systemAddress.ToString());
#ifdef VAULTMP_DEBUG
			debug->PrintFormat("New incoming connection from %s", true, data->systemAddress.ToString());
#endif
			break;
		}

		case ID_INVALID_PASSWORD:
		{
			Utils::timestamp();
			printf("MasterServer version mismatch (%s)\n", data->systemAddress.ToString());
#ifdef VAULTMP_DEBUG
			debug->PrintFormat("MasterServer version mismatch (%s)", true, data->systemAddress.ToString());
#endif
			break;
		}

		case ID_CONNECTED_PING:
		case ID_UNCONNECTED_PING:
		case ID_CONNECTION_ATTEMPT_FAILED:
			break;

		default:
		{
			packet = PacketFactory::CreatePacket(data->data, data->length);

			try
			{
				switch (data->data[0])
				{
					case ID_GAME_AUTH:
					{
						char name[MAX_PLAYER_NAME + 1];
						ZeroMemory(name, sizeof(name));
						char pwd[MAX_PASSWORD_SIZE + 1];
						ZeroMemory(pwd, sizeof(pwd));
						PacketFactory::Access(packet, name, pwd);
						response = Server::Authenticate(data->guid, string(name), string(pwd));
						break;
					}

					case ID_GAME_LOAD:
					{
						response = Server::LoadGame(data->guid);
						break;
					}

					case ID_GAME_END:
					{
						unsigned char reason;
						PacketFactory::Access(packet, &reason);
						response = Server::Disconnect(data->guid, reason);
						break;
					}

					case ID_PLAYER_NEW:
					{
						NetworkID id = GameFactory::CreateKnownInstance(ID_PLAYER, packet);
						response = Server::NewPlayer(data->guid, id);
						break;
					}

					case ID_OBJECT_UPDATE:
					case ID_CONTAINER_UPDATE:
					case ID_ACTOR_UPDATE:
					case ID_PLAYER_UPDATE:
					{
						switch (data->data[1])
						{
							case ID_UPDATE_POS:
							{
								NetworkID id;
								double X, Y, Z;
								PacketFactory::Access(packet, &id, &X, &Y, &Z);
								FactoryObject reference = GameFactory::GetObject(id);
								response = Server::GetPos(data->guid, reference, X, Y, Z);
								break;
							}

							case ID_UPDATE_ANGLE:
							{
								NetworkID id;
								unsigned char axis;
								double value;
								PacketFactory::Access(packet, &id, &axis, &value);
								FactoryObject reference = GameFactory::GetObject(id);
								response = Server::GetAngle(data->guid, reference, axis, value);
								break;
							}

							case ID_UPDATE_CELL:
							{
								NetworkID id;
								unsigned int cell;
								PacketFactory::Access(packet, &id, &cell);
								FactoryObject reference = GameFactory::GetObject(id);
								response = Server::GetCell(data->guid, reference, cell);
								break;
							}

							case ID_UPDATE_CONTAINER:
							{
								NetworkID id;
								ContainerDiff diff;
								PacketFactory::Access(packet, &id, &diff);
								FactoryObject reference = GameFactory::GetObject(id);
								response = Server::GetContainerUpdate(data->guid, reference, diff);
								break;
							}

							case ID_UPDATE_VALUE:
							{
								NetworkID id;
								bool base;
								unsigned char index;
								double value;
								PacketFactory::Access(packet, &id, &base, &index, &value);
								FactoryObject reference = GameFactory::GetObject(id);
								response = Server::GetActorValue(data->guid, reference, base, index, value);
								break;
							}

							case ID_UPDATE_STATE:
							{
								NetworkID id;
								unsigned char index;
								unsigned char moving;
								bool alerted;
								bool sneaking;
								PacketFactory::Access(packet, &id, &index, &moving, &alerted, &sneaking);
								FactoryObject reference = GameFactory::GetObject(id);
								response = Server::GetActorState(data->guid, reference, index, moving, alerted, sneaking);
								break;
							}

							case ID_UPDATE_DEAD:
							{
								NetworkID id;
								bool dead;
								PacketFactory::Access(packet, &id, &dead);
								FactoryObject reference = GameFactory::GetObject(id);
								response = Server::GetActorDead(data->guid, reference, dead);
								break;
							}

							case ID_UPDATE_CONTROL:
							{
								NetworkID id;
								unsigned char control;
								unsigned char key;
								PacketFactory::Access(packet, &id, &control, &key);
								FactoryObject reference = GameFactory::GetObject(id);
								response = Server::GetPlayerControl(data->guid, reference, control, key);
								break;
							}

							default:
								throw VaultException("Unhandled object update packet type %d", (int) data->data[1]);
						}

						break;
					}

					default:
						throw VaultException("Unhandled packet type %d", (int) data->data[0]);
				}
			}
			catch (...)
			{
				PacketFactory::FreePacket(packet);
				throw;
			}

			PacketFactory::FreePacket(packet);
		}
	}

	return response;
}
