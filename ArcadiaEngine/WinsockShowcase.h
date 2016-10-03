#pragma once

#include "Engine/GUIObjectNode.h"
#include "Engine/GUIListBox.h"
#include "Engine/WinsockWrapper.h"

class WinsockShowcaseDialogue : public GUIObjectNode
{
public:
	WinsockShowcaseDialogue();
	~WinsockShowcaseDialogue();
};

inline WinsockShowcaseDialogue::WinsockShowcaseDialogue()
{
	//  Test the Winsock Wrapper
	auto googleIP = Socket::GetHostIP("www.google.com");
	auto ipAddress = winsockWrapper.ConvertIPtoUINT(googleIP.c_str());
	auto ipString = winsockWrapper.ConvertUINTtoIP(ipAddress);
}

inline WinsockShowcaseDialogue::~WinsockShowcaseDialogue()
{
}