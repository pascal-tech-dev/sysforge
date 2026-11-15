# Debian 13 (trixie) sandbox for process, thread, and IPC labs:
# fork/exec/wait, pthreads, pipes, and POSIX / System V IPC.
FROM debian:13

ENV DEBIAN_FRONTEND=noninteractive \
    LANG=en_US.UTF-8 \
    LC_ALL=en_US.UTF-8

# pthread_*(3) pages ship in non-free (manpages-posix-dev). fork(2),
# pipe(2), clone(2), and the rest of the Linux pages are in main.
RUN sed -i 's/^Components: main$/Components: main non-free/' \
        /etc/apt/sources.list.d/debian.sources \
    && apt-get update \
    && apt-get install -y --no-install-recommends \
        build-essential \
        gcc \
        g++ \
        gdb \
        make \
        pkg-config \
        libc6-dev \
        libc6-dbg \
        linux-libc-dev \
        binutils \
        python3 \
        man-db \
        manpages \
        manpages-dev \
        manpages-posix \
        manpages-posix-dev \
        strace \
        ltrace \
        valgrind \
        procps \
        psmisc \
        util-linux \
        iproute2 \
        lsof \
        bsdextrautils \
        xxd \
        time \
        pv \
        moreutils \
        sudo \
        git \
        ca-certificates \
        curl \
        less \
        vim-tiny \
        bash-completion \
        locales \
    && sed -i 's/^# \(en_US.UTF-8 UTF-8\)/\1/' /etc/locale.gen \
    && locale-gen \
    && printf 'ulimit -c unlimited\n' > /etc/profile.d/core-dumps.sh \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

ARG USERNAME=vscode
ARG USER_UID=1000
ARG USER_GID=1000

RUN groupadd --gid "$USER_GID" "$USERNAME" \
    && useradd --uid "$USER_UID" --gid "$USER_GID" -m -s /bin/bash "$USERNAME" \
    && echo "$USERNAME ALL=(ALL) NOPASSWD:ALL" > "/etc/sudoers.d/$USERNAME" \
    && chmod 0440 "/etc/sudoers.d/$USERNAME" \
    && printf '%s\n' \
        'set pagination off' \
        'set print pretty on' \
        '# Fork labs. Defaults follow the parent and detach the child.' \
        '# set follow-fork-mode child' \
        '# set detach-on-fork off' \
        '# set schedule-multiple on' \
        > "/home/$USERNAME/.gdbinit" \
    && chown "$USERNAME:$USERNAME" "/home/$USERNAME/.gdbinit"

USER $USERNAME
WORKDIR /workspaces/sysforge
