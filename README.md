# Triple fault kernel module

## Overview

Inject a triple fault exception [1] at the beginning of a target kernel
function that will trigger a system reboot when that function is called.

**WARNING**: This will make your system reboot, so use with caution (or test it in a VM)!

**NOTE**: This only works on x86.

### Use cases

 - extreme "real man" debugging tool :)

When you need to debug a system that is just crashing or it gets stuck
somewhere and you don't have the possibility to use any debugging tool
whatsoever, let's just make the system reboot when a specific function is
called to see if the kernel gets up to a certain point.

## Interface

```
 # insmod triple-fault.ko function_name=<FUNCTION_NAME>
```

Wait for a system reboot when/if `<FUNCTION_NAME>` is called.

## Example usage

```
 # insmod triple-fault.ko function_name=__do_sys_sync
 # sync

 (system will reboot, enjoy!)
```

## See also

 - [1] [Triple Fault](https://en.wikipedia.org/wiki/Triple_fault)
