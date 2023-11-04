def encrypt_string(input_string, key):
    # Convert the string and key to bytes
    input_bytes = input_string.encode('utf-8')
    key_bytes = key.encode('utf-8')

    # Initialize an empty list to store the encrypted bytes
    encrypted_bytes = []

    # Iterate over the input bytes and XOR each byte with the corresponding key b`yte
    for i in range(len(input_bytes)):
        encrypted_byte = input_bytes[i] ^ key_bytes[i % len(key_bytes)]
        encrypted_bytes.append(encrypted_byte)

    # Convert the encrypted bytes to a hexadecimal string
    encrypted_hex = ''.join([format(byte, '02x') for byte in encrypted_bytes])

    return encrypted_hex

def decrypt_string(encrypted_hex, key):
    # Convert the hexadecimal string back to bytes
    encrypted_bytes = bytes.fromhex(encrypted_hex)
    key_bytes = key.encode('utf-8')

    # Initialize an empty list to store the decrypted bytes
    decrypted_bytes = []

    # Iterate over the encrypted bytes and XOR each byte with the corresponding key byte
    for i in range(len(encrypted_bytes)):
        decrypted_byte = encrypted_bytes[i] ^ key_bytes[i % len(key_bytes)]
        decrypted_bytes.append(decrypted_byte)

    # Convert the decrypted bytes back to a string
    decrypted_string = bytes(decrypted_bytes).decode('utf-8')

    return decrypted_string

def token_generator(n):
    token="AXm"+str(n)
    return token


class Node:
    def __init__(self, name, key,alt):
        self.name = name
        self.key = key
        self.alt= alt

buffer=[]*10
NodeA=Node('A','key1','altkey1')
NodeB=Node('B','key2','altkey2')
NodeC=Node('C','key3','altkey3')

n=int(input("Enter the address required: "))
token=token_generator(n)
fin=["","",""]
accessB=input("Node B, do you grant access for node A to access the buffer?(YES or NO) ")
accessC=input("Node A, do you grant access for node A to access the buffer?(YES or NO) ")
fin[0]=encrypt_string(token,NodeA.key)+str(n)
if (accessB=="YES"):
    fin[1]=encrypt_string(token,NodeB.key)+str(n)
    if (accessC=="YES"):
        fin[2]=encrypt_string(token,NodeC.key)+str(n)
    else:
        fin[2]=encrypt_string(token,NodeC.alt)+str(n)
else:
    fin[1]=encrypt_string(token,NodeB.alt)+str(n)
    if (accessC=="YES"):
        fin[2]=encrypt_string(token,NodeC.key)+str(n)
    else:
        fin[2]=encrypt_string(token,NodeC.alt)+str(n)

final=[encrypt_string(token,NodeA.key),encrypt_string(token,NodeB.key),encrypt_string(token,NodeC.key)]
if fin==final:
    print("Access Granted")
else:
    print("Access denied")
print("Decrypted tokens:")
print("Node A:",fin[0])
print("Node B:",fin[1])
print("Node C:",fin[2])






