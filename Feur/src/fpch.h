/// precompile Headers
/// These headers are not recompiled each build
#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Feur/Log.h"


#if F_PLATFORM_WINDOWS
	#include <Windows.h>
#endif
