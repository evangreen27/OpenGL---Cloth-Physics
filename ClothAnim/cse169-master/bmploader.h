class BMPImage
{
public:
	~BMPImage()
	{
		delete[] data;
	}

	bool load(const char* filename);

	unsigned long sizeX;
	unsigned long sizeY;
	char *data;
};