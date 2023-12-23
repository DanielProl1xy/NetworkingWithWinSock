#pragma once
#define DEBUG_MAX_LEN 256

#define DEBUG(text) WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), text, strlen(text), NULL, NULL); 

#define DEBUG_ARGS(text, ...) char msg_f[DEBUG_MAX_LEN] = ""; \
                            sprintf(msg_f, text, __VA_ARGS__); \
                            DEBUG(msg_f)

