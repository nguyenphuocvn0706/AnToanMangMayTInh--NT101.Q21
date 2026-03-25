from Crypto.Cipher import AES
from Crypto.Util.Padding import pad
from Crypto.Random import get_random_bytes

key = b'1234567890123456'
plaintext = b"UIT_LAB_UIT_LAB_UIT_LAB_UIT_LAB_"

print("Plaintext blocks:")
for i in range(0, len(plaintext), 16):
    print(plaintext[i:i+16])

# ECB
ecb = AES.new(key, AES.MODE_ECB)
ct_ecb = ecb.encrypt(plaintext)
print("\nECB:")
for i in range(0, len(ct_ecb), 16):
    print(ct_ecb[i:i+16].hex())

# CBC
iv = b'0000000000000000'
cbc = AES.new(key, AES.MODE_CBC, iv)
ct_cbc = cbc.encrypt(plaintext)
print("\nCBC:")
for i in range(0, len(ct_cbc), 16):
    print(ct_cbc[i:i+16].hex())

# CFB
cfb = AES.new(key, AES.MODE_CFB, iv=iv, segment_size=128)
ct_cfb = cfb.encrypt(plaintext)
print("\nCFB:")
for i in range(0, len(ct_cfb), 16):
    print(ct_cfb[i:i+16].hex())

# OFB
ofb = AES.new(key, AES.MODE_OFB, iv=iv)
ct_ofb = ofb.encrypt(plaintext)
print("\nOFB:")
for i in range(0, len(ct_ofb), 16):
    print(ct_ofb[i:i+16].hex())

# CTR
ctr = AES.new(key, AES.MODE_CTR, nonce=b'12345678')
ct_ctr = ctr.encrypt(plaintext)
print("\nCTR:")
for i in range(0, len(ct_ctr), 16):
    print(ct_ctr[i:i+16].hex())

# EAX
eax = AES.new(key, AES.MODE_EAX, nonce=b'fixednonce123456')
ct_eax, tag_eax = eax.encrypt_and_digest(plaintext)
print("\nEAX:")
for i in range(0, len(ct_eax), 16):
    print(ct_eax[i:i+16].hex())
print("tag:", tag_eax.hex())

# GCM
gcm = AES.new(key, AES.MODE_GCM, nonce=b'fixednonce123')
ct_gcm, tag_gcm = gcm.encrypt_and_digest(plaintext)
print("\nGCM:")
for i in range(0, len(ct_gcm), 16):
    print(ct_gcm[i:i+16].hex())
print("tag:", tag_gcm.hex())