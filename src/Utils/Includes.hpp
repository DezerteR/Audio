#pragma once

#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <random>
#include <unordered_map>
#include <functional>
#include <sstream>
#include <chrono>
#include <string>
#include <unordered_map>
#include <iomanip>
#include <type_traits>
#include <memory>

#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <list>
#include <fstream>
#include <ctime>
#include <cmath>

#define GLM_FORCE_SWIZZLE
#define GLM_FORCE_RADIANS
#define GLM_FORCE_SSE2
#include "glm/fwd.hpp"
#include "glm/glm.hpp"

#include <AL/al.h>
#include <AL/alc.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

using std::cout;
using std::cerr;
using std::endl;
using std::shared_ptr;
using std::unique_ptr;
using std::make_shared;
using std::make_unique;

#define FORCE_MONO true
