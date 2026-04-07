<!--
SPDX-License-Identifier: CC-BY-SA-4.0

Copyright (C) 2026 embedded brains GmbH & Co. KG
-->

# General Code Review Standards

## License and Copyright Compliance

- Check that every file has an SPDX-License-Identifier at the beginning of the
  file

- Check that every file has a copyright statement at the beginning of the file

## Content Guidelines

- Check that text is written in British English with the following exceptions
  (and similar):

  - authorization

  - customization

  - finalization

  - initialization

  - license

  - organization

  - synchronization

- In `*.md` files, check that text is in MyST Markdown format

- In `*.rst` files, check that text is in reStructuredText format and compatible
  with the Sphinx documentation framework

- In `*.pdf` files, check that only embedded fonts with open source licences
  are present

- In `*.yml` files:

  - Where the `glossary-type` key has the value `term` and the value of the
    `term` key is an acronym (for example FB for "Foo Bar"), check that the
    value of the `text` key uses the phrase pattern:

    This term is an acronym for Foo Bar.

  - Where `text` keys are present, the value is in MyST Markdown format

- In `*.md`, `*.rst`, and `*.yml` files, except
  `.github/copilot-instructions.md`, check that the following terms are not
  used:

  - and/or

  - e.g.

  - etc.

  - i.e.

## File and Directory Naming Conventions

- Check that file and directory names use lower case characters

- Check that file and directory name parts are separated by dashes (`-`)
