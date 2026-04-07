# SPDX-License-Identifier: BSD-2-Clause

# Copyright (C) 2026 embedded brains GmbH & Co. KG

FROM ubuntu:24.04
COPY --from=ghcr.io/astral-sh/uv:0.10.12 /uv /uvx /bin/
COPY pyproject.toml uv.lock .
RUN <<EOF
apt-get -y update
apt-get -y upgrade
DEBIAN_FRONTEND=noninteractive TZ=Europe/Berlin apt-get -qq install -y \
    babeltrace \
    binutils-multiarch \
    bison \
    build-essential \
    bzip2 \
    clang-tools \
    cmake \
    cppcheck \
    expect \
    flex \
    g++ \
    gawk \
    gcc \
    gcovr \
    git \
    git-lfs \
    gnat-11 \
    gnutls-dev \
    graphviz \
    latexmk \
    libpython3-dev \
    libssl-dev \
    lzop \
    make \
    pdftk \
    pkg-config \
    python3 \
    python3-dev \
    python3-pip \
    python3-setuptools \
    python3-venv \
    python-dev-is-python3 \
    python-is-python3 \
    texinfo \
    texlive \
    texlive-fonts-extra \
    texlive-latex-extra \
    unzip \
    xz-utils
echo "main_memory = 12000000" >> /etc/texmf/texmf.d/00debian.cnf
echo "extra_mem_bot = 12000000" >> /etc/texmf/texmf.d/00debian.cnf
echo "font_mem_size = 12000000" >> /etc/texmf/texmf.d/00debian.cnf
echo "pool_size = 12000000" >> /etc/texmf/texmf.d/00debian.cnf
echo "buf_size = 12000000" >> /etc/texmf/texmf.d/00debian.cnf
update-texmf
texhash
git config --global user.email "you@example.com"
git config --global user.name "Your Name"
uv sync --all-groups --cache-dir=/.cache/uv
EOF
