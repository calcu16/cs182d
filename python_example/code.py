def func2(list, num):
        return func1(list, num, func4)

def func4(a, b):
        return a * b

def func1(list, num, f):
        acc = 0
        for i in list:
                acc += f(i, num)
        return acc

def main():
    print(func3([1,2,3,4]))

def func3(list)
    return func2(list, 4)

main()
