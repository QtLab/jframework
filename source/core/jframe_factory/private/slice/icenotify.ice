#pragma once

[["java:package:com.smartsoft.jframe"]]

[["cpp:include:list"]]
#include <Ice/BuiltinSequences.ice>

module notify {
    //
	["cpp:type:::std::list< ::std::string>"] sequence<string> StringList;
	
    // 
    interface JIceNotifyDisp {
    	int sendBuffer(["cpp:array"] Ice::ByteSeq buffer);
    	int sendString(string msg);
    	int sendList(StringList data);
    	void postBuffer(["cpp:array"] Ice::ByteSeq buffer);
    	void postString(string msg);
    	void postList(StringList data);
    };
};
