# lbs-tests

Simple set of userspace test for large block size (LBS) testing work.
We can expand on this with more simple stuff with time.

# build stuff

 * `make`

## roundup

The linux kernel folio indexes depend on the page order. You refer to the
index based on the page order. On an `x86_64` system if the page order for
a folio is 4 that means the folio is 16k bytes in size and you'll have 4 pages
of 4k bytes each. So to access a page in a folio of higher order we must get
the folio using the folio index and that must be rounded down to the respective
folio index. For example to get a folio into the page cache we would use:

  * `filemap_get_folio(struct address_space *mapping, pgoff_t index, ...)`

Of importance to us is the `index`. On a order 4 folio valid indexes for
different pages would be 0, 4, 8.

  * To get page 0 you'd use folio index 0.
  * To get page 1 you'd use folio index 0.
  * To get page 2 you'd use folio index 0.
  * To get page 3 you'd use folio index 0.
  * To get page 4 you'd use folio index 1.
  * To get page 5 you'd use folio index 1.

and so on.

roundup demos why the kernel `round_down()` can be used over and over again.
It also shows how `round_next()` can be used to get the index of the next
folio but care must be taken to ensure the old original original is always used.

License
-------

This work is licensed under the copyleft-next-0.3.1, refer to the [LICENSE](./LICENSE) file
for details. Please stick to SPDX annotations for file license annotations.
If a file has no SPDX annotation the copyleft-next-0.3.1 applies. We keep SPDX annotations
with permissive licenses to ensure upstream projects we embraced under
permissive licenses can benefit from our changes to their respective files.
Likewise GPLv2 files are allowed as copyleft-next-0.3.1 is GPLv2 compatible.
