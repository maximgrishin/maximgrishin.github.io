extern "C" void* memset(void* dest, int ch, unsigned long count) {
	for (unsigned long i = 0; i < count; ++i) {
		static_cast<unsigned char*>(dest)[i] = static_cast<unsigned char>(ch);
	}
	return dest;
}

extern "C" void* memmove(void* dest, const void* src, unsigned long count) {
	if (dest < src) {
		for (unsigned long i = 0; i < count; ++i) {
			static_cast<unsigned char*>(dest)[i] = static_cast<const unsigned char*>(src)[i];
		}
	}
	else {
		for (unsigned long i = count - 1; i <= count; --i) {
			static_cast<unsigned char*>(dest)[i] = static_cast<const unsigned char*>(src)[i];
		}
	}
	return dest;
}


extern "C" void* memcpy(void* dest, const void* src, unsigned long count) {
	return memmove(dest, src, count);
}
