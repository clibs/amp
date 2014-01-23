
//
// amp.h
//
// Copyright (c) 2014 TJ Holowaychuk <tj@vision-media.ca>
//

#ifndef AMP_H
#define AMP_H

/*
 * Protocol version.
 */

#define AMP_VERSION 1

/*
 * Message struct.
 */

typedef struct {
  short version;
  short argc;
  char *buf;
} amp_t;

// prototypes

char *
amp_encode(char **argv, int argc);

void
amp_decode(amp_t *msg, char *buf);

char *
amp_decode_arg(amp_t *msg);

#endif