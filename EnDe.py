def encrypt_string(input_string, key):
    # Convert the string and key to bytes
    input_bytes = input_string.encode('utf-8')
    key_bytes = key.encode('utf-8')

    # Initialize an empty list to store the encrypted bytes
    encrypted_bytes = []

    # Iterate over the input bytes and XOR each byte with the corresponding key byte
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

input_strings = ["Hello, World!", "This is a secret message.", "12345"]
keys = ["Key1", "Key2", "Key3"]
encrypted_strings = []

for i in range(3):
    encrypted = encrypt_string(input_strings[i], keys[i])
    encrypted_strings.append(encrypted)
    print(f"Encrypted #{i + 1}:", encrypted)

# Decrypt the encrypted strings using their respective keys
for i in range(3):
    decrypted = decrypt_string(encrypted_strings[i], keys[i])
    print(f"Decrypted #{i + 1}:", decrypted)
