import random

# Mersenne prime thứ 10
MERSENNE_10 = (1 << 89) - 1

# KIỂM TRA NGUYÊN TỐ
def is_prime(n, k=10):
    if n < 2: return False
    if n in (2, 3): return True
    if n % 2 == 0: return False

    # n - 1 = d * 2^r
    r = 0
    d = n - 1
    while d % 2 == 0:
        d //= 2
        r += 1
    for _ in range(k):
        a = random.randrange(2, n - 1)
        x = pow(a, d, n)

        if x == 1 or x == n - 1:
            continue

        for _ in range(r - 1):
            x = pow(x, 2, n)
            if x == n - 1: break
        else: return False
    return True

def random_prime(bits):
    while True:
        n = random.getrandbits(bits)
        n |= (1 << (bits - 1))   
        n |= 1                   
        if is_prime(n): return n


def ten_largest_primes():
    result = []
    n = MERSENNE_10 - 2  
    while len(result) < 10:
        if is_prime(n):
            result.append(n)
        n -= 2
    return result


# Tìm UCLN
def gcd(a, b):
    while b != 0:
        a, b = b, a % b
    return abs(a)


# Tính lũy thừa 
def mod_pow(a, x, p):
    result = 1
    a %= p

    while x > 0:
        if x & 1:
            result = (result * a) % p
        a = (a * a) % p
        x >>= 1

    return result


print("Số nguyên tố ngẫu nhiên:")
print("8 bits :", random_prime(8))
print("16 bits:", random_prime(16))
print("64 bits:", random_prime(64))

print("\n10 số nguyên tố lớn nhất nhỏ hơn 2^89 - 1:")
primes_below = ten_largest_primes_below_mersenne()
for i, p in enumerate(primes_below, 1):
    print(f"{i}. {p}")

print("\nKiểm tra số nguyên tố nhỏ hơn 2^89 - 1:")
n = int(input("Nhập số cần kiểm tra: "))
if n < MERSENNE_10:
    print("Là số nguyên tố." if is_prime(n) else "Không phải số nguyên tố.")
else:
    print("Số phải nhỏ hơn 2^89 - 1.")

print("\nTính GCD của 2 số lớn:")
a = int(input("Nhập a: "))
b = int(input("Nhập b: "))
print("gcd(", a, ",", b, ") =", gcd(a, b))

print("\nTính a^x mod p:")
a = int(input("Nhập a: "))
x = int(input("Nhập x: "))
p = int(input("Nhập p: "))
print("a^x mod p =", mod_pow(a, x, p))