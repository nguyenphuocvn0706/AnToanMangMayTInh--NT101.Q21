from Crypto.Cipher import AES
from Crypto.Random import get_random_bytes

BLOCK_SIZE = 16

#đảo bit
def flip_bit(data, byte_index, bit_index):
    data_bytearray = bytearray(data) #chuyển qua bytearray để có thể chỉnh sửa
    data_bytearray[byte_index] ^= (1 << bit_index) #tạo mask có bit = 1 ở vị trí cần lật
    return bytes(data_bytearray) #đổi lại thành bytes

#đếm số block lỗi
def count_corrupted_blocks(p1, p2):
    corrupted = 0 
    for i in range(0, len(p1), BLOCK_SIZE):      #duyệt mỗi block = 16 bytes
        if p1[i:i+BLOCK_SIZE] != p2[i:i+BLOCK_SIZE]:     #nếu block gốc khác block giải mã thì biến count + 1
            corrupted += 1
    return corrupted

#hỗ trợ mode
def get_aes_cipher(mode_name, mode, key, iv):
    if mode_name == "ECB":
        return AES.new(key, mode)
    elif mode_name == "CFB":
        return AES.new(key, mode, iv = iv, segment_size = 128) #128 bit
    return AES.new(key, mode, iv = iv) #CBC, OFB

#test các mode
def test_mode(mode_name, mode):
    plaintext = get_random_bytes(1008) #lấy số chia hết cho 16
    key = get_random_bytes(16)
    iv = get_random_bytes(16)
    cipher = get_aes_cipher(mode_name, mode, key, iv)   #mã hóa
    ciphertext = cipher.encrypt(plaintext)
    corrupted_cipher = flip_bit(ciphertext, byte_index = 25, bit_index = 0) #gây lỗi = cách đảo bit thứ 26
    decipher = get_aes_cipher(mode_name, mode, key, iv) #giải mã dữ liệu lỗi
    decrypted = decipher.decrypt(corrupted_cipher)
    corrupted_blocks = count_corrupted_blocks(plaintext, decrypted) #đếm số block lỗi
    print(f"Mode: {mode_name:<4}\n Số blocks bị lỗi: {corrupted_blocks}")
def main():
    modes_to_test = {"ECB": AES.MODE_ECB, "CBC": AES.MODE_CBC, "CFB": AES.MODE_CFB, "OFB": AES.MODE_OFB}
    for name, mode_val in modes_to_test.items():
        test_mode(name, mode_val)
main()