#pragma once
#include "Core.h"
#include "Tokenizer.h"
#include "Channel.h"
#include "Skeleton.h"
class AnimationClip {
public:
	float starttime;
	float endtime;
	int numChannels;
	float systemtime;
	std::vector<Channel*> channels;
	AnimationClip(const char * filename);
	void Evaluate(float time, Skeleton *skel);
	void Load(const char *filename);
	void precompute();
	void Update(Skeleton *skel);
};

