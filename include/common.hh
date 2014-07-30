#ifndef COMMON_HH
# define COMMON_HH

#include <algorithm>
#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <list>
#include <numeric>
#include <map>
#include <vector>
#include <queue>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <random>
#include <iomanip>
#include <stdexcept>
#include <thread>
#include <mutex>
#include <bench_tool.hh>

#define NOT_IMPLEMENTED() throw std::runtime_error(std::string("Not implemented ")+__FUNCTION__+" " __FILE__ ":"+std::to_string(__LINE__));

#endif /* !COMMON_HH */
