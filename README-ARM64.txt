-------------------------------------------------------------------------------
Known Compiling issues
-------------------------------------------------------------------------------

This error can occur during compilation, appears to be an issue with ordering in a CMakeLists.txt, related to MIG:

    [ 45%] Building C object src/external/security/OSX/libsecurityd/CMakeFiles/securityd_ucspc.dir/mig/ucspClientC.c.o
    clang-15: error: no such file or directory: '/home/ec2-user/darling/build/src/external/security/OSX/libsecurityd/mig/ucspClientC.c'
    clang-15: error: no input files
    make[2]: *** [src/external/security/OSX/libsecurityd/CMakeFiles/securityd_ucspc.dir/build.make:96: src/external/security/OSX/libsecurityd/CMakeFiles/securityd_ucspc.dir/mig/ucspClientC.c.o] Error 1
    make[1]: *** [CMakeFiles/Makefile2:98513: src/external/security/OSX/libsecurityd/CMakeFiles/securityd_ucspc.dir/all] Error 2

Workaround is simply to run CMake again and compile again.

Perl & Ruby - compile but not tested. they reference a bunch of stubbed math functions in libm on arm64.

ARM64 is more strict for Macro expansion, so a number of macros like objc_msgSend fail to compile with an error like this:
    "error: too many arguments to function call, expected 0, have 3"
I have ifndef __arm64__ around the code as a temporary workaround, the proper solution would be to fix the macro definition rather than use.
Some code is disabled in:
    src/external/foundation/NSThread, NS*PredicateOperation.m, NSKeyValue*.m
    src/external/cfnetwork/src/URL/CFURLCache.c

Some link dependencies are missing in this branch on both X86 and ARM64, so modules apr and dbuskit are skipped in src/CMakeLists.txt:
    missing Undefined symbols for architecture x86_64: "_opendir", "_readdir_r", "_rewinddir", referenced from: _apr_dir_open, _apr_dir_read, _apr_dir_rewind in dir-32-bit-inode.c.o
	  #add_subdirectory(external/apr)
I also locally modified:
    src/external/IOKitUser/pwr_mgt.subproj/IOPMLibPrivate.c

-------------------------------------------------------------------------------
ARM64 - Darling shell bootstrap issues
-------------------------------------------------------------------------------

Securityd fails to start because a directory is missing. Workaround is:
mkdir ~/.darling/private/var/db/mds/system

bash crashes in bash-3.2/shell.c attempting "setjmp"
[374] /* Catch early SIGINTs. */
[375]  code = setjmp (top_level);
So ShellSpawn now starts zsh instead of bash in this branch.

You may want to also force default shell to zsh instead of bash so it doent keep crashing scripts:
sudo mv /bin/bash /bin/bash_orig
ln -s /bin/bash /bin/zsh

-------------------------------------------------------------------------------
ARM64 - lldb
-------------------------------------------------------------------------------

lldb doesnt work on arm64. This version partially works:

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

Huge number of warnings, so many it must be hurting performance, from:

darlingserver/duct-tape/src/locks.c:
dtape_mutex_lock: "Trying to lock mutex without an active thread!"
dtape_mutex_unlock: "Trying to unlock mutex without an active thread!"

also a lof of this, but not quite at the same scale:
src/external/darlingserver/src/call.cpp:
DarlingServer::Call::PthreadCanceled::processCall() "TODO: DarlingServer::Call::PthreadCanceled::processCall()"

Have experienced deadlock in a mach_thread_self() syscall when called from multiple user threads simulateously (seems ok on main thread),
could be related to the spam above.

MLDR appears to have a memory leak and will eventually crash, can take a day though.

-------------------------------------------------------------------------------
CONFIG_THREAD_GROUPS
-------------------------------------------------------------------------------

src/external/xnu/osfmk/arm/proc_reg.h
[155] /* Thread groups are enabled on all ARM platforms (irrespective of scheduler) */
[156] #define CONFIG_THREAD_GROUPS 0 // was 1 originally but turned off now

This now compiles by adding thread_groups.c and including ast.c in it, but its turned off. It's unclear what it's intended to do.
It does appear to be meant to be on ARM64 but might just be hangover on from the FreeBSD code and not needed for Darling. IDK.
