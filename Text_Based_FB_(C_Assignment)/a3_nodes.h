/********* nodes.h ********
    Student Name 	= Jayven Larsen 
    Student Number	= 101260364
*/
#ifndef __A3_NODES_H__
#define __A3_NODES_H__

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "stdbool.h"

// Structure to represent a linked list of users
typedef struct user
{
    char username[30];
    char password[15];
    struct friend *friends;
    struct post *posts;
    struct user *next;
} user_t;

// Structure to represent linked list of a user's posts
typedef struct friend
{
    char username[30];
    struct post **posts;
    struct friend *next;
}
friend_t;

// Structure to represent linked list of a user's posts
typedef struct post
{
    char content[250];
    struct post *next;
} post_t;


#endif
