#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#ifdef _WIN32
 #include "..\public\dataframe.h"
#else
 #include "../public/dataframe.h"
#endif

DataFrame read_csv();
