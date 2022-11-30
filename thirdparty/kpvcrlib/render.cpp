#include <cstddef>
#include <string>
#include "crengine.h"
#include "lvdocview.h"

class SystemTime {
	struct timespec ts;
public:
	void record() {
		clock_gettime(CLOCK_MONOTONIC, &ts);
	}

	int64_t sub(SystemTime &rhs) const {
		return (ts.tv_sec - rhs.ts.tv_sec) * 1000000000LL + (ts.tv_nsec - rhs.ts.tv_nsec);
	}
};

int main(int argc, char **argv) {
	if (argc < 2) {
		printf("Usage: %s <file> [repeat]\n", argv[0]);
		return 1;
	}

	printf("loading fonts\n");
	const char *noto_fonts[] = {
		"resources/fonts/noto/NotoSerif-Bold.ttf",
		"resources/fonts/noto/NotoSans-Regular.ttf",
		"resources/fonts/noto/NotoSansDevanagariUI-Regular.ttf",
		"resources/fonts/noto/NotoSansBengaliUI-Regular.ttf",
		"resources/fonts/noto/NotoSans-Italic.ttf",
		"resources/fonts/noto/NotoSerif-BoldItalic.ttf",
		"resources/fonts/noto/NotoSansBengaliUI-Bold.ttf",
		"resources/fonts/noto/NotoNaskhArabic-Bold.ttf",
		"resources/fonts/noto/NotoSerif-Regular.ttf",
		"resources/fonts/noto/NotoSansDevanagariUI-Bold.ttf",
		"resources/fonts/noto/NotoSans-BoldItalic.ttf",
		"resources/fonts/noto/NotoSansArabicUI-Bold.ttf",
		"resources/fonts/noto/NotoSerif-Italic.ttf",
		"resources/fonts/noto/NotoSansArabicUI-Regular.ttf",
		"resources/fonts/noto/NotoNaskhArabic-Regular.ttf",
		"resources/fonts/noto/NotoSans-Bold.ttf",
	};

	InitFontManager(lString8());
	for (unsigned i = 0; i < sizeof(noto_fonts) / sizeof(noto_fonts[0]); ++i) {
		fontMan->RegisterFont(lString8(noto_fonts[i]));
	}

	auto *view = new LVDocView(-1, true);
	view->setViewMode(LVDocViewMode::DVM_PAGES, -1);
	view->Resize(768, 1024);
	view->setPageHeaderInfo(PGHDR_AUTHOR|PGHDR_TITLE|PGHDR_PAGE_NUMBER|PGHDR_PAGE_COUNT|PGHDR_CHAPTER_MARKS|PGHDR_CLOCK);

	printf("loading document\n");
	view->LoadDocument(argv[1]);
	printf("loaded\n");

	int repeat = 1;
	if (argc == 3) {
		repeat = std::stoi(argv[2], 0);
	}

	SystemTime begin, end;
	begin.record();
	for (int i = 0; i < repeat; ++i) {
		view->setFontSize(18);
		printf("resized to 18\n");
		view->Render();
		printf("rendered\n");

		view->setFontSize(16);
		printf("resized to 16\n");
		view->Render();
		printf("rendered round %d\n", i);
	}
	end.record();
	printf("elapsed %ld us per round\n", end.sub(begin) / 1000 / repeat);

	return 0;
}
