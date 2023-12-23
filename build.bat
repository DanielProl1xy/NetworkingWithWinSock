
cl /c source/client/chatwidget.cpp source/client/chat.cpp source/client/main.cpp source/client/tcpclient.cpp source/server/server.cpp
lib chatwidget.obj /OUT:build/chatwidget.lib
link tcpclient.obj chat.obj main.obj build/chatwidget.lib /OUT:build/client.exe
link server.obj /OUT:build/server.exe