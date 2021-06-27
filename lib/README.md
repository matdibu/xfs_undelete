# Undelete XFS

## Purpose

This document will describe the general idea of the code, its incomplete features and any oddities.

## Notes

 A thorough read of [XFS Algorithms & Data Structures](https://mirrors.edge.kernel.org/pub/linux/utils/fs/xfs/docs/xfs_filesystem_structure.pdf) is highly suggested beforehand.

This is mainly intented for the 5th version of XFS, but might work on 4th as well.

The main idea is to iterate through the filesystem's **inodes b+tree** and find **inodes** marked as deleted but with recoverable data.

There are a lot of usages of **big-endian** integers, even alongside with little-endian integers within the same struct; additional care is recommended to avoid endianness bugs.

The headers that one would expect to be system headers have been manually copy-pasted into `xfs_*.h` files as appropriately.

## Walkthrough

### Allocation Groups

XFS uses by default several **Allocation Groups**, which are essentially separate filesystems, thus providing excellent parallelization.

The code is single-threaded, by iterating linearly through the **Allocation Groups**'s, but it should be trivial to parallelize by simply accessing the **Allocation Groups**'s independently.

### Inode block trees

**Inodes** are allocated in blocks of 64 (with some exceptions, see [Sparse Inodes](https://mirrors.edge.kernel.org/pub/linux/utils/fs/xfs/docs/xfs_filesystem_structure.pdf#2e)), and indexed by a bplus tree.

The most basic way of finding **deleted inodes**, and thus the deleted files, is to check **free inodes** within the **inode blocks**.

This is done by walking the bplus tree with the root in `agi_root`.

The better way of iterating through potentially recoverable deleted inodes is to use an optional feature (`XFS_SB_FEAT_RO_COMPAT_FINOBT`) of **free inode indexing**, and use `agi_free_root` instead.

The walk algorithm is identical in both cases, and uses templates for the different tree structures (see `xfs_tree.hpp`).

### Tree walking

See [the wiki](https://en.wikipedia.org/wiki/B%2B_tree) for a better explaination.

The tree is composed of **pointers** and **records**, and the algorithm is recursively iterating through the **pointers**, and calling the callback in the case **records**.

### Inode blocks

The **inode block** parsing algorithm takes into consideration both normal and sparse allocation and tries (and throws on failure) to create an inode object for every potential inode

If the constructor succeeds, an `InodeEntry` is created (for the metadata that needs sending) and the integrator's callback is called.

### Inodes

**Inodes** store their extents as either lists or trees.

This poses a problem however, because there is no distinction between the two once the inode is deleted (regardless of its previous state, deletion implies converting it to a 0-length **extent list**)

At the moment, seeing as it's exceedingly improbable to either create or undelete a file big and fragmented enough to use an **extent tree**, the code assumes that all inodes were using extent lists.

The code for **extent trees** exists, but it is not sufficiently tested and thus commented out.

Two vectors consisting of the valid extents obtained from interpreting the unused memory as a list and as a tree are compared, and the one which yielded the most extents is assumed to be the correct one.

### Extents

Since the first bytes are used to determine the file type, inodes without an **extent** starting from the offset of 0 are considered invalid and are not processed.

Validation occurs in two steps: a simple metadata check and a "is-not-allocated" check.

The **extent** is esentially overlapped with the records in the free space tree, thus ignoring non-free disk space.

The algorithm itself is in `Parser::TreeCheck`, with a large comment block describing the how overlapping is done.

## Possible Improvements 

* Parallelize the recovery (the AGs are independent)
* Uncomment and test the extent-tree recovery code (`Parser::ReadInode`, `xfs_parser.cpp`)

## Summary / TL;DR

```
for each AG
  for each inode block in inode tree / free inode tree
    for each inode marked as free
      if a valid extent list was obtained from the free inode
        undelete the inode
```
