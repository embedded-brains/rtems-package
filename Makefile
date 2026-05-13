# SPDX-License-Identifier: BSD-2-Clause

# Copyright (C) 2026 embedded brains GmbH & Co. KG

PACKAGE_NAME ?= gr740

PREFIX ?= rtems-dummy-$(PACKAGE_VERSION)

ARTIFACTS_PREFIX ?= artifacts

GIT_OPTIONS ?=

LOG_OPTIONS ?= --log-level=DEBUG

VENV ?= .venv

VENV_MARKER = $(VENV)/venv-marker

PACKAGE_VERSION = 6.3

all: package-clean package

package: | prepare
	uv run specbuild $(GIT_OPTIONS) $(LOG_OPTIONS) \
	  spec \
	  modules/rtems/spec \
	  modules/rtems-docs-ecss/spec \
	  modules/rtems-docs-ecss/config/dummy-project \
	  config/$(PACKAGE_NAME)

package-move-artifacts:
	mkdir -p $(ARTIFACTS_PREFIX)/delivery $(ARTIFACTS_PREFIX)/internal
	mv $(PREFIX)/*$(PACKAGE_NAME)-$(PACKAGE_VERSION)-scf.pdf $(ARTIFACTS_PREFIX)/delivery
	mv $(PREFIX)/*$(PACKAGE_NAME)-$(PACKAGE_VERSION).tar.xz $(ARTIFACTS_PREFIX)/delivery
	mv $(PREFIX)/package-summary.rst $(ARTIFACTS_PREFIX)/internal

package-update: | prepare
	cp $(PREFIX)/doc/sparc/$(PACKAGE_NAME)/smp/test-logs/*simulator* test-logs/sparc/$(PACKAGE_NAME)/smp
	cp $(PREFIX)/doc/sparc/$(PACKAGE_NAME)/uni/test-logs/*simulator* test-logs/sparc/$(PACKAGE_NAME)/uni
	uv run specupdatetimeouts --spec-directories config/$(PACKAGE_NAME) -- $(PREFIX)/doc/sparc/*/*/test-logs/*-simulator.json
	uv run specupdateperf --spec-directories config/$(PACKAGE_NAME) modules/rtems/spec --lazy /target/sparc/$(PACKAGE_NAME)/sis/perf-smp             $(PREFIX)/doc/sparc/$(PACKAGE_NAME)/smp/test-logs/*-simulator.json
	uv run specupdateperf --spec-directories config/$(PACKAGE_NAME) modules/rtems/spec --lazy /target/sparc/$(PACKAGE_NAME)/sis/perf-default         $(PREFIX)/doc/sparc/$(PACKAGE_NAME)/uni/test-logs/*-simulator.json

package-clean:
	if test -d $(PREFIX) && test -z "$(GIT_OPTIONS)" ; then cd $(PREFIX) && git clean -xdf . && git co . ; fi

package-remove:
	rm -rf $(PREFIX)

specview: | prepare
	uv run specwareview --enabled=sparc/gr712rc,sparc,bsps/sparc/leon3,target/simulator,RTEMS_QUAL --validated=no
	uv run specwareview --enabled=sparc/gr712rc,sparc,bsps/sparc/leon3,target/simulator,RTEMS_QUAL,RTEMS_SMP --validated=no
	uv run specwareview --enabled=sparc/gr740,sparc,bsps/sparc/leon3,target/simulator,RTEMS_QUAL --validated=no
	uv run specwareview --enabled=sparc/gr740,sparc,bsps/sparc/leon3,target/simulator,RTEMS_QUAL,RTEMS_SMP --validated=no

prepare: $(VENV_MARKER)

$(VENV_MARKER): uv.lock
	uv sync --all-groups
	touch $@

ifndef CI
uv.lock: pyproject.toml
	uv lock
	touch $@
endif
