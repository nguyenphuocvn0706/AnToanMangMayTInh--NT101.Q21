from Crypto.Cipher import DES

def avalanche_test(key = b'87654321'):
    p1 = b'STAYHOME'
    p2 = b'STAYHOMA' #Chỉ khác 1 ký tự cuối so với p1

    cipher = DES.new(key, DES.MODE_ECB) #khởi tạo thuật toán DES

    #TODO: Thực hiện mã hóa p1, p2 bằng DES-ECB
    c1 = cipher.encrypt(p1)
    c2 = cipher.encrypt(p2)

    #TODO: Đếm số bit khác nhau giữa hai bản mã thu được
    b1 = bin(int.from_bytes(c1, 'big'))[2:].zfill(64) #áp dụng công thức gợi ý ở 2.3 sử dụng [2:] để loại bỏ ký hiệu
                                                      #của python cho số nhị phân và zfill(64) để fill 64 số 0 ở đầu
    b2 = bin(int.from_bytes(c2, 'big'))[2:].zfill(64)
    diff_bits = sum(b1[i] != b2[i] for i in range(64)) #so sánh số bit khác nhau của b1 và b2

    #TODO: Tính tỷ lệ % bit bị thay đổi 
    percentage = (diff_bits / 64) * 100
    print(f"Tỷ lệ thay đổi:{percentage: .2f}")

def get_key_from_mssv(mssv):
    return mssv.encode()[:8]
print("default key:")
avalanche_test()
print("MSSV: 24521455:")
avalanche_test(get_key_from_mssv("24521455"))
print("MSSV: 24521409:")
avalanche_test(get_key_from_mssv("24521409"))