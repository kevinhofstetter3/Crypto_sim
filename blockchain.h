#include "hash.h"

struct Block{
    int height;                 // block height
    int data;      
    int nonce;             
    struct Digest prev_hash;    // sha_40 digest of previous block
    struct Block *prev_block;
};

struct Blockchain{
    struct Block *head;
    int size;
};

// Sets digest to the SHA_40 digest of block
void set_digest(struct Digest *digest, struct Block *block); 
// Creates and returns a pointer to an empty blockchain 
struct Blockchain *initialize();
// Add a block to blockchain
void add(struct Blockchain *chain, int data);
// Verifies blockchain, used to ensure that blockchain has not been modified
void verify(struct Blockchain *chain);
// Alters data of a block at height 
void alter_block(struct Blockchain *chain, int height, int data);
// Alters data of a block at height and alters the previous hash stored in the block at height+1 
void alter_2_blocks(struct Blockchain *chain, int height, int data);
// Deletes a block at height
void delete_at(struct Blockchain *bc, int height);
// Prints blockchain
void print_blockchain(struct Blockchain *chain);