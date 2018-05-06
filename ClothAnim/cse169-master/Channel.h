#pragma once
#include "Core.h"
#include "KeyFrame.h"
#include "Tokenizer.h"
class Channel {
public:
	std::vector<KeyFrame*> frames;
	char extrapre[32], extraafter[32];
	int numOfKeys;
	Channel();
	float Evaluate(float time);
	void Load(Tokenizer &scanner);
	void precompute();
};