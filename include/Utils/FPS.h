#ifndef FPS_H
#define FPS_H

class FPS
{
public:
	FPS() {}
	~FPS() {}
	
	void update();
	const int getFPS() const;

private:
	unsigned int oldTime { 0 };
	unsigned int frames { 0 };
};

#endif