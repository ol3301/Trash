#pragma once
#define DEBUG

#ifdef DEBUG
#define WRITE_LOG(msg) std::cout << msg << std::endl;
#else
#define WRITE_LOG
#endif