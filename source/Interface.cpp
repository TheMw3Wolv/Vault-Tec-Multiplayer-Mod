#include "Interface.h"

PipeClient* Interface::pipeServer;
PipeServer* Interface::pipeClient;
Interface::ResultHandler Interface::resultHandler;
bool Interface::endThread = false;
bool Interface::wakeup = false;
bool Interface::initialized = false;
Interface::PriorityMap Interface::priorityMap;
Interface::StaticCommandList Interface::static_cmdlist;
Interface::DynamicCommandList Interface::dynamic_cmdlist;
Interface::Native Interface::natives;
thread Interface::hCommandThreadReceive;
thread Interface::hCommandThreadSend;
CriticalSection Interface::static_cs;
CriticalSection Interface::dynamic_cs;

#ifdef VAULTMP_DEBUG
Debug* Interface::debug;
#endif

bool Interface::Initialize(ResultHandler resultHandler, bool steam)
{
	if (!initialized)
	{
		endThread = false;
		wakeup = false;

		Interface::resultHandler = resultHandler;

		pipeClient = new PipeServer();
		pipeServer = new PipeClient();

		hCommandThreadReceive = thread(CommandThreadReceive, steam);
		hCommandThreadSend = thread(CommandThreadSend);

		if (!hCommandThreadReceive.joinable() || !hCommandThreadSend.joinable())
		{
			delete pipeServer;
			delete pipeClient;
			endThread = true;
			return false;
		}

		initialized = true;

#ifdef VAULTMP_DEBUG
		//static_cs.SetDebugHandler(debug);
		//dynamic_cs.SetDebugHandler(debug);
		if (debug)
			debug->PrintFormat("Threads %s %s %p %p", true, CriticalSection::thread_id(hCommandThreadReceive).c_str(), CriticalSection::thread_id(hCommandThreadSend).c_str(), &static_cs, &dynamic_cs);
#endif

		return true;
	}

	return false;
}

void Interface::Terminate()
{
	if (initialized)
	{
		endThread = true;

		if (hCommandThreadReceive.joinable())
			hCommandThreadReceive.join();

		if (hCommandThreadSend.joinable())
			hCommandThreadSend.join();

		static_cmdlist.clear();
		dynamic_cmdlist.clear();
		priorityMap.clear();
		natives.clear();

		delete pipeServer;
		delete pipeClient;

		initialized = false;
	}
}

#ifdef VAULTMP_DEBUG
void Interface::SetDebugHandler(Debug* debug)
{
	Interface::debug = debug;

	if (debug)
		debug->Print("Attached debug handler to Interface class", true);
}
#endif

bool Interface::IsAvailable()
{
	return (wakeup && !endThread && hCommandThreadReceive.joinable() && hCommandThreadSend.joinable());
}

void Interface::StartSetup()
{
	static_cs.StartSession();

	static_cmdlist.clear();
	priorityMap.clear();
}

void Interface::EndSetup()
{
	vector<unsigned int> priorities;
	priorities.reserve(priorityMap.size());

	for (const pair<unsigned int, Native::iterator>& priority : priorityMap)
		priorities.push_back(priority.first);

	vector<unsigned int>::iterator it2 = unique(priorities.begin(), priorities.end());
	priorities.resize(it2 - priorities.begin());

	auto gcd = [](unsigned int x, unsigned int y)
	{
		for (;;)
		{
			if (x == 0)
				return y;

			y %= x;

			if (y == 0)
				return x;

			x %= y;
		}
	};

	auto lcm = [=](int x, int y)
	{
		unsigned int temp = gcd(x, y);
		return temp ? (x / temp * y) : 0;
	};

	unsigned int result = accumulate(priorities.begin(), priorities.end(), 1, lcm);

	for (unsigned int i = 0; i < result; ++i)
	{
		vector<Native::iterator> content;

		for (const pair<unsigned int, Native::iterator>& priority : priorityMap)
			if (((i + 1) % priority.first) == 0)
				content.push_back(priority.second);

		static_cmdlist.push_back(move(content));
	}

	static_cs.EndSession();
}

void Interface::StartDynamic()
{
	dynamic_cs.StartSession();
}

void Interface::EndDynamic()
{
	dynamic_cs.EndSession();
}

void Interface::SetupCommand(string name, ParamContainer&& param, unsigned int priority)
{
	priorityMap.insert(make_pair(priority, natives.insert(make_pair(name, move(param)))));
}

void Interface::ExecuteCommand(string name, ParamContainer&& param, unsigned int key)
{
	dynamic_cmdlist.push_back(make_pair(natives.insert(make_pair(name, move(param))), key));
}

