# PS4 Cloud Browser — OpenOrbis build
TITLE       := PS4 Cloud Browser
VERSION     := 01.00
TITLE_ID    := PSCB00001
CONTENT_ID  := IV0000-PSCB00001_00-PS4CLOUDBROWSER0

TOOLCHAIN   := $(OO_PS4_TOOLCHAIN)
SRCDIR      := src
INTDIR      := build
PROJ        := ps4_cloud_browser
OUT_ELF     := $(INTDIR)/$(PROJ).elf
OUT_OELF    := $(INTDIR)/$(PROJ).oelf
PKGTOOLS    := $(TOOLCHAIN)/bin/linux

CC          := clang
CXX         := clang++
LD          := ld.lld

CORE_LIBS   := -lc -lkernel -lc++ -lSceLibcInternal
ALL_STUBS   := $(sort $(patsubst $(TOOLCHAIN)/lib/lib%.so,-l%,$(wildcard $(TOOLCHAIN)/lib/lib*.so)) \
                      $(patsubst $(TOOLCHAIN)/lib/lib%.a,-l%,$(wildcard $(TOOLCHAIN)/lib/lib*.a)))
EXTRA_LIBS  := $(filter-out $(CORE_LIBS),$(ALL_STUBS))

CFILES      := $(wildcard $(SRCDIR)/*.c)
CPPFILES    := $(wildcard $(SRCDIR)/*.cpp)
SFILES      := $(wildcard $(SRCDIR)/*.s)
OBJS        := $(patsubst $(SRCDIR)/%.c,$(INTDIR)/%.o,$(CFILES)) \
               $(patsubst $(SRCDIR)/%.cpp,$(INTDIR)/%.o,$(CPPFILES)) \
               $(patsubst $(SRCDIR)/%.s,$(INTDIR)/%.o,$(SFILES))

CFLAGS      := --target=x86_64-pc-freebsd12-elf -fPIC -funwind-tables -c \
               -isysroot $(TOOLCHAIN) -isystem $(TOOLCHAIN)/include
CXXFLAGS    := $(CFLAGS) -isystem $(TOOLCHAIN)/include/c++/v1
LDFLAGS     := -m elf_x86_64 -pie --script $(TOOLCHAIN)/link.x --eh-frame-hdr \
               -L$(TOOLCHAIN)/lib $(CORE_LIBS) --as-needed $(EXTRA_LIBS) --no-as-needed \
               $(TOOLCHAIN)/lib/crt1.o

all: $(CONTENT_ID).pkg

$(INTDIR):
	mkdir -p $(INTDIR)

$(INTDIR)/%.o: $(SRCDIR)/%.c | $(INTDIR)
	$(CC) $(CFLAGS) -o $@ $<

$(INTDIR)/%.o: $(SRCDIR)/%.cpp | $(INTDIR)
	$(CXX) $(CXXFLAGS) -o $@ $<

$(INTDIR)/%.o: $(SRCDIR)/%.s | $(INTDIR)
	$(CC) $(CFLAGS) -o $@ $<

eboot.bin: $(OBJS)
	$(LD) $(OBJS) -o $(OUT_ELF) $(LDFLAGS)
	$(PKGTOOLS)/create-fself -in=$(OUT_ELF) -out=$(OUT_OELF) --eboot "eboot.bin" --paid 0x3100000000000001

sce_sys/icon0.png: tools/make_icon.py
	python3 tools/make_icon.py $@

sce_sys/param.sfo: Makefile
	mkdir -p sce_sys
	$(PKGTOOLS)/PkgTool.Core sfo_new $@
	$(PKGTOOLS)/PkgTool.Core sfo_setentry $@ APP_TYPE --type Integer --maxsize 4 --value 1
	$(PKGTOOLS)/PkgTool.Core sfo_setentry $@ APP_VER --type Utf8 --maxsize 8 --value '$(VERSION)'
	$(PKGTOOLS)/PkgTool.Core sfo_setentry $@ ATTRIBUTE --type Integer --maxsize 4 --value 0
	$(PKGTOOLS)/PkgTool.Core sfo_setentry $@ CATEGORY --type Utf8 --maxsize 4 --value 'gd'
	$(PKGTOOLS)/PkgTool.Core sfo_setentry $@ CONTENT_ID --type Utf8 --maxsize 48 --value '$(CONTENT_ID)'
	$(PKGTOOLS)/PkgTool.Core sfo_setentry $@ DOWNLOAD_DATA_SIZE --type Integer --maxsize 4 --value 0
	$(PKGTOOLS)/PkgTool.Core sfo_setentry $@ SYSTEM_VER --type Integer --maxsize 4 --value 0
	$(PKGTOOLS)/PkgTool.Core sfo_setentry $@ TITLE --type Utf8 --maxsize 128 --value '$(TITLE)'
	$(PKGTOOLS)/PkgTool.Core sfo_setentry $@ TITLE_ID --type Utf8 --maxsize 12 --value '$(TITLE_ID)'
	$(PKGTOOLS)/PkgTool.Core sfo_setentry $@ VERSION --type Utf8 --maxsize 8 --value '$(VERSION)'

pkg.gp4: eboot.bin sce_sys/param.sfo sce_sys/icon0.png
	python3 tools/make_gp4.py --out $@ --content-id $(CONTENT_ID) $^

$(CONTENT_ID).pkg: pkg.gp4
	$(PKGTOOLS)/PkgTool.Core pkg_build $< .

clean:
	rm -rf $(INTDIR) eboot.bin pkg.gp4 sce_sys/param.sfo sce_sys/icon0.png *.pkg

.PHONY: all clean
