#include "AnimationClip.h"
AnimationClip::AnimationClip(const char* filename) {
	
	systemtime = ((float)glutGet(GLUT_ELAPSED_TIME))/2000.f;
	Load(filename);
	precompute();
}
void AnimationClip::Evaluate(float time, Skeleton *skel) {
	skel->setTranslate(channels[0]->Evaluate(time), channels[1]->Evaluate(time), channels[2]->Evaluate(time));
	for ( int i = 1; i < numChannels/3; i++) {
		
		float x = channels[3 * i]->Evaluate(time);
		float y = channels[3 * i + 1]->Evaluate(time);
		float z = channels[3 * i + 2]->Evaluate(time);
		skel->joints[i-1]->UpdateDOF(x, y, z);
	}
}
void AnimationClip::Load(const char *filename) {
	Tokenizer scanner;
	scanner.Open(filename);
	char buffer[64];
	scanner.GetToken(buffer);//animation
	scanner.GetToken(buffer);//{
	scanner.GetToken(buffer);//range
	starttime = scanner.GetFloat();
	endtime = scanner.GetFloat();
	scanner.GetToken(buffer);//numchannels
	numChannels = scanner.GetInt();

	for(int i=0;i<numChannels;i++){
		char buffer1[64];
		scanner.GetToken(buffer1);
		if(std::string(buffer1) == "channel") {
			Channel *temp = new Channel();
			temp->Load(scanner);
			channels.push_back(temp);
		}else{
			std::cout << "error reading the " + std::to_string(i) + "th channel" << std::endl;
		}
	}
	scanner.Close();
}
void AnimationClip::precompute() {
	for (int i = 0; i < channels.size(); i++)
	{
		channels[i]->precompute();
	}
	/*for (int i = 0; i < 5; i++) {
		std::cout << channels[6]->frames[i]->TangentIn << " " << channels[7]->frames[i]->TangentOut << std::endl;;
	}*/
}
void AnimationClip::Update(Skeleton *skel) {
	float currenttime = ((float)glutGet(GLUT_ELAPSED_TIME)) / 2000.f;
	Evaluate(currenttime - systemtime, skel);
}