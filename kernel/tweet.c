#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "defs.h"
#include "tweet.h"

int n_tweets;

int tweetchans[NUMTWEETTOPICS];
struct topic listoftopics[NUMTWEETTOPICS];
struct tweet spacefortweets[MAXTWEETTOTAL];

void topicinit(){
    struct tweet *t;
    // loop this, store in listoftopics
    for(int x = 0; x < NUMTWEETTOPICS; x++){
        // init topic list lock
        initlock(&listoftopics[x].lock, "topiclock");
    }
    for(t = spacefortweets; t < &spacefortweets[MAXTWEETTOTAL]; t++){
        initlock(&t->lock, "tweetlock");
        t->allocated = 0;
    }
    for(int x = 0; x < NUMTWEETTOPICS; x++){
        tweetchans[x] = x;
    }
    n_tweets = 0;
}

struct topic* grabarray(){
    return listoftopics;
}


void padstring(char *message){
    int length = strlen(message);

    // char padded_string[141];
    for(int x = length; x != 141; x++){
        //padded_string[x] = " ";
    }

    // return padded_string;
}


int newtweet(topics tag, char *newmessage){
    /*
    * Assumption made that topic lock is held
    */
    int x;
    // create a new tweet
    for(x = 0; x < MAXTWEETTOTAL; x++){
        if(spacefortweets[x].allocated == 0){
            spacefortweets[x].allocated = 1;
            break;
        }
    }

    if(x == MAXTWEETTOTAL){
        // no space for new tweet, throw error
        return -1;
    }
    else{
        // set new message and init lock
        n_tweets++;
        strncpy(spacefortweets[x].message, newmessage, 141);
    }
    for (int y = 0; y < NUMTWEETTOPICS;y++){
        if(y == tag){
            // set old head of list to next
            spacefortweets[x].next = listoftopics[y].tweet_list;
            // set new head
            listoftopics[y].tweet_list = &spacefortweets[x];
        }
    }
    return 0;
}

int removetweet(topics tag, char* message){
    /*
    *   Assumption made that topic lock is held
    */
    
    int x;
    // find the topic
    for(x = 0; x < NUMTWEETTOPICS; x++){
        if(x == tag){
            break;
        }
    }
    // if no tweet exists aka the linked list is empty, return -1
    if(listoftopics[x].tweet_list == 0){
        return -1;
    }
    // copy message into the pointer
    strncpy(message, listoftopics[x].tweet_list->message ,141);
    
    n_tweets--;

    // mark it, as allocatable
    listoftopics[x].tweet_list->allocated = 0;
    // adjust linked list
    listoftopics[x].tweet_list = listoftopics[x].tweet_list->next;
    return 0;
}

