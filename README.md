# FastJudge

  FastJudge is designed to be a fast, secure and extendable judge suite, 
including judge client & sandbox, judge web interface and a judge daemon.
  
## Design of fastjudge

  Currently, most online judge client heavily relies on [**ptrace(2)**] [1]
to secure the sandbox, but it suffers from both performance and security
issues. Each [**syscall(2)**] [2] requires two context switch (from *tracee* to
*tracer*, and *tracer* to *tracee*), causes severe performance issue for
languages which invokes lots of *syscall*s. On the other hand, which is
more important and severe, is the security issues on x86\_64 systems. 

#### Why ptrace is considered insecure?

  Modern x86\_64 CPUs are capable to run both x86 and amd64 code, so
does on modern OSes. But [syscall numbers] [3] are different between x86 and amd64,
and there is no way to distinguish between them in ptrace. For example, 
syscall number 102 on amd64 in linux is [**getpid**] [4], which is usually 
an allowed syscall, but on x86, this syscall number is actually
[**socketcall**] [5]. Attackers can write assembly code which invoke
x86-style syscall(`int 0x80`) to open socket, bind to socket or connect
to remote servers and so on.
  [`ptrace(2)`] [1] is designed to implement breakpoint debugging and
syscall tracing, but not for sandboxes. Instead, for security purpose, we
should use [**namespaces(7)**] [6] instead.

## FastJudge is secure

  FastJudge is implemented based on linux [**namespaces(7)**] [6],
[**cgroups**] [7], and new filter-style [**seccomp(2)**] [8] (on older
linux kernels (< 3.17), [**seccomp(2)**] [8] is actually controlled by
[**prctl(2)**] [9] with `PR_SET_SECCOMP`), which is used by [docker] [10] 
and [chromium] [11]. Unlike [**ptrace(2)**] [1], filter-style 
[**seccomp(2)**] [8] has ability to distinguish between x86 syscalls and
amd64 syscalls. *ptrace-style* syscall number conflict attack no longer
works.

## FastJudge is fast

  All security measures are implemented inside kernel space, using widely
available kernel features. There are no context switching in judging process.
Further more, master process is implemented using patched libev, and is carefully
designed using event-driven model. Small memory footprint allows you to run better
on low-end judge machines.

## Recommended judge environment.

  * linux kernel >= 3.8 (User namespaces is fully functional since kernel 3.8)

## Requirements

  * linux kernel >= 3.5 (or Ubuntu 3.2 kernel)    
(filter-style seccomp jail is introduced in kernel 3.5, and ubuntu has backported
this feature to its 3.2 kernel)
  * libconfig >= 1.4

[1]: http://man7.org/linux/man-pages/man2/ptrace.2.html
[2]: http://man7.org/linux/man-pages/man2/syscall.2.html
[3]: http://man7.org/linux/man-pages/man2/syscalls.2.html
[4]: http://man7.org/linux/man-pages/man2/getpid.2.html
[5]: http://man7.org/linux/man-pages/man2/socketcall.2.html
[6]: http://man7.org/linux/man-pages/man7/namespaces.7.html
[7]: https://www.kernel.org/doc/Documentation/cgroups/cgroups.txt
[8]: http://man7.org/linux/man-pages/man2/seccomp.2.html
[9]: http://man7.org/linux/man-pages/man2/prctl.2.html
[10]: https://www.docker.com/
[11]: https://www.chromium.org/
