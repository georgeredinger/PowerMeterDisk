int detect(int sample) {
	static int maximum=0;
	static int minimum=0;
	maximum = std::max(sample,maximum);
	return maximum;

}

