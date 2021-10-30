#include "blockchain.h"

//Similar structure to a linked list by allocating data for a chain
struct Blockchain *initialize()//Initiallized the blockchain
{
    struct Blockchain *chain = (struct Blockchain*)malloc(sizeof(struct Blockchain));
    chain->size = 0;
    chain->head = NULL;
    return chain;
}

//Prints each block in a given blockchain; height, data, hash value at 
//all rounds (1-5)
void print_blockchain(struct Blockchain *blockchain)//Prints the blockchain
{
    printf("Printing...\n");
    struct Block *current = blockchain->head;
    while(current){
        printf("{height: %d, data: %d, prev_hash: %d %d %d %d %d}\n", 
        current->height, current->data, current->prev_hash.hash0, current->prev_hash.hash1,
        current->prev_hash.hash2, current->prev_hash.hash3, current->prev_hash.hash4);
        current = current->prev_block;
    }
}

//This sets the digest at a given block (used in the add function)
void set_digest(struct Digest *digest, struct Block *block)//Sets the digest
{
    struct Digest *temp = (struct Digest*)SHA_40((unsigned char *)block,BLOCK_SIZE);
    digest->hash0 = temp->hash0;
    digest->hash1 = temp->hash1;
    digest->hash2 = temp->hash2;
    digest->hash3 = temp->hash3;
    digest->hash4 = temp->hash4;
}

//This adds a new block at the head of a blockchain, and
//fills the block with data.
void add(struct Blockchain *chain, int data) //Adds a new block
{
    struct Block *new_block = (struct Block*)malloc(sizeof(struct Block));
    new_block->data = data;
    new_block->height = chain->size;

    new_block->prev_block = chain->head;
    chain->head = new_block;
    
    if(chain->size != 0)
    {
        //struct Digest *digest = (struct Digest*)SHA_40((unsigned char *)new_block, sizeof(new_block));
        set_digest(&(new_block->prev_hash), new_block->prev_block);
    }
    chain->size += 1;
}

//This function loops through a blockchain comparing a blocks hash to the hash 
//of the previous block
void verify(struct Blockchain *chain) //Verifies integrity of blockchain
{
    printf("Verifying...\n");

    struct Block *current = chain->head;
    struct Block *next = chain->head->prev_block;
    int count = 0;

    for(int i = 0; i < chain->size - 1; i++)
    {
        struct Digest digest1 = current->prev_hash;
        struct Digest temp = next->prev_hash; //sets temp for the prev_hash to return to normal after set_digest

        set_digest(&(next->prev_hash), next);
        struct Digest digest2 = next->prev_hash;

        int bool = digest_equal(&digest1, &digest2);
        if(bool == 1)
        {
            printf("Block %d passed\n", current->height);
            count++;
        }
        else
        {
            printf("Block %d failed\n", current->height);
            break;
        }
        
        next->prev_hash = temp;
        current = current->prev_block;
        next = next->prev_block;
    }
    if(count == 4) //If all 4 blocks passed then pass the final block
    {
        printf("Block 0 passed\nAll blocks have been verified\n");
    }
}

//This deletes a block at a certain height without disrupting the integrity 
//of the blockchain
void delete_at(struct Blockchain *chain, int height) //Deletes a block
{
    struct Block *temp = chain->head;

    if(chain->head == NULL)
    {
        return;
    }
    if(height == 0) //if head is removed
    {
        chain->head = temp->prev_block;
        free(temp); //delete/free original head
    }
    for(int i = 0; i<height+1; i++) //find previous node of one to be deleted
    {
        temp = temp->prev_block; 
    }
    if(temp == NULL || temp->prev_block == NULL) //if position is more than number of nodes
    {
        return;
    }

    struct Block *next = temp->prev_block->prev_block;
    free(temp->prev_block);
    temp->prev_block = next;
    return;
}

    //This acts as an attack function. Changes the data at a certain block.
    //Tests the verify function
void alter_block(struct Blockchain *chain, int height, int data) //Alters one block
{
    struct Block *block = chain->head;
    
    for(int i = 0; i < chain->size; i++)
    {
        if(block->height != height)
        {
            block = block->prev_block;
        }
        else
        {
            block->data = data;
        }
    }

}

    //This acts as an attack function. By changing the data at a block at index 
    //HEIGHT in the blockchain AND the hash of every block following, the verify function is tested and can
    //identify where the faulty block is 
void alter_2_blocks(struct Blockchain *chain, int height, int data) //Changes two blocks
    {
        struct Block *current = chain->head->prev_block;
        struct Block *before = chain->head;

        for(int i = 0; i < chain->size; i++)
        {
            if(current->height != height) //find block at height
            {
                current = current->prev_block;
                before = before->prev_block;
            }
            else
            {
                current->data = data; //set data
                set_digest(&(before->prev_hash), current);
            }
        }
    }

int main()
{

    //Initialize
    struct Blockchain *blockchain = initialize();
    for (int i = 1; i<=5; i++){
        add(blockchain, 10*i);
    }
    print_blockchain(blockchain);
    verify(blockchain);
    
    //Test one at a time
    alter_block(blockchain, 2, 300); //Test 1
    print_blockchain(blockchain);
    verify(blockchain);

    // alter_2_blocks(blockchain, 1, 200); //Test 2
    // print_blockchain(blockchain);
    // verify(blockchain);

    // delete_at(blockchain, 1); //Test3
    // print_blockchain(blockchain);
    // verify(blockchain);

    return 0;
}
