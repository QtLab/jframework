#pragma once

[["java:package:com.smartsoft.jframe"]]

[["cpp:include:list"]]
#include <Ice/BuiltinSequences.ice>

module Notify {
    //
	["cpp:type:::std::list< ::std::string>"] sequence<string> StringList;
	
    // 
    interface JIceNotify {
    	int sendBuffer(string domain, ["cpp:array"] Ice::ByteSeq buffer);
    	int sendString(string domain, string msg);
    	int sendList(string domain, StringList data);
    	void postBuffer(string domain, ["cpp:array"] Ice::ByteSeq buffer);
    	void postString(string domain, string msg);
    	void postList(string domain, StringList data);
    };
};
