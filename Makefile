GREENSOFT_DIR    ?= greensoft
GREENSOFT_SDK     = greensoft-sdk-2021-01-25-14ae3
GREENSOFT_SDK_TAR = toolchain/$(GREENSOFT_SDK).tar.xz
GREENSOFT_CC      = $(GREENSOFT_DIR)/output/host/bin/arm-buildroot-linux-uclibcgnueabi-gcc
GREENSOFT_ROOT    = $(GREENSOFT_DIR)/output/host/arm-buildroot-linux-uclibcgnueabi/sysroot
LIBCURL_VERSION   = 8.1.2
CURL_HASH         = sha256  31b1118eb8bfd43cd95d9a3f146f814ff874f6ed3999b29d94f4d1e7dbac5ef6  curl-8.1.2.tar.xz
TARGETS           = native greensoft
TARGET            = native
BUILD_DIR         = build-$(TARGET)
EXEC              = out/webapp

ifeq ($(TARGET),greensoft)
IS_GREENSOFT      = yes
endif

.PHONY: $(EXEC)
$(EXEC): | $(if $(IS_GREENSOFT),$(GREENSOFT_CC)) $(BUILD_DIR)
	cmake --build $(BUILD_DIR)

build-native: $(MAKEFILE_LIST) CMakeLists.txt
	cmake -S . -B $@ $(CMAKE_FLAGS)

build-greensoft: $(MAKEFILE_LIST) CMakeLists.txt
	cmake -S . -B $@ $(CMAKE_FLAGS) -DCMAKE_C_COMPILER=$(abspath $(GREENSOFT_CC)) -DCMAKE_FIND_ROOT_PATH=$(abspath $(GREENSOFT_ROOT))

$(GREENSOFT_CC): $(GREENSOFT_DIR)
	make -C $< toolchain
	make -C $< libcurl LIBCURL_VERSION=$(LIBCURL_VERSION)
	touch $@

$(GREENSOFT_DIR): $(GREENSOFT_SDK_TAR)
	tar -xvf $(GREENSOFT_SDK_TAR)
	rm $@ -rf
	mv $(GREENSOFT_SDK) $@
	mkdir -p $@/dl
	cp toolchain/libdbus-cpp-e02d9095.tar.gz $@/dl
	cp toolchain/.config $@
	cp toolchain/011-fix-sigstksz.patch $@/package/m4
	echo '$(CURL_HASH)' >> $@/package/libcurl/libcurl.hash

cleanall: clean cleangreensoft;

clean:
	rm -rf $(TARGETS:%=build-%)
	rm $(EXEC)

cleangreensoft:
	rm -rf $(GREENSOFT_DIR)

.PHONY: cleanall clean cleangreensoft
