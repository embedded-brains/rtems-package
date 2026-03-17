RTEMS_API ?= ${../component:/rtems-version}
RTEMS_CPU ?= ${../component:/arch}
RTEMS_BSP ?= ${../component:/bsp}${../component:/config:dash}${../component:/bsp-qual-only:dash}
RTEMS_ROOT ?= ${../component:/deployment-directory}

PKG_CONFIG = $$(RTEMS_ROOT)/lib/pkgconfig/$$(RTEMS_CPU)-rtems$$(RTEMS_API)-$$(RTEMS_BSP).pc
BUILDDIR = b-$$(RTEMS_CPU)-$$(RTEMS_BSP)

DEPFLAGS = -MT $$@ -MD -MP -MF $$(basename $$@).d
WARNFLAGS = -Wall -Wextra
OPTFLAGS = -O2 -g -ffunction-sections -fdata-sections
EXEEXT = .exe

ABI_FLAGS = $$(shell pkg-config --define-variable=prefix=$$(RTEMS_ROOT) --cflags $$(PKG_CONFIG))
LDFLAGS = $$(shell pkg-config --define-variable=prefix=$$(RTEMS_ROOT) --variable=LDFLAGS $$(PKG_CONFIG))
CFLAGS = $$(DEPFLAGS) $$(WARNFLAGS) $$(ABI_FLAGS) $$(OPTFLAGS)
CXXFLAGS = $$(DEPFLAGS) $$(WARNFLAGS) $$(ABI_FLAGS) $$(OPTFLAGS)
ASFLAGS = $$(ABI_FLAGS)

CCLINK = $$(CC) $$(CFLAGS) -Wl,-Map,$$(basename $$@).map
CXXLINK = $$(CXX) $$(CXXFLAGS) -Wl,-Map,$$(basename $$@).map

$$(BUILDDIR)/%.o: %.c | $$(BUILDDIR)
	$$(CC) $$(CPPFLAGS) $$(CFLAGS) -c $$< -o $$@

$$(BUILDDIR)/%.o: %.S | $$(BUILDDIR)
	$$(CC) $$(CPPFLAGS) -DASM $$(CFLAGS) -c $$< -o $$@

$$(BUILDDIR)/%.o: %.cc | $$(BUILDDIR)
	$$(CXX) $$(CPPFLAGS) $$(CXXFLAGS) -c $$< -o $$@

$$(BUILDDIR)/%.o: %.cpp | $$(BUILDDIR)
	$$(CXX) $$(CPPFLAGS) $$(CXXFLAGS) -c $$< -o $$@

$$(BUILDDIR)/%.o: %.s | $$(BUILDDIR)
	$$(AS) $$(ASFLAGS) $$< -o $$@

export PATH := $$(RTEMS_ROOT)/bin:$$(PATH)

export AR = $$(RTEMS_CPU)-rtems$$(RTEMS_API)-ar
export AS = $$(RTEMS_CPU)-rtems$$(RTEMS_API)-as
export CC = $$(RTEMS_CPU)-rtems$$(RTEMS_API)-gcc
export CXX = $$(RTEMS_CPU)-rtems$$(RTEMS_API)-g++
export LD = $$(RTEMS_CPU)-rtems$$(RTEMS_API)-ld
export NM = $$(RTEMS_CPU)-rtems$$(RTEMS_API)-nm
export OBJCOPY = $$(RTEMS_CPU)-rtems$$(RTEMS_API)-objcopy
export RANLIB = $$(RTEMS_CPU)-rtems$$(RTEMS_API)-ranlib
export SIZE = $$(RTEMS_CPU)-rtems$$(RTEMS_API)-size
export STRIP = $$(RTEMS_CPU)-rtems$$(RTEMS_API)-strip

APP = $$(BUILDDIR)/example-trace$$(EXEEXT)

APP_OBJS =
APP_OBJS += $$(BUILDDIR)/appl-config.o
APP_OBJS += $$(BUILDDIR)/main.o

all: $$(APP)

$$(BUILDDIR):
	mkdir $$(BUILDDIR)

$$(APP): $$(APP_OBJS)
	$$(CCLINK) $$^ $$(LDFLAGS) -lrtemstest -o $$@

LOG = $$(BUILDDIR)/log.txt

CTFDIR = $$(BUILDDIR)/ctf

$$(LOG): $$(APP)
	${/pkg/deployment/run:/file-name} $$(RTEMS_CPU)-$$(RTEMS_BSP) $$< | tee $$(LOG)

$$(CTFDIR): $$(LOG)
	rm -rf $$@
	mkdir $$@
	rtems-record-lttng -t $$< -o $$@

run: $$(LOG)

ctf: $$(CTFDIR)

babeltrace: $$(CTFDIR)
	babeltrace $$< > $$(BUILDDIR)/babeltrace.txt
	head -n 100 $$(BUILDDIR)/babeltrace.txt

clean:
	rm -rf $$(BUILDDIR)

-include $$(APP_OBJS:%.o=%.d)
