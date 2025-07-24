import hashlib

def sha256_hash_string(input_string: str) -> str:
    input_bytes = input_string.encode("utf-8")
    hash_obj = hashlib.sha256(input_bytes)
    return hash_obj.hexdigest()

def main():
    user_input = input("Enter a string to hash with SHA-256: ")
    hex_digest = sha256_hash_string(user_input)
    print(hex_digest)

if __name__ == "__main__":
    main()