
def function50(i, L):
    return L[i+2]

def function37(L):
    return [L[-1]]+L

def function52(i):
    return function4() * i

def function1(j, k):
    return (j + k) * function52(1)

def function4():
    return 3

def function188(L):
    return function37(L)+[function50(2, L)]

def function0():
    return function188([1,2,3,4,5,6,7,8,9])[function1(0,1)]

x = function0()
print x
