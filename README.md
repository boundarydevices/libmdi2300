libmdi2300
==========

This library is meant to be used with Opticon MDI-2300 Barcode scanners.

It offers the following API:
```
int mdi2300_init(char *device);
int mdi2300_close(char *device);
int mdi2300_scan(char *device, char *buffer, int length);
```

An example is provided which initializes and tries to scan any bar code available:
```
$ mdi2300_scan /dev/ttymxc2
Code: 12345678
```

Build procedure
---------------

The project is based on a simple Makefile:
```
$ make all examples
$ make install install_examples
```

It is possible to change the output (objects) folders:

```
$ make all O=<output_folder>
```

For cross-compilation, one just needs to override the CC variable:
```
$ CC=<cross-gcc> make all
$ DESTDIR=<target-rootfs> make install
```

License
-------

The project is provided under the **LGPLv2.1+** license.

See *LICENSE* file for more information.
