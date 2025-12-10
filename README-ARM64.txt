-------------------------------------------------------------------------------
Known Compiling issues
-------------------------------------------------------------------------------

Perl & Ruby - compile but not tested. reference a lot of stubbed math functions in libm on arm64.

-------------------------------------------------------------------------------
ARM64 - Darling shell bootstrap issues
-------------------------------------------------------------------------------

bash crashes in bash-3.2/shell.c attempting "setjmp"
[374] /* Catch early SIGINTs. */
[375]  code = setjmp (top_level);

force default shell to zsh instead of bash so it doen't keep crashing scripts:
sudo mv /bin/bash /bin/bash_orig
ln -s /bin/bash /bin/zsh

Securityd fails to start because directory is missing. Fix:
mkdir ~/.darling/private/var/db/mds/system

-------------------------------------------------------------------------------
ARM64 - lldb
-------------------------------------------------------------------------------

lldb doesn't work on arm64. This version partially works:

On Mac Silicon machine:
sudo port install lldb-12
scp -i $EC2_KEY -r /opt/local/libexec/llvm-12 ec2-user@$EC2_SERVER:~/llvm-12
scp -i $EC2_KEY -r /opt/local/Library ec2-user@$EC2_SERVER:~/.darling/opt/local/Library

Fails to work in interactive mode, quits with the error:
"terminals database is inaccessible"

but you can pass multiple arguments with -o option and get some info, e.g. a call stack

workaround: download darlingcore and any useful symbols (.dlsym & .dSYM) to a mac and use lldb there.

darling-coredump:
arm assembily code is missing in core generated darlingcore file, you can only get the function names, registers and memory.

There is a long post on this thread about what would need to be done to get a working lldb on arm-64:
https://github.com/darlinghq/darling/issues/642

-------------------------------------------------------------------------------
ARM64 - Thread Local Storage emulation not possible, patch instead
-------------------------------------------------------------------------------

TPIDRRO_EL0 cannot be written to by Darling as it is not running in Kernel Space.
Instead user code needs to call __darling_thread_get_tsd()

This can be done by patching a dylib on load, replacing TPIDRRO_EL0 register access with a call to __darling_thread_get_tsd()

Count how many TPIDRRO_EL0 acceses you have in a dylib using:
otool -tV UnityPlayer.dylib | grep -N12 TPIDRRO_EL0

This will show the surrounding code:
otool -tV UnityPlayer.dylib | grep -N12 TPIDRRO_EL0

Modify tls_fixup.c to patch your dylib on load, and attach it with:
export DYLD_INSERT_LIBRARIES=./libtlsfixup.dylib

-------------------------------------------------------------------------------
Install on AWS Linux 2023:
-------------------------------------------------------------------------------

Download and build FFMPEG from source

export LD_LIBRARY_PATH=/usr/local/lib/:/opt/ffmpeg_build/lib/
export PKG_CONFIG_PATH="/opt/ffmpeg_build/lib/pkgconfig"

sudo yum -y install make cmake clang bison dbus-devel flex glibc-devel fuse-devel \
systemd-devel elfutils-libelf-devel cairo-devel freetype-devel \
libjpeg-turbo-devel fontconfig-devel libglvnd-devel \
mesa-libGL-devel mesa-libEGL-devel mesa-libGLU-devel \
libtiff-devel libxml2-devel libbsd-devel git git-lfs libXcursor-devel \
libXrandr-devel giflib-devel pulseaudio-libs-devel libxkbfile-devel \
openssl-devel llvm libcap-devel vulkan-devel llvm-devel xorg-x11-server-Xvfb

cmake .. -DTARGET_ARM64=ON

Run with a X11 environment and $DISPLAY on a headless server (no desktop installed) for command line apps:
xvfb-run darling shell

-------------------------------------------------------------------------------
Darling Server concerns:
-------------------------------------------------------------------------------

export DSERVER_LOG_LEVEL=info

tail -f ${DARLING_PREFIX}/private/var/log/dserver.log

Huge number of warnings from:

darlingserver/duct-tape/src/locks.c:
dtape_mutex_lock: "Trying to lock mutex without an active thread!"
dtape_mutex_unlock: "Trying to unlock mutex without an active thread!"

So many it must be hurting performance.

also a lof of this, but not quite at the same scale:
src/external/darlingserver/src/call.cpp:
DarlingServer::Call::PthreadCanceled::processCall() "TODO: DarlingServer::Call::PthreadCanceled::processCall()"

Have experienced deadlock in a mach_thread_self() syscall when called from multiple user threads simulateously (seems ok on main thread),
could be related to the spam above.

-------------------------------------------------------------------------------
CONFIG_THREAD_GROUPS
-------------------------------------------------------------------------------

src/external/xnu/osfmk/arm/proc_reg.h
[155] /* Thread groups are enabled on all ARM platforms (irrespective of scheduler) */
[156] #define CONFIG_THREAD_GROUPS 0 // was 1 originally but turned off now

This now compiles by adding thread_groups.c and including ast.c in it, but its turned off. It's unclear what it's intended to do.
It does appear to be meant to be on ARM64 but might just be hanger on from the FreeBSD code and not needed for Darling.
