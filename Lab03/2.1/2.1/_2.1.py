import base64
import math

def egcd(a, b):
    if b == 0:
        return a, 1, 0
    g, x1, y1 = egcd(b, a % b)
    return g, y1, x1 - (a // b) * y1

def mod_inverse(a, m):
    g, x, _ = egcd(a, m)
    if g != 1:
        raise ValueError("Không tồn tại nghịch đảo modulo")
    return x % m

def rsa_apply(x, key):
    k, n = key
    return pow(x, k, n)

def int_to_bytes(x):
    if x == 0:
        return b"\x00"
    return x.to_bytes((x.bit_length() + 7) // 8, byteorder="big")

def bytes_to_int(b):
    return int.from_bytes(b, byteorder="big")

def bytes_to_printable_text(b):
    try:
        return b.decode("utf-8")
    except:
        try:
            return b.decode("ascii")
        except:
            try:
                return b.decode("latin-1")
            except:
                return None


def is_reasonable_text(s):
    if not s or len(s.strip()) == 0:
        return False
    good = sum(1 for c in s if c.isalnum() or c in " .,:;!?-_/()[]{}'\"\n\t&")
    return good / len(s) >= 0.8



# PHẦN 1 - TẠO KHÓA
def create_keys():
    # Key 1
    p1, q1, e1 = 11, 17, 7
    n1 = p1 * q1
    phi1 = (p1 - 1) * (q1 - 1)
    d1 = mod_inverse(e1, phi1)
    pu1, pr1 = (e1, n1), (d1, n1)

    # Key 2
    p2 = 20079993872842322116151219
    q2 = 676717145751736242170789
    e2 = 17
    n2 = p2 * q2
    phi2 = (p2 - 1) * (q2 - 1)
    d2 = mod_inverse(e2, phi2)
    pu2, pr2 = (e2, n2), (d2, n2)

    # Key 3
    p3 = int("F7E75FDC469067FFDC4E847C51F452DF", 16)
    q3 = int("E85CED54AF57E53E092113E62F436F4F", 16)
    e3 = int("0D88C3", 16)
    n3 = p3 * q3
    phi3 = (p3 - 1) * (q3 - 1)
    d3 = mod_inverse(e3, phi3)
    pu3, pr3 = (e3, n3), (d3, n3)

    return {
        "key1": {"p": p1, "q": q1, "phi": phi1, "PU": pu1, "PR": pr1},
        "key2": {"p": p2, "q": q2, "phi": phi2, "PU": pu2, "PR": pr2},
        "key3": {"p": p3, "q": q3, "phi": phi3, "PU": pu3, "PR": pr3},
    }

# PHẦN 2 - MÃ HÓA / GIẢI MÃ M = 5
def part_2(keys):
    pu1 = keys["key1"]["PU"]
    pr1 = keys["key1"]["PR"]
    m = 5

    c_conf = rsa_apply(m, pu1)
    m_conf = rsa_apply(c_conf, pr1)

    c_auth = rsa_apply(m, pr1)
    m_auth = rsa_apply(c_auth, pu1)

    print("==========================================================")
    print("PHẦN 2 - MÃ HÓA / GIẢI MÃ M = 5")
    print("==========================================================")
    print("Confidentiality:")
    print(f"C = {c_conf}")
    print(f"M = {m_conf}")
    print()
    print("Authentication:")
    print(f"C = {c_auth}")
    print(f"M = {m_auth}")
    print()


# PHẦN 3 - MÃ HÓA THÔNG ĐIỆP RA BASE64
def encrypt_text_key1_to_base64(text, pu1):
    e, n = pu1
    raw = text.encode("ascii")
    out = bytearray()

    for b in raw:
        if b >= n:
            raise ValueError("Có byte >= n, không thể mã hóa từng byte với key1")
        c = pow(b, e, n)
        out.append(c)

    return base64.b64encode(bytes(out)).decode("ascii")


def part_3(keys):
    pu1 = keys["key1"]["PU"]
    msg = "The University of Information Technology."
    cipher_b64 = encrypt_text_key1_to_base64(msg, pu1)

    print("==========================================================")
    print("PHẦN 3 - MÃ HÓA THÔNG ĐIỆP RA BASE64")
    print("==========================================================")
    print(f"Plaintext : {msg}")
    print(f"Base64    : {cipher_b64}")
    print()

def try_decode_int_as_text(m):
    # 1) decode trực tiếp từ bytes
    b = int_to_bytes(m)
    s = bytes_to_printable_text(b)
    if s and is_reasonable_text(s):
        return s

    # 2) thử thêm số 0 bên trái để đủ số byte
    if len(b) > 1:
        s2 = bytes_to_printable_text(b.lstrip(b"\x00"))
        if s2 and is_reasonable_text(s2):
            return s2

    return None

def decode_decimal_pairs(num):
    """
    Thử giải mã số theo từng cặp 2 chữ số:
    00-25 -> a-z
    26-51 -> A-Z
    52-61 -> 0-9
    62 -> space
    63 -> .
    64 -> ,
    65 -> !
    66 -> ?
    """
    s = str(num)
    if len(s) % 2 == 1:
        s = "0" + s

    out = []
    for i in range(0, len(s), 2):
        v = int(s[i:i + 2])
        if 0 <= v <= 25:
            out.append(chr(ord('a') + v))
        elif 26 <= v <= 51:
            out.append(chr(ord('A') + (v - 26)))
        elif 52 <= v <= 61:
            out.append(chr(ord('0') + (v - 52)))
        elif v == 62:
            out.append(' ')
        elif v == 63:
            out.append('.')
        elif v == 64:
            out.append(',')
        elif v == 65:
            out.append('!')
        elif v == 66:
            out.append('?')
        else:
            return None

    text = "".join(out)
    if is_reasonable_text(text):
        return text
    return None

def format_result_from_int(m):
    text1 = try_decode_int_as_text(m)
    if text1:
        return text1

    text2 = decode_decimal_pairs(m)
    if text2:
        return text2

    return str(m)


# PHẦN 4 - GIẢI MÃ CIPHERTEXT
def decrypt_cipher3_key1_bytewise(cipher_b64, pr1):
    d, n = pr1
    raw = base64.b64decode(cipher_b64)
    out = bytearray()

    for c in raw:
        m = pow(c, d, n)
        out.append(m)

    return out.decode("ascii")


def decrypt_base64_fixed_blocks(cipher_b64, key):
    """
    Thử giải Base64 như chuỗi các block RSA có độ dài cố định bằng kích thước modulus.
    """
    raw = base64.b64decode(cipher_b64)
    k, n = key
    block_size = (n.bit_length() + 7) // 8

    if len(raw) % block_size != 0:
        return None

    blocks = [raw[i:i + block_size] for i in range(0, len(raw), block_size)]
    plain_bytes = b""

    for block in blocks:
        c = bytes_to_int(block)
        if c >= n:
            return None
        m = pow(c, k, n)
        plain_bytes += int_to_bytes(m)

    text = bytes_to_printable_text(plain_bytes)
    if text and is_reasonable_text(text):
        return text

    return plain_bytes


def decrypt_single_integer(cipher_int, key):
    k, n = key
    if cipher_int >= n:
        return None
    m = pow(cipher_int, k, n)
    return format_result_from_int(m)


def try_all_keys_single_integer(cipher_int, keys):
    order = ["key1", "key2", "key3"]
    for name in order:
        pr = keys[name]["PR"]
        res = decrypt_single_integer(cipher_int, pr)
        if res is not None:
            return res
    return "Không xác định"


def try_cipher1(cipher_b64, keys):
    # Thử kiểu block cố định với key2
    res = decrypt_base64_fixed_blocks(cipher_b64, keys["key2"]["PR"])
    if isinstance(res, str):
        return res

    # Thử kiểu block cố định với key3
    res = decrypt_base64_fixed_blocks(cipher_b64, keys["key3"]["PR"])
    if isinstance(res, str):
        return res

    # Thử bytewise key1
    try:
        res = decrypt_cipher3_key1_bytewise(cipher_b64, keys["key1"]["PR"])
        if is_reasonable_text(res):
            return res
    except:
        pass

    # Nếu không ra text thì thử từng block của key2, rồi ghép kết quả số
    raw = base64.b64decode(cipher_b64)
    d2, n2 = keys["key2"]["PR"]
    block_size2 = (n2.bit_length() + 7) // 8

    if len(raw) % block_size2 == 0:
        blocks = [raw[i:i + block_size2] for i in range(0, len(raw), block_size2)]
        vals = []
        ok = True
        for block in blocks:
            c = bytes_to_int(block)
            if c >= n2:
                ok = False
                break
            vals.append(pow(c, d2, n2))
        if ok:
            return str(vals)

    # Nếu vẫn không được thì thử với key3
    d3, n3 = keys["key3"]["PR"]
    block_size3 = (n3.bit_length() + 7) // 8

    if len(raw) % block_size3 == 0:
        blocks = [raw[i:i + block_size3] for i in range(0, len(raw), block_size3)]
        vals = []
        ok = True
        for block in blocks:
            c = bytes_to_int(block)
            if c >= n3:
                ok = False
                break
            vals.append(pow(c, d3, n3))
        if ok:
            return str(vals)

    return "Không xác định"


def part_4(keys):
    c1 = "raUcesUlOkx/8ZhgodMoo0Uu18sC20yXlQFevSu7W/FDxIy0YRHMyXcHdD9PBvIT2aUft5fCQEGomiVVPv4I"
    c2 = "C87F570FC4F699CEC24020C6F54221ABAB2CE0C3"
    c3 = "Z2BUSkJcg0w4XEpgm0JcMExEQmBlVH6dYEpNTHpMHptMQ7NgTHlgQrNMQ2BKTQ=="
    c4 = (
        "001010000001010011111111101101110010111011001010111011000110011"
        "110111111001111110110100011001111001100001001010001010100111101"
        "010100110011101110111011110101101100000100"
    )

    print("==========================================================")
    print("PHẦN 4 - KẾT QUẢ GIẢI MÃ")
    print("==========================================================")

    # Cipher 1
    print("Cipher 1:")
    print(try_cipher1(c1, keys))
    print()

    # Cipher 2
    print("Cipher 2:")
    c2_int = int(c2, 16)
    print(try_all_keys_single_integer(c2_int, keys))
    print()

    # Cipher 3
    print("Cipher 3:")
    print(decrypt_cipher3_key1_bytewise(c3, keys["key1"]["PR"]))
    print()

    # Cipher 4
    print("Cipher 4:")
    c4_int = int(c4, 2)
    print(try_all_keys_single_integer(c4_int, keys))
    print()


# PHẦN 1 - IN THÔNG TIN KHÓA
def part_1(keys):
    print("==========================================================")
    print("PHẦN 1 - XÁC ĐỊNH KHÓA CÔNG KHAI / KHÓA RIÊNG")
    print("==========================================================")

    for name in ["key1", "key2", "key3"]:
        k = keys[name]
        pu = k["PU"]
        pr = k["PR"]
        print(f"[{name.upper()}]")
        print(f"p   = {k['p']}")
        print(f"q   = {k['q']}")
        print(f"n   = {pu[1]}")
        print(f"phi = {k['phi']}")
        print(f"e   = {pu[0]}")
        print(f"d   = {pr[0]}")
        print(f"PU  = {pu}")
        print(f"PR  = {pr}")
        print()


# MAIN
def main():
    keys = create_keys()
    part_1(keys)
    part_2(keys)
    part_3(keys)
    part_4(keys)


if __name__ == "__main__":
    main()