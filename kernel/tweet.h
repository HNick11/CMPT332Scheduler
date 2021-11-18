struct tweet{
    char message[141];
    int allocated;
    struct spinlock lock;
    struct tweet* next;
};

struct topic{
    struct tweet *tweet_list;
    struct spinlock lock;
};
