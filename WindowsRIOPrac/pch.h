#pragma once

#pragma warning(disable : 4355)
#pragma warning(disable : 4482)

#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>
#include <Windows.h>
#include <map>
#include <vector>
#include <sstream>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <rpc.h>

//////////////////////////////
#pragma comment(lib, "Ws2_32")
#pragma comment(lib, "rpcrt4")


/////////////////////////////
//// CUSTOM
/////////////////////////////

#include "Assert.h"