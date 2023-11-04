    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <time.h>

    char* generateRandomString(int length) {
        char* randomString = (char*)malloc(length + 1);
        const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
        for (int i = 0; i < length; i++) {
            int index = rand() % (sizeof(charset) - 1);
            randomString[i] = charset[index];
        }
        randomString[length] = '\0';
        return randomString;
    }

    char *generateRandomString2(int length) {
        static const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
        char *randomString = (char *)malloc(length + 1);

        if (randomString) {
            for (int i = 0; i < length; i++) {
                int index = rand() % (sizeof(charset) - 1);
                randomString[i] = charset[index];
            }
            randomString[length] = '\0';

        return randomString;
    }}

    void encrypt_string(const char* input_string, const char* key, char* encrypted_hex) {
        const unsigned char* input_bytes = (const unsigned char*)input_string;
        const unsigned char* key_bytes = (const unsigned char*)key;
        int input_length = strlen(input_string);
        int key_length = strlen(key);

        unsigned char encrypted_bytes[input_length];

        for (int i = 0; i < input_length; i++) {
            encrypted_bytes[i] = input_bytes[i] ^ key_bytes[i % key_length];
        }

        for (int i = 0; i < input_length; i++) {
            sprintf(encrypted_hex + 2 * i, "%02x", encrypted_bytes[i]);
        }
    }

    void decrypt_string(const char* encrypted_hex, const char* key, char* decrypted_string,int x) {
        if (x==1){
        int input_length = strlen(encrypted_hex) / 2;
        const unsigned char* key_bytes = (const unsigned char*)key;

        unsigned char decrypted_bytes[input_length];

        for (int i = 0; i < input_length; i++) {
            sscanf(encrypted_hex + 2 * i, "%2hhx", &decrypted_bytes[i]);
        }

        for (int i = 0; i < input_length; i++) {
            decrypted_bytes[i] = decrypted_bytes[i] ^ key_bytes[i % strlen(key)];
        }

        decrypted_bytes[input_length] = '\0';

        strcpy(decrypted_string, (const char*)decrypted_bytes);}
        else if(x==0) {
            decrypted_string=generateRandomString2(5);
        }
    }

    typedef struct {
        int public_section[8];
        int private_section[2];
    } Buffer;

    typedef struct {
        int nodeId;
        char* name;
        char key[256];
        int affirmation[2];
    } Node;

    int nodesAgree(int n, Node* nodes, const char* token, int memoryLocation) {
        int agreed = 1;

        for (int i = 1; i < n; i++) {
            if (nodes[i].affirmation[memoryLocation - 8] == 0) {
                agreed = 0;
                break;
            }
        }

        return agreed;
    }



    int main() {
        srand(time(NULL));

        int n = 3;

        Buffer buffer;
        buffer.public_section[0] = 1;

        Node* nodes = (Node*)malloc(n * sizeof(Node));

        for (int i = 0; i < n; i++) {
            nodes[i].nodeId = i + 1;
            nodes[i].name = (char*)malloc(10);
            snprintf(nodes[i].name, 10, "Node %c", 'A' + i);

            for (int j = 0; j < 2; j++) {
                char randomKey[256];
                snprintf(randomKey, sizeof(randomKey), "Node%dKey%d", i, j);
                strcpy(nodes[i].key, randomKey);
            }

            nodes[i].affirmation[0] = 0;
            nodes[i].affirmation[1] = 0;
        }

        int memoryLocation;
        char randomString[4];
        int validLocation = 0;

        while (!validLocation) {
            printf("Enter the memory location you want to access (8 or 9): ");
            scanf("%d", &memoryLocation);

            if (memoryLocation == 8 || memoryLocation == 9) {
                validLocation = 1;
            } else {
                printf("Invalid memory location. Only locations 8 and 9 are private.\n");
            }
        }

        strcpy(randomString, generateRandomString(3));

        char token[20];
        snprintf(token, sizeof(token), "%s%s%d", nodes[0].name, randomString, memoryLocation);

        printf("Token: %s\n", token);

        printf("Encrypted Keys:\n");
        for (int i = 0; i < n; i++) {
            char encrypted_key[256];
            encrypt_string(token, nodes[i].key, encrypted_key);
            printf("%s: %s\n", nodes[i].name, encrypted_key);
        }

        printf("Node %s requests private access to location %d.\n", nodes[0].name, memoryLocation);
        int arr[]={0,0};
        for (int i = 1; i < n; i++) {
            char userResponse[4];
            printf("%s, do you grant access for %s to access the private memory of BUFFER? (YES or NO): ", nodes[i].name, nodes[0].name);
            scanf("%s", userResponse);

            if (strcmp(userResponse, "YES") == 0) {
                arr[i-1]=1;
                nodes[i].affirmation[memoryLocation - 8] = 1;
            }
        }

        if (nodesAgree(n, nodes, token, memoryLocation)) {
            printf("Access granted to location %d.\n", memoryLocation);
            buffer.private_section[memoryLocation - 8] = 42;
        } else {
            printf("Access denied. Both nodes must agree to grant access.\n");
        }

        int x=0;
        if (nodesAgree(n, nodes, token, memoryLocation)) x=1;

        printf("Decrypted Keys:\n");
        if (x==1){

        for (int i = 0; i < n; i++) {
            char encrypted_key[256];
            encrypt_string(token, nodes[i].key, encrypted_key);
            char decrypted_key[256];
            decrypt_string(encrypted_key, nodes[i].key, decrypted_key,x);
            printf("%s: %s\n", nodes[i].name, decrypted_key);
        }}
        else {
            if (arr[0]==0 && arr[1]==0){
            for (int i=0;i<2;i++){
            char encrypted_key[256];
            encrypt_string(token, nodes[i].key, encrypted_key);
            char decrypted_key[256];  
            printf("%s: %s\n",nodes[i].name,generateRandomString2(5));}}
            else if (arr[0]==1 && arr[1]==0) {
                printf("%s: %s\n%s: Node %s",nodes[1].name,token,nodes[2].name,generateRandomString2(5));
            }
            else if (arr[0]==0 && arr[1]==1){
                printf("%s: Node %s\n%s: %s",nodes[1].name,generateRandomString2(5),nodes[2].name,token);
            }
        }
        free(nodes);

        return 0;
    }
