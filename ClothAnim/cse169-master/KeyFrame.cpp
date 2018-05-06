#pragma once
#include "KeyFrame.h"
KeyFrame::KeyFrame(float Time, float val, char* Rulein, char* Ruleout) {
	this->Time = Time;
	this->Value = val;
	this->RuleIn = std::string(Rulein);
	this->RuleOut = std::string(Ruleout);
}
void KeyFrame::precomputetan(KeyFrame* preframe, KeyFrame* nextframe) {
	//in tangent
	if (std::string(RuleIn) == "flat") {
		TangentIn = 0;
	}
	else if (std::string(RuleIn) == "linear") {
		linearcomputetan(preframe, nextframe, true);
	}
	else if (std::string(RuleIn) == "smooth") {
		if (preframe != NULL && nextframe != NULL) {
			TangentIn = (nextframe->Value - preframe->Value) / (nextframe->Time - preframe->Time);
		}else{
			linearcomputetan(preframe, nextframe, true);
		}
	}
	else
	{
		TangentIn = std::stof(std::string(RuleIn));
	}

	//out tangent
	if (std::string(RuleOut) == "flat") {
		TangentOut = 0;
	}
	else if (std::string(RuleOut) == "linear") {
		linearcomputetan(preframe, nextframe, false);
	}
	else if (std::string(RuleOut) == "smooth") {
		if (preframe != NULL && nextframe != NULL) {
			TangentOut = (nextframe->Value - preframe->Value) / (nextframe->Time - preframe->Time);
		}
		else {
			linearcomputetan(preframe, nextframe, false);
		}
	}
	else
	{
		TangentOut = std::stof(std::string(RuleOut));
	}
}

void KeyFrame::precomputeconsts(KeyFrame* preframe, KeyFrame* nextframe) {
	if (nextframe == NULL) {
		A = 0;
		B = 0;
		C = 0;
		D = 0;
	}
	else {
		float timelap = nextframe->Time - Time;
		A = 2 * Value - 2 * nextframe->Value + timelap*TangentOut + timelap*nextframe->TangentIn;
		B = -3 * Value + 3 * nextframe->Value - 2 * timelap*TangentOut - timelap*nextframe->TangentIn;
		C = timelap*TangentOut;
		D = Value;
	}
}


void KeyFrame::linearcomputetan(KeyFrame* preframe, KeyFrame* nextframe,bool in) {
	if (in) {
		if (preframe != NULL) {
			TangentIn = (preframe->Value - Value) / (preframe->Time - Time);
		}
		else {
			TangentIn = 0;
		}
	} else {
		if (nextframe != NULL) {
			TangentOut = (nextframe->Value - Value) / (nextframe->Time - Time);
		}
		else {
			TangentOut = 0;
		}
	}
}
