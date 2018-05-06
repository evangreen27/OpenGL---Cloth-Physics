#pragma once
#include "Core.h"
#include "Channel.h"
Channel::Channel() {}
float Channel::Evaluate(float time) {
	if (numOfKeys == 0) {
		return 0;
	}
	if (numOfKeys == 1) {
		return frames[0]->Value;
	}
	float tmin = frames[0]->Time;
	float tmax = frames[numOfKeys-1]->Time;
	float valbegin = frames[0]->Value;
	float valend = frames[numOfKeys - 1]->Value;
	float tanbegin = frames[0]->TangentIn;
	float tanend = frames[numOfKeys - 1]->TangentOut;
	float timelap = tmax - tmin;
	if (time < tmin) {
		if (std::string(extrapre) == "constant") {
			return valbegin;
		}else if(std::string(extrapre) == "linear"){
			return (time-tmin)*tanbegin + valbegin;
		}else if(std::string(extrapre) == "cycle"){
			return Evaluate(time + timelap);
		}else if(std::string(extrapre) == "cycle_offset"){
			return Evaluate(time + timelap)-(valend-valbegin);
		}else if(std::string(extrapre) == "bounce"){
			return Evaluate(2 * tmin - time);
		}
		return 0;
	} else if(time>tmax){
		if (std::string(extraafter) == "constant") {
			return valend;
		}
		else if (std::string(extraafter) == "linear") {
			return (tmax - time)*tanend + valend;
		}
		else if (std::string(extraafter) == "cycle") {
			return Evaluate(time - timelap);
		}
		else if (std::string(extraafter) == "cycle_offset") {
			return Evaluate(time - timelap) + (valend - valbegin);
		}
		else if (std::string(extraafter) == "bounce") {
			return Evaluate(2 * tmax - time);
		}
		return 0;
	} else{//within channel span
		int i = 1;
		if (time == tmax) {
			i = numOfKeys - 1;
			return valend;
		}
		else {
			while (frames[i]->Time < time) {
				i++;
			}
			i--;
			float A = frames[i]->A;
			float B = frames[i]->B;
			float C = frames[i]->C;
			float D = frames[i]->D;
			float t = (time - frames[i]->Time)/(frames[i+1]->Time-frames[i]->Time);
			float result= A*t*t*t + B*t*t + C*t + D;
			//std::cout << result << std::endl;
			return result;
		}
	}
}
void Channel::Load(Tokenizer &scanner) {
	char buffer[64];
	scanner.GetToken(buffer);//{
	scanner.GetToken(buffer);//extrapolate
	scanner.GetToken(extrapre);
	scanner.GetToken(extraafter);
	scanner.GetToken(buffer);//keys
	numOfKeys = scanner.GetInt();
	scanner.GetToken(buffer);//{
	for(int i=0;i<numOfKeys;i++){
		char tanin[32];
		char tanout[32];
		float tim=scanner.GetFloat();
		float val = scanner.GetFloat();
		scanner.GetToken(tanin);
		scanner.GetToken(tanout);
		KeyFrame * temp = new KeyFrame(tim, val, tanin, tanout);
		//std::cout << tim << " " << val << " " << std::string(tanin) << " " << std::string(tanout) << std::endl;
		frames.push_back(temp);
	}
	scanner.GetToken(buffer);//}
	scanner.GetToken(buffer);//}
}
void Channel::precompute() {
	if (numOfKeys == 1) {
		frames[0]->precomputetan(nullptr, nullptr);
		frames[0]->precomputeconsts(nullptr, nullptr);
	}
	if (numOfKeys > 1) {
		//compute tangents
		frames[0]->precomputetan(nullptr, frames[1]);
		for (int i = 1; i < frames.size()-1; i++)
		{
			frames[i]->precomputetan(frames[i-1],frames[i+1]);
		}
		frames[numOfKeys - 1]->precomputetan(frames[numOfKeys - 2], nullptr);
		//compute constants
		frames[0]->precomputeconsts(nullptr, frames[1]);
		for (int i = 1; i < frames.size() - 1; i++)
		{
			frames[i]->precomputeconsts(frames[i - 1], frames[i + 1]);
		}
		frames[numOfKeys - 1]->precomputeconsts(frames[numOfKeys - 2], nullptr);
	}
}