CFLAGS          = -Ofast -Wall -DNDEBUG -I src/ -I lib
SOURCE_C        = $(shell find src/* -type f -name '*.c')
SOURCE_C       += $(shell find lib/* -type f -name '*.c')
SOURCE_O_LINUX := $(foreach f,$(SOURCE_C:.c=.o),bin/linux/$f)
SOURCE_O_WIN32 := $(foreach f,$(SOURCE_C:.c=.o),bin/win32/$f)

RELEASE_EXECUTABLE_LINUX := ggc
RELEASE_EXECUTABLE_WIN32 := ggc.exe

# Make build directories
$(shell mkdir -p bin/ $(foreach dir, \
	$(dir $(SOURCE_O_WIN32)) \
	$(dir $(SOURCE_O_LINUX)) \
	, $(dir)))

.PHONY: default \
		linux \
		win32 \
		clean

default: linux
all: linux win32
linux: $(RELEASE_EXECUTABLE_LINUX)
win32: $(RELEASE_EXECUTABLE_WIN32)

clean:
	rm -rf bin
	rm -f $(RELEASE_EXECUTABLE_LINUX)
	rm -f $(RELEASE_EXECUTABLE_WIN32)

# # # # # # # # # # # # # # # # # # # #
# LINUX BUILD                         #
# # # # # # # # # # # # # # # # # # # #

-include $(SOURCE_O_LINUX:.o=.d)

bin/linux/%.o: %.c
	@echo "[$(notdir $@)]"
	@gcc -c -o $@ $< $(CFLAGS)
	@echo -n $(dir $@) > $(@:.o=.d)
	@gcc -MM $(CFLAGS) $< >> $(@:.o=.d)

$(RELEASE_EXECUTABLE_LINUX): $(SOURCE_O_LINUX)
	@echo "[$(notdir $@)] [$(notdir $^)]"
	@gcc -o $@ $^ $(CFLAGS)

# # # # # # # # # # # # # # # # # # # #
# WIN32 BUILD                         #
# # # # # # # # # # # # # # # # # # # #

-include $(SOURCE_O_WIN32:.o=.d)

bin/win32/%.o: %.c
	@echo "[$(notdir $@)]"
	@i686-w64-mingw32.static-gcc -c -o $@ $< $(CFLAGS) -D_WIN32
	@echo -n $(dir $@) > $(@:.o=.d)
	@i686-w64-mingw32.static-gcc -MM $(CFLAGS) -D_WIN32 $< >> $(@:.o=.d)

$(RELEASE_EXECUTABLE_WIN32): $(SOURCE_O_WIN32)
	@echo "[$(notdir $@)] [$(notdir $^)]"
	@i686-w64-mingw32.static-gcc -o $@ $^ $(CFLAGS) -D_WIN32
