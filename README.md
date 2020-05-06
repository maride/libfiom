# libfiom

A set of function overrides to track file input/output calls of binaries, bundled in a shared library to preload it.

## Build

Simply run `make build`. This will build the library as a 64bit and 32bit shared object. The resulting library can be found in the directory `out/`.

If you have trouble compiling the 32bit target on a 64bit machine (missing `bits/libc-header-start.h` and others), you probably need to install 32bit headers (see also [askubuntu](https://askubuntu.com/a/106092))

## Configuration

Configuration is done using *routing files*. As an example:

```
// Routing read() calls from /etc/shadow to /etc/hostname with mode "TRUE"
file /etc/shadow
read true /etc/hostname

// Setting mode "DENY" for write() calls to /tmp/foo
file /tmp/foo
write deny
```

Let's examine:

- To announce a new file in the routing file, use `file <path>`
- To configure read on that file, use `read <mode> [<target path if applicable>]`. Same goes for `write`
- Mode may be `true` (which is the original function call, without libfiom interacting with it), `fake` which simulates an empty read or a successful write, and `deny` which blocks reading and writing activities (comparable to wrong permissions)

## Run and use

After a successful build, the library can be used like this:
`LD_PRELOAD=./out/libfiom-64.so LIBFIOM_LOGSTDOUT=TRUE LIBFIOM_ROUTEFILE=./shadow-to-host cat /etc/shadow`

The specified config file is the same as shown in the *Configuration* section.

The command should produce an output similar to this:

```
[libfiom] Opening file /etc/hostname to file handle 3
[libfiom] Read 5 bytes from file /etc/hostname
X220
[libfiom] Write 5 bytes to file <stdout>
[libfiom] Read 0 bytes from file /etc/hostname
[libfiom] Closing file /etc/hostname (handle 3)
[libfiom] Cleaning up routing table...
[libfiom] Cleaning up descriptor table...
```

## Environment variables

The behaviour of libfiom can be set through environment variables.

| Config name | Values | Description |
| --- | --- | --- |
| `LIBFIOM_LOGSTDOUT` | `TRUE`, `FALSE` | If set to true, logs to `stdout` |
| `LIBFIOM_LIMIT_SCOPE_TO` | `<string>` | If set, limits all activity to the named executable, and let calls from other executables pass through without any modification. |
| `LIBFIOM_ROUTEFILE` | `<string>` | If set, uses the specified route file - see *Configuration* section. |
