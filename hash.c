#include "hash.h"

//SHA_40 implementation 
unsigned char* SHA_40(const unsigned char *message, size_t length){
    unsigned char A, B, C, D, E; //Initial Seed Value
    A = 11;
    B = 22;
    C = 33;
    D = 44;
    E = 55;

    for(int i=0; i<length; i++){
        for(int round = 0; round < 12; round++){
            unsigned char F = (B & C) ^D;
            unsigned char old_A = A;
            A = F + E + (A >> 3) + message[i] + round;
            E = D;
            D = C;
            C = B << 1;
            B = old_A;         
        }    
    }
    unsigned char *digest = (unsigned char*)malloc(5*sizeof(unsigned char));
    digest[0] = A;
    digest[1] = B;
    digest[2] = C;
    digest[3] = D;
    digest[4] = E;
    return digest;
}


int is_digest_low(struct Digest *digest) //Used for mining - checks if first 12 bits of the digest are all 0.
{
    return digest->hash0 == 0 && digest->hash1 < 16;
}

//Compares each round of a digests hash to the respective round
//of the second digest
int digest_equal(struct Digest *digest1, struct Digest *digest2)// compares two digests, returns 1 if equal and 0 if false
{
    return digest1->hash0 == digest2->hash0 &&
    digest1->hash1 == digest2->hash1 &&
    digest1->hash2 == digest2->hash2 &&
    digest1->hash3 == digest2->hash3 &&
    digest1->hash4 == digest2->hash4;
}


// int main(){
//     //experiment with SHA_40
//     unsigned char *message = 500;
//     SHA_40(*message, 30);
//     return 0;
// }

