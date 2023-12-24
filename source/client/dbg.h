#pragma once
#define DEBUG_MAX_LEN 256

#ifdef _DEBUG

#define DEBUG_START() AllocConsole()

#define DEBUG_FINISH() FreeConsole()

#define DEBUG(text) WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), text, strlen(text), NULL, NULL); 

#define DEBUG_ARGS(text, ...) do { char msg_f[DEBUG_MAX_LEN] = ""; \
                            sprintf(msg_f, text, __VA_ARGS__); \
                            DEBUG(msg_f) \
                            } while(0)

#else

#define DEBUG_START() 

#define DEBUG_FINISH()

#define DEBUG(text) 

#define DEBUG_ARGS(text, ...) 

#endif