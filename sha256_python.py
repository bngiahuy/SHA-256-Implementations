import hashlib

def main():
    user_input = input("Enter a string to hash with SHA-256: ")
    input_bytes = user_input.encode("utf-8")
    hash_obj = hashlib.sha256(input_bytes)
    hex_digest = hash_obj.hexdigest()
    print(hex_digest)


if __name__ == "__main__":
    main()