<!--
SPDX-License-Identifier: CC-BY-SA-4.0

Copyright (C) 2026 embedded brains GmbH & Co. KG
-->

## Overview

This repository contains a package specification and configuration.  It is used
to build packages for the [RTEMS](https://www.rtems.org/) real-time operating
system.  The package provides essential documents required for an
[ECSS](https://ecss.nl/) pre-qualification.

## Getting Started

For the package build dependencies, see the
[Dockerfile](.github/container/Dockerfile) used by the GitHub workflows.

To initialize the Git submodules, run:
```
git submodule update --init
```

To build the package in the default configuration, run:
```
make
```

## Git Submodule Overview

The following Git submodules provide required components:

- [modules/rtems](https://github.com/embedded-brains/rtems): Provides the
  pre-qualified RTEMS sources according to ECSS standards.  This repository
  also contains the RTEMS specification.  Currently, the pre-qualified RTEMS
  uses the
  [RTEMS 6 release branch](https://gitlab.rtems.org/rtems/rtos/rtems/-/tree/6?ref_type=heads)
  as the baseline.  In an ongoing ESA activity, it is planned to update the
  baseline.  The following branches are related to the pre-qualified RTEMS:

  - [esa/v4](https://github.com/embedded-brains/rtems/tree/esa/v4)

  - [esa/v4-spec](https://github.com/embedded-brains/rtems/tree/esa/v4-spec)

  - [esa/v5](https://github.com/embedded-brains/rtems/tree/esa/v5)

  - [esa/v5-spec](https://github.com/embedded-brains/rtems/tree/esa/v5-spec)

  - [esa/v6](https://github.com/embedded-brains/rtems/tree/esa/v6)

  - [esa/v6-spec](https://github.com/embedded-brains/rtems/tree/esa/v6-spec)

  - [esa/v6.2](https://github.com/embedded-brains/rtems/tree/esa/v6.2)

  The version names **v4**, **v5**, **v6**, and **v6.2** correspond to the
  [QDP releases available from ESA](https://rtems-qual.io.esa.int/).
  Historically, the RTEMS specification was separate from the RTEMS sources in
  the
  [RTEMS central repository](https://gitlab.rtems.org/rtems/prequal/rtems-central/).
  To simplify maintenance of the pre-qualified RTEMS and avoid unnecessary
  inconsistencies, the RTEMS specification was moved into the RTEMS sources.

  For reference, the `*-spec` branches are provided for older ESA QDP releases.
  There is an active discussion in the RTEMS Project about how the ECSS
  pre-qualified RTEMS can be integrated.

- [modules/rtems-docs](https://github.com/embedded-brains/rtems-docs): Contains
  the pre-qualified RTEMS documentation sources provided by the
  [esa/v6.2](https://github.com/embedded-brains/rtems-docs/tree/esa/v6.2)
  branch.

- [modules/rtems-docs-ecss](https://github.com/embedded-brains/rtems-docs-ecss):
  Provides the RTEMS documentation sources for ECSS projects.

- [modules/rtems-tools](https://github.com/embedded-brains/rtems-tools):
  Provides the RTEMS Tools for reference.

- [modules/rsb](https://github.com/embedded-brains/rtems-source-builder):
  Provides the RTEMS Source Builder for reference.

## Package Specification and Configuration

The package specification provides a reusable specification of the package
build, its basic components, and component templates.  It is customized by the
package configuration.  The package specification is provided by the following
directories:

- [spec](spec)

- [modules/rtems/spec](https://github.com/embedded-brains/rtems/tree/esa/v6.2/spec)

- [modules/rtems-docs-ecss/spec](https://github.com/embedded-brains/rtems-docs-ecss/tree/draft/v6.3/spec)

The package build is customized through a package configuration, for example:

- [modules/rtems-docs-ecss/config/dummy-project](https://github.com/embedded-brains/rtems-docs-ecss/tree/draft/v6.3/config/dummy-project)
- [config/gr740](config/gr740)

## Tooling Overview

The tooling is provided by the [specthings](https://github.com/specthings/)
project through the following Python packages, available at
[PyPI](https://pypi.org/):

- [specitems](https://github.com/specthings/specitems): Provides interfaces to
  work with specification items.  Specifications are written in specification
  items, which may contain dictionaries, lists, integers, floating-point
  numbers, and strings.  The format of these items is extensible,
  human-readable, machine-readable, Git-friendly, and can be customized
  according to domain-specific needs.  The items are connected through links
  which may contain role-specific extra information.  This enables different
  views of a specification item graph depending on the use case.

- [specware](https://github.com/specthings/specware): Provides utilities to
  specify software and generate source code for interfaces and validation tests
  from the specification.

- [specmake](https://github.com/specthings/specmake): Provides utilities to
  build packages of software products along with their documentation.

The initial tool design was carried out in 2019 and 2020.  For reference, see
the
[Software Requirements Engineering](https://docs.rtems.org/docs/main/eng/req/index.html)
section of the
[RTEMS Software Engineering](https://docs.rtems.org/docs/main/eng/)
manual and the
[QT-109 Technical Note: RTEMS SMP Qualification Target, Release 6](https://github.com/embedded-brains/rtems-docs-ecss/blob/main/src/tn/QT-109/QT-109.pdf).
The design follows the
[DRY](https://en.wikipedia.org/wiki/Don%27t_repeat_yourself)
principle.  Each element of the RTEMS specification should be defined at most
once.  The specification forms a directed graph with role-specific links between
the nodes.  This enables traceability across all layers of the technical
specification.  See the following image, which shows the connections between
essential ECSS documents (_ICD_ - Interface Control Document, _SDD_ - Software
Design Document, _SRS_ - Software Requirements Specification, _SUITP_ -
Software Unit and Integration Test Plan, _SVS_ - Software Validation
Specification).  The _SDD_ is generated by [Doxygen](https://www.doxygen.nl/)
from the RTEMS sources.  Doxygen can provide a so-called `tagfile` with a
machine-readable description of the software design elements.  The Doxygen
`tagfile` is used to provide links between the software design and the
specification, and vice versa.

![Traceability](https://github.com/embedded-brains/rtems-docs-ecss/blob/main/src/common/images/traceability.png?raw=true)

The RTEMS real-time operating system is a reusable software component that
provides an implementation of an [API](https://en.wikipedia.org/wiki/API).  The
interfaces of the pre-qualified RTEMS API are defined by its specification.  The
tooling generates the API header files and related documentation sources,
ensuring that the interface specification and the actual interface declarations
in the source code remain internally consistent.

The RTEMS implementation is primarily organized as a collection of finite state
machines.  The tooling enables a semi-formal specification of these finite state
machines, as well as the generation of their corresponding validation code.

This approach was successfully applied to the pre-qualification of device
drivers.

## Other Safety Standards

The RTEMS pre-qualification effort was sponsored by the European Space Agency
(ESA).  While the primary objective was to achieve compliance with the standards
set out by the European Cooperation for Space Standardization
([ECSS](https://ecss.nl/)), it is important to note that neither the
specification nor the tooling developed for this process are limited to any one
safety standard.  Their design is intentionally generic, enabling adaptation to
different regulatory or industry requirements beyond ECSS, see chapter
*Analysis of other standards* in
[QT-109 Technical Note: RTEMS SMP Qualification Target, Release 6](https://github.com/embedded-brains/rtems-docs-ecss/blob/main/src/tn/QT-109/QT-109.pdf).

A key achievement of the RTEMS pre-qualification campaign is the development of
a comprehensive specification for the RTEMS real-time operating system.  This
specification is both human-readable and machine-readable, ensuring clarity for
engineers and suitability for automated processing by tools.  The specification
maintains excellent traceability across all layers -- from high-level
requirements to the actual source code -- and supports linking implementations
directly to their corresponding requirements and documentation.

Moreover, the tooling associated with the specification supports the flexible
generation of documentation sets tailored to various use cases or
standardization needs.  This means that organizations can efficiently produce
evidence of compliance, system overviews, or interface documentation -- all
generated from a single, source-of-truth specification.  As a result, the RTEMS
pre-qualification approach delivers high confidence in compliance, strong
maintainability, and easy reusability across different projects and standards.

## Contributing

Please refer to our
[Contributing Guidelines](CONTRIBUTING.md).