vector<string> Interface::Evaluate(Native::iterator _it)
{
	string name = _it->first;
	ParamContainer& param = _it->second;

	unsigned int i = 0;
	unsigned int rsize = 1;
	unsigned int lsize = param.size();

	vector<unsigned int> mult;
	vector<string> result;
	mult.reserve(lsize);
	result.reserve(lsize);

	for (i = lsize; i != 0; --i)
	{
		param[i - 1].reset(); // reset to initial state (important for functors)
		const vector<string>& params = param[i - 1].get();

		if (params.empty())
			return result;

		mult.insert(mult.begin(), rsize);
		rsize *= params.size();
	}

	for (i = 0; i < rsize; ++i)
	{
		string cmd = name;

		for (unsigned int j = 0; j < lsize; ++j)
		{
			unsigned int idx = ((unsigned int)(i / mult[j])) % param[j].get().size();
			cmd += " " + Utils::str_replace(param[j].get()[idx], " ", "|");
		}

		result.push_back(cmd);
	}

	return result;
}

void Interface::CommandThreadReceive(bool steam)
{
	try
	{
		pipeClient->SetPipeAttributes("BethesdaClient", PIPE_LENGTH);
		pipeClient->CreateServer();
		pipeClient->ConnectToServer();

		pipeServer->SetPipeAttributes("BethesdaServer", PIPE_LENGTH);

		while (!pipeServer->ConnectToServer() && !endThread);

		unsigned char buffer[PIPE_LENGTH];

		buffer[0] = steam;
		pipeClient->Send(buffer);

		if (!endThread)
		{
			unsigned char code;

			do
			{
				ZeroMemory(buffer, sizeof(buffer));

				pipeClient->Receive(buffer);
				code = buffer[0];

				if (code == PIPE_OP_RETURN || code == PIPE_OP_RETURN_BIG || code == PIPE_OP_RETURN_RAW)
				{
					vector<CommandResult> result = API::Translate(buffer);

					for (CommandResult& _result : result)
						resultHandler(get<0>(_result), get<1>(_result), get<2>(_result), get<3>(_result));
				}
				else if (code == PIPE_SYS_WAKEUP)
				{
					wakeup = true;

#ifdef VAULTMP_DEBUG

					if (debug)
						debug->Print("vaultmp process waked up (game patched)", true);

#endif
				}
				else if (code == PIPE_ERROR_CLOSE)
				{
					if (!endThread)
						throw VaultException("Error in vaultmp.dll");
				}
				else if (code)
					throw VaultException("Unknown pipe code identifier %02X", code);
			}
			while (code && code != PIPE_ERROR_CLOSE && !endThread);
		}
	}

	catch (std::exception& e)
	{
		try
		{
			VaultException& vaulterror = dynamic_cast<VaultException&>(e);
			vaulterror.Message();
		}

		catch (std::bad_cast& no_vaulterror)
		{
			VaultException vaulterror(e.what());
			vaulterror.Message();
		}

#ifdef VAULTMP_DEBUG

		if (debug)
			debug->Print("Receive thread is going to terminate (ERROR)", true);

#endif
	}

	endThread = true;
}

void Interface::CommandThreadSend()
{
	try
	{
		while (!wakeup && !endThread)
			this_thread::sleep_for(chrono::milliseconds(10));

		while (!endThread)
		{
			StaticCommandList::iterator it;

			static_cs.StartSession();

			for (it = static_cmdlist.begin(); (it != static_cmdlist.end() || !dynamic_cmdlist.empty()) && !endThread;)
			{
				if (it != static_cmdlist.end())
				{
					vector<Native::iterator>::iterator it2;
					vector<Native::iterator>& next_list = *it;

					for (it2 = next_list.begin(); it2 != next_list.end() && !endThread; ++it2)
					{
						vector<string> cmd = Interface::Evaluate(*it2);

						if (!cmd.empty())
						{
							CommandParsed stream = API::Translate(cmd);
							CommandParsed::iterator it;

							for (it = stream.begin(); it != stream.end() && !endThread; ++it)
								pipeServer->Send(it->get());
						}
					}

					++it;
				}

				dynamic_cs.StartSession();

				for (; !dynamic_cmdlist.empty() && !endThread; natives.erase(dynamic_cmdlist.front().first), dynamic_cmdlist.pop_front())
				{
					dynamic_cs.EndSession();

					auto dynamic = dynamic_cmdlist.front();

					vector<string> cmd = Interface::Evaluate(dynamic.first);

					if (!cmd.empty())
					{
						CommandParsed stream = API::Translate(cmd, dynamic.second);
						CommandParsed::iterator it;

						for (it = stream.begin(); it != stream.end() && !endThread; ++it)
							pipeServer->Send(it->get());
					}

					dynamic_cs.StartSession();
				}

				dynamic_cs.EndSession();

				this_thread::sleep_for(chrono::milliseconds(1));
			}

			static_cs.EndSession();
		}
	}
	catch (std::exception& e)
	{
		try
		{
			VaultException& vaulterror = dynamic_cast<VaultException&>(e);
			vaulterror.Message();
		}
		catch (std::bad_cast& no_vaulterror)
		{
			VaultException vaulterror(e.what());
			vaulterror.Message();
		}

#ifdef VAULTMP_DEBUG

		if (debug)
			debug->Print("Send thread is going to terminate (ERROR)", true);

#endif
	}

	if (wakeup)
	{
		unsigned char buffer[PIPE_LENGTH];
		buffer[0] = PIPE_ERROR_CLOSE;
		pipeServer->Send(buffer);
	}

	endThread = true;
}
