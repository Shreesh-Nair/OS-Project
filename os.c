#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Encryption function
void encrypt_string(const char* input_string, const char* key, char* encrypted_hex) {
    // Convert the input string and key to bytes
    const unsigned char* input_bytes = (const unsigned char*)input_string;
    const unsigned char* key_bytes = (const unsigned char*)key;
    int input_length = strlen(input_string);
    int key_length = strlen(key);

    // Initialize an empty buffer to store the encrypted bytes
    unsigned char encrypted_bytes[input_length];

    // Iterate over the input bytes and XOR each byte with the corresponding key byte
    for (int i = 0; i < input_length; i++) {
        encrypted_bytes[i] = input_bytes[i] ^ key_bytes[i % key_length];
    }

    // Convert the encrypted bytes to a hexadecimal string
    for (int i = 0; i < input_length; i++) {
        sprintf(encrypted_hex + 2 * i, "%02x", encrypted_bytes[i]);
    }
}

// Decryption function
void decrypt_string(const char* encrypted_hex, const char* key, char* decrypted_string) {
    // Calculate the input length from the hexadecimal string length
    int input_length = strlen(encrypted_hex) / 2;
    const unsigned char* key_bytes = (const unsigned char*)key;

    // Initialize an empty buffer to store the decrypted bytes
    unsigned char decrypted_bytes[input_length];

    // Convert the hexadecimal string back to bytes
    for (int i = 0; i < input_length; i++) {
        sscanf(encrypted_hex + 2 * i, "%2hhx", &decrypted_bytes[i]);
    }

    // Iterate over the decrypted bytes and XOR each byte with the corresponding key byte
    for (int i = 0; i < input_length; i++) {
        decrypted_bytes[i] = decrypted_bytes[i] ^ key_bytes[i % strlen(key)];
    }

    // Null-terminate the decrypted string
    decrypted_bytes[input_length] = '\0';

    // Copy the decrypted bytes to the output string
    strcpy(decrypted_string, (const char*)decrypted_bytes);
}

// Define a structure for the buffer memory
typedef struct {
    int public_section[8];
    int private_section[2];
} Buffer;

// Define a structure for a node
typedef struct {
    int nodeId;
    char* name;
    char key[256];  // Change to store the encryption key as a string
    int affirmation[2]; // Track affirmations for both private locations
} Node;

// Function to check if nodes agree on access for both locations
int nodesAgree(int n, Node* nodes, const char* token, int memoryLocation) {
    int agreed = 1; // Assume all nodes agree

    for (int i = 0; i < n; i++) {
        if (nodes[i].affirmation[memoryLocation - 8] == 0) {
            // Decrypt the token using the node's key
            char decrypted_token[256];
            decrypt_string(token, nodes[i].key, decrypted_token);

            if (strcmp(decrypted_token, token) != 0) {
                agreed = 0; // Node disagreed or failed to decrypt
            }
        }
    }

    return agreed;
}

// Generate a random string for the token
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

int main() {
    srand(time(NULL));

    // Number of nodes (you can change this to any value)
    int n = 3; // Example with 3 nodes

    Buffer buffer;
    buffer.public_section[0] = 1; // Initialize the public section

    Node* nodes = (Node*)malloc(n * sizeof(Node));

    for (int i = 0; i < n; i++) {
        nodes[i].nodeId = i + 1;
        nodes[i].name = (char*)malloc(10);
        snprintf(nodes[i].name, 10, "Node %c", 'A' + i); // Node A, Node B, Node C, etc.

        // Generate random keys for each node
        for (int j = 0; j < 2; j++) {
            char randomKey[256];
            snprintf(randomKey, sizeof(randomKey), "Node%dKey%d", i, j);
            strcpy(nodes[i].key, randomKey);
        }

        nodes[i].affirmation[0] = 0; // Initialize affirmation for location 8 to 0
        nodes[i].affirmation[1] = 0; // Initialize affirmation for location 9 to 0
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

    char token[10];
    snprintf(token, sizeof(token), "%s%s%d", nodes[0].name, randomString, memoryLocation);

    printf("Token: %s\n", token);

    printf("Encrypted Keys:\n");
    for (int i = 0; i < n; i++) {
        char encrypted_key[256];
        encrypt_string(token, nodes[i].key, encrypted_key);
        printf("%s: %s\n", nodes[i].name, encrypted_key);
    }

    printf("Node %s requests private access to location %d.\n", nodes[0].name, memoryLocation);

    // Ask for affirmation only once for each node for the specific location
    for (int i = 1; i < n; i++) {
        char userResponse[4];
        printf("ENTER YES IF %s AFFIRMS THAT %s IS ACCESSING THE PRIVATE MEMORY OF BUFFER: ", nodes[i].name, nodes[0].name);
        scanf("%s", userResponse);

        if (strcmp(userResponse, "YES") == 0) {
            nodes[i].affirmation[memoryLocation - 8] = 1;
        }
    }

    if (nodesAgree(n, nodes, token, memoryLocation)) {
        // All nodes agreed, grant access to the memory location
        printf("Access granted to location %d.\n", memoryLocation);
        buffer.private_section[memoryLocation - 8] = 42; // Simulate private access
    } else {
        // At least one node disagreed, deny access to the memory location
        printf("Access denied. One or more nodes denied access.\n");
    }

    printf("Decrypted Keys:\n");
    for (int i = 0; i < n; i++) {
        char encrypted_key[256];
        encrypt_string(token, nodes[i].key, encrypted_key);
        char decrypted_key[256];
        decrypt_string(encrypted_key, nodes[i].key, decrypted_key);
        printf("%s: %s\n", nodes[i].name, decrypted_key);
    }

    // Free allocated memory
    free(nodes);

    return 0;
}