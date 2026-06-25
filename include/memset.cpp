extern "C" void* memset(void* dest, int ch, unsigned long count) {
	for (unsigned long i = 0; i < count; ++i) {
		static_cast<unsigned char*>(dest)[i] = static_cast<unsigned char>(ch);
	}
	return dest;
}
