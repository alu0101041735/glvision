#ifndef IMAGECLASS_H
#define IMAGECLASS_H

#include <string>
#include <vector>
#include <utils>

namespace FileState {
	enum {
		Unopened = 0,
		Loaded = 1 << 0,
		Corrupted = 1 << 1
	};
}

namespace FileFormat {
	enum {
		png = 0,
		jpeg = 1,
		tif = 2,
		bmp = 3
	};
}

namespace glvision {

typename std::vector<std::vector<uint32_t>> imageContainer;

class Image {
	public:
		Image() {}
		Image(std::string path) {}
		Image(const Image &image) {}
		~Image() {}

	private:
		imageContainer image_;
		std::pair<unsigned, unsigned> size_;
		uint8_t format_;
		bool state_;

	public:
		bool load();
		bool save();
		bool save(std::string path);
		bool isLoaded() {return state_ & FileState::Loaded;}
		uint8_t format() {return format_;}
		void applyFunction();

};
}

#endif
