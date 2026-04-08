import hashlib
with open("c0_body.bin", "rb") as f:
    data = f.read()
sha256_hash = hashlib.sha256(data).hexdigest()
print(sha256_hash)