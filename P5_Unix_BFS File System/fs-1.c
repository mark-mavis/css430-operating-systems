// ============================================================================
// fs.c - user FileSytem API
// ============================================================================

#include "bfs.h"
#include "fs.h"

// ============================================================================
// Close the file currently open on file descriptor 'fd'.
// ============================================================================
i32 fsClose(i32 fd) { 
  i32 inum = bfsFdToInum(fd);
  bfsDerefOFT(inum);
  return 0; 
}



// ============================================================================
// Create the file called 'fname'.  Overwrite, if it already exsists.
// On success, return its file descriptor.  On failure, EFNF
// ============================================================================
i32 fsCreate(str fname) {
  i32 inum = bfsCreateFile(fname);
  if (inum == EFNF) return EFNF;
  return bfsInumToFd(inum);
}



// ============================================================================
// Format the BFS disk by initializing the SuperBlock, Inodes, Directory and 
// Freelist.  On succes, return 0.  On failure, abort
// ============================================================================
i32 fsFormat() {
  FILE* fp = fopen(BFSDISK, "w+b");
  if (fp == NULL) FATAL(EDISKCREATE);

  i32 ret = bfsInitSuper(fp);               // initialize Super block
  if (ret != 0) { fclose(fp); FATAL(ret); }

  ret = bfsInitInodes(fp);                  // initialize Inodes block
  if (ret != 0) { fclose(fp); FATAL(ret); }

  ret = bfsInitDir(fp);                     // initialize Dir block
  if (ret != 0) { fclose(fp); FATAL(ret); }

  ret = bfsInitFreeList();                  // initialize Freelist
  if (ret != 0) { fclose(fp); FATAL(ret); }

  fclose(fp);
  return 0;
}


// ============================================================================
// Mount the BFS disk.  It must already exist
// ============================================================================
i32 fsMount() {
  FILE* fp = fopen(BFSDISK, "rb");
  if (fp == NULL) FATAL(ENODISK);           // BFSDISK not found
  fclose(fp);
  return 0;
}



// ============================================================================
// Open the existing file called 'fname'.  On success, return its file 
// descriptor.  On failure, return EFNF
// ============================================================================
i32 fsOpen(str fname) {
  i32 inum = bfsLookupFile(fname);        // lookup 'fname' in Directory
  if (inum == EFNF) return EFNF;
  return bfsInumToFd(inum);
}



// ============================================================================
// Read 'numb' bytes of data from the cursor in the file currently fsOpen'd on
// File Descriptor 'fd' into 'buf'.  On success, return actual number of bytes
// read (may be less than 'numb' if we hit EOF).  On failure, abort
// ============================================================================
   i32 fsRead(i32 fd, i32 numb, void* buf) {

    // ++++++++++++++++++++++++
    // Insert your code here
    // ++++++++++++++++++++++++

    i32 inum = bfsFdToInum(fd);
    Inode inode;
    bfsReadInode(inum, &inode);
    i8 tmpData[BYTESPERBLOCK] = { 0 };

    i8* mydata = malloc(inode.size * sizeof(i8));
    if (mydata)
        for (i16 i = 0; i < inode.size / BYTESPERBLOCK; i++)
        {
            bfsRead(inum, i, tmpData);
            memcpy(mydata + BYTESPERBLOCK * i, tmpData, BYTESPERBLOCK);
        }

    if (inode.size % BYTESPERBLOCK > 0) {
        bfsRead(inum, inode.size / BYTESPERBLOCK, tmpData);
        memcpy(mydata + BYTESPERBLOCK * (inode.size / BYTESPERBLOCK), tmpData, inode.size % BYTESPERBLOCK);
    }

    i32 curs = fsTell(fd);
    i32 numRead = numb;
    if (curs + numb > inode.size)
        numRead = inode.size - curs;

    memcpy(buf, mydata + curs, numRead);

    free(mydata);
    bfsSetCursor(inum, curs + numRead);
    //FATAL(ENYI);                                  
    return numRead;
}


