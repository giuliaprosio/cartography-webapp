include frontend/.phony.mk
include debug/.phony.mk

GREENSOFT_SDK_DIR ?= greensoft-sdk
GREENSOFT_SDK      = greensoft-sdk-2021-01-25-14ae3
GREENSOFT_SDK_TAR  = toolchain/$(GREENSOFT_SDK).tar.xz
GREENSOFT_CC       = $(GREENSOFT_SDK_DIR)/output/host/bin/arm-buildroot-linux-uclibcgnueabi-gcc
GREENSOFT_ROOT     = $(GREENSOFT_SDK_DIR)/output/host/arm-buildroot-linux-uclibcgnueabi/sysroot
SUBDIRS            = frontend debug
LIBCURL_VERSION    = 8.1.2
TARGETS            = native greensoft
BUILD_DIRS         = $(patsubst %,build-%,$(TARGETS))
JOBS              ?= $(shell nproc)
CMAKE_FLAGS       ?=

TARGET            ?= native
BUILD_TYPE        ?= $(if $(filter native,$(TARGET)),Debug,Release)
export BUILD_TYPE

all: $(TARGET) frontend;

$(SUBDIRS):
	$(MAKE) -C $@ $(filter $($@_PHONY),$(MAKECMDGOALS))

$(TARGETS): %: build-%
	cmake --build $< --parallel $(JOBS)

build-native: TARGET = native
build-native: Makefile CMakeLists.txt
	cmake -S . -B $@ -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) $(CMAKE_FLAGS)

build-greensoft: TARGET = greensoft
build-greensoft: Makefile CMakeLists.txt $(GREENSOFT_CC)
	cmake -S . -B $@ -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) $(CMAKE_FLAGS) -DCMAKE_C_COMPILER=$(abspath $(GREENSOFT_CC)) -DCMAKE_FIND_ROOT_PATH=$(abspath $(GREENSOFT_ROOT))

$(GREENSOFT_CC): $(GREENSOFT_SDK_DIR)
	make -C $< toolchain
	make -C $< libcurl LIBCURL_VERSION=$(LIBCURL_VERSION)
	touch $@

$(GREENSOFT_SDK_DIR):
	tar -xvf $(GREENSOFT_SDK_TAR)
	mv $(GREENSOFT_SDK) $@
	mkdir -p $@/dl
	cp toolchain/.config $@
	cp toolchain/011-fix-sigstksz.patch $@/package/m4
	cat toolchain/libcurl.hash >> $@/package/libcurl/libcurl.hash

cleanall: clean cleangreensoftsdk frontend debug;

clean: cleanbuild frontend debug
	rm -rf $(BUILD_DIRS)

cleanbuild: $(patsubst %,clean-%,$(wildcard $(BUILD_DIRS))) frontend;

clean-%:
	-cmake --build $* --target clean

cleangreensoftsdk:
	rm -rf $(GREENSOFT_SDK_DIR)

.PHONY: all $(TARGETS) $(SUBDIRS) cleanall clean cleanbuild clean-% cleangreensoftsdk