// ============================================================================
// Move the cursor for the file currently open on File Descriptor 'fd' to the
// byte-offset 'offset'.  'whence' can be any of:
//
//  SEEK_SET : set cursor to 'offset'
//  SEEK_CUR : add 'offset' to the current cursor
//  SEEK_END : add 'offset' to the size of the file
//
// On success, return 0.  On failure, abort
// ============================================================================
i32 fsSeek(i32 fd, i32 offset, i32 whence) {

  if (offset < 0) FATAL(EBADCURS);
 
  i32 inum = bfsFdToInum(fd);
  i32 ofte = bfsFindOFTE(inum);
  
  switch(whence) {
    case SEEK_SET:
      g_oft[ofte].curs = offset;
      break;
    case SEEK_CUR:
      g_oft[ofte].curs += offset;
      break;
    case SEEK_END: {
        i32 end = fsSize(fd);
        g_oft[ofte].curs = end + offset;
        break;
      }
    default:
        FATAL(EBADWHENCE);
  }
  return 0;
}



// ============================================================================
// Return the cursor position for the file open on File Descriptor 'fd'
// ============================================================================
i32 fsTell(i32 fd) {
  return bfsTell(fd);
}



// ============================================================================
// Retrieve the current file size in bytes.  This depends on the highest offset
// written to the file, or the highest offset set with the fsSeek function.  On
// success, return the file size.  On failure, abort
// ============================================================================
i32 fsSize(i32 fd) {
  i32 inum = bfsFdToInum(fd);
  return bfsGetSize(inum);
}



// ============================================================================
// Write 'numb' bytes of data from 'buf' into the file currently fsOpen'd on
// filedescriptor 'fd'.  The write starts at the current file offset for the
// destination file.  On success, return 0.  On failure, abort
// ============================================================================
i32 fsWrite(i32 fd, i32 numb, void* buf) {

  // ++++++++++++++++++++++++
  // Insert your code here
  // ++++++++++++++++++++++++

    i32 inum = bfsFdToInum(fd);
    Inode inode;
    bfsReadInode(inum, &inode);
    i8 tmpData[BYTESPERBLOCK] = { 0 };

    i32 curs = fsTell(fd);
    i32 newsize = inode.size;
    if (curs + numb > inode.size) newsize = curs + numb;
    i32 numBlocks = newsize / BYTESPERBLOCK;
    if (newsize % BYTESPERBLOCK > 0) numBlocks++;
    i8* mydata = malloc(numBlocks * BYTESPERBLOCK);
    if (!mydata) FATAL(ENOMEM);

    for (i16 i = 0; i < inode.size / BYTESPERBLOCK; i++)
    {
        bfsRead(inum, i, tmpData);
        memcpy(mydata + BYTESPERBLOCK * i, tmpData, BYTESPERBLOCK);
    }
    if (inode.size % BYTESPERBLOCK > 0) {
        bfsRead(inum, inode.size / BYTESPERBLOCK, tmpData);
        memcpy(mydata + BYTESPERBLOCK * (inode.size / BYTESPERBLOCK), tmpData, inode.size % BYTESPERBLOCK);
    }

    if (newsize > inode.size) {
        inode.size = newsize;
        bioRead(inode.indirect, tmpData);
        i16* indirect = (i16*)tmpData;
        indirect[inode.size / BYTESPERBLOCK-5] =  bfsFindFreeBlock();
        bioWrite(inode.indirect, tmpData);
        bfsWriteInode(inum, &inode);
    }

    //i32 curs = fsTell(fd);
    memcpy(mydata + curs, buf, numb);

    //memset(tmpData, 0, BYTESPERBLOCK);
    for (i16 i = 0; i < numBlocks; i++)
    {
        bioWrite(bfsFbnToDbn(inum, i), mydata + BYTESPERBLOCK * i);
    }

    free(mydata);
    bfsSetCursor(inum, curs + numb);

  //FATAL(ENYI);                                  // Not Yet Implemented!
  return 0;
}
